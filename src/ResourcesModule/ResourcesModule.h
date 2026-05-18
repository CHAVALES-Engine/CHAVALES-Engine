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

 //struct AssetInfo {
 //	ChavalesGUID _id;
 //	bool isUpper;
 //};
using ResourceFactory = std::function<core::ResourcePtr(const std::string&, const std::string&, bool)>;
class ENGINE_API  ResourcesModule
{
public:
	ResourcesModule() = default;
	~ResourcesModule();

	/**
	 * @brief Inicializa el modulo y recorre las carpetas guardando las rutas a los assets.
	 * @return bool - Si se ha inicializado correctamente.
	 */
	bool Init();
	/**
	 * @brief Devuelve la ruta completa al recurso.
	 * @param relativePath - ruta desde la carpeta de assets.
	 * @return std::String - ruta completa.
	 */
	std::string getAssetPath(const std::string& relativePath);
	/**
	 * @brief Devuelve un puntero al recurso cargado, si no estaba cargado lo carga.
	 * @param relativePath - ruta relativa a la carpeta assets.
	 * @return core::ResourcePtr - sharedPtr del recurso.
	 */
	core::ResourcePtr getOrLoadAsset(const std::string& relativePath);
	/**
	 * Obtener ID desde nombre de asset.
	 * @param assetName - nombre del asset.
	 * @return ChavalesGUID - guid del asset.
	 */
	ChavalesGUID getResourceId(const std::string& path) const;
	/**
	 * @brief Metodo para el RenderModule para sacar todas las fuentes.
	 */
	 //std::vector<std::pair<std::string, std::string>> getAllFonts() const;

	 /**
	  * @brief Metodo para acceder a todos los assets almacenados en el motor.
	  */
	  /*std::vector<std::pair<std::string, std::string>> getAllAssets();*/

	  /**
	   * @brief Funcion que anyade un metodo factory para un tipo de recurso marcado.
	   * @param type - Tipo de recurso.
	   * @param fact - Funcion factory.
	   */
	void addFactory(core::Resource::Type type, ResourceFactory fact);

	/**
	 * @brief Funcion que precarga recursos.
	 * @return bool - Si se ha precargado correctamente.
	 */
	bool load(const std::string& path, bool preload = false);
	/**
	 * @brief Precarga todos los assets encontrados
	 * @return bool - Si se han precargado correctamente.
	 */
	bool preloadAllAssets();
	void loadAllOfType(core::Resource::Type type);
	void unloadAll();

private:
	/**
	 * @brief Metodo para recorrer todas las carpetas de recursos, es recursivo.
	 * @param sourceName - Nombre de la carpeta de assets
	 */
	bool _loadAsset(const std::string& sourceName);
	/**
	 * @brief Metodo para insertar en el mapa de assets.
	 * @param sourcePath - Nombre de la carpeta de assets
	 */
	bool _addResource(const std::string& sourcePath);

	/**
	 * @brief Funciones que comprueban el tipo de recurso segun la extension
	 * @param path - path al recurso.
	 * @return bool - si es de ese tipo de recurso.
	 */
	bool _isMeshFile(const std::string& path) const;
	bool _isTextureFile(const std::string& path) const;
	bool _isSoundFile(const std::string& path) const;
	bool _isFontFile(const std::string& path) const;
	/**
	 * @brief Devuelve el tipo de recurso a traves del path.
	 * @param filePath - Ruta al fichero.
	 * @return core::Resource::Type - Tipo enumerado del fichero.
	 */
	core::Resource::Type _getResourceType(const std::string& filePath) const;

	/**
	 * Normaliza un path cambiando \ -> / y poniendo el path en minusculas
	 * @param path
	 * @return
	 */
	static std::string _normalizePath(const std::string& path);
	//std::string typeOfFolder; // String de la carpeta segun el tipo de asset
	//std::vector<std::pair<std::string, std::string>> _fontsVector; // Vector para guardar todas las fuentes

	// Rutas
	//std::unordered_map<ChavalesGUID, std::string> _guidPath; // Mapa de ID-path del asset
	std::unordered_map<std::string, ChavalesGUID> _pathToGuid; // Mapa de path - ID del asset
	//std::unordered_map<std::string, ChavalesGUID> _nameGuid; // Mapa de Key-ID del asset
	//std::unordered_map<std::string, AssetInfo> _assetsMaps; // Mapa de assets nombre-ID
	// Funciones constructoras de recursos.
	std::unordered_map<core::Resource::Type, ResourceFactory> _factories;
	// Mapas de recursos precargados.
	std::unordered_map<ChavalesGUID, core::ResourcePtr> _resources;
};
