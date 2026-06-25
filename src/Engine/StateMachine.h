/**
 * @file
 * @brief Maquina de estados de la aplicacion, gestiona la escena actual y el bucle principal.
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

	void endGame()
	{
		_endGame = true;
	}

	/**
	 * @brief Metodo para pedir un cambio de escena
	 * @param sn - nombre de la escena
	 */
	void requestSceneChange(const sceneName& sn, const bool& loadingScreen = false);

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
	 * @brief Inserta una escena nueva en la maquina de estados y la setea como escena activa actualmente.
	 * @param n - Nombre de la escena a insertar.
	 */
	void _addAndSetScene(const sceneName& n, const bool& loadingScreen = false);

	/**
	 * @brief Procesa peticiones de cambio de escena si las ha habido.
	 */
	void _processSceneChange();

#ifdef _DEBUG
	void _processHotLuaReload();
#endif

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

	/**
	 * @brief Si el bucle principal ya esta en ejecucion.
	 */
	bool _isLoopRunning = false;

	/**
	 * @brief Si se esta ejecutando una transicion de escena.
	 */
	bool _isPerformingSceneChange = false;

	/**
	 * @brief Si hay un cambio de escena pendiente de aplicar.
	 */
	bool _hasPendingSceneChange = false;

	/**
	 * @brief Si hay que cargar siguiente escena con pantalla de carga
	 */
	bool _requestedLoadingScreen = false;

	/**
	 * @brief Nombre de la siguiente escena solicitada.
	 */
	sceneName _pendingSceneName;
};
