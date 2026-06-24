#include "LoadingScreen.h"
#include "checkMLNew.h"
#include "Engine.h"
#include "RenderModule.h"
#include "ResourcesModule.h"
#include "PluginSDK.h"

REGISTER_COMPONENT(LoadingScreen);

bool LoadingScreen::init(const Properties& p)
{
	std::string bgImage, fontName;
	core::Color barColor;
	if (!setProperty(p, "bgImage", bgImage)) return false;
	if (!setProperty(p, "barColor", barColor)) return false;
	if (!setProperty(p, "fontName", fontName)) return false;

	core::ResourcePtr res = resources()->getOrLoadAsset(bgImage);
	if (!res || !res->isValid()) {
		Debug::error("[UITextureRect] Textura no encontrada: ", bgImage);
		bgImage = UINT64_MAX;
		return false;
	}

	render()->createLoadingScreenScene(res->getPath(), res->getName(), barColor, fontName);

	return true;
}