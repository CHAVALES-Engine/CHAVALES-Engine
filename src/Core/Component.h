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
		void setEnabled(bool v);

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
		virtual void init() {} // analogo al awake en unity

		/*
		* @brief
		*/
		virtual void ready() {} // analogo al start en unity

		/*
		* @brief
		*/
		virtual void enable() {} // analogo al onEnable en unity

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
		* @brief Comportamiento cuando el componente se desactiva.
		*/
		virtual void disable() {}

		/*
		* @brief Comportamiento cuando el componente se destruye
		*/
		virtual void destroy() {}

		template <typename T, typename... Ts>
		static std::shared_ptr<Component> create(Ts&&... args);

	private: // we allow direct use these fields from subclasses

		//friend

		Entity* entity; // a pointer to the entity, should not be deleted on destruction
		bool enabled; // if the component is currently active
	};


	using ComponentConstructor = std::function<std::shared_ptr<Component>()>;

	struct ComponentDescriptor
	{
		/*
		* @brief Nombre del componente
		*/
		const char* name;

		/*
		* @brief Funcion creadora de un componente
		*/
		ComponentConstructor ComponentConstructor;
	};
} // end of namespace
