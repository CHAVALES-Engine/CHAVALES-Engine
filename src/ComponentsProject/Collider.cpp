#include "Collider.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"

REGISTER_COMPONENT(Collider);

//NO FUNCIONAN LOS COLLIDERS SI NO SON DYNAMIC QUIERO TESTEAR PERO AUN NO VA COMO PARA QUE PUEDA

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
	if (itCen != p.end()) {
		if (auto val = std::get_if<bool>(&itCen->second))
			center = *val;
	}

	return true;
}

void Collider::ready()
{
	//en el ready miro si tiene un rigidbody real y si lo tiene le paso al metodo los datos del rigidbody
	if (!entity) return;

	transform = entity->getComponent<Transform>();
	if (!transform) return;

	//si es dinamico le uno un rigidbody

	core::Vector3<> pos = transform->getGlobalPosition();

	if (isKinematic && !isDynamic) {
		printf("Warning: Collider no puede ser kinematic sin ser dinámico. Corrigiendo a dynamic=true\n");
		isDynamic = true;
	}

	if (isDynamic)
	{
		bool useGravity = !isKinematic;
		physicsID = _eng->createRigidBody(pos, 10.0f, useGravity);//masa por defecto 10

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
	else
	{
		//esatico o trigger sin rigidbody
		switch (shapeType)
		{
		case ShapeType::Box:
			physicsID = _eng->createBoxCollider(center, pos + center, isDynamic, isKinematic);
			break;
		case ShapeType::Capsule:
			physicsID = _eng->createCapsuleCollider(radius, height, center, pos + center, isDynamic, isKinematic);
			break;
		}
	}

}

void Collider::update(uint64_t deltaTime)
{
	if (!entity || physicsID == 0 || !transform) return;

	if (!isDynamic /*&& transform->*/) {
		//estaticos
		core::Vector3<> pos = transform->getGlobalPosition();
		_eng->setPhysicsPosition(physicsID, pos + center);
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