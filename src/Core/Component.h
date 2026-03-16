/**
 * @file Component
 * @brief Clase base abstracta para todos los componentes del sistema EC
 *
 * TODO: MESSAGES
 */
#pragma once
#include <memory>
#include <functional>
#include <cstdint>
#include <variant>
#include <string>
#include <unordered_map>

#include "Debug.h"
#include "ec.h"
#include "EngineAPI.h"
#include "Defs.h"

namespace core
{
	class ENGINE_API Component
	{
	public:
		Component();
		virtual ~Component();

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
		Entity* getEntity() const;
		/**
		* @returs Si esta activo o no
		*/
		bool isEnabled() const;

		// --- LIFECYCLE
		/**
		* @brief Metodo que inicializa las variables del componente tras ser creado
		*/
		virtual bool init(const Properties& p) { return true;  } 

		/**
		* @brief Comportamiento cuando la escena comienza y ya se han inicializado el resto de entidades
		*/
		virtual void ready() {} // analogo al start en unity

		/**
		* @brief Comportamiento cuando el componente se activa
		*/
		virtual void enable() {} // analogo al onEnable en unity

		/**
		* @brief Comportamiento en cada ctualizaci�n en tiempo fijo
		*/
		virtual void fixedUpdate() {}

		/**
		* @brief Comportamiento en cada actualizaci�n por frame
		*/
		virtual void update(uint64_t deltaTime) {}

		///**
		//* @brief Comportamiento de renderizado del componente
		//*/
		//virtual void render() const {}

		/**
		* @brief Comportamiento cuando el componente se desactiva
		*/
		virtual void disable() {}

		/**
		* @brief Comportamiento antes de que el componente se destruya
		*/
		virtual void destroy() {}

		/**
		* @brief Obtiene una propiedad tipada del conjunto de propiedades
		* @param props Propiedades del componente
		* @param key Clave de la propiedad
		* @return Valor de la propiedad o el valor por defecto del tipo
		*/
		template <typename T>
		inline T getProperty(
			const Properties& props,
			const std::string& key
			//const T& param = T()
		)
		{
			auto it = props.find(key);

			// --- comprobamos si la clave existe
			if (it == props.end())
			{
				Debug::error("COMPONENT: No se encontró el parámetro ", key, " en las propiedades del componente.");
				return T(); // devolvemos valor por defecto
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
				return *pval;

			Debug::error("COMPONENT: No se pudo tipar el parámetro ", key, " en las propiedades del componente.");
			return T(); // devolvemos valor por defecto
		}

		/**
		* @brief Asigna a una variable el valor de una propiedad tipada
		* @param props Propiedades del componente
		* @param key Clave de la propiedad
		* @param param Variable donde se almacenara el valor
		* @return true si la propiedad existe y tiene el tipo esperado
		*/
		template <typename T>
		inline bool setProperty(
			const Properties& props,
			const std::string& key,
			T& param
		)
		{
			auto it = props.find(key);

			// --- comprobamos si la clave existe
			if (it == props.end())
			{
				Debug::error("COMPONENT: No se encontró el parámetro ", key, " en las propiedades del componente.");
				// devolvemos valor por defecto
				return false;
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
			{
				param = *pval;
				return true;
			}
			else
			{
				Debug::error("COMPONENT: No se pudo tipar el parámetro ", key, " en las propiedades del componente.");
			}
			return false;
		}

	protected:
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
