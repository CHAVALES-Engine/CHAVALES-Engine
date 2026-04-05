
#include "GameConfigurator.h"
#include <PluginSDK.h>


extern "C" __declspec(dllexport) 
void configureGame()
{
    core::GameConfigurator::_scenesRoot = "game/scenes/";
    core::GameConfigurator::_firstScene = "scene_base";
    core::GameConfigurator::_assetsRoot = "game/assets/";
    core::GameConfigurator::_assetsList = "assetList";
    core::GameConfigurator::_gameDLL = "DLL-Test";
    core::GameConfigurator::_clearColor = core::Color(0.5, 0.5, 0.5, 1);
}