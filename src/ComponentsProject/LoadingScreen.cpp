#include "LoadingScreen.h"
#include "checkMLNew.h"
#include "Engine.h"
#include "RenderModule.h"

bool LoadingScreen::init(const Properties& p)
{
	std::string bgImage, fontName;
	core::Color barColor;
	if (!setProperty(p, "bgImage", bgImage)) return false;
	if (!setProperty(p, "barColor", barColor)) return false;
	if (!setProperty(p, "fontName", fontName)) return false;

	render()->createLoadingScreenScene(bgImage, barColor, fontName);

	return true;
}