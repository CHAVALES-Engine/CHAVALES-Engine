#pragma once
#include <winsock2.h>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "NetworkDefs.h"


/**
* @brief Identificar de conexion que contiene su socket, su id y su ultima vez visto.
*/
struct Peer
{
	sockaddr_in addr;		// Socket.
	uint8_t     id;			// ID.
	uint32_t    lastSeen;   // Ultima vez que se ha detectado, para poder detectar desconexiones por timeout.
};

class NetworkModule
{
public:

	/**
	* @brief Inicializa la conexion.
	*
	* @returns bool - Si se ha podido inicializar o no.
	*/
	bool Init();

	/**
	* @brief Cierre de la conexion.
	*/
	void shutdown();


	/**
	* @brief Hostear una sesion.
	*
	* @param port - Puerto de la sesion.
	*
	* @returns bool - Si se ha podido hostear o no.
	*/
	bool hostSession(uint16_t port);
	/**
	* @brief Unirse a una sesion.
	*
	* @param ip - IP de la sesion.
	* @param port - Puerto de la sesion.
	*/
	void joinSession(const std::string& ip, uint16_t port);
	/**
	* @brief Desconectarse de la sesion.
	*/
	void disconnect();

	static constexpr int MAX_PACKET = 256;
	/**
	* @brief Manda un mensaje.
	*
	* @param type - Tipo de paquete.
	* @param payload - Mensaje.
	*/
	void sendRaw(uint8_t type, const void* data, int size) const { _sendRaw(type, data, size); }

	/**
	* @brief Update de NetworkModule.
	*/
	void update();


	/**
	* @brief Mete un observador nuevo con una funcion asociada.
	*
	* @param type - Tipo de paquete.
	* @param cb - Funcion.
	* 
	* @returns NetworkObserverID - ID asociado automaticamente al nuevo observador.
	*/
	NetworkObserverID addObserver(uint8_t type, PacketCallback cb)
	{
		NetworkObserverID id = _nextObserverID++;
		_packageObservers[type].push_back({ id, cb });
		return id;
	}
	/**
	* @brief Desuscribe un observador.
	*
	* @param type - Tipo de paquete.
	* @param id - ID del observador a desuscribir.
	*/
	void unsubscribe(uint8_t type, NetworkObserverID id)
	{
		auto it = _packageObservers.find(type);
		if (it == _packageObservers.end()) return;
		auto& vec = it->second;
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[id](const auto& o) { return o.first == id; }), vec.end());
	}
	/**
	* @brief Vacia el mapa de observadores.
	*/
	void clearObservers() { _packageObservers.clear(); }


	/**
	* @brief Devuelve el estado de la conexion.
	*
	* @returns NetworkState - Estado de la conexion: IDLE, WAITING, CONNECTED o FAILED.
	*/
	NetworkState getNetworkState() const { return _connState; }
	/**
	* @brief Devuelve si la conexion esta establecida o no.
	*
	* @returns bool - Si esta establecida (NetworkState::CONNECTED) o no.
	*/
	bool isConnected() const { return _connState == NetworkState::CONNECTED; }
	/**
	* @brief Devuelve la IP local.
	*
	* @returns std::string - IP.
	*/
	std::string getLocalIP() const;
	/**
	* @brief Devuelve el rol de quien lo llama.
	*
	* @returns NetworkRole - Rol.
	*/
	NetworkRole getRole() const { return _role; }

private:

	/**
	* @brief Procesa los tipos de paquete.
	*
	* @param header - Cabecera del paquete.
	* @param payload - Mensaje.
	* @param payloadSize - Como de grande es la informacion del mensaje.
	* @param from - Quien lo ha mandado.
	*/
	void _processPacket(const NetworkHeader& header, const void* payload, int payloadSize, const sockaddr_in& from);
	
	/**
	* @brief Manda un mensaje.
	*
	* @param to - Socket a quien mandarle el mensaje.
	* @param type - Tipo de paquete.
	* @param senderId - ID de quien manda el mensaje.
	* @param data - Datos del mensaje.
	* @param size - Como de grande es el mensaje.
	*/
	void _sendTo(const sockaddr_in& to, uint8_t type, uint8_t senderId, const void* data = nullptr, int size = 0) const;
	/**
	* @brief Decide dependiendo de quien manda el mensaje como mandarlo.
	*
	* @param type - Tipo de paquete.
	* @param data - Datos del mensaje.
	* @param size - Como de grande es el mensaje.
	*/
	void _sendRaw(uint8_t type, const void* data, int size) const;

	// -- Metodos auxiliares.

	/**
	* @brief Busca un peer dado su socket.
	*
	* @param addr - Socket a buscar.
	*
	* @returns Peer - Peer que buscas.
	*/
	Peer* _findPeerByAddr(sockaddr_in addr)
	{
		for (auto& p : _peers)
		{
			if (_sameAddr(p.addr, addr))
				return &p;
		}
		return nullptr;
	};

	/**
	* @brief Elimina un peer dado su socket.
	*
	* @param addr - Socket para eliminar el peer.
	*/
	void _removePeerByAddr(const sockaddr_in& addr)
	{
		_peers.erase(std::remove_if(_peers.begin(), _peers.end(),
			[&](const Peer& p) { return _sameAddr(p.addr, addr); }), _peers.end());
	};
	/**
	* @brief Comprueba si dos sockets son el mismo.
	*
	* @param a - Socket A.
	* @param b - Socket B.
	*
	* @returns bool - Si son el mismo.
	*/
	static bool _sameAddr(const sockaddr_in& a, const sockaddr_in& b)
	{
		return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
	}
	/**
	* @brief Asociarse a un puerto.
	*
	* @param port - Puerto al que asociarse.
	*
	* @returns bool - Si se ha podido asociarse o no.
	*/
	bool _bindLocal(uint16_t port) const
	{
		sockaddr_in local{};
		local.sin_family = AF_INET;			// IPv4.
		local.sin_addr.s_addr = INADDR_ANY;	// Escucha por cualquier interfaz de red de esta maquina.
		local.sin_port = htons(port);		// Convierte el puerto al orden de bytes de red.

		// Asocia un socket a la direccion local.
		return ::bind(_socket,		// Descriptor de socket.
			(sockaddr*)&local,		// Direccion de socket.
			sizeof(local))			// Longitud en bytes que ocupa el puntero.
			!= SOCKET_ERROR;
	}

	SOCKET _socket = INVALID_SOCKET;				// Socket.
	NetworkState _connState = NetworkState::IDLE;	// Estado de la conexion.
	NetworkRole _role = NetworkRole::NONE;			// Rol de la conexion.
	
	uint32_t _handshakeStart = 0;   // Momento en que empezo la espera (ms).
	uint32_t _lastSendTime = 0;		// Para reenviar el handshake (cliente).

	std::vector<Peer> _peers;   // El host la llena, el cliente tiene solo 1 (el host).
	uint8_t _myId = 0;          // Mi propio ID de jugador.
	uint8_t _nextClientId = 1;  // Solo el host lo usa para asignar ids.

	NetworkObserverID _nextObserverID = 0; // Siguiente ID que se asigna.
	// Mapa con clave: tipo de paquete y su contenido: vector de pares de IDs de observadores con sus callbacks.
	std::unordered_map<uint8_t, std::vector<std::pair<NetworkObserverID, PacketCallback>>> _packageObservers;
};
