#include "ResourcesModule.h"
#include "Debug.h"
#include "GameConfigurator.h"
#include <filesystem>

ResourcesModule::ResourcesModule(): _luaRoute(), _audioMap(), _modelsMap(), _texturesMap(), _particlesMap()
{
}

ResourcesModule::~ResourcesModule()
{
}

std::pair<FolderName, FileName> ResourcesModule::loadOgreAsset(const std::string& assetName,std::pair<sol::object, sol::object>& assetType)
{ 
	sol::table assetsType = assetType.second;

	std::string sourceFolder = assetsType["sourceFolder"].get<std::string>();
	std::string fileName = assetsType["fileName"].get<std::string>();
	if (std::filesystem::is_regular_file(sourceFolder + fileName) && std::filesystem::is_directory(sourceFolder)) {
		return { sourceFolder, fileName };
	}
	return {};
}

bool ResourcesModule::loadInternalAsset(const sol::table& assetsType, const std::string& typeOfAsset)
{
	Debug::out("RESOURCES: Cargando recursos de tipo ", typeOfAsset);
	for (auto& assets : assetsType) {
		std::string nameOfAsset = assets.first.as<std::string>(); 
		if (std::filesystem::is_directory(core::GameConfigurator::instance()._assetsRoot + typeOfAsset)) 
		{
			if (typeOfAsset == "audio")
			{
				std::string assetPath = assets.second.as<std::string>();
				auto it = _audioMap.find(nameOfAsset);
				if (it != _audioMap.end()) {
					Debug::error("ERROR: Audio ya existente con ese nombre");
					return false;
				}
				Debug::out("RESOURCES: Assetpath ", "./assets/" + assetPath);
				if (std::filesystem::is_regular_file(assetPath)) {
					_audioMap[nameOfAsset] = assetPath;
				}
			}
			else if (typeOfAsset == "mesh")
			{
				auto it = _modelsMap.find(nameOfAsset);
				if (it != _modelsMap.end()) {
					Debug::error("ERROR: Malla ya existente con ese nombre");
					return false;
				}
				_modelsMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
			}
			else if (typeOfAsset == "texture")
			{
				auto it = _texturesMap.find(nameOfAsset);
				if (it != _texturesMap.end()) {
					Debug::error("ERROR: Textura ya existente con ese nombre");
					return false;
				}
				_texturesMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
			}
			else if (typeOfAsset == "particles")
			{
				auto it = _particlesMap.find(nameOfAsset);
				if (it != _particlesMap.end()) {
					Debug::error("ERROR: Particula ya existente con ese nombre");
					return false;
				}
				_particlesMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
			}
			else if (typeOfAsset == "images")
			{
				auto it = _imagesMap.find(nameOfAsset);
				if (it != _imagesMap.end()) {
					Debug::error("ERROR: Imagen ya existente con ese nombre");
					return false;
				}
				_imagesMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
			}
			else if (typeOfAsset == "fonts")
			{
				auto it = _fontsMap.find(nameOfAsset);
				if (it != _fontsMap.end()) {
					Debug::error("ERROR: Fuente ya existente con ese nombre");
					return false;
				}
				_fontsMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
			}
			else {
				Debug::error("ERROR: Tipo de recurso no valido");
				return false;
			}
		}
		Debug::out("RESOURCES: ", nameOfAsset, " cargado");
	}
	return true;
}

bool ResourcesModule::Init()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	try {
		if (std::filesystem::exists(core::GameConfigurator::instance()._assetsRoot) && std::filesystem::is_directory(core::GameConfigurator::instance()._assetsRoot)) {
			std::string path = core::GameConfigurator::instance()._assetsRoot + core::GameConfigurator::instance()._assetsList + ".lua";
			if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) { 
				_luaRoute = path;
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		Debug::error("ERROR: asset folder not found");
	}	
	try
	{
		// intenta leer archivo
		lua.safe_script_file(_luaRoute);
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("RESOURCES: Error cargando assets: ", _luaRoute);
		Debug::error("Lua exception: ", e.what());
		return false;
	}

	sol::optional<sol::table> assetsFile = lua.get<sol::optional<sol::table>>("assets");
	if (!assetsFile.has_value())
	{
		Debug::error("RESOURCES: El archivo lua no contiene tabla 'assets': ", _luaRoute);
		return false;
	}
	Debug::out("RESOURCES: Cargando recursos desde ", _luaRoute);

	for (auto& assets : assetsFile.value()) 
	{
		std::string typeOfAsset = assets.first.as<std::string>();
		sol::table assetsType = assets.second;
		
		if (!loadInternalAsset(assetsType, typeOfAsset)) {
			return false;
		}
	}
	return true;
}

