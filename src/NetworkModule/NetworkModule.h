#pragma once
#include <winsock2.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "NetworkDefs.h"

struct Peer
{
	sockaddr_in addr;
	uint8_t     id;
	uint32_t    lastSeen;   // para detectar desconexiones por timeout
};

class NetworkModule
{
public:

	bool Init();
	void shutdown();

	bool hostSession(uint16_t port);
	bool joinSession(const std::string& ip, uint16_t port);
	void disconnect();

	static constexpr int MAX_PACKET = 256;
	template<typename T>
	void send(uint8_t type, const T& payload)
	{
		static_assert(sizeof(T) + sizeof(NetworkHeader) <= MAX_PACKET,
			"Payload demasiado grande para el paquete");
		_sendRaw(type, &payload, sizeof(T));
	};
	void update();
	NetworkObserverID addObserver(uint8_t type, PacketCallback cb)
	{
		NetworkObserverID id = _nextObserverID++;
		_packageObservers[type].push_back({ id, cb });
		return id;
	}
	void unsubscribe(uint8_t type, NetworkObserverID id)
	{
		auto it = _packageObservers.find(type);
		if (it == _packageObservers.end()) return;
		auto& vec = it->second;
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[id](const auto& o) { return o.first == id; }), vec.end());
	}
	void clearObservers() { _packageObservers.clear(); }

	NetworkState getNetworkState() const { return _connState; }
	bool isConnected() const { return _connState == NetworkState::CONNECTED; }
	std::string getLocalIP() const;
	NetworkRole getRole() const { return _role; }

private:

	void _processPacket(const NetworkHeader& header, const void* payload, int payloadSize, const sockaddr_in& from);
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
	NetworkState _connState = NetworkState::IDLE;
	NetworkRole _role = NetworkRole::NONE;

	uint32_t _handshakeStart = 0;   // momento en que empezo la espera (ms)
	uint32_t _lastSendTime = 0;		// para reenviar el handshake (cliente)

	std::vector<Peer> _peers;   // el host la llena, el cliente tiene solo 1 (el host)
	uint8_t _myId = 0;          // mi propio id de jugador
	uint8_t _nextClientId = 1;  // solo el host lo usa para asignar ids

	NetworkObserverID _nextObserverID = 0;
	std::unordered_map<uint8_t, std::vector<std::pair<NetworkObserverID, PacketCallback>>> _packageObservers;
};
