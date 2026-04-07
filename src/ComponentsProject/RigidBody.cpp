#include "RigidBody.h"
#include "PluginSDK.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"

REGISTER_COMPONENT(RigidBody);

bool RigidBody::init(const Properties& p)
{
	_eng = Engine::instance();

	_id = getProperty<std::string>(p, "physicsID");
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
	if (!entity || physicsID == 0) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	position = _eng->getPhysicsPosition(physicsID);
	velocity = _eng->getLinearVelocity(physicsID);

	transform->setGlobalPosition(position);
}

core::Vector3<> RigidBody::getVelocity() {
	if (physicsID != 0)
		return _eng->getLinearVelocity(physicsID);
	return velocity;
}

core::Vector3<> RigidBody::getPosition() {
	if (physicsID != 0)
		return _eng->getPhysicsPosition(physicsID);
	return position;
}

void RigidBody::setVelocity(core::Vector3<> vel) {
	velocity = vel;
	if (physicsID != 0)
		_eng->setLinearVelocity(physicsID, vel);
}

void RigidBody::setPosition(core::Vector3<> pos) {
	position = pos;
	if (physicsID != 0)
		_eng->setPhysicsPosition(physicsID, pos);
}

void RigidBody::AddForce(core::Vector3<> force) {
	if (physicsID != 0)
		_eng->addForce(physicsID, force);
}

void RigidBody::AddImpulse(core::Vector3<> impulse) {
	if (physicsID != 0)
		_eng->addImpulse(physicsID, impulse);
}