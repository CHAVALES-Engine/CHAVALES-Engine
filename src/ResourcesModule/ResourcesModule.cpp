#include "ResourcesModule.h"
#include "Debug.h"
#include "GameConfigurator.h"
#include <filesystem>
#include "checkMLNew.h"


ResourcesModule::~ResourcesModule()
{
	for (auto [_, resource] : _resources)
		resource->unLoad();
	_resources.clear();
	_factories.clear();
	_pathToGuid.clear();
}

bool ResourcesModule::Init()
{
	if (!_loadAsset(core::GameConfigurator::instance()._assetsRoot)) {
		return false;
	}
	return true;
}

std::string ResourcesModule::getAssetPath(const std::string& relativePath)
{
	core::ResourcePtr ptr = getOrLoadAsset(relativePath);

	if (ptr && ptr->isValid())
		return ptr->getPath();
	Debug::error("[ResourcesModule] Ruta no encontrada: ", relativePath);
	return "";
}

core::ResourcePtr ResourcesModule::getOrLoadAsset(const std::string& relativePath)
{
	std::string fullPath = core::GameConfigurator::instance()._assetsRoot + relativePath;
	ChavalesGUID id = getResourceId(relativePath);

	if (!id.isValid()) return nullptr;

	// Si ya esta cargado, devolverlo
	auto it = _resources.find(id);
	if (it != _resources.end() && it->second && it->second->isValid()) {
		return it->second;
	}

	// Si no, precargarlo
	if (!load(relativePath)) {
		return nullptr;
	}

	return _resources[id];
}

ChavalesGUID ResourcesModule::getResourceId(const std::string& path) const
{
	std::string fullPath = _normalizePath(
		core::GameConfigurator::instance()._assetsRoot + path);

	auto it = _pathToGuid.find(fullPath);
	if (it != _pathToGuid.end())
		return it->second;

	Debug::error("[ResourcesModule] Recurso no encontrado: ", path);
	return ChavalesGUID::invalid();
}

//std::vector<std::pair<std::string, std::string>> ResourcesModule::getAllFonts() const
//{
//	return _fontsVector;
//}

void ResourcesModule::addFactory(core::Resource::Type type, ResourceFactory fact)
{
	if (!fact) {
		Debug::error("[ResourcesModule] Factory nula para tipo: ", static_cast<int>(type));
		return;
	}

	if (_factories.find(type) != _factories.end()) {
		Debug::warning("[ResourcesModule] Factory sobrescrita para tipo: ", static_cast<int>(type));
	}

	_factories[type] = std::move(fact);
}

bool ResourcesModule::load(const std::string& path, bool preload)
{
	std::string fullPath = core::GameConfigurator::instance()._assetsRoot + path;
	ChavalesGUID id = getResourceId(path);

	if (!id.isValid()) return false;
	// Comprueba que no este ya cargado y sea valido.
	auto it = _resources.find(id);
	if (it != _resources.end() && it->second && it->second->isValid()) return false;
	// Crea el recurso dependiendo del archivo.
	if (_isMeshFile(fullPath))
	{
		_resources[id] = _factories[core::Resource::Type::MESH](id.toString(), fullPath, preload);
	}
	else if (_isTextureFile(fullPath))
	{
		_resources[id] = _factories[core::Resource::Type::TEXTURE](id.toString(), fullPath, preload);
	}
	else if (_isFontFile(fullPath))
	{
		_resources[id] = _factories[core::Resource::Type::FONT](id.toString(), fullPath, preload);
	}
	else if (_isSoundFile(fullPath))
	{
		//_resources[id] = _factories[core::Resource::Type::TEXTURE](id.toString(), fullPath, preload); TODO
	}

	return true;
}

bool ResourcesModule::preloadAllAssets()
{
	for (auto [path, _] : _pathToGuid)
		if (!load(path, true)) return false;
	return true;
}

void ResourcesModule::loadAllOfType(core::Resource::Type type)
{
	auto factIt = _factories.find(type);
	if (factIt == _factories.end()) return;

	for (auto& [id, resource] : _resources)
	{
		if (resource && resource->getType() == type && !resource->isValid())
		{
			auto loaded = factIt->second(resource->getName(), resource->getPath(), true);
			if (loaded)
				_resources[id] = loaded;
		}
	}
}

void ResourcesModule::unloadAll()
{
	for (auto& [_, resource] : _resources)
	{
		if (resource) resource->unLoad();
	}
	_resources.clear();
	_pathToGuid.clear();
}

bool ResourcesModule::_loadAsset(const std::string& sourceName)
{
	for (const auto& entry : std::filesystem::directory_iterator(sourceName))
	{
		if (entry.is_directory())
		{
			if (!_loadAsset(entry.path().string()))
				return false;
		}
		else
		{
			std::string normalizedPath = entry.path().generic_string();
			if (!_addResource(normalizedPath))
				return false;
		}
	}
	return true;
}

bool ResourcesModule::_addResource(const std::string& sourcePath)
{
	auto p = std::filesystem::path(sourcePath);
	std::string nombreAsset = p.filename().string();

	// Comprobamos que sea un ficher de un tipo conocido.
	core::Resource::Type type = _getResourceType(sourcePath);
	if (type == core::Resource::Type::NONE) {
		return true; // Skip archivos no soportados
	}

	// Crear guid
	ChavalesGUID aux = ChavalesGUID::generate();

	// Mapeos bidireccionales
	std::string normalizedPath = _normalizePath(sourcePath);
	_pathToGuid[normalizedPath] = aux;
	_resources[aux] = std::make_shared<core::Resource>(nombreAsset, p.parent_path().string()+"/", type);

	// vector especial para fonts.
	/*if (_isFontFile(sourcePath)) {
		_fontsVector.push_back({ p.parent_path().filename().string() + "/" + nombreAsset,sourcePath });
	}*/
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

bool ResourcesModule::_isSoundFile(const std::string& path) const
{
	std::string ext = std::filesystem::path(path).extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	return ext == ".wav" || ext == ".mp3" || ext == ".ogg" || ext == ".flac";
}

bool ResourcesModule::_isFontFile(const std::string& path) const
{
	std::string ext = std::filesystem::path(path).extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	return ext == ".ttf" || ext == ".otf";
}

core::Resource::Type ResourcesModule::_getResourceType(const std::string& filePath) const
{
	if (_isMeshFile(filePath))
		return core::Resource::Type::MESH;
	else if (_isTextureFile(filePath))
		return core::Resource::Type::TEXTURE;
	else if (_isFontFile(filePath))
		return core::Resource::Type::FONT;
	else if (_isSoundFile(filePath))
		return core::Resource::Type::SOUND;

	return core::Resource::Type::NONE;
}

std::string ResourcesModule::_normalizePath(const std::string& path)
{
	std::string normalized = std::filesystem::path(path).generic_string();
	std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
	return normalized;
}