#pragma once
#include <winsock2.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Cabecera comun a todos los paquetes. El juego define los tipos.
struct PacketHeader
{
	uint8_t  type;		// significado definido por el juego
	uint8_t  senderId;  // id de la maquina que lo envia (0 = host, 1..N = clientes)
	uint32_t tick;		// para orden/interpolacion
};

// Tipos reservados por el propio modulo (el juego usa de 10 en adelante)
enum class SystemMsg : uint8_t { HELLO = 1, BYE = 2, ASSIGN_ID = 3 };
enum class NetworkRole : uint8_t { NONE, HOST, CLIENT };
enum class ConnectionState : uint8_t
{
	IDLE,        // sin sesion
	WAITING,     // host esperando cliente / cliente esperando respuesta
	CONNECTED,   // handshake completo
	FAILED       // timeout o error
};
struct Peer
{
	sockaddr_in addr;
	uint8_t     id;
	uint32_t    lastSeen;   // para detectar desconexiones por timeout
};
using ObserverID = uint32_t;

class Network
{
public:
	using PacketCallback = std::function<void(uint8_t type, const void* data, int size)>;

	bool Init();
	void shutdown();

	bool hostSession(uint16_t port);
	bool joinSession(const std::string& ip, uint16_t port);
	void disconnect();

	static constexpr int MAX_PACKET = 256;
	template<typename T>
	void send(uint8_t type, const T& payload)
	{
		static_assert(sizeof(T) + sizeof(PacketHeader) <= MAX_PACKET,
			"Payload demasiado grande para el paquete");
		_sendRaw(type, &payload, sizeof(T));
	};
	void update();
	ObserverID addObserver(uint8_t type, PacketCallback cb)
	{
		ObserverID id = _nextObserverID++;
		_packageObservers[type].push_back({ id, cb });
		return id;
	}
	void unsubscribe(uint8_t type, ObserverID id)
	{
		auto it = _packageObservers.find(type);
		if (it == _packageObservers.end()) return;
		auto& vec = it->second;
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[id](const auto& o) { return o.first == id; }), vec.end());
	}
	void clearObservers() { _packageObservers.clear(); }

	ConnectionState getConnectionState() const { return _connState; }
	bool isConnected() const { return _connState == ConnectionState::CONNECTED; }
	std::string getLocalIP() const;
	NetworkRole getRole() const { return _role; }

private:

	void _processPacket(const PacketHeader& header, const void* payload, int payloadSize, const sockaddr_in& from);
	void _sendTo(const sockaddr_in& to, uint8_t type, uint8_t senderId, const void* data = nullptr, int size = 0) const;
	void _sendRaw(uint8_t type, const void* data, int size) const;

	// helpers
	Peer* _findPeerByAddr(sockaddr_in addr)
	{
		for (auto& p : _peers)
		{
			if (_sameAddr(p.addr, addr))
				return &p;
		}
		return nullptr;
	};
	void _removePeerByAddr(const sockaddr_in& addr)
	{
		_peers.erase(std::remove_if(_peers.begin(), _peers.end(),
			[&](const Peer& p) { return _sameAddr(p.addr, addr); }), _peers.end());
	};
	static bool _sameAddr(const sockaddr_in& a, const sockaddr_in& b)
	{
		return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
	}
	bool _bindLocal(uint16_t port) const
	{
		sockaddr_in local{};
		local.sin_family = AF_INET;			// IPv4 
		local.sin_addr.s_addr = INADDR_ANY;	// Escucha por cualquier interfaz de red de esta maquina.
		local.sin_port = htons(port);		// Convierte el puerto al orden de bytes de red

		// Asocia un shocket a la direccion local
		return ::bind(_socket,			// descriptor de shocket
			(sockaddr*)&local,		// direccion de shocket.
			sizeof(local))	// Longitud en bytes que ocupa el puntero.
			!= SOCKET_ERROR;
	}

	SOCKET _socket = INVALID_SOCKET;
	ConnectionState _connState = ConnectionState::IDLE;
	NetworkRole _role = NetworkRole::NONE;

	uint32_t _handshakeStart = 0;   // momento en que empezo la espera (ms)
	uint32_t _lastSendTime = 0;		// para reenviar el handshake (cliente)

	std::vector<Peer> _peers;   // el host la llena, el cliente tiene solo 1 (el host)
	uint8_t _myId = 0;          // mi propio id de jugador
	uint8_t _nextClientId = 1;  // solo el host lo usa para asignar ids

	ObserverID _nextObserverID = 0;
	std::unordered_map<uint8_t, std::vector<std::pair<ObserverID, PacketCallback>>> _packageObservers;
};
