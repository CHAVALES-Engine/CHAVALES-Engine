/*
 * @file
 * @brief 
 * This file was adapted from a part of the course TPV2@UCM - Samir Genaim
 */

#pragma once

#include "ec.h"

namespace ec {

/*
 * Classes that inherit from Component must define a field
 *
 *   	constexpr static ecs::cmpId_type id = value;
 *
 * where value is from the enum ecs::cmpId (see ecs.h). This
 * how we assign numeric identifiers to components (so we can
 * easily put them in an array). The list of possible identifiers
 * is defined as an enum in ecs.h
 *
 */
class Component {
public:
	Component() : ent(), enabled()
	{
		
	}

	// Destroys the component.
	// Careful! ent_ and mngr_ should not be destroyed
	//
	virtual ~Component() { }

	// This method is used to set the entity in which the component is
	// installed. It will be called by Manager when adding
	// a component.
	//
	inline void setEntity(Entity *e) {
		ent = e;
	}

	// ...
	virtual void onCreate() { }

	// We assume that init will be called when adding a
	// component to an entity, immediately after setEntity.
	//
	virtual void init() { }

	// ...
	virtual void onEnable() { }

	// A component has an update method, and by default it
	// does nothing -- some components will not override this
	// method.
	//
	virtual void update() { }

	// A component has a render method, and by default it
	// does nothing -- some components will not override this
	// method.
	//
	virtual void render() { }

	// ...
	virtual void onDisable() { }

	// ...
	virtual void onDestroy() { }

protected: // we allow direct use these fields from subclasses
	Entity *ent; // a pointer to the entity, should not be deleted on destruction
	bool enabled;

	//void (Component::*createFunc)() = &onCreate;
	//void (Component::*destroyFunc)() = &onDestroy;
};

} // end of namespace
