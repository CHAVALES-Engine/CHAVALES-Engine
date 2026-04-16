#include "Collider.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"
#include "checkMLNew.h"
REGISTER_COMPONENT(Collider);



bool Collider::init(const Properties& p)
{
	_eng = Engine::instance();

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

	return true;
}

void Collider::ready()
{

	if (!entity) return;

	transform = entity->getComponent<Transform>();
	if (!transform) return;

	core::Vector3<> pos = transform->getGlobalPosition();
	core::Quaternion<> rotGlob = transform->getGlobalRotation();

	rigidBody = entity->getComponent<RigidBody>();

	if (rigidBody != NULL && rigidBody->getIsKinematic() && !isDynamic) {
		Debug::warning("[COLLIDER] Collider no puede ser kinematic sin ser dinámico. Corrigiendo a dynamic=true.");
		isDynamic = true;
	}

	if (isDynamic)
	{
		if (rigidBody == NULL)
		{
			Debug::warning("[COLLIDER] Collider no puede ser dinamico sin rigidbody.");
			return;
		}

		//coger el id
		physicsID = rigidBody->getId();
		if (physicsID == 0)
		{
			Debug::warning("[COLLIDER] RigidBody ID no válido aún. Esperando...");
			return;
		}
		

		switch (shapeType)
		{
		case ShapeType::BOX:
			_eng->attachBoxShapeToRigidBody(physicsID, size, center, rotation, isTrigger);
			break;
		case ShapeType::CAPSULE:
			_eng->attachCapsuleShapeToRigidBody(physicsID, radius, height, center, rotation, isTrigger);
			break;
		}
	}
	else
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::BOX:
			physicsID = _eng->createBoxCollider(size, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		case ShapeType::CAPSULE:
			physicsID = _eng->createCapsuleCollider(radius, height, center, pos, rotGlob, rotation, isDynamic, isTrigger);
			break;
		}
	}
}

void Collider::update(uint64_t deltaTime)
{
	if (!entity || physicsID == 0 || !transform) return;

	for (auto& event : _eng->getPhysicsEvents(physicsID)) {
		switch (event.type) {
		case CollisionType::TriggerEnter: onTriggerEnter(event.b); break;
		case CollisionType::TriggerExit: onTriggerExit(event.b); break;
		case CollisionType::CollisionEnter: onCollisionEnter(event.b); break;
		case CollisionType::CollisionExit: onCollisionExit(event.b); break;
		}
	}
	_eng->clearPhysicsEvents();
}


void Collider::onTriggerEnter(ComponentID other) 
{
	Debug::out("[TRIGGER] Trigger enter");
}

void Collider::onTriggerExit(ComponentID other) 
{
	Debug::out("[TRIGGER] Trigger exit");
}

void Collider::onCollisionEnter(ComponentID other) {
	Debug::out("[COLLIDER] Collision enter");
}

void Collider::onCollisionExit(ComponentID other) {
	Debug::out("[COLLIDER] Collision exit");
}
