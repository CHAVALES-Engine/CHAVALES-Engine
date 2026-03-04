#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "Timer.h"

class ComponentTest : public core::Component
{
	float health = 0.0f;

	void init(const Properties& p) override
	{
		// set(...)
	}

	void fixedUpdate() override
	{
		Debug::out(Debug::DebugMode::DEBUG_BOTH, "Mi vida es ", health);
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

		t = core::TimerManager::instance().createTimer(10, [this]() { this->changeScene(); });
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