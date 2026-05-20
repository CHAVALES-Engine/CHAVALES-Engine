#include "ScriptsManager.h"

#ifndef SOL_ALL_SAFETIES_ON
#define SOL_ALL_SAFETIES_ON 1
#endif
#include <sol/sol.hpp>

#include <unordered_map>
#include <iostream>

#include "Debug.h"
#include <Scene.h>
#include <Entity.h>
#include <Component.h>
#include <Vector2.h>
#include <Vector3.h>
#include "PropertyConverters.h"

// patron pImpl, struct con parametros dependientes de lua
struct ScriptsManager::Impl {
	sol::state lua;
	std::unordered_map<ScriptHandle, sol::environment> envs;

	// Almacenar las metatablas de tipos por nombre
	std::unordered_map<std::string, sol::table> typeMethods;

	// Almacenar los user data (punteros a objetos) por nombre global
	std::unordered_map<std::string, std::pair<void*, std::string>> globalObjects;
};

ScriptsManager::ScriptsManager() : pImpl(std::make_unique<Impl>())
{
}

// destructora necesita estar aqui porque tiene que conocer
// la definicion de Imp
ScriptsManager::~ScriptsManager() = default;

inline ScriptsManager& ScriptsManager::instance()
{
	static ScriptsManager instance;
	return instance;
}
void ScriptsManager::init() const
{
	// Inicializamos en la MV las biblioticas necesarias
	pImpl->lua.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::string,
		sol::lib::table
	);
	registerBindings();
}

void ScriptsManager::shutdown() {
	pImpl->envs.clear();
	pImpl->globalObjects.clear();
	pImpl->typeMethods.clear();

	pImpl->lua.collect_garbage();
}
ScriptHandle ScriptsManager::loadScript(const std::string& path) const
{
	// Cogemos el estado Lua del manager.
	auto& lua = pImpl->lua;
	// Compilacion del script a bytecode.
	auto load = lua.load_file(path);
	if (!load.valid()) {
		sol::error err = load;
		Debug::error("[ScriptsManager] Error cargando (", path, "): ", err.what());
		return NULL_SCRIPT;
	}
	// Genera un Guid unico par el script.
	ScriptHandle handle = ScriptHandle::generate();
	auto& env = pImpl->envs[handle];
	// Un sol::environment es esencialmente una tabla Lua 
	// que actua como espacio de nombres propio para el script.
	env = sol::environment(
		lua, sol::create, lua.globals());
	// Enlaza el "chunk" compilado con el entorno creado.
	sol::protected_function script = load;
	sol::set_environment(env, script);
	// Ejecutamos el script en su entorno para que registre sus funciones. 
	auto result = script();
	if (!result.valid()) {
		sol::error err = result;
		Debug::error("[ScriptsManager] Error ejecutando (", path, "): ", err.what());
		pImpl->envs.erase(handle);
		return NULL_SCRIPT;
	}
	return handle;
}

void ScriptsManager::releaseScript(ScriptHandle handle) const {
	pImpl->envs.erase(handle);
}

bool ScriptsManager::execute(ScriptHandle h, const std::string& fn) const {
	auto it = pImpl->envs.find(h);
	if (it == pImpl->envs.end()) return false;

	sol::protected_function func = it->second[fn];
	if (!func.valid()) return false;

	auto result = func();
	if (!result.valid()) {
		sol::error err = result;
		Debug::error("[Lua Error en '", fn, "']");
		Debug::error(err.what());  //  Escribe linea, funcion y stack.
		return false;
	}
	return true;
}

bool ScriptsManager::execute(ScriptHandle h, const std::string& fn, ExecuteArgs args) const
{
	auto it = pImpl->envs.find(h);
	if (it == pImpl->envs.end()) return false;

	sol::protected_function func = it->second[fn];
	if (!func.valid()) return true;

	sol::this_state s{ pImpl->lua.lua_state() };
	std::vector<sol::object> luaArgs;
	luaArgs.reserve(args.size());
	for (const auto& a : args)
		luaArgs.push_back(ScriptManager::propertyToSolObject(a, s));

	auto result = func(sol::as_args(luaArgs));
	if (!result.valid()) {
		sol::error err = result;
		Debug::error("[Lua Error en '", fn, "']");
		Debug::error(err.what());  //  Escribe linea, funcion y stack.
		return false;
	}
	return true;
}

