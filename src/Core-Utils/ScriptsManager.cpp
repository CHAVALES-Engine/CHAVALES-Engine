#include "ScriptsManager.h"

#ifndef SOL_ALL_SAFETIES_ON
#define SOL_ALL_SAFETIES_ON 1
#endif
#include <sol/sol.hpp>

#include <unordered_map>
#include <iostream>

#include "Debug.h"

// patron pImpl, struct con parametros dependientes de lua
struct ScriptsManager::Impl {
	sol::state lua;
	std::unordered_map<ScriptHandle, sol::environment> envs;
};

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

void ScriptsManager::registerBindings() const
{
	auto& lua = pImpl->lua;

	// TODO: exponer API api del motor

	// registro de nuestra funciones debug
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
	// protected_function: captura el error internamente 
	// y te lo devuelve como resultado invalido, sin excepciones.
	sol::protected_function func = it->second[fn];
	if (!func.valid()) return false;
	// Comprobar el resultado de ejecucion.
	auto result = func();
	bool ok = result.valid();
	if (!ok) {
		sol::error err = result;
		Debug::error("[ScriptsManager] Error en '", fn, "': ", err.what());
	}
	return ok;
}

bool ScriptsManager::execute(ScriptHandle h, const std::string& fn, ExecuteArgs args) const {
	auto it = pImpl->envs.find(h);
	if (it == pImpl->envs.end()) return false;
	// protected_function: captura el error internamente 
	// y te lo devuelve como resultado invalido, sin excepciones.
	sol::protected_function func = it->second[fn];
	if (!func.valid()) return false;
	// Crea una tabla de lua temporal con los argumentos que son pasados al metodo
	sol::table argTable = pImpl->lua.create_table();
	int i = 1;
	for (const auto& a : args)
		std::visit([&](auto&& v) { argTable[i++] = v; }, a);
	// Comprobar el resultado de ejecucion.
	auto result = func(argTable);
	bool ok = result.valid();
	if (!ok) {
		sol::error err = result;
		Debug::error("[ScriptsManager] Error en '", fn, "': ", err.what());
	}
	return ok;
}

ScriptsManager::ScriptsManager() : pImpl(std::make_unique<Impl>())
{
}

// destructora necesita estar aqui porque tiene que conocer
// la definicion de Imp
ScriptsManager::~ScriptsManager() = default;