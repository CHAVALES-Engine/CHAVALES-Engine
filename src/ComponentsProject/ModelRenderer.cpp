#include "ModelRenderer.h"
#include "RenderModule.h"
#include "Engine.h"
#include "Entity.h"
#include "PluginSDK.h"
#include "ResourcesModule.h"

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
	std::transform(_modelName.begin(), _modelName.end(), _modelName.begin(), ::tolower);

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
	core::ResourcePtr res = resources()->getOrLoadAsset(_modelName);
	if (!res || !res->isValid()) {
		Debug::error("[ModelRenderer] Modelo no encontrado: ", _modelName);
		return false;
	}
	_modelID = render()->addModel(getEntity()->getEntityID(), res->getPath(), res->getName());

	return true;
}

void ModelRenderer::ready()
{
	// Aplica texturas
	for (auto& texture : _textures)
	{
		core::ResourcePtr res = resources()->getOrLoadAsset(texture[0]);
		if (!res || !res->isValid()) {
			Debug::error("[ModelRenderer] Textura no encontrada: ", texture[0]);
			continue;
		}
		int submesh = std::stoi(texture[1]);
		render()->setDiffuse(_modelID, submesh, res->getPath(), res->getName());
	}
}

void ModelRenderer::setDiffuse(const std::string& textureName, const int& submesh)
{
	core::ResourcePtr res = resources()->getOrLoadAsset(textureName);
	if (!res || !res->isValid()) {
		Debug::error("[ModelRenderer] Textura no encontrada: ", textureName);
		return;
	}
	render()->setDiffuse(_modelID, submesh, res->getPath(), res->getName());
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