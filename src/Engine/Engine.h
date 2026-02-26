#pragma once
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */

#include <cstdint>

class PlatformModule;
class AudioModule;

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
	*	Devuelve el tiempo en segundos desde que se inicializo el motor
	* @return uint64_t - tiempo en segundos desde que se inicializo el motor
	*/
	void setDeltaTime(uint64_t const deltaTime);
	/*
	* @brief
	*	Devuelve el tiempo en segundos entre renderizado de frames
	* @return uint64_t - tiempo en segundos entre renderizado de frames
	*/
	static const uint64_t DeltaTime();
	/*
	* @brief
	*	Devuelve el tiempo en segundos desde que se inicializo el motor
	* @return uint64_t - tiempo en segundos desde que se inicializo el motor
	*/
	const uint64_t getSecSinceStart() const;

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
	*	DeltaTime, segundos entre renderizado de frames
	*/
	uint64_t _deltaTime = 0.0;
};
