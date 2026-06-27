#include "Network.h"

#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "Debug.h"

bool Network::Init()
{
	// Estructura de windows que almacena informacion de winsock
	WSADATA wsa;
	// Inicializacion de winsock, devuelve distinto de 0 si da error
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		Debug::error("[Network] WSAStartup fallo: ", WSAGetLastError());
		return false;
	}
	// Funcion que devuelve mascara de bits con la configuracion del shocket
	_socket = socket(
		AF_INET,
		SOCK_DGRAM,
		IPPROTO_UDP);	// Protocolo UDP
	if (_socket == INVALID_SOCKET)
	{
		Debug::error("[Network] socket() fallo: ", WSAGetLastError());
		return false;
	}

	// No bloqueante
	u_long nonBlocking = 1;
	ioctlsocket(
		_socket,		// Descriptor del shocket
		FIONBIO,		//
		&nonBlocking);	//

	Debug::out("[Network] Winsock inicializado");
	return true;
}

void Network::shutdown()
{
	if (_connState == ConnectionState::CONNECTED)
		disconnect();   // avisa al peer

	// Si el shocket es invalido (no se ha inicializado o ya se habia cerrado)
	if (_socket != INVALID_SOCKET) {
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
	// Limpiamos la movida de la consexion
	WSACleanup();
	_connState = ConnectionState::IDLE;
	_role = NetworkRole::NONE;
}

bool Network::hostSession(uint16_t port)
{
	if (!_bindLocal(port))
	{
		Debug::error("[Network] bind() fallo: ", WSAGetLastError());
		return false;
	}

	// Settea el estado de la maquina
	_role = NetworkRole::HOST;
	_connState = ConnectionState::WAITING;
	_handshakeStart = GetTickCount64();   // marca de tiempo de inicio
	Debug::out("[Network] Esperando cliente en puerto ", port, "...");
	return true;
}

bool Network::joinSession(const std::string& ip, uint16_t port)
{
	// Guardamos la direccion del host como _peers[0]
	Peer host{};
	host.addr.sin_family = AF_INET;
	host.addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &host.addr.sin_addr);
	host.id = 0;                        // el host es el id 0
	host.lastSeen = GetTickCount64();
	_peers.clear();
	_peers.push_back(host);

	// Bind en puerto aleatorio para recibir
	_bindLocal(port);

	// Settea el estado de la maquina
	_role = NetworkRole::CLIENT;
	_connState = ConnectionState::WAITING;
	_handshakeStart = GetTickCount64();
	_lastSendTime = 0;
	Debug::out("[Network] Conectando...");
	return true;
}

void Network::disconnect()
{
	// Avisar al peer de que nos vamos (best-effort, UDP puede perderlo)
	if (_connState == ConnectionState::CONNECTED)
		for (auto& peer : _peers)
			_sendTo(peer.addr, (uint8_t)SystemMsg::BYE, 0);

	// Volver a estado inicial, pero sin cerrar Winsock ni el socket
	_connState = ConnectionState::IDLE;
	_role = NetworkRole::NONE;
	_peers.clear();
	_handshakeStart = 0;
	_lastSendTime = 0;
}

void Network::update()
{
	// si esta esperando una conexion
	if (_connState == ConnectionState::WAITING)
	{
		uint32_t now = GetTickCount64();

		// El cliente reenvia HELLO cada 200ms hasta que el host responde
		if (_role == NetworkRole::CLIENT && now - _lastSendTime > 200)
		{
			_sendTo(_peers[0].addr, (uint8_t)SystemMsg::HELLO, 0);
			_lastSendTime = now;
		}

		// Timeout de conexion
		if (now - _handshakeStart > 10000)
		{
			Debug::error("[Network] Timeout de conexion");
			_connState = ConnectionState::FAILED;
			return;
		}
	}

	// Solo recibimos si estamos esperando o conectados
	if (_connState != ConnectionState::WAITING &&
		_connState != ConnectionState::CONNECTED) return;

	char buffer[MAX_PACKET];
	sockaddr_in from{};
	int fromLen = sizeof(from);

	// Si hay mas de un paquete encolado
	while (true)
	{
		//  va sacando paquete a paquete hasta que da error
		int r = recvfrom(_socket, buffer, sizeof(buffer), 0,
			(sockaddr*)&from, &fromLen);
		if (r == SOCKET_ERROR) break;
		if (r < (int)sizeof(PacketHeader)) continue; // paquete corrupto/incompleto

		PacketHeader header;
		memcpy(&header, buffer, sizeof(header));

		const void* payload = buffer + sizeof(header);
		int payloadSize = r - sizeof(header);

		// Cualquier otro tipo se lo pasamos al juego
		_processPacket(header, payload, payloadSize, from);
	}
}

