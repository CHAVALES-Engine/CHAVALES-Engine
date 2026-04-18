#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <sol.hpp>
#include "guid.h"

/*
 * +------------------+
 * | RESOURCES MODULE |
 * +------------------+
 *
 * --- Ejemplo de uso en lua ---
 * assets = {
 *		Mesh = {
 *			metroid = {
 *				sourceFolder = "metroid-floating/source", -> esta es la ruta donde se encuntra el modelo (para particulas y texturas igual)
 *				fileName = "metroid_final.fbx" -> esta es la malla (para particulas y texturas igual)
 *			}
 *		},
 *		Audio = {
 *			lifeUp = "C:/2526-Grupo03-ChavalesEngine/bin/game/scenes/assets/smb_1-up.wav" -> nombre del audio = ruta del audio
 *		}
 * }
*/


class ResourcesModule
{
public:
	ResourcesModule();
	~ResourcesModule();

	bool Init();

	std::string getAssetSourceFolder(std::string assetName);
	std::vector<std::pair<std::string, std::string>> getAllFonts();

private:
	bool loadAsset(std::string sourceName);

	bool insertAssetMap(std::string sourceName);

	std::unordered_map<ChavalesGUID, std::string> _idMaps;
	std::unordered_map<std::string, ChavalesGUID> _assetsMaps;

	std::string typeOfFolder;
	std::vector<std::pair<std::string, std::string>> _fontsVector;
};

