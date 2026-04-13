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
	
	//cosas de lua
	_eng = Engine::instance();
	
	radius = 0.5f;
	height = 0.0f;
	//SHAPE
	// BOX
	//if (auto it = p.find("box"); it != p.end()) {
	//	shapeType = ShapeType::Box;
	//	if (auto val = std::get_if<core::Vector3<>>(&it->second))
	//		size = *val;
	//}
	// CAPSULE
	//auto it = p.find("capsule");
	//if (it != p.end())
	//{
	//	if (auto val = std::get_if<core::Vector2<>>(&it->second))
	//	{
	//		shapeType = ShapeType::Capsule;
	//		radius = val->getX();
	//		height = val->getY();
	//	}
	//}
	std::string type = getProperty<std::string>(p, "type");
	core::Vector3<> val;
	setProperty(p, "shape", val);
	if (type == "BOX")
	{
		shapeType = ShapeType::Box;
		size = val;
	}
	else if (type == "CAPSULE")
	{
		shapeType = ShapeType::Capsule;
		radius = val.getX();
		height = val.getY();
	}
	else
	{
		Debug::error("[COLLIDER] TIPO INCOMPATIBLE!!");
	}


	//DYNAMIC
	isDynamic = getProperty<bool>(p, "dynamic");
	
	//TRIGGER
	isTrigger = getProperty<bool>(p, "trigger");

	//CENTER
	setProperty(p, "center", center);

	Debug::warning(std::string("[COLLIDER] INIT ") +
		(shapeType == ShapeType::Box ? "caja" : "capsulaaa"));

	//Debug::warning("[COLLIDER] INIT this ptr = " + std::to_string((uintptr_t)this));
	return true;

	Debug::warning(
		"[PTR] " + std::to_string((uintptr_t)this) +
		" shape=" + std::to_string((int)shapeType)
	);
}

void Collider::ready()
{

	if (!entity) return;

	transform = entity->getComponent<Transform>();
	if (!transform) return;

	core::Vector3<> pos = transform->getGlobalPosition();

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
		Debug::warning(std::string("[COLLIDER] READY ") +
			(shapeType == ShapeType::Box ? "caja" : "capsulaaa"));

		switch (shapeType)
		{
		case ShapeType::Box:
			_eng->attachBoxShapeToRigidBody(physicsID, size, center, isTrigger);
			break;
		case ShapeType::Capsule:
			_eng->attachCapsuleShapeToRigidBody(physicsID, radius, height, center, isTrigger);
			break;
		}
	}
	else
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::Box:
			physicsID = _eng->createBoxCollider(size, pos + center, isDynamic, isTrigger);
			break;
		case ShapeType::Capsule:
			physicsID = _eng->createCapsuleCollider(radius, height, center, pos + center, isDynamic, isTrigger);
			break;
		}
	}
	//Debug::warning(
	//	"[PTR] " + std::to_string((uintptr_t)this) +
	//	" shape=" + std::to_string((int)shapeType)
	//);

	//Debug::warning("[COLLIDER] READY this ptr  = " + std::to_string((uintptr_t)this));
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
