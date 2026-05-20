#pragma once
/**
* @file
* @brief ComponentsRegister engloba metodos para el registro de componentes dll.
*/
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <EngineAPI.h>

// forward declarations
namespace core {
	class Component; 
	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentConstructor = std::function<ComponentPtr()>;
}

class ENGINE_API ComponentRegister
{
private:
	ComponentRegister() = default;
	~ComponentRegister() = default;
public:
	/**
	* @brief Devuelve una instancia del registro
	* 
	* @return ComponentRegister& - Instancia del ComponentRegister
	*/
	static ComponentRegister& instance();
	/**
	* @brief Registra un componente en el mapa, con su constructor
	* 
	* @return bool - Se ha registrado correctamente
	*/
	bool registComponent(const std::string& name, core::ComponentConstructor ComponentConstructor);
	/**
	* @brief 
	*	Crea el componente pedido usando su funcion asignada
	* 
	* @return std::shared_ptr<core::Component> - Puntero al componente creado
	*/
	std::shared_ptr<core::Component> create(const std::string& name);
	/**
	* @brief
	*	Desregistra un componente
	* 
	* @return bool - Devuelve si se ha creado o no
	*/
	bool unregisterComponent(const std::string& name);
private:
	/**
	* @brief 
	*	Unordered map:
	*		nombre de componente (clave) - puntero a funcion constructora (valor)
	*/
	static inline std::unordered_map<std::string, core::ComponentConstructor> _components;
};
