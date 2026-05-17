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

bool Skydome::init(const Properties& p)
{
	_enabledOnStart = getProperty<bool>(p, "enabled on start");
	_textureName = getProperty<std::string>(p, "texture");

	_curvature = getProperty<float>(p, "curvature");
	_tiling = getProperty<float>(p, "tiling");
	_distance = getProperty<float>(p, "distance");
	_drawFirst = getProperty<bool>(p, "draw first");
	return true;
}

void Skydome::ready()
{
	if (_enabledOnStart)
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
	std::string skydomePath = resources()->getAssetPath(_textureName);
	if (skydomePath.empty()) {
		Debug::error("[Skydome] Textura no encontrada: ", _textureName);
		return;
	}

	std::string textureFolder = std::filesystem::path(skydomePath).parent_path().string() + "/";
	std::string textureFilename = std::filesystem::path(skydomePath).filename().string();

	render()->setSkydome(textureFolder, textureFilename, _curvature, _tiling, _distance, _drawFirst);
}

void Skydome::setSkydomeNull()
{
	render()->setSkydomeNull();
}