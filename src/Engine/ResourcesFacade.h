#pragma once
#include <string>
#include <vector>
#include "EngineAPI.h"

class Engine;
class ResourcesModule;

class ENGINE_API ResourcesFacade
{
private:
	ResourcesFacade(ResourcesModule* rM);
public:

	std::string getAudioByName(const std::string& name);

	std::pair<std::string, std::string> getModelByName(const std::string& name);
	std::pair<std::string, std::string> getTextureByName(const std::string& name);
	std::pair<std::string, std::string> getParticleByName(const std::string& name);
	std::pair<std::string, std::string> getImageByName(const std::string& name);
	std::pair<std::string, std::string> getFontByName(const std::string& name);

	std::vector<std::pair<std::string, std::string>> getAllFonts();
private:

	friend Engine;

	ResourcesModule* _resourcesModule = nullptr;
};

