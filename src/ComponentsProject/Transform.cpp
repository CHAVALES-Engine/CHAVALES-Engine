#include "Transform.h"

#include <Scene.h> // Primero debe estar scene por que si no usa la forward declare de Scene en Entity
#include <Entity.h>
#include <Engine.h>

#include <Debug.h>
#include <PluginSDK.h>

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

bool Transform::init(const Properties& p)
{
	_localPosition = getProperty<core::Vector3<>>(p, "position");
	_localRotation = getProperty<core::Quaternion<>>(p, "rotation");
	_localScale = getProperty<core::Vector3<>>(p, "scale");
	std::string auxType = getProperty<std::string>(p, "type");
	if (auxType == "ui") {
		_type = TransformType::UI;
	}
	else {
		_type = TransformType::WORLD;
	}
	std::vector<std::string> pendingChildren = getProperty<std::vector<std::string>>(p, "children");
	for (const std::string& childName : pendingChildren) {
		core::Entity* childEntity = getEntity()->getScene()->findEntityByName(childName);
		if (!childEntity) 
			//Debug::warning("Transform: hijo no encontrado: ", childName);
			continue;
		if (Transform* childTransform = childEntity->getComponent<Transform>()) 
			childTransform->setParent(this);
	}
	//pendingChildren.clear();
	_transformID = Engine::instance()->addTransform(getEntity()->getEntityID(), getGlobalPosition(), getGlobalRotation(), getGlobalScale(),_type);
	return true;
}

void Transform::ready()
{
	//Debug::out("[TRANSFORM] ", getEntity()->getName(), " tiene ", _children.size(), " hijo", (_children.size() != 1 ? "s" : ""));
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
}

void Transform::setLocalPosition(const core::Vector3<>& lp)
{
	_localPosition = lp;
	Engine::instance()->setTransformPosition(_transformID, getGlobalPosition());
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
}

void Transform::setLocalRotation(const core::Quaternion<>& lr)
{
	_localRotation = lr.normalized();
	Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());
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
}

void Transform::setLocalScale(const core::Vector3<>& ls)
{
	_localScale = ls;
	Engine::instance()->setTransformScale(_transformID, getGlobalScale());
}

void Transform::lockRotationX(bool v) { _lockRotX = v; }
void Transform::lockRotationY(bool v) { _lockRotY = v; }
void Transform::lockRotationZ(bool v) { _lockRotZ = v; }

core::Vector3<> Transform::getGlobalPosition() const
{
	if (_parent != nullptr)
	{
		core::Vector3<> gpp = _parent->getGlobalPosition();
		// se deberia recalcular con escala y rotacion? misma duda que en setGlobalPosition
		return {
			_localPosition.getX() + gpp.getX(),
			_localPosition.getY() + gpp.getY(),
			_localPosition.getZ() + gpp.getZ(),
		};
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
}

void Transform::rotateLocal(const core::Quaternion<>& q)
{
	//_localRotation = q * _localRotation; 
	_localRotation = (_localRotation * q).normalized(); 
	Engine::instance()->setTransformRotation(_transformID, getGlobalRotation());
}

void Transform::rotateLocal(const core::Vector3<>& v)
{
	//if (_lockRotX) v.setX(0);
	//if (_lockRotY) v.setY(0);
	//if (_lockRotZ) v.setZ(0);

	//rotateLocal(core::Quaternion(v));

	// esto hay que cambiarlo para que angleAxis sea estatico
	core::Quaternion<> qx = core::Quaternion<>().angleAxis(v.getX(), core::Vector3<>(1.0f, 0.0f, 0.0f));
	core::Quaternion<> qy = core::Quaternion<>().angleAxis(v.getY(), core::Vector3<>(0.0f, 1.0f, 0.0f));
	core::Quaternion<> qz = core::Quaternion<>().angleAxis(v.getZ(), core::Vector3<>(0.0f, 0.0f, 1.0f));
	core::Quaternion<> q = qz * qy * qx;

	rotateLocal(q);
	//_localRotation.rotateLocal(v);
}

void Transform::rotateGlobal(const core::Quaternion<>& q)
{
	setGlobalRotation((q * getGlobalRotation()).normalized());
}

void Transform::rotateGlobal(const core::Vector3<>& v)
{
	//if (_lockRotX) v.setX(0);
	//if (_lockRotY) v.setY(0);
	//if (_lockRotZ) v.setZ(0);

	core::Quaternion<> qx = core::Quaternion<>().angleAxis(v.getX(), core::Vector3<>(1.0f, 0.0f, 0.0f));
	core::Quaternion<> qy = core::Quaternion<>().angleAxis(v.getY(), core::Vector3<>(0.0f, 1.0f, 0.0f));
	core::Quaternion<> qz = core::Quaternion<>().angleAxis(v.getZ(), core::Vector3<>(0.0f, 0.0f, 1.0f));
	core::Quaternion<> q = qz * qy * qx;

	rotateGlobal(q);
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
