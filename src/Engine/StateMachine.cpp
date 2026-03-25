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
	Engine::instance()->setAddAndSetScene([this](std::string n) {
			this->addAndSetScene(n);
		});
}

StateMachine::~StateMachine()
{
	
}

void StateMachine::gameLoop()
{
	auto startTime = core::Clock::getNow();

	while (!_endGame) // bucle de juego
	{
		_endGame = Engine::instance()->syncronize();
		core::TimerManager::instance().update();

		if (_currentScene.ptr != nullptr)
		{

			if (GameLoader::reloadLua() || ComponentDLLLoader::instance().checkReload()) // si es necesario recargar...
			{
				Debug::warning("Reloading scene [", _currentScene.name, "]");
				_currentScene.ptr->clearScene(); // elimina escena anterior
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
			//_currentScene.ptr->render();
		}
	}

	// llamar a la destructora de la escena
	if (_currentScene.ptr != nullptr)
	{
		_currentScene.ptr->~Scene();
	}
	_currentScene.ptr = nullptr;


	// vaciar los mapas
	//_stateMachine.clear();
	//_nameToID.clear();
}

/*
void StateMachine::addScene(sceneName n, scenePtr s)
{
	
	// comprobar que no haya una escena con ese nombre ya
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		uint64_t id = _getNextId(); // genera id
		_nameToID.insert({ n, id }); // guarda la escena en el mapa de nombres e id
		_stateMachine.insert({ id, s }); // guarda la escena con id y puntero en la maquina de estados

		//s->onCreate();
		//s->init();
	}
	else
	{
		std::cout << "Ya hay una escena con el nombre " << n << '\n';
	}
	
}*/

/*
void StateMachine::setScene(sceneName n)
{
	// busca si existe una escena con ese nombre
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		std::cout << "No existe escena con nombre " << n << '\n';
	}
	else
	{
		// busca en la stateMachine la escena con el id asignado a ese nombre
		auto itS = _stateMachine.find(itN->second);
		if (itS == _stateMachine.end())
		{
			std::cout << "No existe escena con id " << itN->second << '\n';
		}
		else
		{
			// si no era la escena activa ya
			if (_currentScene.id != itS->first)
			{

				// carga la escena 

				// desactiva la escena actual
				_currentScene.ptr->onDisable();
				//_currentScene.ptr->onDestroy();

				// setea la escena actual a la escena s
				_currentScene.id = itS->first;
				_currentScene.ptr = itS->second;
				_currentScene.name = n;

				// activa la nueva escena actual
				_currentScene.ptr->onEnable();
			}
		}
	}
}*/

void StateMachine::addAndSetScene(const sceneName& n)
{
#if _DEBUG
	// cargar nueva escena
	scenePtr s = std::move(GameLoader::loadScene(n));
#else
	//Cargar escena externa (prueba)
	scenePtr s = std::move(GameLoader::loadSceneFromSearch());
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

/*
void StateMachine::deleteScene(sceneName n)
{
	// comprobar que existe una escena con ese nombre 
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		std::cout << "No existe una escena con el nombre " << n << '\n';
	}
	else
	{
		auto itS = _stateMachine.find(itN->second);
		if (itS != _stateMachine.end())
		{
			// si quieres borrar la escena activa actualmente y aun no se ha acabado el juego
			if ((itN->second == _currentScene.id) && !_endGame)
			{
				std::cout << "No se puede eliminar la escena activa actualmente" << '\n';
			}
			else
			{
				itS->second->onDestroy();

				_stateMachine.erase(itS);
				_nameToID.erase(itN);
			}
		}
	}
	
}*/

/*
uint64_t StateMachine::_parseNameToID(std::string n)
{
	
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		std::cout << "No existe una escena con el nombre " << n << '\n';
	}
	else
	{
		return itN->second;
	}
	
}*/