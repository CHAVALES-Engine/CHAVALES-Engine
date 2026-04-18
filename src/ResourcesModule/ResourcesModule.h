#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <sol.hpp>
#include "guid.h"

class ResourcesModule
{
public:
	ResourcesModule();
	~ResourcesModule();

	bool Init();

	/// <summary>
	/// Method to retrive the path of the desire asset
	/// </summary>
	/// <param name="assetName">Name of the asset, this name is the real name of the file + the type of extension of the file</param>
	/// <returns></returns>
	std::pair<std::string, std::string> getAssetSourceFolder(std::string assetName);

	/// <summary>
	/// Method to the render module to retrive all fonts
	/// </summary>
	/// <returns></returns>
	std::vector<std::pair<std::string, std::string>> getAllFonts();

private:
	/// <summary>
	/// Method to go overall the assets folders, it's recursive which means it will travel across the folder unitl it has reserved all the assets of that folder
	/// </summary>
	/// <param name="sourceName"> Name of the folder where the assets are</param>
	/// <returns></returns>
	bool loadAsset(std::string sourceName); 

	/// <summary>
	/// Method to insert into the maps
	/// </summary>
	/// <param name="sourceName">The path of file in the folder of assets</param>
	/// <returns></returns>
	bool insertAssetMap(std::string sourceName); 

	std::unordered_map<ChavalesGUID, std::string> _idMaps; //Map of IDs with the path of the associated asset
	std::unordered_multimap<std::string, ChavalesGUID> _assetsMaps; //Map of assets sorted by name and with an ID

	std::string typeOfFolder; //String of the folder to identify the type of assset and in which the folder is it
	std::vector<std::pair<std::string, std::string>> _fontsVector; //Vector to save all fonts, in order to render load them
};

