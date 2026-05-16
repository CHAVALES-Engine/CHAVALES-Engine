#include "ModelRenderer.h"

#include "RenderModule.h"
#include "Engine.h"
#include "Entity.h"
#include "PluginSDK.h"

#include <string>
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
 	auto model = Engine::instance()->getAssetSourceFolder(_modelName);
	_modelID = render()->addModel(getEntity()->getEntityID(), model.second, model.first);

	return true;
}

void ModelRenderer::ready()
{
	// Aplica texturas
	for (auto& texture : _textures)
	{
		auto text = Engine::instance()->getAssetSourceFolder(texture[0]);
		render()->setDiffuse(_modelID, std::stoi(texture[1]), text.second, text.first);
	}
}

void ModelRenderer::setDiffuse(const std::string& textureName, const int& submesh)
{
	auto texture = Engine::instance()->getAssetSourceFolder(textureName);
	render()->setDiffuse(_modelID, submesh, texture.second, texture.first);
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