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
	setMass(getProperty<float>(p, "mass"));
	setPosition(getProperty<core::Vector3<>>(p, "position"));
	setVelocity(getProperty<core::Vector3<>>(p, "velocity"));
	useGravity = getProperty<bool>(p, "useGravity");

	return true;
}

void RigidBody::ready()
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	setPosition(transform->getGlobalPosition());

	if (getMass() <= 0.0f)
		setMass(1.0f);

	physicsID = _eng->createRigidBody(getPosition(), getMass(), useGravity);
	_eng->setLinearVelocity(physicsID, getVelocity());

}

void RigidBody::update(uint64_t dt)
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	transform->setGlobalPosition(getPosition());
}

core::Vector3<> RigidBody::getVelocity() {
	return _eng->getLinearVelocity(physicsID);
}

core::Vector3<> RigidBody::getPosition() {
	return _eng->getPhysicsPosition(physicsID);
}

float RigidBody::getMass()
{
	return _eng->getMass(physicsID);
}

void RigidBody::setVelocity(core::Vector3<> vel) {
	_eng->setLinearVelocity(physicsID, vel);
}

void RigidBody::setPosition(core::Vector3<> pos) {
	_eng->setPhysicsPosition(physicsID, pos);
}

void RigidBody::setMass(float mass)
{
	_eng->setMass(physicsID, mass);
}

void RigidBody::AddForce(core::Vector3<> force, char mode) {
	_eng->addForce(physicsID, force, mode);
}

void RigidBody::ClearForce(char mode)
{
	_eng->clearForce(physicsID, mode);
}

