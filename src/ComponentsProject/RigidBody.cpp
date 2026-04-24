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

	useGravity = getProperty<bool>(p, "useGravity");
	isKinematic = getProperty<bool>(p, "isKinematic");
	physicsID = _eng->createRigidBody(getPosition(), getMass(), useGravity, isKinematic);
	setMass(getProperty<float>(p, "mass"));
	setPosition(getProperty<core::Vector3<>>(p, "position"));
	if (!isKinematic)
	{
		setVelocity(getProperty<core::Vector3<>>(p, "velocity"));
		setLinearDamping(getProperty<float>(p, "damping"));
	}
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
	if(!isKinematic)
	_eng->setLinearVelocity(physicsID, getVelocity());

}

void RigidBody::update(uint64_t dt)
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	if (!isKinematic)
	{
		transform->setGlobalPosition(getPosition());
		transform->setGlobalRotation(getRotation());
	}
	else
	{
		setPosition(transform->getGlobalPosition());
		setRotation(transform->getGlobalRotation());
	}
}

core::Vector3<> RigidBody::getVelocity() {
	return _eng->getLinearVelocity(physicsID);
}

core::Vector3<> RigidBody::getPosition() {
	return _eng->getPhysicsPosition(physicsID);
}

core::Quaternion<> RigidBody::getRotation()
{
	return _eng->getPhysicsRotation(physicsID);
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

void RigidBody::setRotation(core::Quaternion<> rot)
{
	_eng->setPhysicsRotation(physicsID, rot);
}

void RigidBody::setMass(float mass)
{
	_eng->setMass(physicsID, mass);
}

void RigidBody::setLinearDamping(float damping)
{
	_eng->setLinearDamping(physicsID, damping);
}

float RigidBody::getLinearDamping()
{
	return _eng->getLinearDamping(physicsID);
}

void RigidBody::AddForce(core::Vector3<> force, char mode) {
	_eng->addForce(physicsID, force, mode);
}

void RigidBody::ClearForce(char mode)
{
	_eng->clearForce(physicsID, mode);
}

void RigidBody::blockAxes(bool x, bool y, bool z)
{
	_eng->blockAxes(physicsID, x, y, z);
}

void RigidBody::blockAngles(bool x, bool y, bool z)
{
	_eng->blockAngles(physicsID, x, y, z);
}

