#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Entity.h"
#include "Timer.h"

class ComponentTest : public core::Component
{
	void fixedUpdate() override
	{
		core::Debug::out(core::Debug::DebugMode::DEBUG_BOTH, "Esto es el FixedUpdate de un componente de tipo ComponentTest y mi entidad se llama ", getEntity()->getName(), "\n");
	}
};

REGISTER_COMPONENT(ComponentTest);

class ComponenteChavalTest : public core::Component
{

};

REGISTER_COMPONENT(ComponenteChavalTest);

class TimerChangescene : public core::Component
{
public:
	TimerChangescene()
	{
		core::TimerManager::createTimer(10, [this]() { this->changeScene(); });
	}
	void changeScene()
	{
		
	}
};

REGISTER_COMPONENT(TimerChangescene);
