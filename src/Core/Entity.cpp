#include "Entity.h"
#include "Component.h"

ec::Entity::Entity(grpId_t gId) : //
	_alive(),  //
	_gId(gId) //
{
	// We reserve the space for the maximum number of
	// components. This way we avoid resizing the vector.
	//_cmps.reserve(maxComponentId);
}

ec::Entity::~Entity()
{
	// we delete all available components
	//
	for (auto c : _cmps)
		delete c;
}
