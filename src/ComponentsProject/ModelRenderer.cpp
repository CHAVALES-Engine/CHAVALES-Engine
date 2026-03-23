#include "ModelRenderer.h"

ModelRenderer::ModelRenderer() {}

ModelRenderer::~ModelRenderer()
{

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
		_textures.emplace(texture[0], Texture(texture[1], texture[2]));
	}
	return true;
}


