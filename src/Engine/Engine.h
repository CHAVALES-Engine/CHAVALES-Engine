#pragma once
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */

#include <cstdint>

class PlatformModule;
class AudioModule;
class PhysicsModule;

class Engine
{
public:
	/*
	* @brief
	*	Inicializacion de modulos
	* @return bool - True si se ha inicializado correctamente
	*/
	static bool init();
	/*
	* @brief Devuelve la instancia del motor
	* @return Engine* - Instancia del motor
	*/
	static Engine* instance();
	/*
	* @brief Cierre limpio de los modulos del motor
	*/
	static void release();
	/*
	* Idealmente este metodo sera eliminado
	*/
	static PlatformModule* getPlatform();
	/*
	* @brief
	*	Actualiza deltaTime.
	*/
	void setDeltaTime(uint64_t dT);
	/*
	* @brief
	*	Devuelve el tiempo en segundos desde que se inicializo el motor
	* @return uint64_t - tiempo en segundos desde que se inicializo el motor
	*/
	uint64_t getSecSinceStart() const;

	// -- Control de tiempo
	/*
	* @brief
	*	Tiempo desde la ultima actualizacion.
	*/
	static uint64_t deltaTime;

	/*
	* @brief
	*	Tasa de frames por ms.
	*/
	static const uint64_t FRAME_RATE = 1000 / 60; // 1000 ms / 60 frames

private:
	/*
	* @brief
	*	Inicializador privado de modulos
	*/
	bool _initPriv();
	/*
	* @brief
	*	Instancia estatica de la clase
	*/
	static Engine* _instance;
	/*
	* @brief
	*	Referencia al modulo de platform
	*/
	PlatformModule* _platformModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de audio
	*/
	AudioModule* _audioModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de fisica
	*/
	PhysicsModule* _physicsModule = nullptr;
};