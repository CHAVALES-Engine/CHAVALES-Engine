#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <sol.hpp>

using AssetName = std::string; //To clarify the string
using FileName = std::string; //To clarify the string
using FolderName = std::string; //To clarify the string


typedef std::unordered_map<AssetName, std::string> Audios;
typedef std::unordered_map<AssetName, std::pair<FolderName, FileName>> Models;
typedef std::unordered_map<AssetName, std::pair<FolderName, FileName>> Particles;
typedef std::unordered_map<AssetName, std::pair<FolderName, FileName>> Textures;


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

	/// <summary>
	/// Getter to recive the desire audio
	/// </summary>
	/// <param name="name">Name of the audio</param>
	/// <returns></returns>
	std::string getAudio(AssetName name);

	/// <summary>
	/// Getter to recive the desire mesh
	/// </summary>
	/// <param name="name">Name of the mesh</param>
	/// <returns></returns>
	std::pair<FolderName, FileName> getMesh(AssetName name);

	/// <summary>
	/// Getter to recive the desire particle
	/// </summary>
	/// <param name="name">Nombre del particle</param>
	/// <returns></returns>
	std::pair<FolderName, FileName> getParticle(AssetName name);

	/// <summary>
	/// Getter to recive the desire texture
	/// </summary>
	/// <param name="name">Nombre del texture</param>
	/// <returns></returns>
	std::pair<FolderName, FileName> getTexture(AssetName name);

	/// <summary>
	/// Method to set the Path of the found audio
	/// </summary>
	/// <param name="name">Name of the desire asset</param>
	/// <param name="newRoute">Name of the new path</param>
	void setAudioSource(AssetName name, FolderName newRoute);

	/// <summary>
	/// Method to set the Path of the found model
	/// </summary>
	/// <param name="name">Name of the desire asset</param>
	/// <param name="newRoute">Name of the new path</param>
	void setMeshSource(AssetName name, FolderName newRoute);

	/// <summary>
	/// Method to set the Path of the found particle
	/// </summary>
	/// <param name="name">Name of the desire asset</param>
	/// <param name="newRoute">Name of the new path</param>
	void setParticleSource(AssetName name, FolderName newRoute);

	/// <summary>
	/// Method to set the Path of the found texture
	/// </summary>
	/// <param name="name">Name of the desire asset</param>
	/// <param name="newRoute">Name of the new path</param>
	void setTextureSource(AssetName name, FolderName newRoute);


private:
	/// <summary>
	/// Private method to load all the assets
	/// </summary>
	/// <param name="assetsType">A table of the desire asset</param>
	/// <param name="typeOfAsset">Name of the desire asset</param>
	/// <param name="asset">Group of all assets</param>
	/// <returns></returns>
	bool loadInternalAsset(const sol::table& assetsType,const std::string& typeOfAsset);
	std::pair<FolderName, FileName> loadOgreAsset(const std::string& assetName,std::pair<sol::object, sol::object>& assetType);

	void buildFileIndex(const std::string& root);
	
	std::string _assetsListPath; // Route of the assets.lua
	std::unordered_map<FileName, FolderName> _fileIndex; 

	Audios _audioMap; // Map to reserve all audios used in the game
	Models _modelsMap;  // Map to reserve all models used in the game
	Particles _particlesMap;  // Map to reserve all particles used in the game
	Textures _texturesMap;  // Map to reserve all textures used in the game

};

