#include "ResourcesModule.h"
#include "Debug.h"
#include "GameConfigurator.h"

ResourcesModule::ResourcesModule(): _assetsRoute(), _audioMap(), _modelsMap(), _texturesMap(), _particlesMap()
{
}

ResourcesModule::~ResourcesModule()
{
}

std::pair<FolderName, FileName> ResourcesModule::loadOgreAsset(const std::string& assetName,std::pair<sol::object, sol::object>& assetType)
{ 
	std::string meshAsset = assetType.first.as<std::string>();
	sol::table assetsType = assetType.second;
	std::vector<std::string> auxVector;
	for (auto& mallas : assetsType) {
		std::string sourceName = mallas.second.as<std::string>();
		auxVector.push_back(sourceName);
	}
	return { auxVector[1],auxVector[0] };
}

bool ResourcesModule::loadInternalAsset(const sol::table& assetsType, const std::string& typeOfAsset)
{
	Debug::out("RESOURCES: Cargando recursos de tipo ", typeOfAsset);
	for (auto& assets : assetsType) {
		std::string nameOfAsset = assets.first.as<std::string>(); 
		if (typeOfAsset == "Audio")
		{
			std::string assetPath = assets.second.as<std::string>(); 
			auto it = _audioMap.find(nameOfAsset);
			if (it != _audioMap.end()) {
				Debug::error("ERROR: Audio ya existente con ese nombre");
				return false;
			}
			Debug::out("RESOURCES: Assetpath ", "./assets/" + assetPath);
			_audioMap[nameOfAsset] = assetPath;
		}
		else if (typeOfAsset == "Mesh")
		{			
			auto it = _modelsMap.find(nameOfAsset);
			if (it != _modelsMap.end()) {
				Debug::error("ERROR: Malla ya existente con ese nombre");
				return false;
			}
			_modelsMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
		}
		else if (typeOfAsset == "Texture")
		{
			auto it = _texturesMap.find(nameOfAsset);
			if (it != _texturesMap.end()) {
				Debug::error("ERROR: Textura ya existente con ese nombre");
				return false;
			}
			_texturesMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
		}
		else if (typeOfAsset == "Particles")
		{
			auto it = _particlesMap.find(nameOfAsset);
			if (it != _particlesMap.end()) {
				Debug::error("ERROR: Particula ya existente con ese nombre");
				return false;
			}
			_particlesMap[nameOfAsset] = loadOgreAsset(nameOfAsset, assets);
		}
		else {
			Debug::error("ERROR: Tipo de recurso no valido");
			return false;
		}
		Debug::out("RESOURCES: ", nameOfAsset, " cargado");
	}
	return true;
}

bool ResourcesModule::Init()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base); 
	std::string path = core::GameConfigurator::_assetsRoot + core::GameConfigurator::_assetsList + ".lua";
	_assetsRoute = path; 
	
	try
	{
		// intenta leer archivo
		lua.safe_script_file(_assetsRoute);
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("RESOURCES: Error cargando assets: ", _assetsRoute);
		Debug::error("Lua exception: ", e.what());
		return false;
	}

	sol::optional<sol::table> assetsFile = lua.get<sol::optional<sol::table>>("assets");
	if (!assetsFile.has_value())
	{
		Debug::error("RESOURCES: El archivo lua no contiene tabla 'assets': ", _assetsRoute);
		return false;
	}
	Debug::out("RESOURCES: Cargando recursos desde ", _assetsRoute);

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
