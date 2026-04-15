#include "ScriptComponent.h"
//#include "Script.h"
#include "checkMLNew.h"

ScriptComponent::ScriptComponent()//: _script(std::make_unique<Script>())
{}

bool ScriptComponent::init(const Properties& p)
{
	std::string scriptPath;
	/*if (!setProperty(p, "script", scriptPath) ||
		!_script->loadScript(scriptPath))
		return false;*/
	// inicializacion de propiedades TODO
	return true;
}

void ScriptComponent::ready()
{
	//_script->executeFunction("ready");
}

void ScriptComponent::enable()
{
	//_script->executeFunction("enable");
}

void ScriptComponent::fixedUpdate()
{
	//_script->executeFunction("fixedUpdate");
}

void ScriptComponent::update(uint64_t deltaTime)
{
	//_script->executeFunction("update", deltaTime);
}

void ScriptComponent::disable()
{
	//_script->executeFunction("disable");
}

void ScriptComponent::destroy()
{
	//_script->executeFunction("destroy");
}
