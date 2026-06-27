#pragma once

#include <functional>

// Cabecera comun a todos los paquetes. El juego define los tipos.
struct NetworkHeader
{
	uint8_t  type;		// significado definido por el juego
	uint8_t  senderId;  // id de la maquina que lo envia (0 = host, 1..N = clientes)
	uint32_t tick;		// para orden/interpolacion
};

// Tipos reservados por el propio modulo (el juego usa de 10 en adelante)
enum class NetworkMsg : uint8_t { HELLO = 1, BYE = 2, ASSIGN_ID = 3 };
enum class NetworkRole : uint8_t { NONE, HOST, CLIENT };
enum class NetworkState : uint8_t
{
	IDLE,        // sin sesion
	WAITING,     // host esperando cliente / cliente esperando respuesta
	CONNECTED,   // handshake completo
	FAILED       // timeout o error
};

using NetworkObserverID = uint32_t;

using PacketCallback = std::function<void(uint8_t type, const void* data, int size)>;
