#pragma once
#include <Component.h>
#include "../../src/PhysicsModule/RigidBody.h"
//#include "../../src/PhysicsModule/PhysicsModule.h"
//using namespace physx;

namespace physx {
	class PxShape;
	class PxRigidActor;
	class PxShapeFlag;
}

class Collider : public core::Component
{
protected:

	bool enabled = true;
	bool isTrigger = false;

	Rigidbody* attachedRigidbody = nullptr;//si tiene rigidbody atacheado

	core::Vector3<> center = { 0,0,0 };//offset al rigidbody

	physx::PxShape* shape = nullptr;//box,sphere,capsule
	physx::PxRigidActor* actor = nullptr;//cosas de physx

public:
	Collider() {};
	Collider(Rigidbody* rb); //constructor base
	Collider(Rigidbody* rb, bool trigger); //constructor de trigger

	~Collider() {};

	//setters
	void SetEnabled(bool value);
	void SetTrigger(bool value);
	void SetRigidbody(Rigidbody* rb);

	//getters
	bool IsEnabled() const;
	bool IsTrigger() const;
	Rigidbody* GetRigidbody() const;

	core::Vector3<> GetWorldPosition();

	bool init(const Properties& p) override;
};