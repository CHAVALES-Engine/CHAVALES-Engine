#pragma once
// TODO: This is an example of a library function
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"

#include "../../src/Core-Defs/Vector3.h"
#include <unordered_map>

using ComponentID = uint32_t; // ID único de tu componente de motor
struct PhysXComponent;

class PhysicsModule
{
public:

	PhysicsModule();
	virtual ~PhysicsModule() = default;

	bool Init();
	void Update(float dt) {};

	ComponentID  CreateBoxShape(core::Vector3<> dimension, core::Vector3<> pos, bool isDynamic);
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);

private:
	ComponentID nextID = 1;
	std::unordered_map<ComponentID, PhysXComponent> physicsMap;

};


//Collider* col = ...; // tu componente de motor
//ComponentID id = CreateBoxInPhysics(gPhysics, scene, col->GetWorldPosition(), col->GetSize(), col->GetRigidbody() != nullptr);
//col->SetPhysicsID(id);