/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "checkML.h"
#include "StateMachine.h"
#include "DLLLoader.h"
#include "GameLoader.h"
#include "Scene.h"

using namespace core;
#ifndef IS_BEING_INCLUDED
int main(int argc, char* argv[]) {
	// Abre archivo .log
	Debug::open();
	Engine::init();

	// Ejemplos de debug:
	//Debug::out(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de mensaje normal con un int: ", 23, "\nY una salto de linea.");
	//Debug::out(Debug::DebugMode::DEBUG_BOTH, "Otro ejemplo");
	//Debug::error(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de error.");
	//Debug::warning(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de Warning");

	DLLLoader dllLoader;
	dllLoader.load("./DLL-Test.dll");

	StateMachine _stateMachine;
	_stateMachine.gameLoop();

	std::shared_ptr<Scene> scene = std::make_shared<Scene>("ScenePrueba");
	_stateMachine.addAndSetScene(scene->getName(), scene);

	// Cierra archivo .log
	Debug::close();
	return 0;
}
#endif