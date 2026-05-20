#pragma once
#include <OgrePrerequisites.h>
#include <OgreTexture.h>

#include "Resource.h"

class TextureResource : public core::Resource
{
public:
	TextureResource(const std::string& id, const std::string& path);
	~TextureResource();
	bool load() override;
	bool unLoad() override;
	Ogre::TexturePtr getTexturePtr() const;
private:
	Ogre::TexturePtr _texturePtr;
};