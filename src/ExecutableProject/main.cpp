/*
 * @file main.cpp
 * @brief Inicializacion.
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

	if (strcmp(argv[2], "NO") == 0) // no usar configuracion guardada
	{
		core::GameConfigurator::_firstScene = argv[3];
		Debug::out("[MAIN] Escena inicial ", core::GameConfigurator::_firstScene);

		core::GameConfigurator::_firstScene = argv[3];
		Debug::out("[MAIN] Escena inicial ", core::GameConfigurator::_firstScene);

		core::GameConfigurator::_windowName = argv[5];
		std::replace(core::GameConfigurator::_windowName.begin(), core::GameConfigurator::_windowName.end(), '_', ' ');
		Debug::out("[MAIN] Nombre de la ventana ", core::GameConfigurator::_windowName);

		core::GameConfigurator::_windowName = argv[5];
		Debug::out("[MAIN] Nombre de la ventana ", core::GameConfigurator::_windowName);

		core::GameConfigurator::_iconRoot = argv[6];
		Debug::out("[MAIN] Ruta del icono ", core::GameConfigurator::_iconRoot);

		core::GameConfigurator::_clearColor = { std::stof(argv[7]) , std::stof(argv[8]) , std::stof(argv[9]) , 1.0f };
		Debug::out("[MAIN] Clear color ",
			core::GameConfigurator::_clearColor.getRed(), " ",
			core::GameConfigurator::_clearColor.getGreen(), " ",
			core::GameConfigurator::_clearColor.getBlue());

		core::GameConfigurator::_windowWidth = std::stoi(argv[10]);
		Debug::out("[MAIN] Ancho ", core::GameConfigurator::_windowWidth);

		core::GameConfigurator::_windowHeight = std::stoi(argv[11]);
		Debug::out("[MAIN] Alto ", core::GameConfigurator::_windowHeight);

		return;
	}

	core::GameConfigurator::LoadFromFile(CONFIGURATOR_PATH);

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
	catch (std::exception e)
	{
		Debug::error(e.what());
		return 1;
	}

	return 0;
}