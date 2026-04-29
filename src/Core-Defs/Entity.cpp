#include "Entity.h"
#include "Component.h"
#include "Debug.h"
#include "ec.h"
#include "guid.h"
#include "checkMLNew.h"

namespace core
{
	Entity::Entity() : Entity("") {}

	Entity::Entity(std::string) :
		components(),
		alive(true),
		visible(true),
		enabled(true),
		scene(nullptr),
		//groupId(),
		name()
	{
	}

	Entity::~Entity()
	{
		disable();
		destroy();
	}

	void Entity::setAlive(bool a) { alive = a; }
	void Entity::setVisible(bool v) { visible = v; }
	void Entity::setEnabled(bool e) { enabled = e; } // deberia hacer enable/disable de cada componente?
	void Entity::setDontDestroyOnLoad(bool ddol) { dontDestroyOnLoad = ddol; }
	void Entity::setScene(Scene* s) { scene = s; }
	void Entity::setEntityID(ChavalesGUID id) { entityID = id; }
	//void Entity::setGroupId(grpId_t id) { groupId = id; }
	void Entity::setName(const std::string& n) { name = n; }

	bool Entity::isAlive() const { return alive; }
	bool Entity::isVisible() const { return visible; }
	bool Entity::isEnabled() const { return enabled; }

	bool Entity::getDontDestroyOnLoad() const { return dontDestroyOnLoad; }

	Scene* Entity::getScene() const { return scene; }
	ChavalesGUID Entity::getEntityID() const { return entityID; }
	//grpId_t Entity::getGroupId() const { return groupId; }
	//bool core::Entity::inGroup(grpId_t id) const;
	const std::string& Entity::getName() const { return name; }
	const std::vector<std::shared_ptr<Component>>& Entity::getComponents() const { return components; }
	size_t Entity::getComponentCount() const { return components.size(); }

	std::shared_ptr<Component> Entity::getComponent(const std::string& name) const {
		for (auto& c : components) {
			if (c->getName() == name)
				return c;
		}
		return nullptr;
	}

	void Entity::removeComponents() 
	{
		if (!components.empty())
		{
			for (auto c : components)
			{
				if (c != nullptr)
				{
					c->disable();
					c->destroy();
				}
			}
			components.clear();
		}
	}

	void Entity::removeComponent(const std::string& name)
	{
		for (auto it = components.begin(); it != components.end(); )
		{
			if ((*it)->getName() == name)
			{
				(*it)->disable();
				(*it)->destroy();
				it = components.erase(it); 
			}
			else
			{
				++it;
			}
		}
	}

	//void Entity::init(const Properties& p)
	//{
	//	for (std::shared_ptr<Component>& c : components)
	//	{
	//		c->init(p);
	//	}
	//}

	void Entity::awake()
	{
		for (std::shared_ptr<Component>& c : components)
		{
			c->awake();
		}
	}

	void Entity::ready()
	{
		for (std::shared_ptr<Component>& c : components)
		{
			c->ready();
		}
	}

	void Entity::fixedUpdate()
	{
		if (!enabled) return;

		// Copiar los componentes para iterar de forma segura
		std::vector<std::shared_ptr<Component>> componentsCopy = components;

		for (auto& c : componentsCopy)
		{
			if (c && c->isEnabled())
				c->fixedUpdate();
		}
	}

	void Entity::update(uint64_t dT)
	{
		if (!enabled) return;

		// Copiar los componentes para iterar de forma segura
		std::vector<std::shared_ptr<Component>> componentsCopy = components;

		for (auto& c : componentsCopy)
		{
			if (c && c->isEnabled())
				c->update(dT);
		}
	}

	void Entity::lateUpdate(uint64_t dT)
	{
		if (!enabled) return;

		// Copiar los componentes para iterar de forma segura
		std::vector<std::shared_ptr<Component>> componentsCopy = components;

		for (auto& c : componentsCopy)
		{
			if (c && c->isEnabled())
				c->lateUpdate(dT);
		}
	}

	void Entity::destroy()
	{
		alive = false;
		removeComponents();
	}

	void Entity::enable()
	{
		if (!enabled)
		{
			enabled = true;
			for (std::shared_ptr<Component>& c : components)
				c->enable();
		}
	}

	void Entity::disable()
	{
		if (enabled)
		{
			enabled = false;
			for (std::shared_ptr<Component>& c : components)
				c->disable();
		}
	}

	Component* Entity::addComponent(std::shared_ptr<Component> c)
	{
		c->setEntity(this);
		c->enable();
		//c->ready();
		components.push_back(std::move(c));
		return components.back().get();
	}
}