bool ScriptsManager::executeWithProps(ScriptHandle h, const std::string& fn,
	const Properties& props) const
{
	auto it = pImpl->envs.find(h);
	if (it == pImpl->envs.end()) return false;

	sol::protected_function func = it->second[fn];
	if (!func.valid()) return true;  // funcion opcional, no es error

	// Convertir Properties → sol::table
	sol::table t = pImpl->lua.create_table();
	for (const auto& [key, prop] : props) {
		t[key] = ScriptManager::propertyToSolObject(prop, sol::this_state{ pImpl->lua.lua_state() });
	}

	auto result = func(t);
	if (!result.valid()) {
		sol::error err = result;
		Debug::error("[ScriptsManager] Error en '", fn, "': ", err.what());
		return false;
	}
	return true;
}

void ScriptsManager::setScriptEntity(ScriptHandle h, core::Entity* entity) const
{
	auto it = pImpl->envs.find(h);
	if (it == pImpl->envs.end()) return;
	it->second["entity"] = entity;
}

void ScriptsManager::bindMethodImpl(const std::string& typeName, const std::string& methodName,
	MethodWrapper wrapper) const
{
	{
		auto& lua = pImpl->lua;

		// Si el tipo no habia sido definido, lo definimos.
		auto it = pImpl->typeMethods.find(typeName);
		if (it == pImpl->typeMethods.end()) {
			Debug::warning("[ScriptsManager] Declarando tipo: ", typeName);
			_declareTypeImpl(typeName);
		}
		it = pImpl->typeMethods.find(typeName);
		if (it == pImpl->typeMethods.end()) { // si aun asi no se encuentra return
			Debug::error("[ScriptsManager] Tipo no declarado: ", typeName);
			_declareTypeImpl(typeName);
		}

		sol::table mt = it->second;

		// Crear el metodo Lua que llama al wrapper
		mt[methodName] = [wrapper, typeName](sol::this_state s, sol::object self, sol::variadic_args args) -> sol::object {
			try {
				// Extraer el void* del objeto Lua
				void* ptr = nullptr;

				// self es una tabla con el puntero almacenado en __ptr
				if (self.is<sol::table>()) {
					sol::table t = self.as<sol::table>();
					sol::object ptrObj = t["__ptr"];
					if (ptrObj.valid() && ptrObj.is<void*>())
						ptr = ptrObj.as<void*>();
				}

				if (!ptr) {
					Debug::error("[ScriptsManager] No se encontro puntero en objeto Lua");
					return sol::nil;
				}

				// Convertir argumentos Lua a vector<Property>
				std::vector<Property> luaArgs;

				for (const auto& arg : args)
					luaArgs.push_back(ScriptManager::solObjectToProperty(arg));

				// Llamar al wrapper
				Property result = wrapper(ptr, luaArgs);

				//Convertir Property a sol::object
				return ScriptManager::propertyToSolObject(result, s);
			}
			catch (const std::exception& e) {
				Debug::error("[ScriptsManager] Error ejecutando metodo: ", e.what());
				return sol::nil;
			}
			};

		Debug::out("[ScriptsManager] Metodo registrado: ", typeName, "::", methodName);
	}
}

void ScriptsManager::bindGlobalImpl(const std::string& globalName,
	const std::string& typeName, void* instance) const
{
	auto& lua = pImpl->lua;

	auto it = pImpl->typeMethods.find(typeName);
	if (it == pImpl->typeMethods.end()) {
		Debug::error("[ScriptsManager] Tipo no declarado: ", typeName);
		return;
	}

	sol::table mt = it->second;

	// Crear una tabla que actue como instancia
	sol::table instance_table = lua.create_table();
	instance_table["__ptr"] = instance;

	// Asignar la metatabla
	instance_table[sol::metatable_key] = mt;

	// Registrar como global en Lua
	lua[globalName] = instance_table;

	// Almacenar referencia
	pImpl->globalObjects[globalName] = { instance, typeName };

	Debug::out("[ScriptsManager] Global registrado: ", globalName, " (tipo: ", typeName, ")");
}

