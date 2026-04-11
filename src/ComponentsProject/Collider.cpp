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

	//SHAPE
	// BOX
	if (auto it = p.find("box"); it != p.end()) {
		shapeType = ShapeType::Box;
		if (auto val = std::get_if<core::Vector3<>>(&it->second))
			size = *val;
	}
	// CAPSULE
	auto it = p.find("capsule");
	if (it != p.end()) {
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, Properties>) {
				radius = std::get<float>(arg.at("radius"));
				height = std::get<float>(arg.at("height"));
			}}, it->second);
	}

	//DYNAMIC
	auto itDyn = p.find("dynamic");
	if (itDyn != p.end()) {
		if (auto val = std::get_if<bool>(&itDyn->second))
			isDynamic = *val;
	}
	//KINEMATIC
	if (auto it = p.find("kinematic"); it != p.end()) {
		if (auto val = std::get_if<bool>(&it->second))
			isKinematic = *val;
	}
	//TRIGGER
	auto itTrig = p.find("trigger");
	if (itTrig != p.end()) {
		if (auto val = std::get_if<bool>(&itTrig->second))
			isTrigger = *val;
	}

	//CENTER
	auto itCen = p.find("center");
	if (auto val = std::get_if<core::Vector3<>>(&itCen->second))
		center = *val;

	return true;
}

void Collider::ready()
{
	if (!entity) return;

	transform = entity->getComponent<Transform>();
	if (!transform) return;

	core::Vector3<> pos = transform->getGlobalPosition();

	//!!!IMPORTANTE QUE LA KINEMATIC LA MANEJO YO TENGO QUE MIRAR ESTO
	if (isKinematic && !isDynamic) {
		printf("Warning: Collider no puede ser kinematic sin ser dinámico. Corrigiendo a dynamic=true\n");
		isDynamic = true;
	}


	//CUANDO SEA KINEMATIC TENGO QUE USAR UN RIGIDBODY KINEMATIC PERO AUN NO EXISTE
	rigidBody = entity->getComponent<RigidBody>();
	if (isDynamic )
	{
		if (rigidBody == NULL)
		{
			printf("Warning: Collider no puede ser dinamico sin rigidbody ");
			return;
		}

		//coger el id
		physicsID = rigidBody->getId();

		switch (shapeType)
		{
		case ShapeType::Box:
			_eng->attachBoxShapeToRigidBody(physicsID, size, center);
			break;
		case ShapeType::Capsule:
			_eng->attachCapsuleShapeToRigidBody(physicsID, radius, height, center);
			break;
		}
	}
	else//ETO EXPLOTA
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::Box:
			physicsID = _eng->createBoxCollider(size, pos + center, isDynamic, isKinematic, isTrigger);
			break;
		case ShapeType::Capsule:
			physicsID = _eng->createCapsuleCollider(radius, height, center, pos + center, isDynamic, isKinematic, isTrigger);
			break;
		}
	}
}

void Collider::update(uint64_t deltaTime)
{
	if (!entity || physicsID == 0 || !transform) return;

	if (isKinematic/*&& transform->*/) {
		//estaticos
		core::Vector3<> pos = transform->getGlobalPosition();
		core::Quaternion rot = transform->getGlobalRotation();
		_eng->setPhysicsTransform(physicsID, pos + center, rot);//rotaciones y posicion
	}

	for (auto& event : _eng->getPhysicsEvents(physicsID)) {
		switch (event.type) {
		case CollisionType::TriggerEnter: onTriggerEnter(event.b); break;
		case CollisionType::TriggerExit: onTriggerExit(event.b); break;
		case CollisionType::CollisionEnter: onCollisionEnter(event.b); break;
		case CollisionType::CollisionExit: onCollisionExit(event.b); break;
		}
	}
}


void Collider::onTriggerEnter(ComponentID other) {

}

void Collider::onTriggerExit(ComponentID other) {

}

void Collider::onCollisionEnter(ComponentID other) {

}

void Collider::onCollisionExit(ComponentID other) {

}
