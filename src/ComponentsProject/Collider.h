#pragma once
#include  "../../src/Core-Defs/Component.h"
#include "../../src/PhysicsModule/RigidBody.h"

namespace physx {
	class PxShape;
	class PxRigidActor;
}

/*
 * +----------+
 * | COLLIDER |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * Collider = {
 *		...
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * ...
 *		# Ej2, setter:
 * ...
 *
*/
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

	////setters
	//void SetEnabled(bool value);
	//void SetTrigger(bool value);
	//void SetRigidbody(Rigidbody* rb);

	////getters
	//bool IsEnabled() const;
	//bool IsTrigger() const;
	//Rigidbody* GetRigidbody() const;

	//core::Vector3<> GetWorldPosition();


	//setters
	void SetEnabled(bool value) {
		enabled = value;
	}

	void SetTrigger(bool value) {
		isTrigger = value;
	}

	void SetRigidbody(Rigidbody* rb) { attachedRigidbody = rb; }

	//Getters
	bool IsEnabled() const { return enabled; }
	bool IsTrigger() const { return isTrigger; }
	Rigidbody* GetRigidbody() const { return attachedRigidbody; }

	core::Vector3<> GetWorldPosition() const {
		if (attachedRigidbody)
			return attachedRigidbody->getPosition() + center;
		return center;
	}

	bool init(const Properties& p) override;

	void SetShape(physx::PxShape* s) { shape = s; }
	physx::PxShape* GetShape() const { return shape; }

	void SetActor(physx::PxRigidActor* a) { actor = a; }
	physx::PxRigidActor* GetActor() const { return actor; }
};