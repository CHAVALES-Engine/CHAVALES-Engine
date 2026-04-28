#include "Transform.h"

#include "Scene.h"
#include <Engine.h>
#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"
#include <cmath>

REGISTER_COMPONENT(Transform);
//
//Transform::Transform() :
//	_localPosition(),
//	_localRotation(),
//	_localScale(1),
//	_parent(nullptr),
//	_children() {
//};
//
//Transform::~Transform() {}

Transform::Transform()
{
	registerMethod("setGlobalPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setGlobalPosition(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("setLocalPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLocalPosition(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("setGlobalRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setGlobalRotation(std::any_cast<core::Quaternion<>>(args[0]));
		}
		});

	registerMethod("setLocalRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLocalRotation(std::any_cast<core::Quaternion<>>(args[0]));
		}
		});

	registerMethod("setGlobalScale", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setGlobalScale(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("setLocalScale", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLocalScale(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("lockRotationX", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			lockRotationX(std::any_cast<bool>(args[0]));
		}
		});

	registerMethod("lockRotationY", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			lockRotationY(std::any_cast<bool>(args[0]));
		}
		});

	registerMethod("lockRotationZ", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			lockRotationZ(std::any_cast<bool>(args[0]));
		}
		});

	registerMethod("getGlobalPosition", [this](const std::vector<std::any>& args) {
		return getGlobalPosition();
		});

	registerMethod("getLocalPosition", [this](const std::vector<std::any>& args) {
		return getLocalPosition();
		});

	registerMethod("getGlobalRotation", [this](const std::vector<std::any>& args) {
		return getGlobalRotation();
		});

	registerMethod("getLocalRotation", [this](const std::vector<std::any>& args) {
		return getLocalRotation();
		});

	registerMethod("getGlobalScale", [this](const std::vector<std::any>& args) {
		return getGlobalScale();
		});

	registerMethod("getLocalScale", [this](const std::vector<std::any>& args) {
		return getLocalScale();
		});

	registerMethod("getParent", [this](const std::vector<std::any>& args) {
		return getParent();
		});

	registerMethod("setParent", [this](const std::vector<std::any>& args) {
		if (args.size() == 1) {
			setParent(std::any_cast<Transform*>(args[0]));
		}
		else if (args.size() >= 2) {
			setParent(std::any_cast<Transform*>(args[0]), std::any_cast<bool>(args[1]));
		}
		});

	registerMethod("getChildren", [this](const std::vector<std::any>& args) {
		return getChildren();
		});

	registerMethod("detachChild", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			detachChild(std::any_cast<Transform*>(args[0]));
		}
		});

	registerMethod("detachChildren", [this](const std::vector<std::any>& args) {
		detachChildren();
		});

	registerMethod("translate", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			translate(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("rotateLocal", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1)
		{
			const std::any& arg = args[0];

			if (arg.type() == typeid(core::Quaternion<>)) {
				rotateLocal(std::any_cast<core::Quaternion<>>(arg));
			}
			else if (arg.type() == typeid(core::Vector3<>)) {
				rotateLocal(std::any_cast<core::Vector3<>>(arg));
			}
		}
		});

	registerMethod("rotateGlobal", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1)
		{
			const std::any& arg = args[0];

			if (arg.type() == typeid(core::Quaternion<>)) {
				rotateGlobal(std::any_cast<core::Quaternion<>>(arg));
			}
			else if (arg.type() == typeid(core::Vector3<>)) {
				rotateGlobal(std::any_cast<core::Vector3<>>(arg));
			}
		}
		});

	registerMethod("right", [this](const std::vector<std::any>& args) {
		return right();
		});

	registerMethod("up", [this](const std::vector<std::any>& args) {
		return up();
		});

	registerMethod("forward", [this](const std::vector<std::any>& args) {
		return forward();
		});

	registerMethod("LookAt", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			LookAt(std::any_cast<core::Vector3<>>(args[0]));
		}
		});

	registerMethod("getComponentInParents", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1)
		{
			return getComponentInParents(std::any_cast<std::string>(args[0]));
		}
		});

	registerMethod("getComponentsInParents", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1)
		{
			return getComponentsInParents(std::any_cast<std::string>(args[0]));
		}
		});
}

