#pragma once
#include <string>
#include <unordered_map>
#include <variant>

#include "InputDefs.h"


namespace input
{

	using InputMappperEntry = std::pair<DeviceID, InputEvent>;
	/**
	* @brief dedsasfsfsfs
	* Una accion no puede estar vacia, si se queda vacia se elimina.
	*/
	class InputMapper
	{
	public:

		/**
		 * @brief Constructora de InputMapper.
		 */
		InputMapper();

		/**
		 * @brief Anyade un evento asociada a un nombre de accion.
		 *
		 * @param actionName - Nombre de la accion.
		 * @param InputEvent - Input que lanza el evento.
		 * @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 */
		void addEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id = -1);
		/**
		 * @brief Quita una evento asociado a una accion.
		 *
		 * @param actionName - Accion de la que eliminar un input.
		 * @param InputEvent - Evento que quitar del mapa.
		 * @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 */
		void removeEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id = -1);

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
		 * @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 */
		void removeEventsFromID(const std::string& actionName, DeviceID id = -1);

		/**
		 * @brief Devuelve todos los eventos correspondientes a una accion.
		 *
		 * @param actionName - Accion a consultar.
		 * @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 *
		 * @return std::vector<InputAction> - Vector de InputActions correspondientes.
		 */
		std::vector<InputEvent> getInputEvents(const std::string& actionName, DeviceID id = -1);
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
		/*
		 * @brief Devuelve cuanto de accionado esta la accion a comprobar.
		 *
		 * @param actionName - nombre de la accion a comprobar.
		 * @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 *
		 * @return float - Devuelve de -1 a 1.
		 */
		float getActionAxis(const std::string& actionName, DeviceID id = -1) const;

	private:

		/**
		 * @brief Mapa de acciones.
		 */
		std::unordered_map < std::string, std::vector<InputMappperEntry>> _mapper;

		/**
		 * @brief Elimina una accion al quedarse vacia.
		 * 
		 * @param actionName - Nombre de la accion a eliminar.
		 */
		void _removeAction(const std::string& actionName);

		/**
		 * @brief Comprobar que una entrada esta en una accion.
		 *
		 * @param actionName - Nombre de la accion a eliminar.
		 * @param entry - Entrada a buscar.
		 * 
		 * @return bool - Devuelve si la entrada esta en la accion
		 */
		bool _entryIsInMap(const std::string& actionName, InputMappperEntry entry);
	};
};
