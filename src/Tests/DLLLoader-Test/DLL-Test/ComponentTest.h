#pragma once
#include <Component.h>
#include <PluginSDK.h>

#include "AudioSource.h"
#include "Camera.h"
#include "Debug.h"
#include "Engine.h"
#include "Entity.h"
#include "InputFacade.h"
#include "Scene.h"
#include "TimerManager.h"
#include "Collider.h"
#include "Transform.h"

//PRELOAD_RESOURCE("mesh/girl.fbx");
//PRELOAD_RESOURCE("texture/bake_girl.png");
//PRELOAD_RESOURCE("mesh/arena2.fbx");
//PRELOAD_RESOURCE("mesh/arena.fbx");


class ComponentTest : public core::Component
{
public:
	int velocity = 0;
	int device;
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
	std::shared_ptr<Component> _transform = nullptr;
	core::Entity* _esfera = nullptr;

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
		setProperty(p, "device", device);
		return setProperty(p, "velocity", velocity);
	}

	void awake() override
	{
		Input()->addEventToAction("left", input::KEY_A, input::KEYBOARD_ID);
		Input()->addEventToAction("left", input::BUTTON_GP_LEFT, device);
		Input()->addEventToAction("right", input::KEY_D, input::KEYBOARD_ID);
		Input()->addEventToAction("right", input::BUTTON_GP_RIGHT, device);
		Input()->addEventToAction("front", input::KEY_W, input::KEYBOARD_ID);
		Input()->addEventToAction("front", input::BUTTON_GP_UP, device);
		Input()->addEventToAction("back", input::KEY_S, input::KEYBOARD_ID);
		Input()->addEventToAction("back", input::BUTTON_GP_DOWN, device);

		Input()->addEventToAction("lock_h", input::GAMEPAD_AXIS_RIGHT_X, device);
		Input()->addEventToAction("lock_h", input::MOUSE_AXIS_REL_X, input::KEYBOARD_ID);
		Input()->addEventToAction("lock_v", input::GAMEPAD_AXIS_RIGHT_Y, device);
		Input()->addEventToAction("lock_v", input::MOUSE_AXIS_REL_Y, input::KEYBOARD_ID);
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
		_transform = getEntity()->getComponent("Transform");
		// bloquea el cursor
		Input()->setRelativeMouseMode(false);
		if (_transform)
		{
			_transform->call("lockRotationZ", { true });
			_transform->call("LookAt", { core::Vector3<>(0, 150, 0) });
		}

		_esfera = getEntity()->getScene()->findEntityByName("esfera");
		if (!_esfera) {
			Debug::error("[ComponentTest] No encontrada la esfera");
			return;
		}

		auto col_esfera = _esfera->getComponent<Collider>();
		if (!col_esfera) {
			Debug::error("[ComponentTest] La esfera no tiene Collider");
			return;
		}

		col_esfera->_onCollisionEnter.subscribe([](core::Entity* other) {
			Debug::out("SPHERE");
			});

		core::Entity* cube = getEntity()->getScene()->findEntityByName("cube");
		if (!cube) {
			Debug::error("[ComponentTest] No encontrado el cube");
			return;
		}

		auto col_cube = cube->getComponent<Collider>();
		if (!col_cube) {
			Debug::error("[ComponentTest] El cube no tiene Collider");
			return;
		}

		col_cube->_onCollisionEnter.subscribe([](core::Entity* other) {
			Debug::out("CUBE");
			});
		/*
		call("subscribeOnCollisionEnter",
			{
		);*/

	}

	void update(uint64_t deltaTime) override
	{

		if (!Input()->isDeviceConnected(device)) return;

		float speed = velocity * (float)deltaTime / 1000.0f;
		float mouseSensitivity = velocity / 100.0f;
		if (Input()->isJustPressed(input::KEY_R))
		{
			Engine::instance()->requestSceneChange("scene_base");
		}
		if (Input()->isJustPressed(input::KEY_K)) {
			//entity->getScene()->findEntityByName("cube2")->destroy();
			entity->getScene()->findEntityByName("esfera")->getComponent<AudioSource>()->playSound();
			if (std::shared_ptr<Camera> camera = getEntity()->getScene()->
				findEntityByName("camera")->getComponent<Camera>())
			{
				float mouseX = Input()->getAxis(input::MOUSE_AXIS_X);
				float mouseY = Input()->getAxis(input::MOUSE_AXIS_Y);
				/*float mouseX = 0;
				float mouseY =0;*/
				core::Vector2 mousePos(mouseX, mouseY);
				core::Vector3<> rayDir;

				core::Vector3<> rayOrigin = camera->screenToWorld(mousePos,
					Engine::instance()->getWindowWidth(),
					Engine::instance()->getWindowHeight(),
					rayDir);

				//Debug::out("Mouse: " + std::to_string(mouseX) + ", " + std::to_string(mouseY));
				//Debug::out("Window: " + std::to_string(Engine::instance()->getWindowWidth()) + ", " + std::to_string(Engine::instance()->getWindowHeight()));
				//Debug::out("RayOrigin: " + std::to_string(rayOrigin.getX()) + ", " + std::to_string(rayOrigin.getY()) + ", " + std::to_string(rayOrigin.getZ()));
				//Debug::out("RayDir: " + std::to_string(rayDir.getX()) + ", " + std::to_string(rayDir.getY()) + ", " + std::to_string(rayDir.getZ()));
				//Debug::out("FOVy: " + std::to_string(camera->getFOVy()));
				//Debug::out("Near: " + std::to_string(camera->getNearClipDistance()));

				RayInfo rayInfo;
				if (Engine::instance()->rayCast(rayOrigin, rayDir, 10000.0f, rayInfo))
				{
					Debug::out("HitPos: " + std::to_string(rayInfo.hitPos.getX()) + ", " + std::to_string(rayInfo.hitPos.getY()) + ", " + std::to_string(rayInfo.hitPos.getZ()));
					core::Entity* e = Engine::instance()->instantiatePrefab("prefabs/testPoint");
					e->getComponent<Transform>()->setGlobalPosition(rayInfo.hitPos);
				}
				else
				{
					Debug::out("NO HIT");
				}
			}
		}
		if (Input()->isJustPressed(input::KEY_O)) {
			auto sphere = Engine::instance()->getScene()->findEntityByName("esfera");
			if (sphere)
			{
				auto rb = sphere->getComponent("RigidBody");

				if (rb)
				{
					rb->call("AddForce", { core::Vector3<>(0, 100, 0),'I' });
				}
			}
		}
		// --- Movimiento WASD
		if (!_transform)
			_transform = getEntity()->getComponent("Transform");

		if (!_transform) return;
		auto forward = _transform->call<core::Vector3<>>("forward");
		auto right = _transform->call<core::Vector3<>>("right");

		if (Input()->isActionPressed("front", device))
			_transform->call("translate", { forward.value() * -speed });
		if (Input()->isActionPressed("back", device))
			_transform->call("translate", { forward.value() * speed });
		if (Input()->isActionPressed("left", device))
			_transform->call("translate", { right.value() * -speed });
		if (Input()->isActionPressed("right", device))
		{
			Debug::error("H>ola");
			_transform->call("translate", { right.value() * speed });
		}

		//Debug::error(Input()->isActionPressed("right", device));

		// --- Rotacion con raton
		if (Input()->isJustPressed(input::KEY_CTRL))
			moveCamera = !moveCamera;
		//Debug::out(Input()->getAxis(input::MOUSE_AXIS_REL_X), "/", Input()->getAxis(input::MOUSE_AXIS_REL_Y));
		if (moveCamera)
		{
			// bloquea el cursor
			Input()->setRelativeMouseMode(true);
			float mouseX = Input()->getActionAxis("lock_h", device);
			float mouseY = Input()->getActionAxis("lock_v", device);

			if (mouseX != 0)
				_transform->call("rotateGlobal", { core::Vector3<>(0, -mouseX * mouseSensitivity, 0) });
			if (mouseY != 0)
				_transform->call("rotateLocal", { core::Vector3<>(-mouseY * mouseSensitivity, 0, 0) });
		}
		else
		{
			// bloquea el cursor
			Input()->setRelativeMouseMode(false);
		}
		if (_esfera != nullptr)
		{
			//_transform->LookAt(_esfera->getComponent<Transform>()->getGlobalPosition());
		}

	};

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

		Engine::instance()->requestSceneChange("scene2");
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

