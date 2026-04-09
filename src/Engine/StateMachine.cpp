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

class GameLoader;

uint64_t StateMachine::_nextId = -1;

StateMachine::StateMachine() :
	_endGame(false)
{
	_deltaTime = 0;
}

StateMachine::~StateMachine()
{
	
}

void StateMachine::gameLoop()
{
	auto startTime = core::Clock::getNow();

	while (!_endGame) // bucle de juego
	{
		_endGame = Engine::instance()->pollEvents();
		core::TimerManager::instance().update();

		if (_currentScene.ptr != nullptr)
		{
			if (GameLoader::reloadLua() || ComponentDLLLoader::instance().checkReload()) // si es necesario recargar...
			{
				Debug::warning("Reloading scene [", _currentScene.name, "]");
				//limpia logica
				_currentScene.ptr->clearScene(); // elimina escena anterior
				_currentScene.ptr->onDestroy();
				//limpia render
				//Engine::instance()->cleanScene();  // limpia la escena
				scenePtr s = std::move(GameLoader::loadScene(_currentScene.name)); // vuelve a cargar
				_currentScene.ptr = s;
			}

			_deltaTime = core::Clock::calculateDeltaTime(startTime);

			core::Clock::setDeltaTime(_deltaTime); // para acceso general

			if (_deltaTime >= core::Clock::FRAME_RATE)
			{
				_currentScene.ptr->fixedUpdate();
				startTime = core::Clock::getNow();
			}

			_currentScene.ptr->update(_deltaTime);
			Engine::instance()->renderFrame();
		}
	}

	// llamar a la destructora de la escena
	if (_currentScene.ptr != nullptr)
	{
		_currentScene.ptr->onDestroy();

		Engine::instance()->cleanScene();  // limpia la escena
		Engine::instance()->renderFrame(); // renderiza frame vacío
	}
	_currentScene.ptr = nullptr;
}

void StateMachine::addAndSetScene(const sceneName& n)
{
#if _DEBUG
	// cargar nueva escena
	scenePtr s = std::move(GameLoader::loadScene(n));
#else
	//Cargar escena externa (prueba)
	scenePtr s = std::move(GameLoader::loadSceneFromSearch(n));
#endif

	if (s != nullptr) // si se ha cargado correctamente
	{
		Debug::out("STATEMACHINE: Entrando a escena ", n);

		// destruye la escena actual
		if (_currentScene.ptr != nullptr)
			_currentScene.ptr->onDestroy();

		// setea nueva escena actual
		_currentScene.ptr = s;
		_currentScene.name = n;
	}
	else
	{
		Debug::out("[STATEMACHINE] No se pudo cargar la escena ", n);
	}
}