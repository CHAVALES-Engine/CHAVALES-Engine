/*
 * @file
 * @brief
 *
 * TODO: MESSAGES
 */
#pragma once
#include <memory>
#include <functional>

#include "ec.h"

namespace core
{
	class Component
	{
	public:
		Component();
		// Destroys the component.
		// Careful! entity should not be destroyed
		virtual ~Component() {}

		// --- SETTERS
		// This method is used to set the entity in which the component is
		// installed. 
		void setEntity(Entity* e);
		/*
		* @brief
		*/
		void enable();
		/*
		* @brief
		*/
		void disable();

		// --- GETTERS
		/*
		* @brief
		*/
		const Entity* getEntity() const;
		/*
		* @brief
		*/
		bool isEnabled() const;

		// --- LIFECYCLE
		/*
		* @brief
		*/
		virtual void onCreate() {}

		// We assume that init will be called when adding a
		// component to an entity, immediately after setEntity.
		virtual void init() {}

		/*
		* @brief
		*/
		virtual void onEnable() {}

		/*
		* @brief
		*/
		virtual void fixedUpdate() {}

		// A component has an update method, and by default it
		// does nothing -- some components will not override this
		// method.
		virtual void update(uint64_t deltaTime) {}

		// A component has a render method, and by default it
		// does nothing -- some components will not override this
		// method.
		virtual void render() const {}

		/*
		* @brief
		*/
		virtual void onDisable() {}

		/*
		* @brief 
		*/
		virtual void onDestroy() {}

		template <typename T, typename... Ts>
		static std::unique_ptr<Component> create(Ts&&... args);

	private: // we allow direct use these fields from subclasses

		//friend

		Entity* entity; // a pointer to the entity, should not be deleted on destruction
		bool enabled; // if the component is currently active

		void (Component::*createFunc)() = &Component::onCreate;
		void (Component::*destroyFunc)() = &Component::onDestroy;
	};


	using ComponentConstructFunc = std::function<std::unique_ptr<Component>()>;

	struct ComponentDescriptor
	{
		/*
		* @brief Nombre del componente
		*/
		const char* name;

		/*
		* @brief Funcion creadora de un componente
		*/
		ComponentConstructFunc factory;
	};
} // end of namespace
