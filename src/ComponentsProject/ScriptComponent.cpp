#include "ScriptComponent.h"
#include "Script.h"
#include "PluginSDK.h"
#include "Clock.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(ScriptComponent);

ScriptComponent::ScriptComponent() : _script(std::make_unique<Script>())
{}

ScriptComponent::~ScriptComponent() = default;

bool ScriptComponent::init(const Properties& p)
{
	std::string scriptPath;
	if (!setProperty(p, "script", scriptPath) ||
		!_script->loadScript(scriptPath))
		return false;
	// inicializacion de propiedades TODO

	_script->executeFunction("init");
	return true;
}

void ScriptComponent::ready()
{
	if (!_script->executeFunction("ready"))
		Debug::warning("[Component: ]");
}

void ScriptComponent::enable()
{
	_script->executeFunction("enable");
}

void ScriptComponent::fixedUpdate()
{
	//_script->executeFunction("fixedUpdate");
}

void ScriptComponent::update(uint64_t deltaTime)
{
	//_script->executeFunction("update", { (int)deltaTime });
}

void ScriptComponent::disable()
{
	_script->executeFunction("disable");
}

void ScriptComponent::destroy()
{
	_script->executeFunction("destroy");
}
