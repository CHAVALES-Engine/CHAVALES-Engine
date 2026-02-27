#include "Entity.h"
#include "Component.h"
#include "ec.h"

ec::Entity::Entity() : 
	components(),
    alive(true),
    visible(true),
    enabled(true),
    scene(nullptr),
    groupId(),
    name()
{

}

ec::Entity::~Entity()
{
	// we delete all available components
	for (auto c : components)
	{
		c->onDestroy();
		delete c;
	}
}

void ec::Entity::setAlive(bool a) { alive = a; }
void ec::Entity::setVisible(bool v) { visible = v; }
void ec::Entity::setEnabled(bool e) { enabled = e; }
void ec::Entity::setScene(Scene* s) { scene = s; }
void ec::Entity::setGroupId(grpId_t id) { groupId = id; }
void ec::Entity::setName(const std::string& n) { name = n; }

bool ec::Entity::isAlive() const { return alive; }
bool ec::Entity::isVisible() const { return visible; }
bool ec::Entity::isEnabled() const { return enabled; }
const Scene* ec::Entity::getScene() const { return scene; }
ec::grpId_t ec::Entity::getGroupId() const { return groupId; }
//bool ec::Entity::inGroup(grpId_t id) const;
const std::string& ec::Entity::getName() const { return name; }
const std::vector<ec::Component*>& ec::Entity::getComponents() const { return components; }

void ec::Entity::init()
{
	for (Component* c : components)
	{
		c->init();
	}
}

void ec::Entity::fixedUpdate()
{
	if (!enabled) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->fixedUpdate();
	}
}

void ec::Entity::update(double deltatime)
{
	if (!enabled) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->update(deltatime);
	}
}

void ec::Entity::render() const
{
	if (!visible) return;

	for (Component* c : components)
	{
		if (c->isEnabled())
			c->render();
	}
}

void ec::Entity::destroy()
{
	alive = false;
}

void ec::Entity::enable()
{
	if (!enabled)
	{
		enabled = true;
		for (auto c : components)
			c->enable();
	}
}

void ec::Entity::disable()
{
	if (enabled)
	{
		enabled = false;
		for (auto c : components)
			c->disable();
	}
}

template <typename T, typename ... Ts>
T* ec::Entity::addComponent(Ts&&... args)
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
void ec::Entity::removeComponent()
{
	for (auto it = components.begin(); it != components.end(); ++it) 
	{
		if (dynamic_cast<T*>(*it) != nullptr) 
		{
			(*it)->onDestroy();
			delete* it;
			*it = nullptr;
			components.erase(it);
			return;
		}
	}
}

template <typename T>
T* ec::Entity::getComponent() 
{
	for (Component* c : components) 
	{
		if (T* ptr = dynamic_cast<T*>(c))
			return ptr;
	}

	return nullptr;
}

template <typename T>
bool ec::Entity::hasComponent() const
{
	return getComponent<T>() != nullptr;
}
