
#include "GameConfigurator.h"


extern "C" __declspec(dllexport)
void configureGame()
{
    if (!core::GameConfigurator::instance()._useTOML.empty()) return;
    // configuracion desde dll
    core::GameConfigurator::instance()._scenesRoot = "game/scenes/";
    core::GameConfigurator::instance()._firstScene = "scene_base";
    core::GameConfigurator::instance()._assetsRoot = "game/assets/";
    core::GameConfigurator::instance()._assetsList = "assetList";
    core::GameConfigurator::instance()._gameDLL = "DLL-Test";
    core::GameConfigurator::instance()._clearColor = core::Color(1.0, 0.0, 0.0, 1.0);
}