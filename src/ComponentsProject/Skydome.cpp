#include "Skydome.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "RenderModule.h"
#include "ResourcesModule.h"
#include <filesystem>
#include "checkMLNew.h"

REGISTER_COMPONENT(Skydome);

Skydome::Skydome()
{
	registerMethod("setSkydome", [this](const std::vector<std::any>& args) {
		setSkydome();
		});

	registerMethod("setSkydomeNull", [this](const std::vector<std::any>& args) {
		setSkydomeNull();
		});
}

Skydome::~Skydome()
{
}

//bool Skydome::init(const Properties& p)
//{
//	enabledOnStart = getProperty<bool>(p, "enabledOnStart");
//	textureName = getProperty<std::string>(p, "textureName");
//
//	curvature = getProperty<float>(p, "curvature");
//	tiling = getProperty<float>(p, "tiling");
//	distance = getProperty<float>(p, "distance");
//	drawFirst = getProperty<bool>(p, "drawFirst");
//	return true;
//}

void Skydome::ready()
{
	if (enabledOnStart)
	{
		setSkydome();
	}
}

void Skydome::disable()
{
	setSkydomeNull();
}

void Skydome::setSkydome()
{
	core::ResourcePtr res = resources()->getOrLoadAsset(textureName);
	if (!res || !res->isValid()) {
		Debug::error("[Skydome] Textura no encontrada: ", textureName);
		return;
	}
	render()->setSkydome(res->getPath(), res->getName(), curvature, tiling, distance, drawFirst);
}

void Skydome::setSkydomeNull()
{
	render()->setSkydomeNull();
}