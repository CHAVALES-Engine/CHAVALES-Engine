#pragma once
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */
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
	*	Devuelve el tiempo en segundos entre renderizado de frames
	* @return UINT64 - tiempo en segundos entre renderizado de frames
	*/
	UINT64 getDeltatime();
	/*
	* @brief
	*	Devuelve el tiempo en segundos desde que se inicializo el motor
	* @return UINT64 - tiempo en segundos desde que se inicializo el motor
	*/
	UINT64 getSecSinceStart();

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

};
