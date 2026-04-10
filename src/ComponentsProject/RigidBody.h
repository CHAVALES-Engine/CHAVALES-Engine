#pragma once
#include "../../src/Core-Defs/Vector3.h"
#include "../../src/Core-Defs/Component.h"

class Engine;

class RigidBody : public core::Component {
public:
	// VARIABLES
	Engine* _eng = nullptr;

	float gravVal = 0;

	unsigned int physicsID = 0;

	float mass = 1.0f;

	core::Vector3<> position = { 0,0,0 };
	core::Vector3<> velocity = { 0,0,0 };

	bool useGravity = true;
	core::Vector3<> gravity = { 0,-9.80665f,0 };

	// MÉTODOS
	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;

	core::Vector3<> getVelocity();
	core::Vector3<> getPosition();

	void setVelocity(core::Vector3<> vel);
	void setPosition(core::Vector3<> pos);

	void AddForce(core::Vector3<> force);
	void AddImpulse(core::Vector3<> impulse);
};