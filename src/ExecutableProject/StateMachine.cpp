#include "StateMachine.h"

#include <chrono>
#include <stdexcept>

#include "Timing.h"
#include <Scene.h>
#include <Engine.h>

StateMachine::StateMachine() :
	_endGame(false)
{

}

StateMachine::~StateMachine()
{
	
}

void StateMachine::gameLoop()
{
	auto startTime = std::chrono::high_resolution_clock::now();

	while (!_endGame) // bucle de juego
	{
		_deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - startTime)).count();

		Engine::instance()->setDeltaTime(_deltaTime); // para acceso general desde Timing

		if (_deltaTime >= Timing::FRAME_RATE)
		{
			_currentScene.second->fixedUpdate();
			startTime = std::chrono::high_resolution_clock::now();
		}

		_currentScene.second->update();
		_currentScene.second->render();
	}
}

void StateMachine::addScene(sceneID i, scenePtr s)
{
	_stateMachine.insert({i, s});
}

void StateMachine::setScene(sceneID s)
{
	auto itS = _stateMachine.find(s);
	if (itS == _stateMachine.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena en el mapa");
	}

	_currentScene = { itS->first, itS->second };
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

uint64_t StateMachine::parseNameToID(std::string n)
{
	auto itN = _nameToID.find(n);
	if (itN == _nameToID.end())
	{
		// gestion de errores etc...
		throw std::domain_error("No existe escena en el mapa");
	}

	return itN->second;
}