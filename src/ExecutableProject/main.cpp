/*
 * @file main.cpp
 * @brief Inicializacion.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "GameConfigurator.h"

static void configGame(size_t argc, char* argv[])
{
	if (argc > 2 && strcmp(argv[2], "NO") == 0) // no usar configuracion guardada, carga lo del editor
	{
		core::GameConfigurator::instance()._firstScene = argv[3];
		Debug::out("[MAIN] Escena inicial ", core::GameConfigurator::instance()._firstScene);

		core::GameConfigurator::instance()._gameDLL = argv[4];
		Debug::out("[MAIN] Nombre de la DLL ", core::GameConfigurator::instance()._gameDLL);

		core::GameConfigurator::instance()._windowName = argv[5];
		std::replace(core::GameConfigurator::instance()._windowName.begin(), core::GameConfigurator::instance()._windowName.end(), '_', ' ');
		Debug::out("[MAIN] Nombre de la ventana ", core::GameConfigurator::instance()._windowName);

		core::GameConfigurator::instance()._iconRoot = argv[6];
		Debug::out("[MAIN] Ruta del icono ", core::GameConfigurator::instance()._iconRoot);

		core::GameConfigurator::instance()._clearColor = { std::stof(argv[7]) , std::stof(argv[8]) , std::stof(argv[9]) , 1.0f };
		Debug::out("[MAIN] Clear color ",
			core::GameConfigurator::instance()._clearColor.getRed(), " ",
			core::GameConfigurator::instance()._clearColor.getGreen(), " ",
			core::GameConfigurator::instance()._clearColor.getBlue());

		core::GameConfigurator::instance()._windowWidth = std::stoi(argv[10]);
		Debug::out("[MAIN] Ancho ", core::GameConfigurator::instance()._windowWidth);

		core::GameConfigurator::instance()._windowHeight = std::stoi(argv[11]);
		Debug::out("[MAIN] Alto ", core::GameConfigurator::instance()._windowHeight);

		return;
	}

	// si no estas usando los datos del editor carga el toml
	core::GameConfigurator::instance().LoadFromFile(CONFIGURATOR_PATH);
}

int main(int argc, char* argv[])
{
	Debug::out("[MAIN] Inicializando ChavalesEngine");

	// Inicializa configuracion
	configGame(argc, argv);

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
	catch (const std::exception& e)
	{
		Debug::error("Exception: ", e.what());
	}
	catch (...)
	{
		Debug::error("Unknown exception");
	}

	Engine::release();

	return 0;
}