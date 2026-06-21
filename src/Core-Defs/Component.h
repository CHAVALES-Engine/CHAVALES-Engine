/**
 * @file Component
 * @brief Clase base abstracta para todos los componentes del sistema EC
 *
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
	/**
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
	class ComponentProperty;
	class Component
	{
	public:
		Component();
		~Component();

		/**
		* @brief Lista de propiedades del componente que se registrarán desde el .lua
		*/
		std::vector<ComponentProperty*> registeredProperties;

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
		virtual bool init(const Properties& p);

		/**
		* @brief Comportamiento cuando todos los componentes de una entidad se han inicializado pero el resto de entidades no tienen por que estar inicializadas
		*/
		virtual void awake() {}

		/**
		* @brief Comportamiento cuando la escena comienza y ya se han inicializado y despertado el resto de entidades
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

		/**
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
		* @brief DEPRACATED -> Usar SetProperty para control de errores
		* Obtiene una propiedad tipada del conjunto de propiedades. En casode error devuelve el valor por defecto del tipo
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
					Debug::warning("COMPONENT: GetProperty - No se encontró el parametro ", key, " en las propiedades del componente ", getName(), ".");
				return T(); // devolvemos valor por defecto
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
				return *pval;

			if (warn)
				Debug::error("COMPONENT: No se pudo tipar el parametro ", key, " en las propiedades del componente ", getName(), ".");
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
					Debug::error("COMPONENT: SetProperty - No se encontró el parametro ", key, " en las propiedades del componente ", getName(), ".");
				return false;
			}

			// --- comprobamos si esta tipificado a lo requerido
			if (const T* pval = std::get_if<T>(&it->second))
			{
				param = *pval;
				return true;
			}
			if (warn)
				Debug::error("COMPONENT: No se pudo tipar el parametro ", key, " en las propiedades del componente ", getName(), ".");
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
				else if constexpr (std::is_same_v<T, std::any>) {
					if (!result.has_value())
						return std::optional<std::any>(std::nullopt);
					return std::optional<std::any>(result);
				}
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
			catch (...) {
				Debug::error("COMPONENT: Excepcion desconocida en: ", method);
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
		std::string _name;
		Entity* entity;
		bool enabled;
	};

	using ComponentConstructor = std::function<std::shared_ptr<Component>()>;

	struct ComponentDescriptor
	{
		/**
		* @brief Nombre del componente
		*/
		const char* name;

		/**
		* @brief Funcion creadora de un componente
		*/
		ComponentConstructor ComponentConstructor;
	};

	struct ComponentProperty {
		std::string name;
		virtual bool setProperty(Component* comp, const Properties& p) = 0;
		virtual ~ComponentProperty() = default;
	};

	template<typename T>
	class TypeProperty : public ComponentProperty {
	private:
		T value;
	public:
		TypeProperty(const std::string& propertyName, Component* comp) {
			name = propertyName;
			if(comp != nullptr) comp->registeredProperties.push_back(this);
		}
		bool setProperty(Component* comp, const Properties& p) override {
			return comp->setProperty<T>(p, name, value);
		}

		operator T& () { return value; }
		operator const T& () const { return value; }
		T* operator&() { return &value; }
		const T* operator&() const { return &value; }
		//Operadores sobre la variable
		T& operator=(const T& newValue) { value = newValue; return value; }
		T& operator+=(const T& v) { value += v; return value; }
		T& operator-=(const T& v) { value -= v; return value; }
		T& operator*=(const T& v) { value *= v; return value; }
		T& operator/=(const T& v) { value /= v; return value; }
		T& operator++() { ++value; return value; }
		T& operator--() { --value; return value; }
		T operator++(int) { T temp = value; ++value; return temp; }
		T operator--(int) { T temp = value; --value; return temp; }
		//Operadores aritmeticos
		T operator+(const T& v) const { return value + v; }
		T operator-(const T& v) const { return value - v; }
		T operator*(const T& v) const { return value * v; }
		T operator/(const T& v) const { return value / v; }
		//arrays
		auto& operator[](size_t index) { return value[index]; }
		const auto& operator[](size_t index) const { return value[index]; }
		//Comparadores logicos
		bool operator==(const T& v) const { return value == v; }
		bool operator!=(const T& v) const { return value != v; }
		bool operator==(const TypeProperty<T>& v) const { return value == v.value; }
		bool operator!=(const TypeProperty<T>& v) const { return value != v.value; }
		bool operator<(const T& v) const { return value < v; }
		bool operator>(const T& v) const { return value > v; }
		bool operator<=(const T& v) const { return value <= v; }
		bool operator>=(const T& v) const { return value >= v; }
		//Acceso a la variable y sus metodos de tipo
		T* operator->() { return &value; }
		const T* operator->() const { return &value; }
		T& get() { return value; }
		const T& get() const { return value; }

		friend std::ostream& operator<<(std::ostream& os, const TypeProperty<T>& p) {
			os << p.value;
			return os;
		}
	}; // end of namespace
}
	/**
	 * @brief Inicializa una propiedad para su auto-lectura en el init del componente
	 */
#define PROPERTY(TYPE, NAME) \
    core::TypeProperty<TYPE> NAME = core::TypeProperty<TYPE>(#NAME, this)