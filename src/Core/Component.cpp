#include "Component.h"

core::Component::Component()
	: entity(nullptr), enabled(true) {}

core::Component::~Component()
{
}

void core::Component::setEntity(Entity* e)
{
	entity = e;
}

void core::Component::setEnabled(bool v)
{
	enabled = v;
	enabled ? enable() : disable();
}

core::Entity* core::Component::getEntity() const
{
	return entity;
}

bool core::Component::isEnabled() const
{
	return enabled;
}
