#pragma once

#include <functional>


/**
* @brief Cabecera comun a todos los paquetes. El juego define los tipos.
*/
struct NetworkHeader
{
	uint8_t  type;		// Significado definido por el juego.
	uint8_t  senderId;  // ID de la maquina que lo envia (0 = host, 1..N = clientes).
	uint32_t tick;		// Para orden/interpolacion.
};

// Tipos reservados por el propio modulo (el juego usa de 10 en adelante)
/**
* @brief Enumerado con los distintos tipos de cabeceras de los mensajes. Los juegos DEBEN usar del 10 en adelante.
*/
enum class NetworkMsg : uint8_t
{
	/** Para establecer conexion. */
	HELLO = 1,
	/** Para despedirse. */
	BYE = 2,
	/** Para asignar un ID. */
	ASSIGN_ID = 3
};
/**
* @brief Enumerado con los posibles roles.
*/
enum class NetworkRole : uint8_t
{
	/** No asignado. */
	NONE,
	/** El host de la sesion. */
	HOST,
	/** un cliente de la sesion. */
	CLIENT
};
/**
* @brief Enumerado de los posibles estados de la conexsion.
*/
enum class NetworkState : uint8_t
{
	/** Sin sesion. */
	IDLE,
	/** Host esperando cliente/cliente esperando respuesta. */
	WAITING,
	/** Conexion establecida tras handshake completo. */
	CONNECTED,
	/** Conexion fallida por timeout o error. */
	FAILED
};

// ID para los observadores.
using NetworkObserverID = uint32_t;

// Funcion asocidada a un paquete. Con el tipo dado por el desarroolador, la informacion y como de grande es.
using PacketCallback = std::function<void(uint8_t type, const void* data, int size)>;
