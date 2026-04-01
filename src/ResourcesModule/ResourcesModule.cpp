#include "ResourcesModule.h"
#include "Debug.h"

ResourcesModule::ResourcesModule(): _assetsRoute(), _audioMap(), _modelsMap(), _texturesMap(), _particlesMap()
{
}

ResourcesModule::~ResourcesModule()
{
}

bool ResourcesModule::loadInternalAsset(sol::table assetsType, std::string typeOfAsset)
{
	for (auto& assets : assetsType) {
		std::string nameOfAsset = assets.first.as<std::string>(); 
		std::string assetPath = assets.second.as<std::string>(); 
		if (typeOfAsset == "Audio")
		{
			auto it = _audioMap.find(nameOfAsset);
			if (it != _audioMap.end()) {
				Debug::error("ERROR: Audio existente con ese nombre");
				return false;
			}
			_audioMap[nameOfAsset] = assetPath;
		}
		else if (typeOfAsset == "Mesh")
		{
			auto it = _modelsMap.find(nameOfAsset);
			if (it != _modelsMap.end()) {
				Debug::error("ERROR: Malla existente con ese nombre");
				return false;
			}
			_modelsMap[nameOfAsset] = assetPath;
		}
		else if (typeOfAsset == "Texture")
		{
			auto it = _texturesMap.find(nameOfAsset);
			if (it != _texturesMap.end()) {
				Debug::error("ERROR: Textura existente con ese nombre");
				return false;
			}
			_texturesMap[nameOfAsset] = assetPath;
		}
		else if (typeOfAsset == "Particles")
		{
			auto it = _particlesMap.find(nameOfAsset);
			if (it != _particlesMap.end()) {
				Debug::error("ERROR: Particulas existente con ese nombre");
				return false;
			}
			_particlesMap[nameOfAsset] = assetPath;
		}
	}
	return true;

}

bool ResourcesModule::Init(const std::string& n, const std::string& p)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base); 
	std::string path = p + n + ".lua";
	_assetsRoute = path; 
	
	try
	{
		// intenta leer archivo
		lua.safe_script_file(_assetsRoute);
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("GAMELOADER: Error cargando assets: ", _assetsRoute);
		Debug::error("Lua exception: ", e.what());
		return;
	}

	sol::table assetsFile = lua["assets"];
	Debug::out("GAMELOADER: Assets cargando.");

	for (auto& assets : assetsFile) 
	{
		std::string typeOfAsset = assets.first.as<std::string>();
		sol::table assetsType = assets.second;
		
		if (typeOfAsset == "Audio") 
		{
			sol::table audio = assetsType["Audio"]; 
			if (!loadInternalAsset(audio, typeOfAsset)) {
				return false;
			}
		}
		else if (typeOfAsset == "Mesh") 
		{
			sol::table mesh = assetsType["Mesh"];
			if (!loadInternalAsset(mesh, typeOfAsset)) {
				return false;
			}
		}
		else if (typeOfAsset == "Texture")
		{
			sol::table texture = assetsType["Texture"];
			if (!loadInternalAsset(texture, typeOfAsset)) {
				return false;
			}
		}
		else if (typeOfAsset == "Particles") 
		{ 
			sol::table particles = assetsType["Particles"];
			if (!loadInternalAsset(particles, typeOfAsset)) {
				return false;
			}
		} 
	}
	return true;
}

std::string ResourcesModule::loadAudio(std::string name)
{
	auto it = _audioMap.find(name);
	if (it != _audioMap.end()) {
		Debug::error("ERROR: Audio no encontrado");
	}
	return it->second;
}

std::string ResourcesModule::loadMesh(std::string name)
{
	auto it = _modelsMap.find(name);
	if (it != _modelsMap.end()) {
		Debug::error("ERROR: Modelo no encontrado");
	}
	return it->second;
}

std::string ResourcesModule::loadParticle(std::string name)
{
	auto it = _particlesMap.find(name);
	if (it != _particlesMap.end()) {
		Debug::error("ERROR: Particulas no encontrada");
	}
	return it->second;
}

std::string ResourcesModule::loadTexture(std::string name)
{
	auto it = _texturesMap.find(name);
	if (it != _texturesMap.end()) {
		Debug::error("ERROR: Textura no encontrada");
	}
	return it->second;
}
