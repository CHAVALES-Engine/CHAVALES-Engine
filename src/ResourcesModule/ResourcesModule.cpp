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
		_fontsVector.push_back({ typeOfFolder + "/"+ nombreAsset,sourceName });;
	}
	bool mayus = false;
	if (std::isupper(nombreAsset[0])) {
		mayus = true;
	}
	nombreAsset[0] = std::tolower(nombreAsset[0]);
	_assetsMaps.insert({ typeOfFolder + "/" + nombreAsset,{aux,mayus} });
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
	std::string parentPath = std::filesystem::path(assetName).parent_path().string() + "/";
	std::string comprobante = std::filesystem::path(assetName).filename().string();

	std::string aux = comprobante;
	if (std::isupper(aux[0])) {
		aux[0] = std::tolower(aux[0]);
	}

	auto it = _assetsMaps.find(parentPath + aux);
	
	if (it == _assetsMaps.end())
	{
		Debug::error("ERROR: Name of the asset NOT FOUND, searched name:",assetName);
		return { "", ""};
	}

	std::string realPath = std::filesystem::path(it->first).filename().string();
	ChavalesGUID id = it->second._id; 

	if (it->second.isUpper) {
		realPath[0] = std::toupper(realPath[0]);
	}
	return  { realPath,_idMaps[id] };
}

std::vector<std::pair<std::string, std::string>> ResourcesModule::getAllFonts()
{
	return _fontsVector;
}

