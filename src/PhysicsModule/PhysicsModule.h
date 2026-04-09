#pragma once
// TODO: This is an example of a library function
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"

#include "../../src/Core-Defs/Vector3.h"
#include <unordered_map>

using ComponentID = uint32_t;//ID unico de tu componente de motor
struct PhysXComponent;

class PhysicsModule
{
public:

	PhysicsModule();
	virtual ~PhysicsModule() = default;

	bool Init();
	core::Vector3<> GetPhysicsPosition(ComponentID id);
	void Update(float dt) ;


	ComponentID  CreateBoxShape(core::Vector3<> dimension, core::Vector3<> pos, bool isDynamic, bool isKinematic);
	ComponentID CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, bool isDynamic, bool isKinematic);
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);

	ComponentID CreateRigidBody(core::Vector3<> pos, float mass, bool useGravity);

	core::Vector3<> GetLinearVelocity(ComponentID id);

	void SetLinearVelocity(ComponentID id, core::Vector3<> vel);

	void AddForce(ComponentID id, core::Vector3<> force);
	void AddImpulse(ComponentID id, core::Vector3<> impulse);

	uint32_t CreateMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	void UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
private:
	ComponentID nextID = 1;
	std::unordered_map<ComponentID, PhysXComponent> physicsMap;

};


//Collider* col = ...; // tu componente de motor
//ComponentID id = CreateBoxInPhysics(gPhysics, scene, col->GetWorldPosition(), col->GetSize(), col->GetRigidbody() != nullptr);
//col->SetPhysicsID(id);