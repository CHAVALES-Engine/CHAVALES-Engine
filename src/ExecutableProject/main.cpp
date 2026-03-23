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
	// Inicializa el Engine
	Engine::init();
	// Lanza el bucle de juego
	Engine::instance()->startLoop();
	// Cierre limpio del engine
	Engine::release();
	return 0;
}
