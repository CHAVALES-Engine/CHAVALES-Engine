/**
 * @file
 * @brief Define las funciones del Modulo de Recursos
 */

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <sol.hpp>
#include "guid.h"
#include "EngineAPI.h"

struct AssetInfo {
	ChavalesGUID _id;
	bool isUpper;
};
class ENGINE_API  ResourcesModule
{
public:
	ResourcesModule();
	~ResourcesModule();

	bool Init();

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

private:
	/**
	 * @brief Metodo para recorrer todas las carpetas de recursos, es recursivo.
	 * @param sourceName - Nombre de la carpeta de assets
	 */
	bool loadAsset(const std::string& sourceName); 

	/**
	 * @brief Metodo para insertar en el mapa de assets.
	 * @param sourceName - Nombre de la carpeta de assets
	 */
	bool insertAssetMap(const std::string& sourceName); 

	std::unordered_map<ChavalesGUID, std::string> _idMaps; // Mapa de ID-path del asset
	std::unordered_multimap<std::string, AssetInfo> _assetsMaps; // Mapa de assets nombre-ID

	std::string typeOfFolder; // String de la carpeta segun el tipo de asset
	std::vector<std::pair<std::string, std::string>> _fontsVector; // Vector para guardar todas las fuentes
};
