#include "Entity.h"
#include "Component.h"
#include "Debug.h"
#include "ec.h"

namespace core
{
	Entity::Entity() :
		components(),
		alive(true),
		visible(true),
		enabled(true),
		scene(nullptr),
		groupId(),
		name()
	{
	}

	Entity::~Entity()
	{
		// we delete all available components
		for (std::shared_ptr<Component>& c : components)
		{
			c->destroy();
			//delete c;
		}
	}

	void Entity::setAlive(bool a) { alive = a; }
	void Entity::setVisible(bool v) { visible = v; }
	void Entity::setEnabled(bool e) { enabled = e; }
	void Entity::setScene(Scene* s) { scene = s; }
	void Entity::setGroupId(grpId_t id) { groupId = id; }
	void Entity::setName(const std::string& n) { name = n; }

	bool Entity::isAlive() const { return alive; }
	bool Entity::isVisible() const { return visible; }
	bool Entity::isEnabled() const { return enabled; }
	const Scene* Entity::getScene() const { return scene; }
	grpId_t Entity::getGroupId() const { return groupId; }
	//bool core::Entity::inGroup(grpId_t id) const;
	const std::string& Entity::getName() const { return name; }
	const std::vector<std::shared_ptr<Component>>& Entity::getComponents() const { return components; }

	void Entity::init()
	{
		for (std::shared_ptr<Component>& c : components)
		{
			c->init();
		}
	}

	void Entity::fixedUpdate()
	{
		if (!enabled) return;

		for (std::shared_ptr<Component>& c : components)
		{
			if (c->isEnabled())
				c->fixedUpdate();
		}
	}

	void Entity::update(uint64_t dT)
	{
		if (!enabled) return;

		for (std::shared_ptr<Component>& c : components)
		{
			if (c->isEnabled())
				c->update(dT);
		}
	}

	void Entity::render() const
	{
		if (!visible) return;

		for (const std::shared_ptr<Component>& c : components)
		{
			if (c->isEnabled())
				c->render();
		}
	}

	void Entity::destroy()
	{
		alive = false;
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
		//c->ready();
		//c->init();

		components.push_back(std::move(c));
		return components.back().get();
	}

	template <typename T, typename... Ts>
	T* Entity::addComponent(Ts&&... args)
	{
		// evitando duplicados
		// esto lo hace O(n)
		if (hasComponent<T>())
			return getComponent<T>();

		std::shared_ptr<Component> c = std::make_shared<T>(std::forward<Ts>(args)...);
		//T* c = new T(std::forward<Ts>(args)...);

		c->setEntity(this);
		c->init();
		c->ready();

		T* c_ref = c.get();
		components.push_back(std::move(c));
		return c_ref;
	}

	template <typename T>
	void Entity::removeComponent()
	{
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			if (dynamic_cast<T*>(it->get()) != nullptr)
			{
				(*it)->destroy();
				components.erase(it);
				return;
			}
		}
	}

	template <typename T>
	T* Entity::getComponent()
	{
		for (std::shared_ptr<Component>& c : components)
		{
			if (T* ptr = dynamic_cast<T*>(c.get()))
				return ptr;
		}
		return nullptr;
	}

	template <typename T>
	bool Entity::hasComponent() const
	{
		for (const auto& c : components)
		{
			if (dynamic_cast<T*>(c.get()) != nullptr)
				return true;
		}
		return false;
	}
}
