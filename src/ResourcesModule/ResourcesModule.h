/**
 * @file
 * @brief Define las funciones del Modulo de Recursos
 */

#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include "guid.h"
#include "EngineAPI.h"
#include "Resource.h"

struct AssetInfo {
	ChavalesGUID _id;
	bool isUpper;
};
class ENGINE_API  ResourcesModule
{
public:
	ResourcesModule() = default;
	~ResourcesModule();

	bool Init();

	/**
	 * @brief Precarga todos los assets encontrados
	 * @return 
	 */
	bool preloadAllAssets();

	/**
	 * @brief Metodo para sacar la ruta del asset deseado.
	 * @param assetName - Nombre del asset: nombre de la carpeta + archivo + extension del archivo
	 */
	std::pair<std::string, std::string> getAssetSourceFolder(const std::string& assetName);

	/**
	 * @brief Metodo para el RenderModule para sacar todas las fuentes.
	 */
	std::vector<std::pair<std::string, std::string>> getAllFonts();

	/**
	 * @brief Metodo para acceder a todos los assets almacenados en el motor.
	 */
	std::vector<std::pair<std::string, std::string>> getAllAssets();

	/**
	 * @brief Funcion que anyade un metodo factory para un tipo de recurso marcado.
	 * @param type - Tipo de recurso.
	 * @param fact - Funcion factory.
	 */
	void addFactory(core::Resource::Type type, std::function<core::ResourcePtr(const std::string&, const std::string&)> fact);

	/**
	 * @brief Funcion que precarga recursos.
	 * @return bool - Si se ha precargado correctamente.
	 */
	bool preload(const std::string& path);

private:
	/**
	 * @brief Metodo para recorrer todas las carpetas de recursos, es recursivo.
	 * @param sourceName - Nombre de la carpeta de assets
	 */
	bool _loadAsset(const std::string& sourceName); 

	/**
	 * @brief Metodo para insertar en el mapa de assets.
	 * @param sourceName - Nombre de la carpeta de assets
	 */
	bool _insertAssetMap(const std::string& sourceName);

	/**
	 * @brief Funciones que comprueban el tipo de recurso segun la extension
	 * @param path - path al recurso.
	 * @return bool - si es de ese tipo de recurso.
	 */
	bool _isMeshFile(const std::string& path) const;
	bool _isTextureFile(const std::string& path) const;

	std::unordered_map<ChavalesGUID, std::string> _idMaps; // Mapa de ID-path del asset
	std::unordered_multimap<std::string, AssetInfo> _assetsMaps; // Mapa de assets nombre-ID

	std::string typeOfFolder; // String de la carpeta segun el tipo de asset
	std::vector<std::pair<std::string, std::string>> _fontsVector; // Vector para guardar todas las fuentes

	// Funciones constructoras de recursos.
	std::unordered_map<core::Resource::Type, std::function<core::ResourcePtr(const std::string&, const std::string&)>> _factories;
	// Mapas de recursos precargados.
	std::unordered_map<std::string, core::ResourcePtr> _resources;
};
