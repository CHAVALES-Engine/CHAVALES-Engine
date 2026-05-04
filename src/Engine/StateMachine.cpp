#include "StateMachine.h"

#include <chrono>
#include <stdexcept>

#include "Clock.h"
#include <Scene.h>
#include <Engine.h>
#include <iostream>

#include "ComponentDLLLoader.h"
#include "ComponentRegister.h"
#include "Debug.h"
#include "GameLoader.h"
#include "TimeManager.h"
#include "checkMLNew.h"

class GameLoader;

StateMachine::StateMachine() :
	_endGame(false)
{
	_deltaTime = 0;
}

StateMachine::~StateMachine()
{
	_currentScene.ptr = nullptr;
}

void StateMachine::gameLoop()
{
	auto startTime = core::Clock::getNow();
	_isLoopRunning = true;

	while (!_endGame) // bucle de juego
	{
		_processSceneChange();
		_endGame = Engine::instance()->pollEvents();
		core::TimerManager::instance().update();

		if (_currentScene.ptr != nullptr)
		{
			_deltaTime = core::Clock::calculateDeltaTime(startTime);

			core::Clock::setDeltaTime(_deltaTime); // para acceso general

			if (_deltaTime >= core::Clock::FRAME_RATE)
			{
				_currentScene.ptr->fixedUpdate();
				startTime = core::Clock::getNow();
			}

			_currentScene.ptr->update(_deltaTime);
			_currentScene.ptr->lateUpdate(_deltaTime);
			Engine::instance()->update(_deltaTime);
			Engine::instance()->renderFrame();
			// Gestion de creacion y eliminado de entidades en runtime
			_currentScene.ptr->addListedEntities();
			_currentScene.ptr->destroyDeadEntities();
			_processHotLuaReload();
		}
	}

	_isLoopRunning = false;

	// llamar a la destructora de la escena
	if (_currentScene.ptr != nullptr)
	{
		_currentScene.ptr->destroy();

		Engine::instance()->cleanScene();  // limpia la escena
		Engine::instance()->renderFrame(); // renderiza frame vacío
	}
	_currentScene.ptr = nullptr;
}

void StateMachine::_addAndSetScene(const sceneName& n)
{
	_isPerformingSceneChange = true;
	std::vector<core::Entity*> persistentEntities;

	if (_currentScene.ptr != nullptr) // Esto NO deberia ir antes de saber si se ha cargado la escena o no pero como muchos inits() de componentes que se hacen en loadScene() dependen de IDs que luego se borran en el clearScene() peta -> UIPanel.init() hace addUIPanel() y guarda _panelID despues destruye la escena vieja y llama a cleanScene(), se pierde la referencia y peta
	{
		persistentEntities = _currentScene.ptr->getDDOLEntities();
		_currentScene.ptr->clearScene();
		Engine::instance()->cleanScene();
	}
	else
	{
		_currentScene.ptr = std::make_shared<core::Scene>("");
	}

	// cargar nueva escena
	GameLoader::loadScene(n, _currentScene.ptr);

	if (_currentScene.ptr != nullptr) // si se ha cargado correctamente
	{
		Debug::out("STATEMACHINE: Entrando a escena ", n);

		// anyade las entidades que sean persistentes de la escena anterior saltandose sus readys
		for (core::Entity* pe : persistentEntities)
			_currentScene.ptr->addEntity(pe);
		// como son dool no hace falta llamar otra vez awake y ready
		_currentScene.ptr->addListedEntities();

		// --- a este nivel se llama al ready:
		// garantizamos que en el ready el resto de entidades y sus componentes estan inicializados 
		_currentScene.ptr->awake();
		_currentScene.ptr->ready();

		// setea nueva escena actual
		_currentScene.ptr = _currentScene.ptr;
		_currentScene.name = n;
	}
	else
	{
		Debug::out("[STATEMACHINE] No se pudo cargar la escena ", n);
	}

	_isPerformingSceneChange = false;
}

void StateMachine::requestSceneChange(const sceneName& sn)
{
	if (_isLoopRunning || _isPerformingSceneChange)
	{
		_pendingSceneName = sn;
		_hasPendingSceneChange = true;
		Debug::out("STATEMACHINE: Cambio de escena encolado a ", sn);
		return;
	}

	Debug::out("STATEMACHINE: Cambio de escena a ", sn);
	_addAndSetScene(sn);
}

void StateMachine::_processSceneChange()
{
	if (!_hasPendingSceneChange) return;

	sceneName nextScene = _pendingSceneName; // guardar antes de clar
	_hasPendingSceneChange = false;
	_pendingSceneName.clear();
	_addAndSetScene(nextScene);
}

void StateMachine::_processHotLuaReload()
{
	if (GameLoader::reloadLua() || ComponentDLLLoader::instance().checkReload()) // si es necesario recargar...
	{
		Debug::warning("Reloading scene [", _currentScene.name, "]");

		//limpia logica
		//_currentScene.ptr->destroy(); // elimina escena
		//_currentScene.ptr->clearScene();
		////limpia render
		//Engine::instance()->cleanScene();  // limpia la escena
		//scenePtr s = std::move(GameLoader::loadScene(_currentScene.name)); // vuelve a cargar
		//_currentScene.ptr = s;

		//if (_currentScene.ptr != nullptr)
		//{
		//	// --- a este nivel se llama al ready:
		//	// garantizamos que en el ready el resto de entidades y sus componentes estan inicializados 
		//	_currentScene.ptr->ready();
		//}

		_addAndSetScene(_currentScene.name);
	}
}
