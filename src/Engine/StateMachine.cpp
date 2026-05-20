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
#include "TimerManager.h"
#include "checkMLNew.h"
#include "MessagesManager.h"
#include "ScriptsManager.h"

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
	uint64_t accumulator = 0; 
	_isLoopRunning = true;

	while (!_endGame) // bucle de juego
	{
		_processSceneChange();
		if (_currentScene.ptr != nullptr)
		{
			auto now = core::Clock::getNow();
			_deltaTime = core::Clock::calculateDeltaTime(startTime, now);
			startTime = now;

			core::Clock::setDeltaTime(_deltaTime); // para acceso general

			accumulator += _deltaTime;
			int fixedSteps = 0;
			while (accumulator >= core::Clock::FRAME_RATE 
				&& fixedSteps < core::Clock::MAX_FIXED_STEPS)
			{
				_currentScene.ptr->fixedUpdate();
				Engine::instance()->fixedUpdate(core::Clock::FRAME_RATE);
				accumulator -= core::Clock::FRAME_RATE;
				++fixedSteps;
			}

			_currentScene.ptr->update(_deltaTime);
			_currentScene.ptr->lateUpdate(_deltaTime);
			//Si no se puede renderizar se sale del juego
			if (!Engine::instance()->renderFrame())
			{
				_endGame = true;
			}
			// Gestion de creacion y eliminado de entidades en runtime
			_currentScene.ptr->addListedEntities();
			_currentScene.ptr->destroyDeadEntities();
		}
		_processHotLuaReload();
		_endGame = _endGame || Engine::instance()->update(_deltaTime) ;
	}

	_isLoopRunning = false;
	core::MessagesManager::instance().shutdown();
	ScriptsManager::instance().shutdown();

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
	//std::vector<core::Entity*> persistentEntities;

	if (_currentScene.ptr != nullptr) // Esto NO deberia ir antes de saber si se ha cargado la escena o no pero como muchos inits() de componentes que se hacen en loadScene() dependen de IDs que luego se borran en el clearScene() peta -> UIPanel.init() hace addUIPanel() y guarda _panelID despues destruye la escena vieja y llama a cleanScene(), se pierde la referencia y peta
	{
		core::MessagesManager::instance().clearNonPersistants();
		_currentScene.ptr->clearScene();
		Engine::instance()->cleanScene();
	}
	else
	{
		_currentScene.ptr = std::make_shared<core::Scene>(n);
	}

	// cargar nueva escena
	GameLoader::loadScene(n, _currentScene.ptr);

	if (_currentScene.ptr != nullptr) // si se ha cargado correctamente
	{
		Debug::out("STATEMACHINE: Entrando a escena ", n);

		// anade las entidades que sean persistentes de la escena anterior saltandose sus readys
		_currentScene.ptr->addListedEntities();

		// --- a este nivel se llama al ready:
		// garantizamos que en el ready el resto de entidades y sus componentes estan inicializados 
		_currentScene.ptr->awake();
		_currentScene.ptr->ready();

		// setea nueva escena actual
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
	if (!_currentScene.ptr) Engine::instance()->quitGame();
}

void StateMachine::_processSceneChange()
{
	if (!_hasPendingSceneChange) return;

	sceneName nextScene = _pendingSceneName; // guardar antes de clar
	_hasPendingSceneChange = false;
	_pendingSceneName.clear();
	_addAndSetScene(nextScene);
	if (!_currentScene.ptr) Engine::instance()->quitGame();
}

void StateMachine::_processHotLuaReload()
{
	if (GameLoader::reloadLua() || ComponentDLLLoader::instance().checkReload()) // si es necesario recargar...
	{
		Debug::warning("Reloading scene [", _currentScene.name, "]");

		_addAndSetScene(_currentScene.name);

		if (!_currentScene.ptr)
		{
			Debug::warning("[HOT RELOADING] No se recargó bien la escena.");
			Engine::instance()->cleanScene();
			Engine::instance()->renderFrame();
		}
	}
}
