#include "Collider.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "Entity.h"
#include "Transform.h"

REGISTER_COMPONENT(Collider);

bool Collider::init(const Properties& p)
{
	//cosas de lua
	_eng = Engine::instance();

	//DYNAMIC
	auto itDyn = p.find("dynamic");
	if (itDyn != p.end())
	{
		if (auto val = std::get_if<bool>(&itDyn->second))
			isDynamic = *val;
	}

	//TRIGGER
	auto itTrig = p.find("trigger");
	if (itTrig != p.end())
	{
		if (auto val = std::get_if<bool>(&itTrig->second))
			isTrigger = *val;
	}

	//CENTER
	auto itCen = p.find("center");
	if (itCen != p.end())
	{
		if (auto val = std::get_if<bool>(&itCen->second))
			center = *val;
	}

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

	//SHAPE
	switch (shapeType) {
	case ShapeType::Box:
		physicsID = _eng->createBoxCollider(center, pos + center, isDynamic);
		break;
	case ShapeType::Capsule:
		physicsID = _eng->createCapsuleCollider(radius, height, center, pos + center, isDynamic);
		break;
	}
}

	void Collider::update(uint64_t deltaTime)
	{
		if (!entity || physicsID == 0 || !transform) return;

		if (isDynamic) {
			core::Vector3<> physPos = _eng->getPhysicsPosition(physicsID);
			//como physx devuelcve la pos del collider debo restar el centro para saber donde esta realmente la entidad
			transform->setGlobalPosition(physPos - center);
		}
		else {
			//estaticos
			core::Vector3<> pos = transform->getGlobalPosition();
			_eng->setPhysicsPosition(physicsID, pos + center);
		}
	}
