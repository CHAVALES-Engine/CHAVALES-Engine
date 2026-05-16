/*
 * @file main.cpp
 * @brief Inicializacion.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "GameConfigurator.h"
#include "checkML.h" // es importante que vaya despues de todo porque redefine new y no queremos que lo haga tmb en las dependencias

static void configGame(size_t argc, char* argv[])
{
	if (argc > 11 && strcmp(argv[2], "NO") == 0) // no usar configuracion guardada, carga lo del editor
	{
		try
		{
			std::string windowName = argv[5];
			std::replace(windowName.begin(), windowName.end(), '_', ' ');
			const float clearR = std::stof(argv[7]);
			const float clearG = std::stof(argv[8]);
			const float clearB = std::stof(argv[9]);
			const int windowWidth = std::stoi(argv[10]);
			const int windowHeight = std::stoi(argv[11]);

			auto& cfg = core::GameConfigurator::instance();
			cfg._firstScene = argv[3];
			cfg._gameDLL = argv[4];
			cfg._windowName = windowName;
			cfg._iconRoot = argv[6];
			cfg._clearColor = { clearR, clearG, clearB, 1.0f };
			cfg._windowWidth = windowWidth;
			cfg._windowHeight = windowHeight;

			Debug::out("[MAIN] Escena inicial ", cfg._firstScene);
			Debug::out("[MAIN] Nombre de la DLL ", cfg._gameDLL);
			Debug::out("[MAIN] Nombre de la ventana ", cfg._windowName);
			Debug::out("[MAIN] Ruta del icono ", cfg._iconRoot);
			Debug::out("[MAIN] Clear color ", cfg._clearColor);
			Debug::out("[MAIN] Ancho ", cfg._windowWidth);
			Debug::out("[MAIN] Alto ", cfg._windowHeight);
		}
		catch (const std::exception& e)
		{
			Debug::warning("[MAIN] Argumentos invalidos (", e.what(), "). Se cargara el archivo de configuracion.");
			core::GameConfigurator::instance().LoadFromFile(CONFIGURATOR_PATH);
		}
		return;
	}

	// si no estas usando los datos del editor carga el toml
	Debug::warning("[MAIN] Argumentos insuficientes para configuracion por linea de comandos. Se cargara el archivo de configuracion.");
	core::GameConfigurator::instance().LoadFromFile(CONFIGURATOR_PATH);
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc();
#endif
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