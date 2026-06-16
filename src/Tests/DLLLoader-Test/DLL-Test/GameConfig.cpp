
#include "GameConfigurator.h"
#include "checkMLNew.h"


extern "C" __declspec(dllexport)
void configureGame()
{
	// configuracion desde dll
	core::GameConfigurator::instance()._configType = "DLL";
	core::GameConfigurator::instance()._scenesRoot = "./game/scenes/";
	core::GameConfigurator::instance()._firstScene = "scene_base";
	core::GameConfigurator::instance()._assetsRoot = "./game/assets/";
	core::GameConfigurator::instance()._gameDLL = "DLL-Test";
	core::GameConfigurator::instance()._clearColor = core::Color(0.2f, 0.5f, 0.75f, 1.0f);
	core::GameConfigurator::instance()._windowHeight = 720;
	core::GameConfigurator::instance()._windowWidth = 1280;
}