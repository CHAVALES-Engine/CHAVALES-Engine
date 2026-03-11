#pragma once
#include <string>
#include <unordered_map>
#include <variant>

#include "InputDefs.h"
namespace input
{
	class InputMapper
	{
	public:
		InputMapper();
		/**
		 * @brief Anyade una accion a sociada a un nombre de accion.
		 * @param actionName - Nombre de la accion.
		 * @param inputAction - Input que lanza la accion.
		 */
		void addAction(const std::string& actionName, InputAction inputAction);
		/**
		 * @brief Quita una accion asociada a un nombre de accion.
		 * @param actionName - accion de la que eliminar un input.
		 * @param inputAction - Input que quitar del mapa.
		 */
		void removeAction(std::string& const actionName, InputAction inputAction);
		/**
		 * @brief Elimina una accion del mapa.
		 * @param actionName - accion a eliminar.
		 */
		void removeActions(std::string& const actionName);
		/**
		 * @brief Devuelve todos los input actions correspondientes a un evento.
		 * @param actionName - Accion a consultar.
		 * @return std::vector<InputAction> - Vector de InputActions correspondientes.
		 */
		std::vector<InputAction> getEvents(std::string& const actionName);
		/**
		 * @brief Devuelve todas las acciones.
		 * @return std::vector<std::string> - Nombres de acciones registradas.
		 */
		std::vector<std::string> getActions();
		/**
		 * @brief Devuelve si tiene un nombre de accion registrada.
		 * @param actionName - Nombre de la accion a consultar.
		 * @return bool - Devuelve true si esta mapeada.
		 */
		bool hasAction(std::string& const actionName) const;
	private:
		/**
		 * @brief Mapa de acciones.
		 */
		std::unordered_map<std::string, std::vector<InputAction>> mapper;
	};

}

