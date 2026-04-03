#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "InputFacade.h"
#include "Scene.h"
#include "TimeManager.h"
#include "../../../ComponentsProject/AudioSource.h"

class AudioSource;

class ComponentTest : public core::Component
{
	int health = 0;
	int test = 0;
	std::vector<int> vecInts;
	std::vector<float> vecFloats;
	std::vector<std::string> vecString;
	std::vector<bool> vecBools;
	std::vector<core::Vector3<>> vecVecs;
	std::vector<core::Vector2<>> vec1;
	std::vector<core::Vector4<>> vec2;
	std::vector<core::Color> vec3;
	std::vector<core::Quaternion<>> vec4;

	bool init(const Properties& p) override
	{
		// ejemplos de inicializacion:

		// ejemplo 1
		//health = getProperty<int>(p, "health");

		// ejemplo 2
		//setProperty(p, "vec1", vec1);
		//setProperty(p, "vec2", vec2);
		//setProperty(p, "vec3", vec3);
		//setProperty(p, "vec4", vec4);
		return setProperty(p, "health", health);
	}

	void ready() override
	{
		//Debug::out(">>READY<<");
		//Debug::out(">>Vector2<<");
		//for (const auto& s : vec1)
		//	Debug::out(s);
		//Debug::out(">>Vector4<<");
		//for (const auto& s : vec2)
		//	Debug::out(s);
		//Debug::out(">>Color<<");
		//for (const auto& s : vec3)
		//	Debug::out(s);
		//Debug::out(">>Quaternion<<");
		//for (const auto& s : vec4)
		//	Debug::out(s);

		Debug::out("Hola :-) Mi vida es ", health);
		Debug::out("test ", test);
	}

	void fixedUpdate() override
	{
		if (Engine::instance()->input()->isKeyPressed(input::KEY_A))
			getEntity()->getScene()->findEntityByName("entidad1")->getComponent<AudioSource>()->playSound();
		if (Engine::instance()->input()->isKeyPressed(input::KEY_S))
			getEntity()->getScene()->findEntityByName("entidad1")->getComponent<AudioSource>()->stopSound();
		/*if (health >= 0)
		{
			health -= 1;
			Debug::out("Augh... Me han herido y mi vida ahora es ", health);
			if (health < 0)
				morir();
		}*/
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