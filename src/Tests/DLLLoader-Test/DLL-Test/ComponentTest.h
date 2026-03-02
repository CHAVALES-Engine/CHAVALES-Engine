#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"

class ComponentTest : public core::Component
{
	void fixedUpdate() override
	{
		core::Debug::out(core::Debug::DebugMode::DEBUG_BOTH, "Esto es el FixedUpdate de un componente de tipo ComponentTest y mi entidad se llama ", getEntity(), "\n");
	}
};

REGISTER_COMPONENT(ComponentTest);

class ComponenteChavalTest : public core::Component
{

};

REGISTER_COMPONENT(ComponenteChavalTest);
