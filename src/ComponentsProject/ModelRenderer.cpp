#include "ModelRenderer.h"

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
	//Engine::instance()->deleteModel(_modelID);
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
    _modelID = Engine::instance()->addModel(getEntity()->getEntityID(), _modelName);
	return true;
}

void ModelRenderer::ready()
{
    // Aplica texturas
    for (auto& texture : _textures)
    {
        Engine::instance()->setSubmeshDiffuse(_modelID, texture[0], std::stoi(texture[1]));
    }
}

void ModelRenderer::setDiffuse(const std::string& textureName, const int& submesh)
{
	Engine::instance()->setSubmeshDiffuse(_modelID, textureName, submesh);
}

void ModelRenderer::setTint(const core::Color& tint, const int& submesh)
{
	Engine::instance()->setSubmeshTint(_modelID, tint, submesh);
}

core::Color ModelRenderer::getTint()
{
    return Engine::instance()->getSubmeshTint();
}

void ModelRenderer::setVisible(const bool& visible)
{
	Engine::instance()->setModelVisible(_modelID, visible);
}

modelID ModelRenderer::getModelID()
{
    return _modelID;
}

void ModelRenderer::enable()
{
    Engine::instance()->setModelVisible(_modelID, true);
}

void ModelRenderer::disable()
{
    Engine::instance()->setModelVisible(_modelID, false);
}

void ModelRenderer::destroy()
{
    Engine::instance()->deleteModel(_modelID);
}


