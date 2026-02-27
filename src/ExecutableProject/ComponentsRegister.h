#pragma once
/**
* @file ComponentsLoader
* @brief ComponentsRegister contains methods to instantiate dll components.
*/
#include <unordered_map>
#include <string>


namespace core {
	class Component;

	// Definimos el tipo de puntero a funci�n estilo C
	typedef core::Component* (*ComponentFactory)();
}

class ComponentsRegister
{
public:
	/*
	* @brief Registra un componente en el mapa, con su constructor
	*/
	bool regist(const std::string &name, core::ComponentFactory factory);
	/*
	* @brief 
	*	Crea el componente pedido usando su funcion asignada
	* 
	* @return Component* - Puntero al componente creado
	*/
	core::Component* create(const std::string& name);
private:
	/*
	* @brief 
	*	Unordered map:
	*		nombre de componente (clave) - puntero a funcion constructora (valor)
	*/
	std::unordered_map<std::string, core::ComponentFactory> _components;
};

