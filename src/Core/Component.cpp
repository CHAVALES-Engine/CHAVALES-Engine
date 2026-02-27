#include "Component.h"

ec::Component::Component()
	: entity(nullptr), enabled(true) 
{
}

void ec::Component::setEntity(Entity* e)
{
	entity = e;
}

void ec::Component::enable()
{
	if (!enabled) 
	{
		enabled = true;
		onEnable();
	}
}

void ec::Component::disable()
{
	if (enabled)
	{
		enabled = false;
		onDisable();
	}
}

const ec::Entity* ec::Component::getEntity() const
{
	return entity;
}

bool ec::Component::isEnabled() const
{
	return enabled;
}
