#include "Collider.h"
#include "PluginSDK.h"
#include "PhysicsModule.h"
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

	return true;
}

void Collider::awake()
{
	createPhysics();
}

void Collider::update(uint64_t deltaTime)
{

	if (!physicsCreated) {
		createPhysics();
		return;
	}

	if (!entity || physicsID == 0 || !transform) return;

	std::vector<PhysicsEvent> events = physics()->consumeEventsFor(physicsID);
	for (auto& event : events) {
		switch (event.type) {
		case CollisionType::TriggerEnter: onTriggerEnter(event.otherEntity); break;
		case CollisionType::TriggerExit: onTriggerExit(event.otherEntity); break;
		case CollisionType::CollisionEnter: onCollisionEnter(event.otherEntity); break;
		case CollisionType::CollisionExit: onCollisionExit(event.otherEntity); break;
		}
	}
}

void Collider::enable()
{
	desiredEnabled = true;
	if (physicsCreated)
		physics()->SetActorEnabled(physicsID, true, isTrigger);
}

void Collider::disable()
{
	desiredEnabled = false;
	if (physicsCreated)
		physics()->SetActorEnabled(physicsID, false, isTrigger);
}

void Collider::onTriggerEnter(core::Entity* other)
{
	Debug::out("[TRIGGER] Trigger enter");
	_onTriggerEnter.emit(other);
}

void Collider::onTriggerExit(core::Entity* other)
{
	Debug::out("[TRIGGER] Trigger exit");
	_onTriggerExit.emit(other);
}

void Collider::onCollisionEnter(core::Entity* other) {
	Debug::out("[COLLIDER] Collision enter");
	_onCollisionEnter.emit(other);
}

void Collider::onCollisionExit(core::Entity* other) {
	Debug::out("[COLLIDER] Collision exit");
	_onCollisionExit.emit(other);

}

bool Collider::createPhysics() {

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
			Debug::warning("[COLLIDER] RigidBody ID no valido aún.");
			return false;
		}


		switch (shapeType)
		{
		case ShapeType::BOX:
			physics()->AttachBoxShape(physicsID, size, center, rotation, isTrigger);
			break;
		case ShapeType::CAPSULE:
			physics()->AttachCapsuleShape(physicsID, radius, height, center, rotation, isTrigger);
			break;
		}
	}
	else
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::BOX:
			physicsID = physics()->CreateBoxShape(size, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		case ShapeType::CAPSULE:
			physicsID = physics()->CreateCapsuleShape(radius, height, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		}
	}
	physics()->setActorEntity(physicsID, getEntity());

	physicsCreated = true;

	physics()->SetActorEnabled(physicsID, desiredEnabled, isTrigger);
	return true;
}