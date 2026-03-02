#pragma once
/**
* @file ComponentsLoader
* @brief ComponentsRegister contains methods to instantiate dll components.
*/
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

// forward declarations
namespace core {
	class Component; 
	using ComponentPtr = std::unique_ptr<Component>;
	using ComponentConstructor = std::function<ComponentPtr()>;
}

class ComponentRegister
{
public:
	/*
	* @brief Devuelve una instancia del registro
	* 
	* @return ComponentRegister& - Instancia del ComponentRegister
	*/
	static ComponentRegister& instance();
	/*
	* @brief Registra un componente en el mapa, con su constructor
	* 
	* @return bool - Se ha registrado correctamente
	*/
	bool registComponent(const std::string& name, core::ComponentConstructor ComponentConstructor);
	/*
	* @brief 
	*	Crea el componente pedido usando su funcion asignada
	* 
	* @return std::unique_ptr<core::Component> - Puntero al componente creado
	*/
	template <typename... Args>
	std::unique_ptr<core::Component> create(const std::string& name, Args&& ... args);
	/*
	* @brief
	*	Desregistra un componente
	* 
	* @return bool - Devuelve si se ha creado o no
	*/
	bool unregisterComponent(const std::string& name);
private:
	/*
	* @brief 
	*	Unordered map:
	*		nombre de componente (clave) - puntero a funcion constructora (valor)
	*/
	static inline std::unordered_map<std::string, core::ComponentConstructor> _components;
};

/*
	* @brief
	*	Struct que al crearse registra el componente de tipo T en el
	*	registro local del plugin
	*/
template<typename T>
struct AutoRegisterComponent {
	/*
	* @brief
	*	Constructor explicito que aniade una funcion constructora
	*	lambda del objeto T (En nuestro caso, clases hijas de component)
	*/
	explicit AutoRegisterComponent(const std::string& name) {
		ComponentRegister::instance().registComponent(name, []() {
			return std::make_unique<T>();
			});
	}
};

/*
* @brief
*	Macro para anyadir un componente en el registro
* @param TYPE - Clase a registrar
*/
#define REGISTER_COMPONENT(TYPE) \
    inline static ::AutoRegisterComponent<TYPE> reg_##TYPE(#TYPE)