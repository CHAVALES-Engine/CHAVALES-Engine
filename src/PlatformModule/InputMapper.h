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
		void addAction(const std::string& actionName, InputAction inputAction, DeviceID = -1);
		/**
		 * @brief Quita una accion asociada a un nombre de accion.
		 * @param actionName - accion de la que eliminar un input.
		 * @param inputAction - Input que quitar del mapa.
		 */
		void removeAction(const std::string& actionName, InputAction inputAction, DeviceID = -1);
		/**
		 * @brief Elimina una accion del mapa.
		 * @param actionName - accion a eliminar.
		 */
		void removeActions(const std::string&  actionName);
		/**
		 * @brief Devuelve todos los input actions correspondientes a un evento.
		 * @param actionName - Accion a consultar.
		 * @return std::vector<InputAction> - Vector de InputActions correspondientes.
		 */
		std::vector<InputAction> getInputActions(const std::string& actionName);
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
		bool hasAction(const std::string& actionName) const;
		/*
		 * @brief Devuelve cuanto de accionado esta la accion a comprobar
		 * @param actionName - nombre de la accion a comprobar
		 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
		 * @return float - Devuelve de -1 a 1
		 */
		float getActionAxis(const std::string& actionName, int deviceId = -1) const;
	private:
		/**
		 * @brief Mapa de acciones.
		 */
		std::unordered_map<std::string, std::vector<std::pair<DeviceID, InputAction>>> mapper;
	};

}

