#pragma once
/**
* @file ComponentsLoader
* @brief ComponentsRegister contains methods to instantiate dll components.
*/
#include <unordered_map>
#include <string>

namespace ec {
	class Component;

	// Definimos el tipo de puntero a función estilo C
	typedef ec::Component* (*ComponentFactory)();
}

class ComponentsRegister
{
public:
	/*
	* @brief Registra un componente en el mapa, con su constructor
	*/
	bool regist(const std::string &name, ec::ComponentFactory factory);
	/*
	* @brief 
	*	Crea el componente pedido usando su funcion asignada
	* 
	* @return Component* - Puntero al componente creado
	*/
	ec::Component* create(const std::string& name);
private:
	/*
	* @brief 
	*	Unordered map:
	*		nombre de componente (clave) - puntero a funcion constructora (valor)
	*/
	std::unordered_map<std::string, ec::ComponentFactory> _components;
};

