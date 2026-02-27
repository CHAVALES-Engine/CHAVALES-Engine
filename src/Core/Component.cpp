#include "Component.h"

core::Component::Component()
	: entity(nullptr), enabled(true)
{
}

void core::Component::setEntity(Entity* e)
{
	entity = e;
}

void core::Component::enable()
{
	if (!enabled)
	{
		enabled = true;
		onEnable();
	}
}

void core::Component::disable()
{
	if (enabled)
	{
		enabled = false;
		onDisable();
	}
}

const core::Entity* core::Component::getEntity() const
{
	return entity;
}

bool core::Component::isEnabled() const
{
	return enabled;
}