void ScriptsManager::registerBindings() const
{
	auto& lua = pImpl->lua;

	// ===== MATH Y CORE =====
	lua.new_usertype<core::Vector2<>>("Vector2",
		sol::constructors<core::Vector2<>(), core::Vector2<>(float), core::Vector2<>(float, float)>(),
		"x", &core::Vector2<>::getX, "y", &core::Vector2<>::getY,
		"magnitude", &core::Vector2<float>::magnitude, "normalized", &core::Vector2<float>::normalized,
		sol::meta_function::addition, static_cast<core::Vector2<float>(core::Vector2<float>::*)(const core::Vector2<float>&) const>(&core::Vector2<float>::operator+),
		sol::meta_function::subtraction, static_cast<core::Vector2<float>(core::Vector2<float>::*)(const core::Vector2<float>&) const>(&core::Vector2<float>::operator-),
		sol::meta_function::multiplication, static_cast<core::Vector2<float>(core::Vector2<float>::*)(float) const>(&core::Vector2<float>::operator*)
	);

	lua.new_usertype<core::Vector3<>>("Vector3",
		sol::constructors<core::Vector3<>(), core::Vector3<>(float), core::Vector3<>(float, float, float)>(),
		"x", &core::Vector3<>::getX, "y", &core::Vector3<>::getY, "z", &core::Vector3<>::getZ,
		"magnitude", &core::Vector3<float>::magnitude, "normalized", &core::Vector3<float>::normalized,
		sol::meta_function::addition, static_cast<core::Vector3<float>(core::Vector3<float>::*)(const core::Vector3<float>&) const>(&core::Vector3<float>::operator+),
		sol::meta_function::subtraction, static_cast<core::Vector3<float>(core::Vector3<float>::*)(const core::Vector3<float>&) const>(&core::Vector3<float>::operator-),
		sol::meta_function::multiplication, static_cast<core::Vector3<float>(core::Vector3<float>::*)(float) const>(&core::Vector3<float>::operator*)
	);

	lua.new_usertype<core::Vector4<>>("Vector4",
		sol::constructors<core::Vector4<>(), core::Vector4<>(float), core::Vector4<>(float, float, float, float)>(),
		"x", &core::Vector4<>::getX, "y", &core::Vector4<>::getY, "z", &core::Vector4<>::getZ, "w", &core::Vector4<>::getW
	);

	lua.new_usertype<core::Quaternion<>>("Quaternion",
		sol::constructors<core::Quaternion<>(), core::Quaternion<>(float, float, float, float)>(),
		"x", &core::Quaternion<>::getX, "y", &core::Quaternion<>::getY, "z", &core::Quaternion<>::getZ, "w", &core::Quaternion<>::getW
	);

	lua.new_usertype<core::Color>("Color",
		sol::constructors<core::Color(float, float, float, float)>(),
		"r", &core::Color::getRed, "g", &core::Color::getGreen, "b", &core::Color::getBlue, "a", &core::Color::getAlpha
	);

	// ===== ENTITY =====
	lua.new_usertype<core::Entity>("Entity",
		// GETTERS
		"getName", &core::Entity::getName,
		"isAlive", &core::Entity::isAlive,
		"isVisible", &core::Entity::isVisible,
		"isEnabled", &core::Entity::isEnabled,
		"getComponent", [](core::Entity* e, const std::string& name) {
			return e->getComponent(name).get();
		},
		"getDontDestroyOnLoad", &core::Entity::getDontDestroyOnLoad,
		"getComponentCount", &core::Entity::getComponentCount,
		"getScene", &core::Entity::getScene,

		// SETTERS
		"setName", &core::Entity::setName,
		"setAlive", &core::Entity::setAlive,
		"setVisible", &core::Entity::setVisible,
		"setEnabled", &core::Entity::setEnabled,
		"setDontDestroyOnLoad", &core::Entity::setDontDestroyOnLoad,

		// LIFECYCLE
		"enable", &core::Entity::enable,
		"disable", &core::Entity::disable,
		"awake", &core::Entity::awake,
		"ready", &core::Entity::ready,
		"update", [](core::Entity* e, uint64_t dt) { e->update(dt); },
		"fixedUpdate", &core::Entity::fixedUpdate,
		"lateUpdate", [](core::Entity* e, uint64_t dt) { e->lateUpdate(dt); },
		"destroy", &core::Entity::destroy,

		// COMPONENT MANAGEMENT
		"removeComponents", &core::Entity::removeComponents,
		"removeComponent", static_cast<void(core::Entity::*)(const std::string&)>(&core::Entity::removeComponent)
	);

	// ===== SCENE =====
	lua.new_usertype<core::Scene>("Scene",
		// GETTERS
		"getName", &core::Scene::getName,

		// SETTERS
		"setName", &core::Scene::setName,

		// LIFECYCLE
		"onEnable", &core::Scene::onEnable,
		"awake", &core::Scene::awake,
		"ready", &core::Scene::ready,
		"update", [](core::Scene* s, uint64_t dt) { s->update(dt); },
		"fixedUpdate", &core::Scene::fixedUpdate,
		"lateUpdate", [](core::Scene* s, uint64_t dt) { s->lateUpdate(dt); },
		"onDisable", &core::Scene::onDisable,
		"destroy", &core::Scene::destroy,
		"clearScene", &core::Scene::clearScene,

		// ENTITY MANAGEMENT
		"addEntity", &core::Scene::addEntity,
		"destroyEntity", &core::Scene::destroyEntity,
		"destroyDeadEntities", &core::Scene::destroyDeadEntities,
		"addListedEntities", &core::Scene::addListedEntities,
		"findEntityByName", &core::Scene::findEntityByName,
		"getDDOLEntities", &core::Scene::getDDOLEntities
	);

	// ===== COMPONENT =====
	lua.new_usertype<core::Component>("Component",
		// GETTERS
		"getName", &core::Component::getName,
		"getEntity", &core::Component::getEntity,
		"isEnabled", &core::Component::isEnabled,

		// SETTERS
		"setName", &core::Component::setName,
		"setEnabled", &core::Component::setEnabled,

		// LIFECYCLE
		"awake", &core::Component::awake,
		"ready", &core::Component::ready,
		"enable", &core::Component::enable,
		"disable", &core::Component::disable,
		"destroy", &core::Component::destroy,
		"update", [](core::Component* c, uint64_t dt) { c->update(dt); },
		"fixedUpdate", &core::Component::fixedUpdate,
		"lateUpdate", [](core::Component* c, uint64_t dt) { c->lateUpdate(dt); },

		// METHOD CALLING
		"call", [](core::Component* c, const std::string& method,
			sol::variadic_args args, sol::this_state s) -> sol::object {
				try {
					auto anyArgs = ScriptManager::solArgsToAnyVector(args);
					// Llamar al metodo con sus argumentos
					auto result = c->call<std::any>(method, anyArgs);

					// Si retorna empty optional (metodo no existe o devolvio void), nil
					if (!result.has_value()) return sol::nil;

					// Convertir resultado std::any -> Property -> sol::object
					Property prop = ScriptManager::anyToProperty(result.value());
					return ScriptManager::propertyToSolObject(prop, s);
				}
				catch (const std::exception& e) {
					Debug::error("[Lua] Error en Component::call('", method, "'): ", e.what());
					return sol::nil;
				}
				catch (...) {
					Debug::error("[Lua] Excepcion desconocida en Component::call('", method, "')");
					return sol::nil;
				}
		}
	);

	// ===== DEBUG TABLE =====
	lua["Debug"] = lua.create_table();
	lua["Debug"]["out"] = [](const std::string& msg) {
		Debug::out("[Lua] ", msg);
		};
	lua["Debug"]["warning"] = [](const std::string& msg) {
		Debug::warning("[Lua] ", msg);
		};
	lua["Debug"]["error"] = [](const std::string& msg) {
		Debug::error("[Lua] ", msg);
		};
	lua["Debug"]["isRelease"] = []() {
#ifdef _DEBUG
		return false;
#else
		return true;
#endif
		};

	sol::table inputT = lua.create_table();

	// Usamos offsets para que los valores de cada categoria no colisionen
	constexpr int OFFSET_MOUSE_BTN = 1000;
	constexpr int OFFSET_MOUSE_AXIS = 2000;
	constexpr int OFFSET_GP_BTN = 3000;
	constexpr int OFFSET_GP_AXIS = 4000;

#define X(name, value) inputT[#name] = static_cast<int>(input::name);
	INPUT_KEYS(X)
#undef X
#define X(name, value) inputT[#name] = OFFSET_MOUSE_BTN + static_cast<int>(input::name);
		INPUT_MOUSE_BUTTONS(X)
#undef X
#define X(name, value) inputT[#name] = OFFSET_MOUSE_AXIS + static_cast<int>(input::name);
		INPUT_MOUSE_AXES(X)
#undef X
#define X(name, value) inputT[#name] = OFFSET_GP_BTN + static_cast<int>(input::name);
		INPUT_GAMEPAD_BUTTONS(X)
#undef X
#define X(name, value) inputT[#name] = OFFSET_GP_AXIS + static_cast<int>(input::name);
		INPUT_GAMEPAD_AXES(X)
#undef X

		inputT["ANY_DEVICE"] = -1;
	inputT["KEYBOARD_ID"] = 0;
	lua["input"] = inputT;
}

void ScriptsManager::_declareTypeImpl(const std::string& typeName) const
{
	auto& lua = pImpl->lua;

	// Crear la metatabla para el tipo
	sol::table mt = lua.create_table();
	// Configurar __index para que los metodos sean accesibles
	mt["__index"] = mt;
	// Almacenar la metatabla
	pImpl->typeMethods[typeName] = mt;
	// Registrar la metatabla en la tabla global de Lua
	lua[typeName] = mt;

	Debug::out("[ScriptsManager] Tipo registrado: ", typeName);
}
