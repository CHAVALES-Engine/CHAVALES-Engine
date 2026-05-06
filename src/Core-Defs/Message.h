/*
 * @file
 * @brief
 */

#pragma once

#include <vector>
#include <functional>

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
			for (auto s : _subscribers)
			{
				if (s)
				{
					try
					{
						s(args...);
					}
					catch (const std::exception& e)
					{
						Debug::error("Error en subscriber: ", e.what());
					}
				}
			}
			//for (int i = 0; i < _subscribers.size(); i++)
			//{
			//	//Debug::out("EJECUTANDO SUBSCRIBER");
			//	_subscribers[i](args...);
			//}
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
			if (id < 0 || id >= static_cast<int>(_subscribers.size()))
			{
				Debug::error("Mensage con ID invalido: ", id);
				return false;
			}
			_subscribers.erase(_subscribers.begin() + id);
			return true;
		}

		void unsubscribeAll()
		{
			try
			{
				_subscribers.clear();
			}
			catch (const std::exception& e)
			{
				Debug::error("Error al desuscribir todos los callbacks: ", e.what());
			}
		}

		~Message() { unsubscribeAll(); }
	private:

		/**
		* @brief Vector de punteros a funcion.
		*/
		std::vector < std::function < void(Args...) >> _subscribers;
	};

}
