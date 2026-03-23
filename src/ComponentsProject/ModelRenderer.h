/*
 * @file
 * @brief Componente que gestiona un modelo.
 */
#pragma once

#include <Component.h>

#include <utility>

using modelID = uint64_t;

struct Texture
{
	std::string textureFolder;
	std::string textureFile;

	Texture(const std::string& folder, const std::string& file) : textureFolder(folder), textureFile(file) {}
};

class ModelRenderer : public core::Component
{
	modelID _modelID;

	std::string _modelFolder;
	std::string _modelFile;

	int _nTextures;
	//Clave: Nombre textura		Valor: struct Texture(carpeta archivo, nombre archivo)
	std::unordered_map<std::string, Texture> _textures;

public:
	ModelRenderer();
	~ModelRenderer();

	bool init(const Properties& p) override;
};

