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
	Timing::startTime = Timing::calculateNow();

	while (!_endGame) // bucle de juego
	{
		_deltaTime = Timing::calculateDeltaTime();

		Timing::setDeltaTime(_deltaTime); // para acceso general

		if (_deltaTime >= Timing::FRAME_RATE)
		{
			_currentScene.ptr->fixedUpdate();
			Timing::startTime = Timing::calculateNow();
		}

		_currentScene.ptr->update(_deltaTime);
		_currentScene.ptr->render();
	}
}

void StateMachine::addScene(sceneName n, scenePtr s)
{
	// comprobar que no haya una escena con ese nombre ya
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		std::cout << "Ya hay una escena con el nombre " << n << '\n';
	}
	else
	{
		uint64_t id = _getNextId(); // genera id
		_nameToID.insert({n, id}); // guarda la escena en el mapa de nombres e id
		_stateMachine.insert({id, s}); // guarda la escena con id y puntero en la maquina de estados
	}
}

void StateMachine::setScene(sceneID s)
{
	auto itS = _stateMachine.find(s);
	if (itS == _stateMachine.end())
	{
		throw std::domain_error("No existe escena en el mapa");
	}
	else
	{
		_currentScene = { itS->first, itS->second };
	}
}

void StateMachine::goToScene(sceneName n)
{
	// busca si existe una escena con ese nombre
	auto itS = _nameToID.find(n);
	if (itS == _nameToID.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena con ese nombre");
	}

	auto itF = _stateMachine.find(itS->second);

	if (itS == _nameToID.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena en el mapa");
	}

	_currentScene = { itF->first, itF->second };
}

void StateMachine::deleteScene(sceneID s)
{
	auto itS = _stateMachine.find(s);
	if (itS == _stateMachine.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena en el mapa");
	}

	_stateMachine.erase(itS);
}

uint64_t StateMachine::_parseNameToID(std::string n)
{
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena en el mapa");
	}

	return itN->second;
}