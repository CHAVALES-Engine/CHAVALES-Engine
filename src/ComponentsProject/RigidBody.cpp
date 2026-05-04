#include "RigidBody.h"
#include "PluginSDK.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"
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
	//_eng = Engine::instance();

	useGravity = getProperty<bool>(p, "useGravity");
	isKinematic = getProperty<bool>(p, "isKinematic");
	physicsID = Engine::instance()->createRigidBody(getPosition(), getMass(), useGravity, isKinematic);
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
		Engine::instance()->setLinearVelocity(physicsID, getVelocity());

}

void RigidBody::update(uint64_t dt)
{
	if (!entity) return;

	auto transform = entity->getComponent<Transform>();
	if (!transform) return;

	if (!isKinematic)
	{
		core::Vector3<> physicsPos = getPosition();

		if (firstFrame)
		{
			smoothedPosition = physicsPos;
			firstFrame = false;
		}

		float smoothFactor = 0.35f;

		smoothedPosition =
			smoothedPosition + (physicsPos - smoothedPosition) * smoothFactor;

		transform->setGlobalPosition(smoothedPosition);
		transform->setGlobalRotation(getRotation());
	}
	else
	{
		setPosition(transform->getGlobalPosition());
		setRotation(transform->getGlobalRotation());
	}
}

core::Vector3<> RigidBody::getVelocity() {
	return Engine::instance()->getLinearVelocity(physicsID);
}

core::Vector3<> RigidBody::getPosition() {
	return Engine::instance()->getPhysicsPosition(physicsID);
}

core::Quaternion<> RigidBody::getRotation()
{
	return Engine::instance()->getPhysicsRotation(physicsID);
}

float RigidBody::getMass()
{
	return Engine::instance()->getMass(physicsID);
}

void RigidBody::setVelocity(core::Vector3<> vel) {
	Engine::instance()->setLinearVelocity(physicsID, vel);
}

void RigidBody::setPosition(core::Vector3<> pos) {
	Engine::instance()->setPhysicsPosition(physicsID, pos);
}

void RigidBody::setRotation(core::Quaternion<> rot)
{
	Engine::instance()->setPhysicsRotation(physicsID, rot);
}

void RigidBody::setMass(float mass)
{
	Engine::instance()->setMass(physicsID, mass);
}

void RigidBody::setLinearDamping(float damping)
{
	Engine::instance()->setLinearDamping(physicsID, damping);
}

float RigidBody::getLinearDamping()
{
	return Engine::instance()->getLinearDamping(physicsID);
}

void RigidBody::AddForce(core::Vector3<> force, char mode) {
	Engine::instance()->addForce(physicsID, force, mode);
}

void RigidBody::ClearForce(char mode)
{
	Engine::instance()->clearForce(physicsID, mode);
}

void RigidBody::blockAxes(bool x, bool y, bool z)
{
	Engine::instance()->blockAxes(physicsID, x, y, z);
}

void RigidBody::blockAngles(bool x, bool y, bool z)
{
	Engine::instance()->blockAngles(physicsID, x, y, z);
}

