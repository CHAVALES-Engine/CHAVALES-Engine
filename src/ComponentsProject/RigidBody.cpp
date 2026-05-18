#include "RigidBody.h"
#include "PluginSDK.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"
#include "PhysicsModule.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(RigidBody);

RigidBody::RigidBody()
{
	// Getters
	registerMethod("getVelocity", [this](const std::vector<std::any>& args) {
		return getVelocity();
		});

	registerMethod("getPosition", [this](const std::vector<std::any>& args) {
		return getPosition();
		});

	registerMethod("getRotation", [this](const std::vector<std::any>& args) {
		return getRotation();
		});

	registerMethod("getMass", [this](const std::vector<std::any>& args) {
		return getMass();
		});

	registerMethod("getLinearDamping", [this](const std::vector<std::any>& args) {
		return getLinearDamping();
		});

	registerMethod("getIsKinematic", [this](const std::vector<std::any>& args) {
		return getIsKinematic();
		});

	// Setters
	registerMethod("setVelocity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVelocity(std::any_cast<core::Vector3<>>(args[0]));
		}
		return std::any();
		});

	registerMethod("setPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPosition(std::any_cast<core::Vector3<>>(args[0]));
		}
		return std::any();
		});

	registerMethod("setRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setRotation(std::any_cast<core::Quaternion<>>(args[0]));
		}
		return std::any();
		});

	registerMethod("setMass", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMass(std::any_cast<float>(args[0]));
		}
		return std::any();
		});

	registerMethod("setLinearDamping", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLinearDamping(std::any_cast<float>(args[0]));
		}
		return std::any();
		});

	// Metodos con multiples parametros
	registerMethod("AddForce", [this](const std::vector<std::any>& args) {
		if (args.size() >= 2) {
			AddForce(
				std::any_cast<core::Vector3<>>(args[0]),
				std::any_cast<char>(args[1])
			);
		}
		return std::any();
		});

	registerMethod("ClearForce", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			ClearForce(std::any_cast<char>(args[0]));
		}
		return std::any();
		});

	registerMethod("blockAxes", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			blockAxes(
				std::any_cast<bool>(args[0]),
				std::any_cast<bool>(args[1]),
				std::any_cast<bool>(args[2])
			);
		}
		return std::any();
		});

	registerMethod("blockAngles", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			blockAngles(
				std::any_cast<bool>(args[0]),
				std::any_cast<bool>(args[1]),
				std::any_cast<bool>(args[2])
			);
		}
		return std::any();
		});
}

bool RigidBody::init(const Properties& p)
{
	useGravity = getProperty<bool>(p, "useGravity");
	isKinematic = getProperty<bool>(p, "isKinematic");
	setMass(getProperty<float>(p, "mass"));
	setPosition(getProperty<core::Vector3<>>(p, "position"));
	physicsID = physics()->CreateRigidBody(getPosition(), getMass(), useGravity, isKinematic);
	if (!isKinematic)
	{
		setVelocity(getProperty<core::Vector3<>>(p, "velocity"));
		setLinearDamping(getProperty<float>(p, "damping"));
		std::vector<bool> blockAxs(3, false);
		setProperty(p, "blockAxes", blockAxs, false);
		if (blockAxs.size() >= 3) {
			blockAxes(blockAxs[0], blockAxs[1], blockAxs[2]);
		}
		std::vector<bool> blockAngs(3, false);
		setProperty(p, "blockAngles", blockAngs, false);
		if (blockAngs.size() >= 3) {
			blockAngles(blockAngs[0], blockAngs[1], blockAngs[2]);
		}
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
		physics()->SetLinearVelocity(physicsID, getVelocity());

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
	else if(isKinematic || !useGravity)
	{
		setPosition(transform->getGlobalPosition());
		setRotation(transform->getGlobalRotation());
	}
}

core::Vector3<> RigidBody::getVelocity() const {
	return physics()->GetLinearVelocity(physicsID);
}

core::Vector3<> RigidBody::getPosition() const {
	return physics()->GetPhysicsPosition(physicsID);
}

core::Quaternion<> RigidBody::getRotation() const
{
	return physics()->GetPhysicsRotation(physicsID);
}

float RigidBody::getMass() const
{
	return physics()->GetMass(physicsID);
}

void RigidBody::setVelocity(const core::Vector3<>& vel) {
	physics()->SetLinearVelocity(physicsID, vel);
}

void RigidBody::setPosition(const core::Vector3<>& pos) {
	physics()->SetPhysicsPosition(physicsID, pos);
}

void RigidBody::setRotation(const core::Quaternion<>& rot)
{
	physics()->SetPhysicsRotation(physicsID, rot);
}

void RigidBody::setMass(float mass)
{
	physics()->SetMass(physicsID, mass);
}

void RigidBody::setLinearDamping(float damping)
{
	physics()->SetLinearDamping(physicsID, damping);
}

float RigidBody::getLinearDamping() const
{
	return physics()->GetLinearDamping(physicsID);
}

void RigidBody::AddForce(const core::Vector3<>& force, char mode) {
	physics()->AddForce(physicsID, force, mode);
}

void RigidBody::ClearForce(char mode)
{
	physics()->ClearForce(physicsID, mode);
}

void RigidBody::blockAxes(bool x, bool y, bool z)
{
	physics()->BlockAxes(physicsID, x, y, z);
}

void RigidBody::blockAngles(bool x, bool y, bool z)
{
	physics()->BlockAngles(physicsID, x, y, z);
}