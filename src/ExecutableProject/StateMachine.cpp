#include "StateMachine.h"

#include <stdexcept>

#include "Timing.h"
#include "../Core/Scene.h"

StateMachine::StateMachine()
{

}

StateMachine::~StateMachine()
{
	
}

void StateMachine::gameLoop()
{
	// getTicks de SDL
	// uint32_t startTime = deltaTime;

	// uint32_t elapsed = deltaTime + startTime;

	while (true) 
	{
		_currentScene.second->fixedUpdate();
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