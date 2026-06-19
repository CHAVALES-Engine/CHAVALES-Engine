#include "Component.h"
#include "checkMLNew.h"

core::Component::Component()
	: entity(nullptr), enabled(true)
{
}

core::Component::~Component()
{
	destroy();
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

void core::Component::setName(std::string const& name)
{
	_name = name;
}

core::Entity* core::Component::getEntity() const
{
	return entity;
}

bool core::Component::isEnabled() const
{
	return enabled;
}

const std::string& core::Component::getName() const
{
	return _name;
}

bool core::Component::init(const Properties& p) {
	for (auto* prop : registeredProperties) {
		if (!prop->setProperty(this, p)) return false;
	}
	return true;
}