#include "RigidBody.h"
#include "PluginSDK.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(RigidBody);

bool RigidBody::init(const Properties& p)
{
	_eng = Engine::instance();

	physicsID = getProperty<int>(p, "RBid");
	mass = getProperty<float>(p, "mass");
	position = getProperty<core::Vector3<>>(p, "position");
	velocity = getProperty<core::Vector3<>>(p, "velocity");
	useGravity = getProperty<bool>(p, "useGravity");
	gravity = getProperty<core::Vector3<>>(p, "gravity");

	return true;
}

void RigidBody::ready()
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	position = transform->getGlobalPosition();

	if (mass <= 0.0f)
		mass = 1.0f;

	physicsID = _eng->createRigidBody(position, mass, useGravity);

	_eng->setLinearVelocity(physicsID, velocity);
}

void RigidBody::update(uint64_t dt)
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	position = _eng->getPhysicsPosition(physicsID);
	velocity = _eng->getLinearVelocity(physicsID);
	if (useGravity) {
		if (gravVal < gravity.getY())
			gravVal += 0.000981;
		velocity += gravity;
	}
	transform->setGlobalPosition(position + velocity);
	/*_eng->setPhysicsPosition(physicsID, transform->getGlobalPosition());
	_eng->setLinearVelocity(physicsID, velocity);*/
}

core::Vector3<> RigidBody::getVelocity() {
	return _eng->getLinearVelocity(physicsID);
	return velocity;
}

core::Vector3<> RigidBody::getPosition() {
	return _eng->getPhysicsPosition(physicsID);
	return position;
}

void RigidBody::setVelocity(core::Vector3<> vel) {
	velocity = vel;
	_eng->setLinearVelocity(physicsID, vel);
}

void RigidBody::setPosition(core::Vector3<> pos) {
	position = pos;
	_eng->setPhysicsPosition(physicsID, pos);
}

void RigidBody::AddForce(core::Vector3<> force) {
	_eng->addForce(physicsID, force);
}

void RigidBody::AddImpulse(core::Vector3<> impulse) {
	_eng->addImpulse(physicsID, impulse);
}
