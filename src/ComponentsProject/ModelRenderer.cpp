#include "ModelRenderer.h"

#include "RenderModule.h"
#include "ResourcesModule.h"
#include "Engine.h"
#include "Entity.h"
#include "PluginSDK.h"

#include <string>
#include <filesystem>
#include "checkMLNew.h"

REGISTER_COMPONENT(ModelRenderer);

ModelRenderer::ModelRenderer()
{
	registerMethod("setDiffuse", [this](const std::vector<std::any>& args) {
		if (args.size() == 1) {
			setDiffuse(std::any_cast<std::string>(args[0]));
		}
		else if (args.size() >= 2) {
			setDiffuse(std::any_cast<std::string>(args[0]), std::any_cast<int>(args[1]));
		}
		});

	registerMethod("setTint", [this](const std::vector<std::any>& args) {
		if (args.size() == 1) {
			setTint(std::any_cast<core::Color>(args[0]));
		}
		else if (args.size() >= 2) {
			setTint(std::any_cast<core::Color>(args[0]), std::any_cast<int>(args[1]));
		}
		});

	registerMethod("setVisible", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVisible(std::any_cast<bool>(args[0]));
		}
		});
}

ModelRenderer::~ModelRenderer()
{
}

bool ModelRenderer::init(const Properties& p)
{
	// Lee la clave del archivo almacenado en resources
	_modelName = getProperty<std::string>(p, "file");

	// Lee cuantas texturas tiene
	int nTextures = getProperty<int>(p, "number of textures");

	// Lee cada textura: { nombre, nombre archivo, submesh }
	for (int i = 0; i < nTextures; i++)
	{
		std::vector<std::string> texture;
		if (setProperty(p, "texture" + std::to_string(i), texture))
			_textures.push_back(texture);
	}

	//Carga el modelo en ogre y se guarda una referencia a el
	std::string modelPath = resources()->getAssetPath(_modelName);
	if (modelPath.empty()) {
		Debug::error("[ModelRenderer] Modelo no encontrado: ", _modelName);
		return false;
	}
	std::string folder = std::filesystem::path(modelPath).parent_path().string() + "/";
	std::string filename = std::filesystem::path(modelPath).filename().string();

	_modelID = render()->addModel(getEntity()->getEntityID(), folder, filename);

	return true;
}

void ModelRenderer::ready()
{
	// Aplica texturas
	for (auto& texture : _textures)
	{
		std::string texturePath = resources()->getAssetPath(texture[0]);
		if (texturePath.empty()) {
			Debug::error("[ModelRenderer] Textura no encontrada: ", texture[0]);
			continue;
		}
		std::string textureFolder = std::filesystem::path(texturePath).parent_path().string() + "/";
		std::string textureFilename = std::filesystem::path(texturePath).filename().string();
		int submesh = std::stoi(texture[1]);

		render()->setDiffuse(_modelID, submesh, textureFolder, textureFilename);
	}
}

void ModelRenderer::setDiffuse(const std::string& textureName, const int& submesh)
{
	std::string texturePath = resources()->getAssetPath(textureName);
	if (texturePath.empty()) {
		Debug::error("[ModelRenderer] Textura no encontrada: ", textureName);
		return;
	}

	std::string textureFolder = std::filesystem::path(texturePath).parent_path().string() + "/";
	std::string textureFilename = std::filesystem::path(texturePath).filename().string();

	render()->setDiffuse(_modelID, submesh, textureFolder, textureFilename);
}

void ModelRenderer::setTint(const core::Color& tint, const int& submesh)
{
	render()->setTint(_modelID, submesh, tint);
}

core::Color ModelRenderer::getTint(const int& submesh) const
{
	return render()->getTint(_modelID, submesh);
}

void ModelRenderer::setVisible(const bool& visible)
{
	render()->setModelVisible(_modelID, visible);
}

modelID ModelRenderer::getModelID() const
{
	return _modelID;
}

void ModelRenderer::enable()
{
	render()->setModelVisible(_modelID, true);
}

void ModelRenderer::disable()
{
	render()->setModelVisible(_modelID, false);
}

void ModelRenderer::destroy()
{
	render()->deleteModel(_modelID);
}