bool Transform::init(const Properties& p)
{
	_localPosition = getProperty<core::Vector3<>>(p, "position");
	core::Vector3<> _rot;
	if (!setProperty(p, "rotation", _rot, false))
	{
		if (!setProperty(p, "rotation", _localRotation))
			Debug::warning("[TRANSFORM] Rotacion no inicializada, predeterminada a ", core::ZERO);
	}
	else
	{
		_localRotation = core::Quaternion().fromEuler(_rot);
	}
	_localScale = getProperty<core::Vector3<>>(p, "scale");
	_pendingChildren = getProperty<std::vector<std::string>>(p, "children");
	//pendingChildren.clear();
	_transformID = Engine::instance()->addTransform(getEntity()->getEntityID(), getGlobalPosition(), getGlobalRotation(), getGlobalScale());
	return true;
}

void Transform::ready()
{
	//Debug::out("[TRANSFORM] ", getEntity()->getName(), " tiene ", _children.size(), " hijo", (_children.size() != 1 ? "s" : ""));
	for (const std::string& childName : _pendingChildren) {
		core::Entity* childEntity = getEntity()->getScene()->findEntityByName(childName);
		if (!childEntity)
			//Debug::warning("Transform: hijo no encontrado: ", childName);
			continue;
		if (Transform* childTransform = childEntity->getComponent<Transform>())
			childTransform->setParent(this);
	}
}

void Transform::setGlobalPosition(core::Vector3<> gp)
{
	if (_parent != nullptr)
	{
		core::Vector3<> gpp = _parent->getGlobalPosition();
		_localPosition = gp - gpp;
	}
	else
	{
		_localPosition = gp;
	}
	Engine::instance()->setTransformPosition(_transformID, gp);

	for (auto& c : _children)
	{
		c->refreshPostion();
	}
}

void Transform::setLocalPosition(const core::Vector3<>& lp)
{
	_localPosition = lp;
	Engine::instance()->setTransformPosition(_transformID, getGlobalPosition());

	for (auto& c : _children)
	{
		c->refreshPostion();
	}
}

