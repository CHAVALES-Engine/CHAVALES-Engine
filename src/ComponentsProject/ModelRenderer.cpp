#include "ModelRenderer.h"

#include "Engine.h"
#include "Entity.h"
#include "PluginSDK.h"

REGISTER_COMPONENT(ModelRenderer);

ModelRenderer::ModelRenderer() {}

ModelRenderer::~ModelRenderer()
{
	//Engine::deleteModel(_modelID);
}

bool ModelRenderer::init(const Properties& p)
{
    // Lee la carpeta donde esta el modelo
    _modelFolder = getProperty<std::string>(p, "folder");

    // Lee el nombre del archivo .obj/.fbx/etc
    _modelFile = getProperty<std::string>(p, "file");

    // Lee cuantas texturas tiene
    _nTextures = getProperty<int>(p, "number of textures");

    // Lee cada textura: { nombre, carpeta, archivo }
    for (int i = 0; i < _nTextures; i++)
    {
        std::vector<std::string> texture;
        setProperty(p, "texture" + std::to_string(i), texture);
        // _textures["nombreTextura"] = { carpeta, archivo }
        _textures.emplace(texture[0], std::pair(texture[1], texture[2]));
        setDiffuse(texture[1], texture[2], i);
    }
	_modelID = Engine::instance()->addModel(getEntity()->getEntityID(), _modelFolder, _modelFile);
	return true;
}

void ModelRenderer::setDiffuse(std::string textureFolder, std::string textureName, int submesh)
{
	Engine::instance()->setSubmeshDiffuse(_modelID, textureFolder, textureName, submesh);
}

void ModelRenderer::setTint(core::Color tint, int submesh)
{
	Engine::instance()->setSubmeshTint(_modelID, tint, submesh);
}

void ModelRenderer::setVisible(bool visible)
{
	Engine::instance()->setModelVisible(_modelID, visible);
}


