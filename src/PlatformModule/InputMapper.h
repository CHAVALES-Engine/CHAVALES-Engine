#pragma once
#include <string>
#include <unordered_map>
#include <variant>

#include "InputDefs.h"

class PlatformModule;


namespace input
{
	using InputMappperEntry = std::pair<DeviceID, InputEvent>;
	/**
	* @brief Clase para el mapeado de Input.
	* Guarda un mapa de accion-entrada cuyas entradas son ID-Evento.
	* Una accion no puede estar vacia, si se queda vacia se elimina.
	* ID por defecto -1 (ANY_DEVICE).
	*/
	class InputMapper
	{
	public:

		/**
		* @brief Mete un evento asociada a un nombre de accion.
		*
		* @param actionName - Nombre de la accion.
		* @param InputEvent - Input que lanza el evento.
		* @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		*/
		void addEventToAction(const std::string& actionName, InputEvent inputEvent, DeviceID id = ANY_DEVICE);

		/**
		* @brief Quita una evento asociado a una accion.
		*
		* @param actionName - Accion de la que eliminar un input.
		* @param InputEvent - Evento que quitar del mapa.
		* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos del tipo dado.
		*/
		void removeEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id = ANY_DEVICE);
		/**
		* @brief Elimina todos los eventos asociados a una accion.
		*
		* @param actionName - Accion cuyos eventos hay que eliminar.
		*/
		void removeEvents(const std::string& actionName);
		/**
		* @brief Elimina todos los eventos asociados a una accion y a un id.
		*
		* @param actionName - Accion cuyos eventos hay que eliminar.
		* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos de la accion (llama a removeEvents(actionName)).
		*/
		void removeEventsFromID(const std::string& actionName, DeviceID id = ANY_DEVICE);

		/**
		* @brief Devuelve todos los eventos correspondientes a una accion.
		*
		* @param actionName - Accion a consultar.
		* @param id - Id del dispositivo a comprobar. -1 por defecto => Devuelve todos los eventos de esa accion.
		*
		* @return std::vector<InputAction> - Vector de InputActions correspondientes.
		*/
		std::vector<InputEvent> getInputEvents(const std::string& actionName, DeviceID id = ANY_DEVICE);
		/**
		* @brief Devuelve todas las acciones.
		*
		* @return std::vector<std::string> - Nombres de acciones registradas.
		*/
		std::vector<std::string> getActions();

		/**
		* @brief Devuelve si tiene un nombre de accion registrada.
		*
		* @param actionName - Nombre de la accion a consultar.
		*
		* @return bool - Devuelve true si esta mapeada.
		*/
		bool hasAction(const std::string& actionName) const;

	private:

		/**
		* @brief Constructora de InputMapper.
		*/
		InputMapper() = default;

		/**
		* @brief Mapa de acciones.
		*/
		std::unordered_map < std::string, std::vector<InputMappperEntry>> _mapper;

		/*
		* @brief Elimina una accion al quedarse vacia.
		*
		* @param actionName - Nombre de la accion a eliminar.
		*/
		void _removeAction(const std::string& actionName);

		/**
		* @brief Comprueba que una entrada esta en una accion.
		*
		* @param actionName - Nombre de la accion a eliminar.
		* @param entry - Entrada a buscar.
		*
		* @return bool - Devuelve si la entrada esta en la accion
		*/
		bool _entryIsInMap(const std::string& actionName, InputMappperEntry entry);
		/**
		* @brief Elimina todos los eventos asociados a una accion del tipo dado.
		*
		* @param actionName - Accion cuyos eventos hay que eliminar.
		* @param inputEvent - Eventos que quitar del mapa.
		*/
		void _removeEvents(const std::string& actionName, InputEvent inputEvent);

		friend class PlatformModule;
	};
};
