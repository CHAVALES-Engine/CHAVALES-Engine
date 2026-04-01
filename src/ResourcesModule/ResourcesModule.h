#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include <sol.hpp>

typedef std::unordered_map<std::string, std::string> Audios;
typedef std::unordered_map<std::string, std::string> Models;
typedef std::unordered_map<std::string, std::string> Particles; 
typedef std::unordered_map<std::string, std::string> Textures; 


class ResourcesModule
{
public:
	ResourcesModule();
	~ResourcesModule();

	bool Init(const std::string& n,const std::string& p);

	/// <summary>
	/// Getter to recive the desire audio
	/// </summary>
	/// <param name="name">Name of the audio</param>
	/// <returns></returns>
	std::string loadAudio(std::string name);

	/// <summary>
	/// Getter to recive the desire mesh
	/// </summary>
	/// <param name="name">Name of the mesh</param>
	/// <returns></returns>
	std::string loadMesh(std::string name);

	/// <summary>
	/// Getter to recive the desire particle
	/// </summary>
	/// <param name="name">Nombre del particle</param>
	/// <returns></returns>
	std::string loadParticle(std::string name);

	/// <summary>
	/// Getter to recive the desire texture
	/// </summary>
	/// <param name="name">Nombre del texture</param>
	/// <returns></returns>
	std::string loadTexture(std::string name);


private:
	/// <summary>
	/// Private method to load all the assets
	/// </summary>
	/// <param name="assetsType">A table of the desire asset</param>
	/// <param name="typeOfAsset">Name of the desire asset</param>
	/// <param name="asset">Group of all assets</param>
	/// <returns></returns>
	bool loadInternalAsset(sol::table assetsType,std::string typeOfAsset);
	
	
	std::string _assetsRoute; //Route of the assets.lua

	Audios _audioMap; //Map to reserve all audios used in the game
	Models _modelsMap;  //Map to reserve all models used in the game
	Particles _particlesMap;  //Map to reserve all particles used in the game
	Textures _texturesMap;  //Map to reserve all textures used in the game
};

