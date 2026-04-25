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

void core::Component::call(const std::string& method, const std::vector<std::any>& args) const 
{
	auto it = _methods.find(method);
	if (it != _methods.end()) {
		try {
			it->second(args);
		}
		catch (const std::bad_any_cast& e) {
			Debug::error("Invalid arguments for method: ", method);
		}
	}
	else {
		Debug::warning("Method not found: ", method);
	}
}

template<typename Func>
void core::Component::registerMethod(const std::string& name, Func&& f) {
	_methods[name] = std::move(f);
}	