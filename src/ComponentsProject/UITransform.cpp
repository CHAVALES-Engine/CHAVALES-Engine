#include "UITransform.h"
#include "Scene.h"
#include <Engine.h>
#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"


REGISTER_COMPONENT(UITransform);

UITransform::UITransform()
{
	registerMethod("setPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPosition(std::any_cast<core::Vector2<>>(args[0]));
		}
		});
	registerMethod("setDimension", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setDimension(std::any_cast<core::Vector2<>>(args[0]));
		}
		});
	registerMethod("setRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setRotation(std::any_cast<float>(args[0]));
		}
		});
	registerMethod("setZbuffer", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setZbuffer(std::any_cast<int>(args[0]));
		}
		});
	registerMethod("getPosition", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getPosition();
		}
		});
	registerMethod("getDimension", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getDimension();
		}
		});
	registerMethod("getRotation", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getRotation();
		}
		});
	registerMethod("getZBuffer", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getZBuffer();
		}
		});
	registerMethod("setParent", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setParent(std::any_cast<UITransform*>(args[0]));
		}
		});
	registerMethod("getParent", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getParent();
		}
		});
	registerMethod("getChildren", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getChildren();
		}
		});
	registerMethod("detachChild", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			detachChild(std::any_cast<UITransform*>(args[0]));
		}
		});
	registerMethod("detachChildren", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			detachChildren();
		}
		});
	registerMethod("getComponentInParents", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			return getComponentInParents(std::any_cast<std::string>(args[0]));
		}
		});
}

bool UITransform::init(const Properties& p)
{
	_position = getProperty<core::Vector2<>>(p, "position");
	_dimension = getProperty<core::Vector2<>>(p, "dimension");
	_rotation = getProperty<float>(p, "rotation");
	_zBuffer = getProperty<int>(p, "zBuffer");
	std::vector<std::string> children = getProperty<std::vector<std::string>>(p, "children");
	for (const std::string& name : children)
	{
		core::Entity* e = getEntity()->getScene()->findEntityByName(name);
		if (!e) continue;

		if (std::shared_ptr<UITransform> t = e->getComponent<UITransform>())
			t->setParent(this);
	}
	_UItransformID = Engine::instance()->addUITransform(getEntity()->getEntityID(),getPosition(), getZBuffer(),getDimension(), getRotation());

	return true;
}



void UITransform::setPosition(const core::Vector2<>& pos)
{
	_position = pos;
	Engine::instance()->setUITransformPos(_UItransformID, getPosition());
}

void UITransform::setDimension(const core::Vector2<>& dim)
{
	_dimension = dim;
	Engine::instance()->setUITransformDimension(_UItransformID, dim);
}

void UITransform::setRotation(float r)
{
	_rotation = r;
	Engine::instance()->setUITransformRotation(_UItransformID, r);
}

void UITransform::setZbuffer(int zBuff)
{
	_zBuffer = zBuff;
	Engine::instance()->setUITransformZBuffer(_UItransformID, zBuff);
}

core::Vector2<> UITransform::getPosition() const {
	return _position;
}
core::Vector2<> UITransform::getDimension() const {
	return _dimension;
}
float UITransform::getRotation() const {
	return _rotation;
}

int UITransform::getZBuffer() const
{
	return _zBuffer;
}

void UITransform::setParent(UITransform* t)
{
	if (!t || t == _parent) return;

	if (_parent)
		_parent->detachChild(this);

	_parent = t;
	_parent->_children.push_back(this);
}

UITransform* UITransform::getParent() const {
	return _parent;
}

std::vector<UITransform*>& UITransform::getChildren() {
	return _children;
}

void UITransform::detachChild(UITransform* uT)
{
	if (!uT) return;

	auto it = std::find(_children.begin(), _children.end(), uT);
	if (it != _children.end())
	{
		uT->_parent = nullptr;
		_children.erase(it);
	}
}
void UITransform::detachChildren() {
	for (auto* c : _children)
		c->_parent = nullptr;

	_children.clear();
}

std::shared_ptr<core::Component> UITransform::getComponentInParents(const std::string& name) const {
	const UITransform* parent = getParent();
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
std::vector<std::shared_ptr<core::Component>> UITransform::getComponentsInParents(const std::string& name) const {
	std::vector<std::shared_ptr<Component>> result;
	const UITransform* parent = getParent();
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
