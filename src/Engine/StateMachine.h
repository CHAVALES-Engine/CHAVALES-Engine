/*
 * @file
 * @brief
 */
#pragma once
#include <memory>
#include <string>

#include <EngineAPI.h>

namespace core
{
	class Scene;
	class Clock;
}

using sceneName = std::string;
using scenePtr = std::shared_ptr<core::Scene>;

struct scene 
{
	scenePtr ptr = nullptr;
	sceneName name = " ";
};

/**
* @brief StateMachine.
*
*	Maquina de estados de la aplicacion, gestiona la escena actual y el bucle principal.
*/
class ENGINE_API StateMachine
{
public:
	/**
	* @brief Constructora de una StateMachine.
	*/
	StateMachine();
	~StateMachine();

	/**
	 * @brief Bucle principal.
	 */
	void gameLoop();

	/**
	 * @brief Inserta una escena nueva en la maquina de estados y la setea como escena activa actualmente.
	 * @param n - Nombre de la escena a insertar.
	 * @param s - Puntero a la escena a insertar.
	 */
	void addAndSetScene(const sceneName& n);

	/**
	 * @brief Devuelve el nombre de la escena activa actualmente.
	 */
	inline sceneName getCurrentScnName() const {
		return _currentScene.name;
	}

	/**
	 * @brief Devuelve un puntero a la escena activa actualmente.
	 */
	inline scenePtr getCurrentScnPtr() const {
		return _currentScene.ptr;
	}

	/**
	 * @brief Devuelve la escena activa actualmente.
	 */
	inline scene getCurrentScn() const {
		return _currentScene;
	}

private:
	/**
	 * @brief Tiempo desde la ultima actualizacion.
	 */
	uint64_t _deltaTime;

	/**
	 * @brief ID y referencia de la escena activa actualmente.
	 */
	scene _currentScene;

	/**
	 * @brief Booleano de control de ejecucion del bucle de juego.
	 */
	bool _endGame = false;
};