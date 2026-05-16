#pragma once
#include <OgrePrerequisites.h>

#include "Resource.h"

namespace Ogre
{
	class Mesh;
	using MeshPtr = SharedPtr<Mesh>;
}

class MeshResource: public core::Resource
{
public:
	MeshResource();
	bool load() override;
	bool unLoad() override;
	Ogre::MeshPtr getMeshPtr() const;
};

