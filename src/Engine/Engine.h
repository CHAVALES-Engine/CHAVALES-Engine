#pragma once
#include <functional>
#include <memory>
#include <string>
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */

class PlatformModule;
class RenderModule;
class AudioModule;
class PhysicsModule;

namespace core
{
	class Scene;
}

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
	* @brief Metodo que sincroniza los modulos con el juego
	*/
	const bool syncronize();
	/**
	 *
	 */
	const void addAndSetScene(std::string n, std::shared_ptr<core::Scene> s) const;
	const void setAddAndSetScene(std::function<void(std::string, std::shared_ptr<core::Scene>)> func);
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
	*	Referencia al modulo de render
	*/
	RenderModule* _renderModule = nullptr;
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
	std::function<void(std::string, std::shared_ptr<core::Scene>)> _addAndSetScene; 
};