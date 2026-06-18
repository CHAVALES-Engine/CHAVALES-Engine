#include "ScriptComponent.h"
#include "Script.h"
#include "PluginSDK.h"
#include "Clock.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(ScriptComponent);

ScriptComponent::ScriptComponent() : _script(std::make_unique<Script>())
{
}

ScriptComponent::~ScriptComponent() = default;

bool ScriptComponent::init(const Properties& p)
{
	if (!setProperty(p, "script", _scriptPath) ||
		!_script->loadScript(_scriptPath))
		return false;

	bool edit = false;
	if (!setProperty(p, "edit", edit))
		return false;

	// Inyecta entity en el entorno del script para que pueda usar
	_script->setEntity(getEntity());
	// Pasa las Properties a la funcion init(properties) del Lua
	_script->executeWithProps("init", p);

	if (edit) {
		
	}

	return true;
}

void ScriptComponent::awake()
{
	try
	{
		_script->executeFunction("awake");
	}
	catch (...)
	{
	}
}
void ScriptComponent::ready() {
	try
	{
		_script->executeFunction("ready");
	}
	catch (...)
	{
	}
}
void ScriptComponent::enable() {
	try
	{
		_script->executeFunction("enable");
	}
	catch (...)
	{
	}
}
void ScriptComponent::disable() {
	try
	{
		_script->executeFunction("disable");
	}
	catch (...)
	{
	}
}
void ScriptComponent::destroy() {
	try
	{
		_script->executeFunction("destroy");
	}
	catch (...)
	{
	}
}
void ScriptComponent::fixedUpdate() {
	try
	{
		_script->executeFunction("fixedUpdate");
	}
	catch (...)
	{
	}
}

void ScriptComponent::update(uint64_t dt) {
	try
	{
		_script->executeFunction("update", { static_cast<float>(dt) });
	}
	catch (...)
	{
	}
}
void ScriptComponent::lateUpdate(uint64_t dt) {
	try
	{
		_script->executeFunction("lateUpdate", { static_cast<float>(dt) });
	}
	catch (...)
	{
	}
}