/*
 * @file main.cpp
 * @brief Inicializacion.
 */
#include <iostream>

#include <Engine.h>
#include <Debug.h>
#include <filesystem>

#include "GameConfigurator.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "checkML.h" // es importante que vaya despues de todo porque redefine new y no queremos que lo haga tmb en las dependencias

static void launchEditorScriptsOnly()
{
#ifdef _WIN32
#ifdef _DEBUG
	const wchar_t* editor_cmd = L"ChavalesEditor_d.exe -scriptsOnly";
#else
	const wchar_t* editor_cmd = L"ChavalesEditor_r.exe -scriptsOnly";
#endif

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	std::wstring cmd(editor_cmd);

	if (CreateProcess(NULL, const_cast<LPWSTR>(cmd.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		Debug::out("[MAIN] Editor lanzado en modo solo scripts.");
	}
	else {
		Debug::warning("[MAIN] No se pudo lanzar el editor de scripts.");
	}
#endif
}

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
		catch (...)
		{
			Debug::warning("[MAIN] Error desconocido leyendo argumentos. Se cargara el archivo de configuracion.");
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

	bool editorConnected = false;
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "-editorConnected") == 0) {
			editorConnected = true;
			break;
		}
	}

	if (!editorConnected) {
		launchEditorScriptsOnly();
	}
	
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