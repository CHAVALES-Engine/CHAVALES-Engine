#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "Timer.h"

class ComponentTest : public core::Component
{
	int health = 0;

	void init(const Properties& p) override
	{
		// ejemplos de inicializacion:
		// ejemplo 1
		setProperty(p, "health", health);
		// ejemplo 2
		health = getProperty<int>(p, "health");
	}
	
	void ready() override
	{
		Debug::out("Hola :-) Mi vida es ", health);
	}

	void fixedUpdate() override
	{
		if (health >= 0)
		{
			health -= 1;
			Debug::out("Augh... Me han herido y mi vida ahora es ", health);
			if (health < 0)
				morir();
		}
	} 

	void morir()
	{
		Debug::out("Oh, no... Me he muerto... x_x");
		getEntity()->setEnabled(false);
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

		Engine::instance()->addAndSetScene("scene2");
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