#include "ResourcesModule.h"
#include "Debug.h"
#include "GameConfigurator.h"
#include <filesystem>
#include "checkMLNew.h"

ResourcesModule::~ResourcesModule()
{

}

bool ResourcesModule::_loadAsset(const std::string& sourceName)
{
	for (const auto& entry : std::filesystem::directory_iterator(sourceName))
	{
		if (entry.is_directory())
		{
			std::string previousType = typeOfFolder;

			typeOfFolder = entry.path().filename().string();
			if (!_loadAsset(sourceName + typeOfFolder + "/")) {
				return false;
			}
			typeOfFolder = previousType;
		}
		else
		{
			if (!_insertAssetMap(entry.path().string())) {
				return false;
			}
		}
	}
	typeOfFolder = "";
	return true;
}

bool ResourcesModule::_insertAssetMap(const std::string& sourceName)
{
	std::string nombreAsset = std::filesystem::path(sourceName).filename().string();
	std::string nombreCarpeta = std::filesystem::path(sourceName).parent_path().string();

	ChavalesGUID aux = ChavalesGUID::generate();
	_idMaps[aux] = nombreCarpeta + "/";

	if (typeOfFolder == "fonts") {
		_fontsVector.push_back({ typeOfFolder + "/" + nombreAsset,sourceName });
	}

	bool mayus = false;
	if (std::isupper(nombreAsset[0])) {
		mayus = true;
	}
	nombreAsset[0] = std::tolower(nombreAsset[0]);

	auto it = _assetsMaps.find(typeOfFolder + "/" + nombreAsset);
	if (it != _assetsMaps.end()) {
		Debug::error("[ResourcesModule] Asset con NOMBRE EXISTENTE");
		return false;
	}


	_assetsMaps.insert({ typeOfFolder + "/" + nombreAsset,{aux,mayus} });
	return true;
}

bool ResourcesModule::_isMeshFile(const std::string& path) const
{
	std::string ext = std::filesystem::path(path).extension().string();
	// Convertir a minusculas para comparar
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	return ext == ".mesh" || ext == ".fbx";
}

bool ResourcesModule::_isTextureFile(const std::string& path) const
{
	std::string ext = std::filesystem::path(path).extension().string();
	// Convertir a minusculas para comparar
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	return ext == ".png" || ext == ".jpg" || ext == ".jpeg";
}

bool ResourcesModule::Init()
{
	if (!_loadAsset(core::GameConfigurator::instance()._assetsRoot)) {
		return false;
	}
	return true;
}

bool ResourcesModule::preloadAllAssets()
{

}

std::pair<std::string, std::string> ResourcesModule::getAssetSourceFolder(const std::string& assetName)
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
		Debug::error("[ResourcesModule] Name of the asset NOT FOUND, searched name: ", assetName);
		return { "", "" };
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

std::vector<std::pair<std::string, std::string>> ResourcesModule::getAllAssets()
{
	std::vector<std::pair<std::string, std::string>> auxiliar;
	for (auto i = _assetsMaps.begin(); i != _assetsMaps.end(); ++i) {

		auto id = _idMaps.find(i->second._id);
		std::string ruta = id->second;
		auxiliar.push_back({ i->first,ruta });
	}
	return auxiliar;
}

void ResourcesModule::addFactory(core::Resource::Type type,
	std::function<core::ResourcePtr(const std::string&, const std::string&)> fact)
{
	_factories[type] = fact;
}

bool ResourcesModule::preload(const std::string& path)
{
	std::filesystem::path p(path);
	if (p.empty()) return false;
	// Conseguir la id.
	std::string id = p.parent_path().filename().string() + "/" + p.filename().string();
	// Comprueba que no este ya cargado.
	if (_resources.find(id) != _resources.end()) return false;
	// Crea el recurso dependiendo del archivo.
	if (_isMeshFile(path))
		_resources[id] = _factories[core::Resource::Type::MESH](id, path);
	// resto de tipos
	return true;
}

