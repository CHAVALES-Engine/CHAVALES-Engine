#include "Transform.h"

#include "Debug.h"

namespace core
{
	Transform::Transform() :
		_localPosition(),
		_localRotation(),
		_localScale(1),
		_parent(nullptr),
		_children() {};

	Transform::~Transform() {}

	void Transform::setGlobalPosition(Vector3<> gp)
	{
		if (_parent != nullptr)
		{
			Vector3<> gpp = _parent->getGlobalPosition();
			_localPosition = gp - gpp;
		}
		else
		{
			_localPosition = gp;
		}
	}

	void Transform::setLocalPosition(Vector3<> lp) { _localPosition = lp; }

	void Transform::setGlobalRotation(Quaternion gr)
	{
		if (_parent != nullptr)
		{
			//Quaternion gpri = _parent->getGlobalRotation().inversa(); // TODO
			//_localRotation = gpri * gr;
		}
		else
		{
			_localRotation = gr;
		}
	}

	void Transform::setLocalRotation(Quaternion lr) { _localRotation = lr; }

	void Transform::setGlobalScale(Vector3<> gs)
	{
		if (_parent != nullptr)
		{
			Vector3<> gps = _parent->getGlobalScale();
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
	}

	void Transform::setLocalScale(Vector3<> ls) { _localScale = ls; }

	Vector3<> Transform::getGlobalPosition() const
	{
		if (_parent != nullptr)
		{
			Vector3<> gpp = _parent->getGlobalPosition();
			// se deberia recalcular con escala y rotacion? misma duda que en setGlobalPosition
			return {
				_localPosition.getX() + gpp.getX(),
				_localPosition.getY() + gpp.getY(),
				_localPosition.getZ() + gpp.getZ(),
			};
		}

		return _localPosition;
	}

	Vector3<> Transform::getLocalPosition() const { return _localPosition; }

	Quaternion Transform::getGlobalRotation() const
	{
		if (_parent != nullptr)
		{
			//return _parent->getGlobalRotation() * _localRotation;
		}

		return _localRotation;
	}

	Quaternion Transform::getLocalRotation() const { return _localRotation; }

	Vector3<> Transform::getGlobalScale() const
	{
		if (_parent != nullptr)
		{
			Vector3<> gps = _parent->getGlobalScale();
			return {
				_localScale.getX() * gps.getX(),
				_localScale.getY() * gps.getY(),
				_localScale.getZ() * gps.getZ()
			};
		}

		return _localScale;
	}

	Vector3<> Transform::getLocalScale() const { return _localScale; }

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

		Vector3<> globalPos;
		Quaternion globalRot;
		Vector3<> globalScale;

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
				Vector3<> worldPos = c->getGlobalPosition();
				Quaternion worldRot = c->getGlobalRotation();
				Vector3<> worldScale = c->getGlobalScale();

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

	void Transform::translate(Vector3<> t)
	{
		_localPosition = _localPosition + t;
	}

	void Transform::rotate(Quaternion q)
	{
		//_localRotation = q * _localRotation; // en este orden
	}

	void Transform::rotate(Vector3<> v)
	{
		//Quaternion q(v); 
		//rotate(q);
	}

	Vector3<> Transform::right() const
	{
		//return (getGlobalRotation() * Vector3<>(1, 0, 0)).normalized();
	}

	Vector3<> Transform::up() const
	{
		//return (getGlobalRotation() * Vector3<>(0, 1, 0)).normalized();
	}

	Vector3<> Transform::forward() const
	{
		//return (getGlobalRotation() * Vector3<>(0, 0, 1)).normalized();
	}
}
