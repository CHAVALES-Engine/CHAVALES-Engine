/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "StateMachine.h"
#include "ComponentDLLLoader.h"
#include "GameLoader.h"
#include "Scene.h"

using namespace core;

int main(int argc, char* argv[])
{
	// Abre archivo .log
	Debug::open();

	// Inicializa el Engine
	Engine::init();


	ComponentDLLLoader dllLoader;
#if _DEBUG
	dllLoader.load("./ComponentsProject_d.dll");
#else 
	dllLoader.load("./ComponentsProject_r.dll");
#endif

	dllLoader.load("./game/DLL-Test.dll");

	StateMachine _stateMachine;

	_stateMachine.addAndSetScene("scene1");
	_stateMachine.gameLoop();

	// Cierra archivo .log
	Debug::close();
	return 0;
}
