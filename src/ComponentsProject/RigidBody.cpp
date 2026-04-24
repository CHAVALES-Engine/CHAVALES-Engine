#include "RigidBody.h"
#include "PluginSDK.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(RigidBody);

RigidBody::RigidBody()
{
	// Getters (retornan valores)
	registerMethod("getVelocity", [this](const std::vector<std::any>& args) {
		getVelocity();
		});

	registerMethod("getPosition", [this](const std::vector<std::any>& args) {
		getPosition();
		});

	registerMethod("getRotation", [this](const std::vector<std::any>& args) {
		getRotation();
		});

	registerMethod("getMass", [this](const std::vector<std::any>& args) {
		getMass();
		});

	registerMethod("getLinearDamping", [this](const std::vector<std::any>& args) {
		getLinearDamping();
		});

	registerMethod("getId", [this](const std::vector<std::any>& args) {
		getId();
		});

	registerMethod("getIsKinematic", [this](const std::vector<std::any>& args) {
		getIsKinematic();
		});

	// Setters
	registerMethod("setVelocity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVelocity(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("setPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPosition(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("setRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setRotation(std::any_cast<core::Quaternion<>>(args[0]));
		}
		});

	registerMethod("setMass", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMass(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setLinearDamping", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLinearDamping(std::any_cast<float>(args[0]));
		}
		});

	// Metodos con multiples parametros
	registerMethod("AddForce", [this](const std::vector<std::any>& args) {
		if (args.size() >= 2) {
			AddForce(
				std::any_cast<core::Vector3<>>(args[0]),
				std::any_cast<char>(args[1])
			);
		}
		});

	registerMethod("ClearForce", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			ClearForce(std::any_cast<char>(args[0]));
		}
		});

	registerMethod("blockAxes", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			blockAxes(
				std::any_cast<bool>(args[0]),
				std::any_cast<bool>(args[1]),
				std::any_cast<bool>(args[2])
			);
		}
		});

	registerMethod("blockAngles", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			blockAngles(
				std::any_cast<bool>(args[0]),
				std::any_cast<bool>(args[1]),
				std::any_cast<bool>(args[2])
			);
		}
		});
}

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
		std::vector<bool> blockAxis(3, false); // valor por defecto
		setProperty(p, "blockAxes", blockAxis, false);
		std::vector<bool> blockAngles(3, false); // valor por defecto
		setProperty(p, "blockAxes", blockAngles, false);
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
	if (!isKinematic)
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

