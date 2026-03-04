#include "ComponentRegister.h"
#include <Component.h>
#include <Debug.h>

ComponentRegister& ComponentRegister::instance() {
	// Usamos la inicializacion de mayers porque es mas limpia y garantiza Thread-Safe
	static ComponentRegister instance;
	return instance;
}

bool ComponentRegister::registComponent(const std::string& name, core::ComponentConstructor ComponentConstructor)
{
	// Intenta registrar un componente
	auto [it, inserted] = _components.try_emplace(name, ComponentConstructor);
	// Si no se ha registrado lanza un warning
	if (!inserted)
		Debug::warning("Component: [", name, "] Already registered.");
	else
		Debug::out("Component: [", name, "] Registered");
	return inserted;
}

std::shared_ptr<core::Component> ComponentRegister::create(const std::string& name)
{
	auto it = _components.find(name);
	if (it == _components.end()) {
		Debug::error("Component: [", name, "] not registered.");
		return nullptr;
	}
	Debug::out("Component: [", name, "] created.");

	return it->second();
}

bool ComponentRegister::unregisterComponent(const std::string& name)
{
	bool removed = (_components.erase(name) > 0);
	if (!removed)
		Debug::warning("Component: [", name, "] not registered.");
	return removed;
}