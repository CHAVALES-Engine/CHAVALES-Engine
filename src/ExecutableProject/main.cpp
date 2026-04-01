/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "GameConfigurator.h"


int main(int argc, char* argv[])
{
	
	// Inicializa el Engine
	Engine::init();
	
	// Inicializa configuracion
	if (argc > 1){
		core::GameConfigurator::_firstScene = argv[2];
		Debug::out("[MAIN] Escena inicial setteada a " + core::GameConfigurator::_firstScene);
		core::GameConfigurator::_scenesRoot = argv[3];
		Debug::out("[MAIN] Ruta de escenas setteada a " + core::GameConfigurator::_scenesRoot);
	}
	else
		core::GameConfigurator::LoadFromFile(CONFIGURATOR_PATH);
	
	Debug::out("[MAIN] Inicializando ChavalesEngine");

	Debug::out(core::GameConfigurator::_scenesRoot);

	// Lanza el bucle de juego
	Engine::instance()->startLoop();
	// Cierre limpio del engine
	Engine::release();

	return 0;
}