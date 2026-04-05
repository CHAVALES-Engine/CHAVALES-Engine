#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "InputFacade.h"
#include "TimeManager.h"
#include "../../../ComponentsProject/Transform.h"

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
	Transform* _transform = nullptr;
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
		_transform = getEntity()->getComponent<Transform>();
	}

	void update(uint64_t deltaTime) override
	{
		
		float speed = 5.0f * (float)deltaTime / 1000.0f;
		float rotSpeed = 90.0f * (float)deltaTime / 1000.0f;

		if (Engine::instance()->input()->isKeyPressed(input::KEY_W))
		{
			Debug::out("DELANTE ", speed);
			_transform->translate(core::Vector3<>(0, 0, -speed));
		}
		if (Engine::instance()->input()->isKeyPressed(input::KEY_S))
		{
			Debug::out("DELANTE ", speed);
			_transform->translate(core::Vector3<>(0, 0, speed));
		}

		if (Engine::instance()->input()->isKeyPressed(input::KEY_A)) 
		{
			Debug::out("ROTAR IZQUIERDA ", rotSpeed);
			_transform->rotate(core::Vector3<>(0, rotSpeed, 0)); // Y positivo = izquierda
		}
		if (Engine::instance()->input()->isKeyPressed(input::KEY_D))
		{
			Debug::out("ROTAR DERECHA ", rotSpeed);
			_transform->rotate(core::Vector3<>(0, -rotSpeed, 0)); // Y negativo = derecha
		}
	}

	void fixedUpdate() override
	{

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