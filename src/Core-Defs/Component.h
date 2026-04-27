/**
 * @file Component
 * @brief Clase base abstracta para todos los componentes del sistema EC
 *
 * TODO: MESSAGES
 */
#pragma once
#include <any>
#include <memory>
#include <functional>
#include <cstdint>
#include <optional>
#include <variant>
#include <string>
#include <unordered_map>

#include "Debug.h"
#include "ec.h"
#include "Defs.h"

namespace core
{
	/*
	 * +-----------+
	 * | COMPONENT |
	 * +-----------+
	 *
	 * --- Ejemplo de uso en lua ---
	 * Component = {
	 *		-- ejemplo de tipo basico/tipos del proyecto
	 *		atributo1 = tipo,
	 *		-- ejemplo de TAD vector
	 *		atributo2 =
	 *		{
	 *			tipo,
	 *			tipo
	 *		}
	 * }
	 *
	 * --- Ejemplo de inicializacion ---
	 * En bool init(const Properties& p):
	 *		# Ej1, asignacion:
	 * component = getProperty<tipo>(properties, "atributo1");
	 *		# Ej2, setter:
	 * return setProperty(properties, "atributo1", component);
	 *
	*/

	class Component
	{
	public:
		Component();
		~Component();

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
		void setName(std::string const& name);

		// --- GETTERS
		/**
		* @returns Devuelve la entidad propietaria del componente
		*/
		Entity* getEntity() const;
		/**
		* @returs Si esta activo o no
		*/
		bool isEnabled() const;
		/**
		 * @return El nombre del componente
		 */
		const std::string& getName() const;

		// --- LIFECYCLE
		/**
		* @brief Metodo que inicializa las variables del componente tras ser creado
		*/
		virtual bool init(const Properties& p) { return true; }

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

		/**
		* @brief Comportamiento en cada actualizaci�n por frame despues de update
		*/
		virtual void lateUpdate(uint64_t deltaTime) {}

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
		* @param warn Si mostrar por consola errores o no
		* @return Valor de la propiedad o el valor por defecto del tipo
		*/
		template <typename T>
		inline T getProperty(
			const Properties& props,
			const std::string& key,
			bool warn = true
		)
		{
			auto it = props.find(key);

			// --- comprobamos si la clave existe
			if (it == props.end())
			{
				if (warn)
					Debug::warning("COMPONENT: No se encontró el parámetro ", key, " en las propiedades del componente ", getName(), ".");
				return T(); // devolvemos valor por defecto
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
				return *pval;

			if (warn)
				Debug::error("COMPONENT: No se pudo tipar el parámetro ", key, " en las propiedades del componente ", getName(), ".");
			return T(); // devolvemos valor por defecto
		}

		/**
		* @brief Asigna a una variable el valor de una propiedad tipada
		* @param props Propiedades del componente
		* @param key Clave de la propiedad
		* @param param Variable donde se almacenara el valor
		* @param warn Si mostrar por consola errores o no
		* @return true si la propiedad existe y tiene el tipo esperado
		*/
		template <typename T>
		inline bool setProperty(
			const Properties& props,
			const std::string& key,
			T& param,
			bool warn = true
		)
		{
			auto it = props.find(key);

			// --- comprobamos si la clave existe
			if (it == props.end())
			{
				if (warn)
					Debug::warning("COMPONENT: No se encontró el parámetro ", key, " en las propiedades del componente ", getName(), ".");
				// devolvemos valor por defecto
				return false;
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
			{
				param = *pval;
				return true;
			}
			if (warn)
				Debug::error("COMPONENT: No se pudo tipar el parámetro ", key, " en las propiedades del componente ", getName(), ".");
			return false;
		}

		/**
		 * @brief Llamar un método registrado
		 * @tparam T - Tipo de retorno (void si no devuelve nada)
		 * @param method - nombre del método
		 * @param args - argumentos
		 * @return std::optional<T> si T != void, bool si T == void
		 */
		template<typename T = void>
		auto call(const std::string& method, const std::vector<std::any>& args = {}) const
		{
			auto it = _methods.find(method);
			if (it == _methods.end()) {
				Debug::warning("COMPONENT: Metodo no encontrado: ", method);
				if constexpr (std::is_void_v<T>)
					return false;
				else
					return std::optional<T>(std::nullopt);
			}
			try {
				std::any result = it->second(args);
				if constexpr (std::is_void_v<T>) return true;
				else {
					// Metodo que devuelve algo - devuelve std::optional<T>
					try {
						return std::optional<T>(std::any_cast<T>(result));
					}
					catch (const std::bad_any_cast&) {
						Debug::error("COMPONENT: No se pudo hacer el anycast en: ", method);
						return std::optional<T>(std::nullopt);
					}
				}
			}
			catch (const std::exception& e) {
				Debug::error("COMPONENT: Excepcion en: ", method);
				if constexpr (std::is_void_v<T>) return false; // Devuelve void
				else return std::optional<T>(std::nullopt);	// Devuelve algo
			}
		}
	protected:
		/**
		 * @brief Registra un metodo
		 */
		template<typename Func>
		void registerMethod(const std::string& name, Func&& f)
		{
			_methods[name] = [f = std::forward<Func>(f)](const std::vector<std::any>& args) -> std::any {
				using ReturnType = std::invoke_result_t<Func, const std::vector<std::any>&>;
				if constexpr (std::is_void_v<ReturnType>) {
					// Metodos que devuelven void
					f(args);
					return std::any();
				}
				else // Metodos que devuelven algo
					return std::any(f(args));
				};
		}

		std::unordered_map<std::string, std::function<std::any(const std::vector<std::any>&)>> _methods;

		template<typename T>
		T safeAnyCast(const std::optional<std::any>& opt) {
			if (opt.has_value()) {
				try {
					return std::any_cast<T>(opt.value());
				}
				catch (const std::bad_any_cast&) {
					Debug::error("Bad any_cast");
					return T();
				}
			}
			return T();
		}

		std::string _name;
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
