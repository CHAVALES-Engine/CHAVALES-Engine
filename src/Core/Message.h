/*
 * @file
 * @brief
 */

#pragma once

#include <vector>
#include <functional>
#include "EngineAPI.h"


namespace core
{
	/**
	* @brief Clase plantilla de mensajes.
	*/
	template<typename... Args>
	class ENGINE_API Message
	{

	public:

		/**
		* @brief Constructor por defecto.
		*/
		Message() = default;
		/**
		* @brief Destructor por defecto.
		*/
		~Message() = default;

		/**
		* @brief Emite el mensaje.
		* Llama a todos los punteros a funcion registradas al mensaje con los argumentos dados.
		*
		* @param ...args - Argumentos de las funciones llamadas.
		*/
		void emit(Args... args)
		{
			for (int i = 0; i < _subscribers.size(); i++)
			{
				 _subscribers[i](args...);
			}
		}

		/**
		* @brief Suscribe un puntero a funcion al mensaje.
		*
		* @param funcptr - Funcion a suscribir.
		*/
		void subscribe(std::function < void(Args...) > func)
		{
			_subscribers.push_back(func);
		}

	private:

		/**
		* @brief Vector de punteros a funcion.
		*/
		std::vector < std::function < void(Args...) >> _subscribers;
	};
}
