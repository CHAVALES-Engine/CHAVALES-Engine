#include "ModelRenderer.h"

#include "Engine.h"
#include "Entity.h"
#include "PluginSDK.h"

#include <string>

REGISTER_COMPONENT(ModelRenderer);

ModelRenderer::ModelRenderer() {}

ModelRenderer::~ModelRenderer()
{
	Engine::instance()->deleteModel(_modelID);
}

bool ModelRenderer::init(const Properties& p)
{
    // Lee la clave del archivo almacenado en resources
    _modelName = getProperty<std::string>(p, "file");

    // Lee cuantas texturas tiene
    int nTextures = getProperty<int>(p, "number of textures");

	_modelID = Engine::instance()->addModel(getEntity()->getEntityID(), _modelName);

    // Lee cada textura: { nombre, nombre archivo, submesh }
    for (int i = 0; i < nTextures; i++)
    {
        std::vector<std::string> texture;
        setProperty(p, "texture" + std::to_string(i), texture);
        Engine::instance()->setSubmeshDiffuse(_modelID, texture[1], std::stoi(texture[2]));
    }
	return true;
}

void ModelRenderer::setDiffuse(std::string textureName, int submesh)
{
	Engine::instance()->setSubmeshDiffuse(_modelID, textureName, submesh);
}

void ModelRenderer::setTint(core::Color tint, int submesh)
{
	Engine::instance()->setSubmeshTint(_modelID, tint, submesh);
}

void ModelRenderer::setVisible(bool visible)
{
	Engine::instance()->setModelVisible(_modelID, visible);
}


