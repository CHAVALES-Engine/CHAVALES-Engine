#include "Collider.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"
#include "checkMLNew.h"
REGISTER_COMPONENT(Collider);


Collider::Collider()
{
	// Getters
	registerMethod("getCenter", [this](const std::vector<std::any>& args) {
		return getCenter();
		});

	registerMethod("getIsTrigger", [this](const std::vector<std::any>& args) {
		return getIsTrigger();
		});

	registerMethod("getSize", [this](const std::vector<std::any>& args) {
		return getSize();
		});

	registerMethod("getRotation", [this](const std::vector<std::any>& args) {
		return getRotation();
		});

	registerMethod("getId", [this](const std::vector<std::any>& args) {
		return getId();
		});

	// Metodos de colision
	registerMethod("onTriggerEnter", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			onTriggerEnter(std::any_cast<core::Entity*>(args[0]));
		}
		});

	registerMethod("onTriggerExit", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			onTriggerExit(std::any_cast<core::Entity*>(args[0]));
		}
		});

	registerMethod("onCollisionEnter", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			onCollisionEnter(std::any_cast<core::Entity*>(args[0]));
		}
		});

	registerMethod("onCollisionExit", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			onCollisionExit(std::any_cast<core::Entity*>(args[0]));
		}
		});

	// callbacks
	registerMethod("subscribeOnTriggerEnter", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			auto func = std::any_cast<std::function<void(core::Entity*)>>(args[0]);
			_onTriggerEnter.subscribe(func);
		}
		});

	registerMethod("subscribeOnTriggerExit", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			auto func = std::any_cast<std::function<void(core::Entity*)>>(args[0]);
			_onTriggerExit.subscribe(func);
		}
		});

	registerMethod("subscribeOnCollisionEnter", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			auto func = std::any_cast<std::function<void(core::Entity*)>>(args[0]);
			_onCollisionEnter.subscribe(std::any_cast<std::function<void(core::Entity*)>>(args[0]));
		}
		});

	registerMethod("subscribeOnCollisionExit", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			auto func = std::any_cast<std::function<void(core::Entity*)>>(args[0]);
			_onCollisionExit.subscribe(func);
		}
		});
}

bool Collider::init(const Properties& p)
{
	//Default
	radius = 0.5f;
	height = 0.0f;

	//SHAPE
	std::string type = getProperty<std::string>(p, "type");
	core::Vector3<> val;
	setProperty(p, "shape", val);
	if (type == "BOX")
	{
		shapeType = ShapeType::BOX;
		size = val;
	}
	else if (type == "CAPSULE")
	{
		shapeType = ShapeType::CAPSULE;
		radius = val.getX();
		height = val.getY();
	}
	else
	{
		Debug::error("[COLLIDER] TIPO INCOMPATIBLE!!");
	}

	//ROTATION
	/*rotation = getProperty<core::Quaternion<>>(p, "rotation");*/
	core::Vector3<> r;
	setProperty(p, "rotation", r);
	core::Quaternion<float> q;
	rotation = q.fromEuler(r);
	//DYNAMIC
	isDynamic = getProperty<bool>(p, "dynamic");

	//TRIGGER
	isTrigger = getProperty<bool>(p, "trigger");

	//CENTER
	setProperty(p, "center", center);

	if (!entity) return false;

	transform = entity->getComponent<Transform>();
	if (!transform) return false;

	core::Vector3<> pos = transform->getGlobalPosition();
	core::Quaternion<> rotGlob = transform->getGlobalRotation();

	rigidBody = entity->getComponent<RigidBody>();

	if (rigidBody != NULL && rigidBody->getIsKinematic() && !isDynamic) {
		Debug::warning("[COLLIDER] Collider no puede ser kinematic sin ser dinamico. Corrigiendo a dynamic=true.");
		isDynamic = true;
	}

	if (isDynamic)
	{
		if (rigidBody == NULL)
		{
			Debug::warning("[COLLIDER] Collider no puede ser dinamico sin rigidbody.");
			return false;
		}

		//coger el id
		physicsID = rigidBody->getId();
		if (physicsID == 0)
		{
			Debug::warning("[COLLIDER] RigidBody ID no valido aún. Esperando...");
			return false;
		}


		switch (shapeType)
		{
		case ShapeType::BOX:
			Engine::instance()->attachBoxShapeToRigidBody(physicsID, size, center, rotation, isTrigger);
			break;
		case ShapeType::CAPSULE:
			Engine::instance()->attachCapsuleShapeToRigidBody(physicsID, radius, height, center, rotation, isTrigger);
			break;
		}
	}
	else
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::BOX:
			physicsID = Engine::instance()->createBoxCollider(size, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		case ShapeType::CAPSULE:
			physicsID = Engine::instance()->createCapsuleCollider(radius, height, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		}
	}

	Engine::instance()->registerActorEntity(physicsID, getEntity());


	return true;
}

void Collider::ready()
{

}

void Collider::update(uint64_t deltaTime)
{
	if (!entity || physicsID == 0 || !transform) return;

	for (auto& event : Engine::instance()->consumeEvents(physicsID)) {
		switch (event.type) {
		case CollisionType::TriggerEnter: onTriggerEnter(event.otherEntity); break;
		case CollisionType::TriggerExit: onTriggerExit(event.otherEntity); break;
		case CollisionType::CollisionEnter: onCollisionEnter(event.otherEntity); break;
		case CollisionType::CollisionExit: onCollisionExit(event.otherEntity); break;
		}
	}
	//Engine::instance()->clearPhysicsEvents();
}

void Collider::enable()
{
	Engine::instance()->setActorEnabled(physicsID, true, isTrigger);
}

void Collider::disable()
{
	Engine::instance()->setActorEnabled(physicsID, false, isTrigger);
}

void Collider::onTriggerEnter(core::Entity* other)
{
	Debug::out("[TRIGGER] Trigger enter");
	_onTriggerEnter.emit(other);
	hasTriggered = true;
}

void Collider::onTriggerExit(core::Entity* other)
{
	Debug::out("[TRIGGER] Trigger exit");
	_onTriggerExit.emit(other);
	hasTriggered = false;
}

void Collider::onCollisionEnter(core::Entity* other) {
	Debug::out("[COLLIDER] Collision enter");
	_onCollisionEnter.emit(other);
	hasCollided = true;
}

void Collider::onCollisionExit(core::Entity* other) {
	Debug::out("[COLLIDER] Collision exit");
	_onCollisionExit.emit(other);
	hasCollided = false;
}

