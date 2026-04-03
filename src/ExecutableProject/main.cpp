/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "GameConfigurator.h"

static void configureGame(size_t argc, char* argv[])
{
	if (argc <= 2)
		return;

	std::cout << argv[2] << std::endl;

	if (strcmp(argv[2], "NO") == 0) // no usar configuracion guardada
	{
		/*
		argv[2] -> usar(1)/no usar(0) toml
		argv[3] -> primera escena
		argv[4] -> .dll
		argv[5] -> nombre ventana
		argv[6] -> icono
		argv[7] -> clear color r
		argv[8] -> clear color g
		argv[9] -> clear color b
		argv[10] -> ancho
		argv[11] -> alto
		*/

		core::GameConfigurator::_firstScene = argv[3];
		Debug::out("[MAIN] Escena inicial ", core::GameConfigurator::_firstScene);

		core::GameConfigurator::_gameDLL = argv[4];
		Debug::out("[MAIN] Nombre de la DLL ", core::GameConfigurator::_gameDLL);

		core::GameConfigurator::_windowName = argv[5];
		std::replace(core::GameConfigurator::_windowName.begin(), core::GameConfigurator::_windowName.end(), '_', ' ');
		Debug::out("[MAIN] Nombre de la ventana ", core::GameConfigurator::_windowName);

		core::GameConfigurator::_iconRoot = argv[6];
		Debug::out("[MAIN] Ruta del icono ", core::GameConfigurator::_iconRoot);

		core::GameConfigurator::_clearColor = { std::stof(argv[7]) , std::stof(argv[8]) , std::stof(argv[9]) , 1.0f};
		Debug::out("[MAIN] Clear color ", 
			core::GameConfigurator::_clearColor.getRed(), " ",
			core::GameConfigurator::_clearColor.getGreen(), " ",
			core::GameConfigurator::_clearColor.getBlue());

		core::GameConfigurator::_windowWidth = std::stoi(argv[10]);
		Debug::out("[MAIN] Ancho " , core::GameConfigurator::_windowWidth);

		core::GameConfigurator::_windowHeight = std::stoi(argv[11]);
		Debug::out("[MAIN] Alto " , core::GameConfigurator::_windowHeight);
	}
	else
	{
		core::GameConfigurator::LoadFromFile(CONFIGURATOR_PATH);
	}
}

int main(int argc, char* argv[])
{
	Debug::out("[MAIN] Inicializando ChavalesEngine");

	// Inicializa configuracion
	configureGame(argc, argv);

	// Inicializa el Engine
	if (!Engine::init()) 
	{
		Engine::release();
		return 1;
	}
	
	try
	{
		// Lanza el bucle de juego
		Engine::instance()->startLoop();
	}
	catch (exception e) 
	{
		Debug::error(e.what());
		return 1;
	}

	return 0;
}