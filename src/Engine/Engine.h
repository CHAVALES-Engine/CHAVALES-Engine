#pragma once
#include <functional>
#include <memory>
#include <string>
#include "EngineAPI.h"
#include "Vector3.h"
/*
 * @file Engine.h
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

class ENGINE_API Engine
{
public:
	~Engine();
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
	* @brief Metodo que sincroniza los modulos con el juego
	*/
	const bool syncronize();
	/**
	 *
	 */
	const void addAndSetScene(std::string n) const;
	const void setAddAndSetScene(std::function<void(std::string)> func);

	//Métodos del modulo de audio para poder ejecutarlos desde el juego
	void loadSound(const char* path, std::string id, bool sound3D = true, bool soundLooping = false, bool soundStream = false);
	void unloadSound(std::string id);
	int playSound(std::string id, const core::Vector3<> vec3 = { 0.0f,0.0f,0.0f }, float soundVolume = 0.0f, int looping = 0);
	void setChannelVolume(int chID, float newVolume = 0.0f);
	void getLooping(int chID, int* typeOfLooping);

	bool stopPlaying(int chID);

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
	std::unique_ptr<PlatformModule> _platformModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de render
	*/
	std::unique_ptr<RenderModule> _renderModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de audio
	*/
	std::unique_ptr<AudioModule> _audioModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de fisica
	*/
	std::unique_ptr<PhysicsModule> _physicsModule = nullptr;
	std::function<void(std::string)> _addAndSetScene;
};