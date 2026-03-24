#include "ModelRenderer.h"

#include "Engine.h"
#include "Entity.h"

ModelRenderer::ModelRenderer() {}

ModelRenderer::~ModelRenderer()
{
	//
}

bool ModelRenderer::init(const Properties& p)
{
	_modelFolder = getProperty<std::string>(p, "model folder");
	_modelFile = getProperty<std::string>(p, "near clip distance");
	_nTextures = getProperty<int>(p, "number of textures");
	for (int i = 1; i <= _nTextures; i++)
	{
		std::vector<std::string> texture;
		setProperty(p, "texture" + std::to_string(i), texture);
		_textures.emplace(texture[0], std::pair(texture[1], texture[2]));
	}
	
	//_modelID = Engine::addModel(getEntity()->getEntityID(), _modelFolder, _modelFile);
	return true;
}

void ModelRenderer::setDiffuse(std::string textureName, int submesh)
{
	//Engine::setDiffuse(_modelID, textureName, submesh)
}


