#pragma once
#include "Rigidbody.h"
#include <PxPhysicsAPI.h>
using namespace physx;

class Collider
{
protected:

	bool enabled = true;
	bool isTrigger = false;

	Rigidbody* attachedRigidbody = nullptr;//si tiene rigidbody atacheado

	core::Vector3<> center = { 0,0,0 };//offset al rigidbody

	PxShape* shape = nullptr;//box,sphere,capsule
	PxRigidActor* actor = nullptr;//cosas de physx

public:

	Collider(Rigidbody* rb) { attachedRigidbody = rb; }//constructor base
	Collider(Rigidbody* rb, bool trigger) : attachedRigidbody(rb), isTrigger(trigger) {} //constructor de trigger

	//setters
	void SetEnabled(bool value) {
		enabled = value;
		//enable y !trigger = colision fisica
		//enable y trigger no es colision fisica solo deteccion
		//!enable entonces desactivado y nada
		if (shape) {
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, value && !isTrigger);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value && isTrigger);//activaria modo trigger
		}
	}

	void SetTrigger(bool value) {
		isTrigger = value;
		if (shape) {
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !value);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value);
		}
	}

	void SetRigidbody(Rigidbody* rb) { attachedRigidbody = rb; }

	//getters
	bool IsEnabled() const { return enabled; }
	bool IsTrigger() const { return isTrigger; }
	Rigidbody* GetRigidbody() const { return attachedRigidbody; }

	core::Vector3<> GetWorldPosition() {
		if (attachedRigidbody)
			return attachedRigidbody->getPosition() + center;
		return center;
	}
};