void Transform::setGlobalRotation(const core::Quaternion<>& gr)
{
	core::Quaternion<> normalized = gr.normalized();
	if (_parent != nullptr)
	{
		core::Quaternion<> gpri = _parent->getGlobalRotation().inversed(); 
		_localRotation = (gpri * normalized).normalized();
	}
	else
	{
		_localRotation = normalized;
	}
	Engine::instance()->setTransformRotation(_transformID, normalized);

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

void Transform::setLocalRotation(const core::Quaternion<>& lr)
{
	_localRotation = lr.normalized();
	Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

void Transform::setGlobalScale(const core::Vector3<>& gs)
{
	if (_parent != nullptr)
	{
		core::Vector3<> gps = _parent->getGlobalScale();
		_localScale.set(
			gs.getX() / gps.getX(),
			gs.getY() / gps.getY(),
			gs.getZ() / gps.getZ()
		);
	}
	else
	{
		_localScale = gs;
	}
	Engine::instance()->setTransformScale(_transformID, gs);

	for (auto& c : _children)
	{
		c->refreshScale();
	}
}

void Transform::setLocalScale(const core::Vector3<>& ls)
{
	_localScale = ls;
	Engine::instance()->setTransformScale(_transformID, getGlobalScale());

	for (auto& c : _children)
	{
		c->refreshScale();
	}
}

void Transform::lockRotationX(bool v) { _lockRotX = v; }
void Transform::lockRotationY(bool v) { _lockRotY = v; }
void Transform::lockRotationZ(bool v) { _lockRotZ = v; }

core::Vector3<> Transform::getGlobalPosition() const
{
	if (_parent != nullptr)
	{
		core::Vector3<> gpp = _parent->getGlobalPosition();
		core::Quaternion<> gpr = _parent->getGlobalRotation();
		core::Vector3<> gps = _parent->getGlobalScale();

		core::Vector3<> scaledLocal(
			_localPosition.getX() * gps.getX(),
			_localPosition.getY() * gps.getY(),
			_localPosition.getZ() * gps.getZ()
		);

		return gpp + (gpr * scaledLocal);
	}

	return _localPosition;
}

core::Vector3<> Transform::getLocalPosition() const { return _localPosition; }

core::Quaternion<> Transform::getGlobalRotation() const
{
	if (_parent != nullptr)
	{
		return _parent->getGlobalRotation() * _localRotation;
	}

	return _localRotation;
}

core::Quaternion<> Transform::getLocalRotation() const { return _localRotation; }

core::Vector3<> Transform::getGlobalScale() const
{
	if (_parent != nullptr)
	{
		core::Vector3<> gps = _parent->getGlobalScale();
		return {
			_localScale.getX() * gps.getX(),
			_localScale.getY() * gps.getY(),
			_localScale.getZ() * gps.getZ()
		};
	}

	return _localScale;
}

core::Vector3<> Transform::getLocalScale() const { return _localScale; }

Transform* Transform::getParent() const { return _parent; }


void Transform::setParent(Transform* t, bool keepWorldMeasures)
{
	if (t == nullptr) return;
	if (_parent != nullptr && _parent == t) return; // si ya es padre de su hijo

	for (Transform* padre = t; padre != nullptr; padre = padre->getParent())
		if (padre == this) // de padre en padre buscamos ciclos
		{
			Debug::error("Parenting cycle detected, cannot make ", padre, " and ", t, " related.");
			return; // si lo hay no hacemos nada
		}

	core::Vector3<> globalPos;
	core::Quaternion globalRot;
	core::Vector3<> globalScale;

	if (keepWorldMeasures)
	{
		globalPos = getGlobalPosition();
		globalRot = getGlobalRotation();
		globalScale = getGlobalScale();
	}

	// si ya tiene padre
	if (_parent)
		_parent->detachChild(this);

	_parent = t;
	_parent->_children.push_back(this);

	if (keepWorldMeasures)
	{
		setGlobalPosition(globalPos);
		setGlobalRotation(globalRot);
		setGlobalScale(globalScale);
	}
	else
	{
		Engine::instance()->setTransformPosition(_transformID, getGlobalPosition());
		Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());
		Engine::instance()->setTransformScale(_transformID, getGlobalScale());
	}
}

std::vector<Transform*>& Transform::getChildren() { return _children; }

void Transform::detachChild(Transform* c)
{
	if (c == nullptr) return;

	std::vector<Transform*>::iterator it = _children.begin();
	while (it != _children.end())
	{
		if (*it == c)
		{
			// deshacemos la herencia de medidas
			core::Vector3<> worldPos = c->getGlobalPosition();
			core::Quaternion worldRot = c->getGlobalRotation();
			core::Vector3<> worldScale = c->getGlobalScale();

			c->_parent = nullptr;
			_children.erase(it);

			c->setLocalPosition(worldPos);
			c->setLocalRotation(worldRot);
			c->setLocalScale(worldScale);

			it = _children.end();
		}
		++it;
	}
}

void Transform::detachChildren()
{
	for (Transform* c : _children)
	{
		detachChild(c);
	}
	_children.clear();
}

void Transform::translate(const core::Vector3<>& t)
{
	_localPosition = _localPosition + t; 
	Engine::instance()->setTransformPosition(_transformID, getGlobalPosition());

	for (auto& c : _children)
	{
		c->refreshPostion();
	}
}

void Transform::rotateLocal(const core::Quaternion<>& q)
{
	//_localRotation = q * _localRotation; 
	_localRotation = (_localRotation * q).normalized(); 
	Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

void Transform::rotateLocal(const core::Vector3<>& v)
{
	//if (_lockRotX) v.setX(0);
	//if (_lockRotY) v.setY(0);
	//if (_lockRotZ) v.setZ(0);

	// esto hay que cambiarlo para que fromEuler sea estatico
	core::Quaternion<> q = core::Quaternion<>().fromEuler(v);

	rotateLocal(q);
	//_localRotation.rotateLocal(v);

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

void Transform::rotateGlobal(const core::Quaternion<>& q)
{
	setGlobalRotation((q * getGlobalRotation()).normalized());

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

void Transform::rotateGlobal(const core::Vector3<>& v)
{
	//if (_lockRotX) v.setX(0);
	//if (_lockRotY) v.setY(0);
	//if (_lockRotZ) v.setZ(0);

	core::Quaternion<> q = core::Quaternion<>().fromEuler(v);
	rotateGlobal(q);

	for (auto& c : _children)
	{
		c->refreshRotation();
	}
}

core::Vector3<> Transform::right() const
{
	return (getGlobalRotation() * core::Vector3<>(1, 0, 0)).normalized();
}

core::Vector3<> Transform::up() const
{
	return (getGlobalRotation() * core::Vector3<>(0, 1, 0)).normalized();
}

core::Vector3<> Transform::forward() const
{
	return (getGlobalRotation() * core::Vector3<>(0, 0, 1)).normalized();
}

void Transform::LookAt(const core::Vector3<>& target)
{
	// LAS CAMARAS DE OGRE MIRAN POR EL -Z !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	core::Vector3<> currentPosition = getGlobalPosition();
	core::Vector3<> direction = getGlobalPosition() - target;

	const float sqrMagnitude =
		direction.getX() * direction.getX() +
		direction.getY() * direction.getY() +
		direction.getZ() * direction.getZ();

	if (sqrMagnitude <= 0.000001f)
		return;
	
	direction /= core::Maths::Sqrt(sqrMagnitude);

	const float horizontalLength = core::Maths::Sqrt(
		direction.getX() * direction.getX() +
		direction.getZ() * direction.getZ()
	);

	const float yaw = std::atan2(direction.getX(), direction.getZ()) * 180.0f / M_PI;
	const float pitch = -std::atan2(direction.getY(), horizontalLength) * 180.0f / M_PI;

	core::Quaternion<> rotation;
	setGlobalRotation(rotation.fromEuler(core::Vector3<>(pitch, yaw, 0.0f)));
}

std::shared_ptr<core::Component> Transform::getComponentInParents(const std::string& name) const
{
	const Transform* parent = getParent();
	while (parent != nullptr)
	{
		core::Entity* e = parent->getEntity();
		if (e != nullptr)
		{
			auto c = e->getComponent(name);
			if (c != nullptr) 
				return c;
		}
		parent = parent->getParent();
	}

	return nullptr;
}

std::vector<std::shared_ptr<core::Component>> Transform::getComponentsInParents(const std::string& name) const
{
	std::vector<std::shared_ptr<Component>> result;
	const Transform* parent = getParent();
	while (parent != nullptr)
	{
		core::Entity* e = parent->getEntity();
		if (e != nullptr)
		{
			auto c = e->getComponent(name);
			if (c != nullptr)
				result.push_back(c);
		}
		parent = parent->getParent();
	}
	return result;
}

void Transform::refreshPostion()
{
	Engine::instance()->setTransformPosition(_transformID, getGlobalPosition());
}

void Transform::refreshRotation()
{
	Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());
}

void Transform::refreshScale()
{
	Engine::instance()->setTransformScale(_transformID, getGlobalScale());
}
