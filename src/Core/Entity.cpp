#include "Entity.h"
#include "Component.h"
#include "ec.h"

core::Entity::Entity() :
	components(),
	alive(true),
	visible(true),
	enabled(true),
	scene(nullptr),
	groupId(),
	name()
{
}

core::Entity::~Entity()
{
	// we delete all available components
	for (auto c : components)
	{
		c->onDestroy();
		delete c;
	}
}

void core::Entity::setAlive(bool a) { alive = a; }
void core::Entity::setVisible(bool v) { visible = v; }
void core::Entity::setEnabled(bool e) { enabled = e; }
void core::Entity::setScene(Scene* s) { scene = s; }
void core::Entity::setGroupId(grpId_t id) { groupId = id; }
void core::Entity::setName(const std::string& n) { name = n; }

bool core::Entity::isAlive() const { return alive; }
bool core::Entity::isVisible() const { return visible; }
bool core::Entity::isEnabled() const { return enabled; }
const Scene* core::Entity::getScene() const { return scene; }
core::grpId_t core::Entity::getGroupId() const { return groupId; }
//bool core::Entity::inGroup(grpId_t id) const;
const std::string& core::Entity::getName() const { return name; }
const std::vector<core::Component*>& core::Entity::getComponents() const { return components; }

void core::Entity::init()
{
	for (Component* c : components)
	{
		c->init();
	}
}

void core::Entity::fixedUpdate()
{
	if (!enabled) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->fixedUpdate();
	}
}

void core::Entity::update(uint64_t dT)
{
	if (!enabled) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->update(dT);
	}
}

void core::Entity::render() const
{
	if (!visible) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->render();
	}
}

void core::Entity::destroy()
{
	alive = false;
}

void core::Entity::enable()
{
	if (!enabled)
	{
		enabled = true;
		for (auto c : components)
			c->enable();
	}
}

void core::Entity::disable()
{
	if (enabled)
	{
		enabled = false;
		for (auto c : components)
			c->disable();
	}
}

template <typename T, typename... Ts>
T* core::Entity::addComponent(Ts&&... args)
{
	// evitando duplicados
	// esto lo hace O(n)
	if (hasComponent<T>())
		return getComponent<T>();

	T* c = new T(std::forward<Ts>(args)...);

	c->setEntity(this);
	components.push_back(c);
	c->onCreate();
	c->init();

	return c;
}

template <typename T>
void core::Entity::removeComponent()
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		if (dynamic_cast<T*>(*it) != nullptr)
		{
			(*it)->onDestroy();
			delete*it;
			*it = nullptr;
			components.erase(it);
			return;
		}
	}
}

template <typename T>
T* core::Entity::getComponent()
{
	for (Component* c : components)
	{
		if (T* ptr = dynamic_cast<T*>(c))
			return ptr;
	}

	return nullptr;
}

template <typename T>
bool core::Entity::hasComponent() const
{
	return getComponent<T>() != nullptr;
}
