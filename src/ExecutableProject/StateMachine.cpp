#include "StateMachine.h"

#include <chrono>
#include <stdexcept>

#include "Timing.h"
#include <Scene.h>
#include <Engine.h>
#include <iostream>

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
	auto startTime = core::Timing::getNow();

	while (!_endGame) // bucle de juego
	{
		_deltaTime = core::Timing::calculateDeltaTime(startTime);

		core::Timing::setDeltaTime(_deltaTime); // para acceso general

		if (_deltaTime >= core::Timing::FRAME_RATE)
		{
			_currentScene.ptr->fixedUpdate();
			startTime = core::Timing::getNow();
		}

		_currentScene.ptr->update(_deltaTime);
		_currentScene.ptr->render();
	}

	// borrar todas las escenas con la finalizacion del juego
	// llamar al onDestroy de todas las escenas
	// vaciar los mapas
	_stateMachine.clear();
	_nameToID.clear();
}

void StateMachine::addScene(sceneName n, scenePtr s)
{
	// comprobar que no haya una escena con ese nombre ya
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		uint64_t id = _getNextId(); // genera id
		_nameToID.insert({ n, id }); // guarda la escena en el mapa de nombres e id
		_stateMachine.insert({ id, s }); // guarda la escena con id y puntero en la maquina de estados

		s->init();
	}
	else
	{
		std::cout << "Ya hay una escena con el nombre " << n << '\n';
	}
}

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
				// desactiva la escena actual
				_currentScene.ptr->onDisable();

				// setea la escena actual a la escena s
				_currentScene.id = itS->first;
				_currentScene.ptr = itS->second;
				_currentScene.name = n;

				// activa la nueva escena actual
				_currentScene.ptr->onEnable();
			}
		}
	}
}

void StateMachine::addAndSetScene(sceneName n, scenePtr s)
{
	// comprobar que no haya una escena con ese nombre ya
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		uint64_t id = _getNextId(); // genera id
		_nameToID.insert({ n, id }); // guarda la escena en el mapa de nombres e id
		_stateMachine.insert({ id, s }); // guarda la escena con id y puntero en la maquina de estados

		// desactiva la escena actual
		_currentScene.ptr->onDisable();

		_currentScene.id = id;
		_currentScene.ptr = s;
		_currentScene.name = n;

		// activa la nueva escena actual
		_currentScene.ptr->init();
		_currentScene.ptr->onEnable();
	}
	else
	{
		std::cout << "Ya hay una escena con el nombre " << n << '\n';
	}
}

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
}

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
}