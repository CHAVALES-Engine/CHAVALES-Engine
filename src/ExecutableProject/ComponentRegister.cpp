#include "ComponentRegister.h"
#include <Debug.h>
#include <PluginSDK.h>
#include <Component.h>

ComponentRegister& ComponentRegister::instance() {
	// Usamos la inicializacion de mayers para garantizar Thread-Safe
	static ComponentRegister instance;
	return instance;
}

bool ComponentRegister::registComponent(const char* name, core::ComponentConstructor factory)
{
	// Intenta registrar un componente
	auto [it, inserted] = _components.try_emplace(name, factory);
	// Si no se ha registrado lanza un warning
	if (!inserted)
		core::Debug::warning("Component: ", name, " Already registered.");
	else
		core::Debug::out("Component: [", name, "] Registered");
	return inserted;
}

template<typename ...Args>
inline std::unique_ptr<core::Component> ComponentRegister::create(const char* name, Args && ...args)
{
	auto it = _components.find(name);
	if (it == _components.end()) {
		core::Debug::error("Component: ", name, " not registered.");
		return nullptr;
	}

	return it->second();
}

bool ComponentRegister::unregisterComponent(const char* name)
{
	bool removed = (_components.erase(name) > 0);
	if (!removed)
		core::Debug::warning("Component: ", name, " not registered.");
	return removed;
}