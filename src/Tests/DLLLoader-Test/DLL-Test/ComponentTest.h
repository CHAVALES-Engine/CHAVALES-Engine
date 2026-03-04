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
	core::Timer t;
public:
	TimerChangescene()
	{

	}
	void ready() override
	{
		core::Debug::out("ready");

		t = core::TimerManager::instance().createTimer(10, [this]() { this->changeScene(); });
	}

	void changeScene()
	{
		core::Debug::out("CAMBIO DE ESCENA");
	}
	void fixedUpdate() override
	{
		//if (!t.isEnded())
			//core::Debug::out("Timer: ", t.id(), " - time left:", t.timeLeftSec());
	}
};

REGISTER_COMPONENT(TimerChangescene);