#pragma once
//#include <PxPhysicsAPI.h>
// TODO: This is an example of a library function
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"

#include <vector>
#include "../../src/ComponentsProject/Collider.h"

class PhysicsModule
{
public:

	PhysicsModule();
	virtual ~PhysicsModule() = default;

	bool Init();
	void Update(float dt) {};
	void AddCollider(Collider* col);

private:

	std::vector<Collider*> colliders;

	void CreatePhysXShape(Collider* col);

};