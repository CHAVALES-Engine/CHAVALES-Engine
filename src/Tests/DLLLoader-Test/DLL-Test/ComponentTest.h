#pragma once
#include <Component.h>
#include <PluginSDK.h>
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "InputFacade.h"
#include "Scene.h"
#include "TimeManager.h"
#include "../../../ComponentsProject/Transform.h"

class AudioSource;

class ComponentTest : public core::Component
{
	int velocity = 0;
	bool moveCamera = true;
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
		return setProperty(p, "velocity", velocity);
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

		/*Debug::out("Hola :-) Mi vida es ", health);
		Debug::out("test ", test);*/
		_transform = getEntity()->getComponent<Transform>();
		// bloquea el cursor
		Engine::instance()->input()->setRelativeMouseMode(false);
		_transform->lockRotationZ(true);
		Engine::input()->addEventToAction("left", input::KEY_A);
		Engine::input()->addEventToAction("left", input::BUTTON_GP_LEFT);
		Engine::input()->addEventToAction("right", input::KEY_D);
		Engine::input()->addEventToAction("right", input::BUTTON_GP_LEFT);
		Engine::input()->addEventToAction("front", input::KEY_W);
		Engine::input()->addEventToAction("front", input::BUTTON_GP_UP);
		Engine::input()->addEventToAction("back", input::KEY_S);
		Engine::input()->addEventToAction("back", input::BUTTON_GP_DOWN);

		Engine::input()->addEventToAction("lock_h", input::GAMEPAD_AXIS_RIGHT_X);
		Engine::input()->addEventToAction("lock_h", input::MOUSE_AXIS_REL_X);
		Engine::input()->addEventToAction("lock_v", input::GAMEPAD_AXIS_RIGHT_Y);
		Engine::input()->addEventToAction("lock_v", input::MOUSE_AXIS_REL_Y);
	}

	void update(uint64_t deltaTime) override
	{


		float speed = velocity * (float)deltaTime / 1000.0f;
		float mouseSensitivity = velocity/100.0f;

		if (Engine::input()->isKeyReleased(input::KEY_K))
			entity->getScene()->findEntityByName("cube2")->destroy();

		// --- Movimiento WASD
		if (Engine::input()->isKeyPressed(input::KEY_W))
			_transform->translate(_transform->forward() * -speed);
		if (Engine::input()->isKeyPressed(input::KEY_S))
			_transform->translate(_transform->forward() * speed);
		if (Engine::input()->isKeyPressed(input::KEY_A))
			_transform->translate(_transform->right() * -speed);
		if (Engine::input()->isKeyPressed(input::KEY_D))
			_transform->translate(_transform->right() * speed);

		// --- Rotacion con raton
		if (Engine::input()->isJustPressed(input::KEY_CTRL))
			moveCamera = !moveCamera;
		//Debug::out(Engine::input()->getAxis(input::MOUSE_AXIS_REL_X), "/", Engine::input()->getAxis(input::MOUSE_AXIS_REL_Y));
		if (moveCamera)
		{
			// bloquea el cursor
			Engine::input()->setRelativeMouseMode(true);
			float mouseX = Engine::input()->getActionAxis("lock_h");
			float mouseY = Engine::input()->getActionAxis("lock_v");

			if (mouseX != 0)
				_transform->rotateGlobal(core::Vector3<>(0, -mouseX * mouseSensitivity, 0));
			if (mouseY != 0)
				_transform->rotateLocal(core::Vector3<>(-mouseY * mouseSensitivity, 0, 0));
		}
		else
		{
			// bloquea el cursor
			Engine::input()->setRelativeMouseMode(false);
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