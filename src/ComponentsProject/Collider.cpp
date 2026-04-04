#include "Collider.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "../../src/Core-Defs/Entity.h"
#include "../../src/ComponentsProject/Transform.h"

REGISTER_COMPONENT(Collider);

bool Collider::init(const Properties& p)
{
	//cosas de lua
	_eng = Engine::instance();


    //SIZE
    auto itSize = p.find("size");
    if (itSize != p.end())
    {
        if (auto val = std::get_if<core::Vector3<>>(&itSize->second))
            center = *val;
    }

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

    return true;
}

void Collider::ready()
{
	//en el ready miro si tiene un rigidbody real y si lo tiene le paso al metodo los datos del rigidbody
    if (!entity) return;

    auto transform = entity->getComponent<Transform>();
    if (!transform) return;

    core::Vector3<> pos = transform->getGlobalPosition();

   // physicsID = _eng->createBoxCollider(center, pos, isDynamic);
}

void Collider::update(uint64_t deltaTime)
{
    if (!entity || physicsID == 0) return;

    auto transform = entity->getComponent<Transform>();
    if (!transform) return;

    core::Vector3<> pos = transform->getGlobalPosition();

   // _eng->setPhysicsPosition(physicsID, pos);
}