void Network::_processPacket(const PacketHeader& header, const void* payload,
	int payloadSize, const sockaddr_in& from)
{

	// Mensajes de sistema los maneja el modulo
	switch ((SystemMsg)header.type)
	{
	case SystemMsg::HELLO:	// Es un paquete handshake?
		// Solo relevante mientras conectamos
		if (_role == NetworkRole::HOST) {
			// Ya lo conociamos? (HELLO reenviado)
			Peer* existing = _findPeerByAddr(from);
			if (!existing) {
				Peer p;
				p.addr = from;
				p.id = _nextClientId++;
				p.lastSeen = GetTickCount64();
				_peers.push_back(p);

				// Le respondemos diciendole que id le toca

				_sendTo(p.addr, (uint8_t)SystemMsg::ASSIGN_ID, 0, &p.id, sizeof(p.id));
				_connState = ConnectionState::CONNECTED;
				Debug::out("[Network] Cliente conectado con id ", (int)p.id);
			}
		}
		return;

	case SystemMsg::BYE:	// Es un paquete de despedida?
		if (_role == NetworkRole::HOST)
		{
			Peer* p = _findPeerByAddr(from);
			if (p) {
				Debug::out("[Network] Jugador ", (int)p->id, " se desconecto.");
				_removePeerByAddr(from);
			}
		}
		else
		{
			// Si soy cliente y el host se va, me desconecto del todo
			Debug::out("[Network] El host cerro la sesion.");
			_connState = ConnectionState::IDLE;
			_peers.clear();
		}
		return;
	case SystemMsg::ASSIGN_ID: // Es un paquete de asignacion de ID?
		if (_role == NetworkRole::CLIENT)
		{
			// El payload trae mi id de jugador
			memcpy(&_myId, payload, sizeof(_myId));
			_connState = ConnectionState::CONNECTED;
			Debug::out("[Network] Conectado! Mi id es ", (int)_myId);
		}
		return;
	default:
		// AMPLIACION N JUGADORES: el host reenvia a los demas peers.
		// Con 2 jugadores el bucle no encuentra a nadie mas, asi que no hace nada,
		// pero queda preparado para cuando haya mas clientes.

		// Si soy el host, reenvio a todos los demas clientes
		if (_role == NetworkRole::HOST)
			for (auto& peer : _peers)
				if (peer.id != header.senderId)
					_sendTo(peer.addr, header.type, header.senderId, payload, payloadSize);

		// En cualquier caso, lo entrego a mis observers locales
		auto it = _packageObservers.find(header.type);
		if (it != _packageObservers.end())
			for (auto& obs : it->second)
				obs.second(header.senderId, payload, payloadSize);
	}
}

std::string Network::getLocalIP() const
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		Debug::error("[Network] gethostname fallo : ", WSAGetLastError());
		return "127.0.0.1";
	}

	addrinfo hints{};
	hints.ai_family = AF_INET;      // solo IPv4
	hints.ai_socktype = SOCK_DGRAM;

	addrinfo* result = nullptr;
	if (getaddrinfo(hostname, nullptr, &hints, &result) != 0)
	{
		Debug::error("[Network] getaddrinfo fallo: ", WSAGetLastError());
		return " ";
	}

	std::string ip = "127.0.0.1";
	// Recorremos las direcciones y nos quedamos con la primera IPv4 valida
	for (addrinfo* p = result; p != nullptr; p = p->ai_next) {
		sockaddr_in* addr = (sockaddr_in*)p->ai_addr;
		char buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr->sin_addr, buffer, sizeof(buffer));

		// Descartamos loopback, queremos la IP de red real
		std::string candidate = buffer;
		if (candidate != "127.0.0.1")
		{
			ip = candidate;
			break;
		}
	}

	freeaddrinfo(result);
	return ip;
}

void Network::_sendTo(const sockaddr_in& to, uint8_t type, uint8_t senderId,
	const void* data, int size) const
{
	char buf[MAX_PACKET];
	PacketHeader h{ type, senderId, 0 };
	memcpy(buf, &h, sizeof(h));

	if (data && size) 
		memcpy(buf + sizeof(h), data, size);

	sendto(_socket, buf, sizeof(h) + size, 0, (sockaddr*)&to, sizeof(to));
}

void Network::_sendRaw(uint8_t type, const void* data, int size) const
{
	if (_connState != ConnectionState::CONNECTED) 
		return;

	if (_role == NetworkRole::HOST)
	{
		for (auto& peer : _peers)
		{
			_sendTo(peer.addr, type, _myId, data, size);
		}
	}
	else
	{
		_sendTo(_peers[0].addr, type, _myId, data, size);
	}
}