std::string ResourcesModule::getAudio(AssetName name)
{
	auto it = _audioMap.find(name);
	if (it == _audioMap.end()) {
		Debug::error("ERROR: Audio no encontrado");
	}
	Debug::warning("BUSCANDO AUDIO ", it->first, " ", it->second);
	return it->second;
}

std::pair<FolderName, FileName> ResourcesModule::getMesh(AssetName name)
{
	auto it = _modelsMap.find(name);
	if (it == _modelsMap.end()) {
		Debug::error("ERROR: Modelo no encontrado");
	}
	return it->second;
}

std::pair<FolderName, FileName> ResourcesModule::getParticle(AssetName name)
{
	auto it = _particlesMap.find(name);
	if (it == _particlesMap.end()) {
		Debug::error("ERROR: Particulas no encontrada");
	}
	return it->second;
}

std::pair<FolderName, FileName> ResourcesModule::getTexture(AssetName name)
{
	auto it = _texturesMap.find(name);
	if (it == _texturesMap.end()) {
		Debug::error("ERROR: Textura no encontrada");
	}
	return it->second; 
}

std::pair<FolderName, FileName> ResourcesModule::getImages(AssetName name)
{
	auto it = _imagesMap.find(name);
	if (it == _imagesMap.end()) {
		Debug::error("ERROR: Imagen no encontrada");
	}
	return it->second;
}

std::pair<FolderName, FileName> ResourcesModule::getFonts(AssetName name)
{
	auto it = _fontsMap.find(name);
	if (it == _fontsMap.end()) {
		Debug::error("ERROR: Textura no encontrada");
	}
	return it->second;
}

void ResourcesModule::setAudioSource(AssetName name, FolderName newRoute)
{
	auto it = _audioMap.find(name);
	if (it == _audioMap.end()) {
		Debug::error("ERROR: Audio no encontrado");
	}
	_audioMap[name] = newRoute;
}

void ResourcesModule::setMeshSource(AssetName name, FolderName newRoute)
{
	auto it = _modelsMap.find(name);
	if (it == _modelsMap.end()) {
		Debug::error("ERROR: Modelo no encontrado");
	}
	_modelsMap[name].first = newRoute;
}

void ResourcesModule::setParticleSource(AssetName name, FolderName newRoute)
{
	auto it = _particlesMap.find(name);
	if (it == _particlesMap.end()) {
		Debug::error("ERROR: Particulas no encontrada");
	}
	_particlesMap[name].first = newRoute;
}

void ResourcesModule::setTextureSource(AssetName name, FolderName newRoute)
{
	auto it = _texturesMap.find(name);
	if (it == _texturesMap.end()) {
		Debug::error("ERROR: Textura no encontrado");
	}
	_texturesMap[name].first = newRoute;
}

void ResourcesModule::setImageSource(AssetName name, FolderName newRoute)
{
	auto it = _imagesMap.find(name);
	if (it == _imagesMap.end()) {
		Debug::error("ERROR: Textura no encontrado");
	}
	_imagesMap[name].first = newRoute;
}

void ResourcesModule::setFontSource(AssetName name, FolderName newRoute)
{
	auto it = _fontsMap.find(name);
	if (it == _fontsMap.end()) {
		Debug::error("ERROR: Textura no encontrado");
	}
	_fontsMap[name].first = newRoute;
}

std::vector<std::pair<AssetName, FileName>> ResourcesModule::getAllFonts()
{
	std::vector<std::pair<AssetName, FileName>> aux;
	for (auto& i : _fontsMap) {
		std::string assetName = i.first;
		std::string assetFolder = i.second.first + i.second.first;
		aux.push_back({assetName,assetFolder});
	}
	return aux;
}
