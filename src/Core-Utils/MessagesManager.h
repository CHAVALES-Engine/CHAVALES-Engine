#pragma once
/**
* @file MessagesManager
* @brief Clase de manejo de Mensajes.
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
		* @param persistant - Si persiste entre escenas o no.
		*
		* @returns bool - True si ha sido posible crear el mensaje.
		*/
		template<typename... Args>
		bool createMessage(const std::string& name, bool persistant)
		{
			if (_messages.find(name) != _messages.end())
			{
				Debug::warning("[MessageManager] Mensaje con nombre: \"", name, "\" ya existe. Elimina los suscritos anteriormente.");
				clearMessage(name);
				return false;
			}

			std::pair<bool, std::any> aux(persistant, core::Message<Args...>{});
			_messages.emplace(name, aux);
			Debug::out("[MessageManager] Mensaje con nombre: \"", name, "\" creado.");
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
			auto it = _messages.find(name);
			if (it == _messages.end())
			{
				Debug::warning("[MessageManager] Mensaje con nombre: \"", name, "\" no existe.");
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
			auto it = _messages.find(name);
			if (it == _messages.end())
			{
				Debug::warning("[MessageManager] Mensaje con nombre: \"", name, "\" no existe se creara uno nuevo.");
				createMessage(name, false);
				it = _messages.find(name);
			}

			core::Message<Args...>* msg = std::any_cast<core::Message<Args...>>(&it->second);
			if (msg) {
				msg->subscribe(func);
				return true;
			}
			Debug::error("[MessageManager] No se pudo subscribir al mensaje");
			return false;

		}

		/**
		 * @brief Borra los mensajes actuales
		 */
		void clearMessages()
		{
			for (auto [n, _] : _messages)
				clearMessage(n);
		}
		/**
		* @brief Elimina un mensaje y todos sus subscribers.
		* Llamar al destruir una escena para limpiar sus callbacks.
		*/
		void clearMessage(const std::string& name)
		{
			if (_destroyed) return;
			_messages.erase(name);
		}
		/**
		* @brief Elimina los mensajes que no tienen que persistir al cambiar de escena.
		* Llamar al destruir una escena para limpiar los callbacks que no se quiere que persistan entre escenas.
		*/
		void clearNonPersistants()
		{
			auto it = _messages.begin();
			while (it != _messages.end())
			{
				if (!it->second.first)
				{
					it = _messages.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		/**
		* @brief DEBE llamarse antes del cierre del engine.
		* Limpia todos los mensajes y subscribers de forma controlada,
		* ANTES de que el orden de destruccion de atexit cause crashes.
		*/
		void shutdown()
		{
			if (_destroyed) return;
			_destroyed = true;
			_messages.clear(); // Destruye cada Message<T> mientras sus capturas aun son validas
		}
		/**
		* @brief Consulta si el manager ya fue destruido.
		*/
		bool isDestroyed() const { return _destroyed; }
	private:
		/**
		* @brief Constructora por defecto.
		*/
		MessagesManager() : _destroyed(false) {};
		/**
		* @brief Destructora por defecto.
		*/
		~MessagesManager() {
			// Esto protege contra el crash de atexit si no se ha hecho clear.
			if (!_destroyed)
			{
				_destroyed = true;
				try { _messages.clear(); }
				catch (const std::exception& e) {
					Debug::error("[MessageManager] Error limpiando mensajes en destructor: ", e.what());
				}
				catch (...) {
					Debug::error("[MessageManager] Error desconocido limpiando mensajes en destructor.");
				}
			}
		};
		/**
		* @brief Mapa de Mensajes.
		*/
		std::unordered_map<std::string, std::pair<bool, std::any>> _messages;
		/**
		 * @brief Marca cuando se ha destruido el MessagesManager.
		 */
		bool _destroyed;
	};
}
