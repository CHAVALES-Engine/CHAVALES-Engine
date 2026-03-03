/**
 * @file Component
 * @brief Clase base abstracta para todos los componentes del sistema EC
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
		virtual ~Component() {}

		// --- SETTERS
		/**
		 * @brief Asigna la entidad propietaria del componente
		 * @param e Puntero a la entidad que contiene el componente
		 */
		void setEntity(Entity* e);
		/**
		* @brief Activa o desactiva el componente
		* @param v - estado
		*/
		void setEnabled(bool v);

		// --- GETTERS
		/**
		* @returns Devuelve la entidad propietaria del componente
		*/
		const Entity* getEntity() const;
		/**
		* @returs Si esta activo o no
		*/
		bool isEnabled() const;

		// --- LIFECYCLE
		/**
		* @brief Metodo que inicializa las variables del componente tras ser creado
		*/
		virtual void init() {} // analogo al awake en unity

		/**
		* @brief Comportamiento cuando la escena comienza y ya se han inicializado el resto de entidades
		*/
		virtual void ready() {} // analogo al start en unity

		/**
		* @brief Comportamiento cuando el componente se activa
		*/
		virtual void enable() {} // analogo al onEnable en unity

		/**
		* @brief Comportamiento en cada ctualización en tiempo fijo
		*/
		virtual void fixedUpdate() {}

		/**
		* @brief Comportamiento en cada actualización por frame
		*/
		virtual void update(uint64_t deltaTime) {}

		/**
		* @brief Comportamiento de renderizado del componente
		*/
		virtual void render() const {}

		/**
		* @brief Comportamiento cuando el componente se desactiva
		*/
		virtual void disable() {}

		/**
		* @brief Comportamiento antes de que el componente se destruya
		*/
		virtual void destroy() {}

	private: 
		Entity* entity;
		bool enabled; 
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
