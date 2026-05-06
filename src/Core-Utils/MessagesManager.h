#pragma once
/**
* @file MessagesManager
* @brief Clase manejadora de Mensajes.
*		TODO: Descripcion de los mensajes
*/

#include "Message.h"
#include <unordered_map>
#include <string>
#include <any>

#include "EngineAPI.h"

namespace core
{
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
		bool createMessage(const std::string& name)
		{
			if (_messages->find(name) != _messages->end())
			{
				Debug::warning("Mensaje con nombre: \"", name, "\" ya existe.");
				return false;
			}

			_messages->emplace(name, core::Message<Args...>{});
			Debug::out("Mensaje con nombre: \"", name, "\" creado.");
			return true;
		}
		/**
		* @brief Devuelve un mensaje dado un nombre.
		*
		* @param name - Nombre del mensaje.
		*
		* @returns Message<Args...>* - Mensaje pedido. Nullptr si no existe.
		*/
		template<typename... Args>
		Message<Args...>* getMessage(const std::string& name)
		{
			auto it = _messages->find(name);
			if (it == _messages->end())
			{
				Debug::warning("Mensaje con nombre: \"", name, "\" no existe.");
				return nullptr;
			}

			return std::any_cast<core::Message<Args...>>(&it->second);
		}
		/**
		* @brief Subscribe una funcion a un mensaje
		*
		* @param name - Nombre del mensaje.
		* @param func - Funcion a suscribir.
		*
		* @returns bool - Si se ha podido suscribir.
		*/
		template<typename... Args>
		bool subscribeInMessage(const std::string& name, std::function < void(Args...) > func)
		{
			auto it = _messages->find(name);
			if (it == _messages->end())
			{
				Debug::warning("Mensaje con nombre: \"", name, "\" no existe se creara uno nuevo.");
				createMessage(name);
				it = _messages->find(name);
			}

			auto* msg = std::any_cast<core::Message<Args...>>(&it->second);
			msg->subscribe(func);
			return true;

		}
	private:
		/**
		* @brief Constructora por defecto.
		*/
		MessagesManager();
		/**
		* @brief Destructora por defecto.
		*/
		~MessagesManager();
		/**
		* @brief Mapa de Mensajes.
		*/
		std::unordered_map<std::string, std::any>* _messages;
	};
}
