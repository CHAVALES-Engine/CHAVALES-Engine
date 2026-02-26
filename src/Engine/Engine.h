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
	*	Devuelve actualiza el deltatime
	*/
	void setDeltaTime(uint64_t const deltaTime);
	/*
	* @brief
	*	Devuelve el tiempo en segundos entre renderizado de frames
	* @return double - tiempo en segundos entre renderizado de frames
	*/
	static const double DeltaTime();
	/*
	* @brief
	*	Devuelve el tiempo en segundos desde que se inicializo el motor
	* @return double - tiempo en segundos desde que se inicializo el motor
	*/
	const double getSecSinceStart() const;

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
	double _deltaTime = 0.0;
};
/*
* @brief
*	Atajo para el DeltaTime
*	segundos entre renderizado de frames
*/
#define DELTA_TIME Engine::DeltaTime()
/*
* @brief
*	Atajo para el tiempo total
*	DeltaTime, segundos entre renderizado de frames
*/
#define TIME_SINCE_START Engine::instance()->getSecSinceStart()
