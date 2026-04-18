#include "ResourcesModule.h"
#include "Debug.h"
#include "GameConfigurator.h"
#include <filesystem>
#include "checkMLNew.h"

ResourcesModule::ResourcesModule()
{
}

ResourcesModule::~ResourcesModule()
{

}

bool ResourcesModule::loadAsset(std::string sourceName)
{
	if (!std::filesystem::is_directory(sourceName) && std::filesystem::is_regular_file(sourceName)) insertAssetMap(sourceName);
	if (!std::filesystem::is_directory(sourceName) && !std::filesystem::is_regular_file(sourceName)) return false;
	for (const auto& entry : std::filesystem::directory_iterator(sourceName)) 
	{
		if (entry.is_directory()) 
		{
			typeOfFolder = entry.path().filename().string();
			loadAsset(sourceName + typeOfFolder + "/");
		}
		else 
		{
			if (!insertAssetMap(entry.path().string())) {
				return false;
			}
		}
	}
	typeOfFolder = "";
	return true;
}

bool ResourcesModule::insertAssetMap(std::string sourceName)
{
	std::string nombreAsset = std::filesystem::path(sourceName).filename().string();
	std::string nombreCarpeta = std::filesystem::path(sourceName).parent_path().string();

	ChavalesGUID aux = ChavalesGUID::generate();
	_idMaps[aux] = nombreCarpeta + "/";

	auto it = _assetsMaps.find(typeOfFolder + "/" + sourceName);
	if (it != _assetsMaps.end()) {
		Debug::error("ERROR: asset con NOMBRE EXISTENTE");
		return false;
	}

	if (typeOfFolder == "fonts") {
		_fontsVector.push_back({ nombreAsset,sourceName });;
	}
	_assetsMaps.insert({ typeOfFolder + "/" + nombreAsset,aux});
	return true;
} 

bool ResourcesModule::Init()
{
	if (!loadAsset(core::GameConfigurator::instance()._assetsRoot)) {
		return false;
	}
	return true;
}

std::pair<std::string, std::string> ResourcesModule::getAssetSourceFolder(std::string assetName)
{
	auto it = _assetsMaps.find(assetName);
	if (it == _assetsMaps.end())
	{
		Debug::error("ERROR: Name of the asset NOT FOUND, searched name:",assetName);
		return { "", ""};
	}
	ChavalesGUID id = it->second;
	std::string realName = std::filesystem::path(it->first).filename().string(); 
	return  { realName,_idMaps[id] };
}

std::vector<std::pair<std::string, std::string>> ResourcesModule::getAllFonts()
{
	return _fontsVector;
}

