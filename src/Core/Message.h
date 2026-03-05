/*
 * @file
 * @brief
 */

#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <any>

#include "EngineAPI.h"

#include <Debug.h>


namespace core
{
	/**
	* @brief Clase plantilla de mensajes.
	*/
	template<typename... Args>
	class Message
	{
	public:

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
		int subscribe(std::function < void(Args...) > func)
		{
			_subscribers.push_back(func);
			return _subscribers.size() - 1;
		}

		bool unsubscribre(int id)
		{
			int ini = _subscribers.size();

			_subscribers.erase(_subscribers.begin() + id);

			if (_subscribers.size() >= ini)
			{
				Debug::error("Mensage con id:", id, " no se ha eliminado.");
				return false;
			}
			Debug::out("Mensage con id:", id, " eliminado.");
			return true;
		}

	private:

		/**
		* @brief Vector de punteros a funcion.
		*/
		std::vector < std::function < void(Args...) >> _subscribers;
	};


	/**
	* @brief Clase manejadora de Mensajes.
	*/
	class ENGINE_API MessagesManager
	{
	public:

		static MessagesManager& instance();

		// Eliminar copia y movimiento
		MessagesManager(const MessagesManager&) = delete;
		MessagesManager& operator=(const MessagesManager&) = delete;
		MessagesManager(MessagesManager&&) = delete;
		MessagesManager& operator=(MessagesManager&&) = delete;

		/**
		* @brief Crea un mensaje dado un nombre.
		*
		* @param name - Nombre del nuevo mensaje.
		*
		* @returns bool - True si ha sido posible crear el mensaje.
		*/
		template<typename... Args>
		bool createMessage(const std::string& name);

		/**
		* @brief Devuelve un mensaje dado un nombre.
		*
		* @param name - Nombre del mensaje.
		*
		* @returns Message<Args...>* - Mensaje pedido. Nullptr si no existe.
		*/
		template<typename... Args>
		Message<Args...>* getMessage(const std::string& name);

		template<typename... Args>
		bool subscribeInMessage(const std::string& name, std::function < void(Args...) > func);

	private:
		/**
		* @brief Constructora por defecto.
		*/
		MessagesManager() = default;
		/**
		* @brief Destructora por defecto.
		*/
		~MessagesManager() = default;
		/**
		* @brief Mapa de Mensajes.
		*/
		std::unordered_map<std::string, std::any> _messages;
	};
}
