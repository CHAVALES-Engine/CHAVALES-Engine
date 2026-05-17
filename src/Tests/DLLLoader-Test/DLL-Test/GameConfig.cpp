
#include "GameConfigurator.h"
#include "checkMLNew.h"


extern "C" __declspec(dllexport)
void configureGame()
{
	// configuracion desde dll
	core::GameConfigurator::instance()._configType = "DLL";
	core::GameConfigurator::instance()._scenesRoot = "game/scenes/";
	core::GameConfigurator::instance()._firstScene = "initial_scene";
	core::GameConfigurator::instance()._assetsRoot = "game/assets/";
	core::GameConfigurator::instance()._gameDLL = "DLL-Test";
	core::GameConfigurator::instance()._clearColor = core::Color(0.2, 0.5, 0.75, 1.0);
	core::GameConfigurator::instance()._windowHeight = 720;
	core::GameConfigurator::instance()._windowWidth = 1280;
}