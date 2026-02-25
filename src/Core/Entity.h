// This file was adapted from a part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <vector>
#include "ec.h"

namespace ec {

/*
 * A struct that represents a collection of components.
 *
 */

class Entity {
public:
	Entity(grpId_t gId);
	virtual ~Entity(); // Destroys the entity

	// we delete the copy constructor/assignment because it is
	// not clear how to copy the components
	// tenemos que valorar esto
	//Entity(const Entity&) = delete;
	//Entity& operator=(const Entity&) = delete;	

private:

	// We could make the constructors private as well, so only
	// the manager can create instances (because it is a friend)
	friend Manager;// so we can update these fields directly from the manager

	std::vector<Component*> _cmps;
	bool _alive;
	grpId_t _gId;
};

} // end of name space
