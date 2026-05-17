#pragma once
#include "Resource.h"
#include <OgrePrerequisites.h>
#include <Ogre.h>

class MeshResource: public core::Resource
{
private:
	Ogre::MeshPtr _meshPtr;
public:
	MeshResource(const std::string& id, const std::string& path);
	bool load() override;
	bool unLoad() override;
	Ogre::MeshPtr getMeshPtr() const;
};

