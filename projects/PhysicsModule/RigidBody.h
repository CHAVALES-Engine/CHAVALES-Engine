#pragma once
#include "../../src/Core/Vector3.h"
class Rigidbody {
public:

	float mass = 1.0f;

	core::Vector3<> position = { 0,0,0 };
	core::Vector3<> velocity = { 0,0,0 };

	core::Vector3<> accumulatedForce = { 0,0,0 };

	bool useGravity = true;

	core::Vector3<> gravity = { 0,-9.80665f,0 };

	core::Vector3<> getVelocity() { return velocity; };
	core::Vector3<> getPosition() { return position; };

	void setVelocity(core::Vector3<> vel) { velocity = vel; };
	void setPosition(core::Vector3<> pos) { position = pos; };

	void AddForce(core::Vector3<> force) {
		accumulatedForce += force;
	}

	void AddImpulse(core::Vector3<> impulse) {
		velocity += impulse * (1.0f / mass);
	}

	//HAY QUE LLAMARLO EN UPDATE DE PHYSICS MODULE CUANDO LO HAYA
	void Update(float dt) {

		if (useGravity)
			accumulatedForce += gravity * mass;

		core::Vector3<> acceleration = accumulatedForce * (1.0f / mass);

		velocity += acceleration * dt;
		position += velocity * dt;

		accumulatedForce = { 0,0,0 };
	}
};