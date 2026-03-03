/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "StateMachine.h"
#include "DLLLoader.h"
#include "Scene.h"
#include "registComponents.h"

using namespace core;

int main(int argc, char* argv[])
{
	// Abre archivo .log
	Debug::open();

	// Inicializa el Engine
	Engine::init();

	// Funcion estatica que autoregistra componentes
	registBaseComponent();

	DLLLoader dllLoader;
	dllLoader.load("./DLL-Test.dll");

	StateMachine _stateMachine;
	std::shared_ptr<Scene> scene = std::make_shared<Scene>("scene_prueba");
	_stateMachine.addAndSetScene(scene->getName(), scene);
	_stateMachine.gameLoop();

	// Cierra archivo .log
	Debug::close();
	return 0;
}
