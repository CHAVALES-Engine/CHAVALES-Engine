/*
 * @file
 * @brief 
 * 
 * TODO: MESSAGES
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
	Component();
	// Destroys the component.
	// Careful! entity should not be destroyed
	virtual ~Component() { }

	// --- SETTERS
	// This method is used to set the entity in which the component is
	// installed. 
	void setEntity(Entity* e);
	// ...
	void enable();
	// ...
	void disable();

	// --- GETTERS
	// ...
	const Entity* getEntity() const;
	// ...
	bool isEnabled() const;

	// --- LIFECYCLE
	// ...
	virtual void onCreate() { }

	// We assume that init will be called when adding a
	// component to an entity, immediately after setEntity.
	virtual void init() { }

	// ...
	virtual void onEnable() { }

	// ...
	virtual void fixedUpdate() { }

	// A component has an update method, and by default it
	// does nothing -- some components will not override this
	// method.
	virtual void update(double deltaTime) { }

	// A component has a render method, and by default it
	// does nothing -- some components will not override this
	// method.
	virtual void render() const { }

	// ...
	virtual void onDisable() { }

	// ...
	virtual void onDestroy() { }

protected: // we allow direct use these fields from subclasses
	Entity *entity; // a pointer to the entity, should not be deleted on destruction
	bool enabled; // if the component is currently active

	void (Component::*createFunc)() = &Component::onCreate;
	void (Component::*destroyFunc)() = &Component::onDestroy;
};

} // end of namespace
