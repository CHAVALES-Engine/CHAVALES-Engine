#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "Timer.h"

class ComponentTest : public core::Component
{
	void fixedUpdate() override
	{
		Debug::out(Debug::DebugMode::DEBUG_BOTH, "Esto es el FixedUpdate de un componente de tipo ComponentTest y mi entidad se llama ", getEntity()->getName(), "\n");
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

	int tic = 1000;

public:
	TimerChangescene()
	{
		
	}
	void ready() override
	{
		Debug::out("ready");

		//t = core::TimerManager::createTimer(10, [this]() { this->changeScene(); });
	}

	void changeScene()
	{
		Debug::out("CAMBIO DE ESCENA");

		//Engine::instance()->addAndSetScene("scene2");
	}

	void fixedUpdate() override
	{
		tic -= 1;

		Debug::out(tic);

		if (tic <= 0) changeScene();

		Debug::out("Timer: ", t.id(), " - time left:", t.timeLeftSec());
	}
};

REGISTER_COMPONENT(TimerChangescene);