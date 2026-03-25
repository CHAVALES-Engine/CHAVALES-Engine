#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "TimeManager.h"

class ComponentTest : public core::Component
{
	int health = 0;
	int test = 0;
	std::vector<int> vecInts;
	std::vector<float> vecFloats;
	std::vector<std::string> vecString;
	std::vector<bool> vecBools;
	std::vector<core::Vector3<>> vecVecs;

	bool init(const Properties& p) override
	{
		// ejemplos de inicializacion:
		
		// ejemplo 1
		//health = getProperty<int>(p, "health");

		// ejemplo 2
		return setProperty(p, "health", health)
			&& setProperty(p, "vecString", vecString)
			&& setProperty(p, "vecInts", vecInts)
			&& setProperty(p, "vecFloats", vecFloats)
			&& setProperty(p, "vecBools", vecBools)
			&& setProperty(p, "vecVecs", vecVecs);
	}
	
	void ready() override
	{
		//Debug::out(">>READY<<");
		//Debug::out(">>ints<<");
		//for (const auto& s : vecInts)
		//	Debug::out(s);
		//Debug::out(">>floats<<");
		//for (const auto& s : vecFloats)
		//	Debug::out(s);
		//Debug::out(">>strings<<");
		//for (const auto& s : vecString)
		//	Debug::out(s);
		//Debug::out(">>bools<<");
		//for (const auto& s : vecBools)
		//	Debug::out(s);
		//for (const auto& s : vecVecs)
		//	Debug::out(s);
		
		Debug::out("Hola :-) Mi vida es ", health);
		Debug::out("test ", test);
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

	bool init(const Properties& p) override
	{
		return true;
	}

	void ready() override
	{
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