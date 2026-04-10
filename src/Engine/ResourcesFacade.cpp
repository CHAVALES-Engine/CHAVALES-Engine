#include "ResourcesFacade.h"
#include <ResourcesModule.h>

ResourcesFacade::ResourcesFacade(ResourcesModule* rM): _resourcesModule(rM)
{
}

std::string ResourcesFacade::getAudioByName(const std::string& name)
{
	return _resourcesModule->getAudio(name);
}

std::pair<std::string, std::string> ResourcesFacade::getModelByName(const std::string& name)
{
	return _resourcesModule->getMesh(name);
}

std::pair<std::string, std::string> ResourcesFacade::getTextureByName(const std::string& name)
{
	return _resourcesModule->getTexture(name);
}

std::pair<std::string, std::string> ResourcesFacade::getParticleByName(const std::string& name)
{
	return _resourcesModule->getParticle(name);
}

std::pair<std::string, std::string> ResourcesFacade::getImageByName(const std::string& name)
{
	return _resourcesModule->getImages(name);
}

std::pair<std::string, std::string> ResourcesFacade::getFontByName(const std::string& name)
{
	return _resourcesModule->getFonts(name);
}

std::vector<std::pair<std::string, std::string>> ResourcesFacade::getAllFonts()
{
	return _resourcesModule->getAllFonts();
}