class InitialTest : public core::Component
{
	void ready() override
	{
		Debug::out("READY INITIAL");
		Engine::instance()->requestSceneChange("scene_base");
	}

	void update(uint64_t deltaTime) override
	{
	}
};

REGISTER_COMPONENT(InitialTest);

class ChangeTest : public core::Component
{
	std::string toScene;

public:
	bool init(const Properties& p) override
	{
		return setProperty(p, "scene", toScene);
	}

	void ready() override
	{
		Debug::out("READY CHANGE");
		Engine::instance()->setWindowMaximizable(false);
		Engine::instance()->setWindowResizable(false);
	}

	void update(uint64_t deltaTime) override
	{
		if (Input()->isJustPressed(input::KEY_F11))
		{
			const bool targetFullscreen = !Engine::instance()->isFullscreen();
			Debug::out(Engine::instance()->setFullscreen(targetFullscreen) ? "CAMBIO OK" : "ERROR");
			//Engine::instance()->requestSceneChange(toScene);
		}
	}
};

REGISTER_COMPONENT(ChangeTest);

class PersistentTest : public core::Component
{
	void ready() override
	{
		Debug::out("READY PERSISTENT");
	}

	void update(uint64_t deltaTime) override
	{
		//Debug::out("UPDATE PERSISTENT");
	}
};

REGISTER_COMPONENT(PersistentTest);
