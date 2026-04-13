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

	bool useGravity = true;
	bool isKinematic;

	// MÉTODOS
	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;

	core::Vector3<> getVelocity();
	core::Vector3<> getPosition();
	float getMass();

	void setVelocity(core::Vector3<> vel);
	void setPosition(core::Vector3<> pos);
	void setMass(float mass);

	void setLinearDamping(float damping);
	float getLinearDamping();

	void AddForce(core::Vector3<> force, char mode);
	void ClearForce(char mode);

	uint32_t getId() { return physicsID; };
	bool getIsKinematic() { return isKinematic; };

};