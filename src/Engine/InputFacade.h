#pragma once
#include <string>
#include <vector>

#include "InputDefs.h"
#include "EngineAPI.h"

class Engine;
class PlatformModule;

class ENGINE_API InputFacade
{
private:
	InputFacade(PlatformModule* platform);
public:
	//------Raw input:

	/**
	 * @brief Comprueba si un dispositivo esta conectado.
	 * @param device - id del dispositivo a comprobar.
	 * @return bool - True o false si esta conectado o no.
	 */
	bool isDeviceConnected(input::DeviceID device) const;
	/*
	* @brief Devuelve si una tecla esta pulsada
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isKeyPressed(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha dejado de pulsar una tecla
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isKeyReleased(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve cuanto de accionado esta la accion a comprobar
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	* @return float - Devuelve de -1 a 1
	*/
	float getAxis(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha pulsado una accion
	* @param actionName - accion a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isActionPressed(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha dejado de pulsar una accion
	* @param actionName - accion a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isActionReleased(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Indica a la ventana que tome input de texto.
	*/
	void startTextInput(bool blockKeyboard = true) const;
	/*
	* @brief Indica a la ventana que deje de tomar input de texto.
	*/
	void stopTextInput() const;
	/*
	* @brief Devuelve el texto introducido por el dispositivo
	* @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => la suma del input de todos los dispositivos.
	*/
	std::string getTextInput(input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Borra el buffer del input escrito.
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => la suma del input de todos los dispositivos.
	 */
	void clearTextInput(input::DeviceID device = input::ANY_DEVICE) const;

	//------Metodos de InputMapper:

	/**
	* @brief Mete un evento asociada a un nombre de accion.
	*
	* @param actionName - Nombre de la accion.
	* @param InputEvent - Input que lanza el evento.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	void addEventToAction(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id = input::ANY_DEVICE) const;

	/**
	* @brief Quita una evento asociado a una accion.
	*
	* @param actionName - Accion de la que eliminar un input.
	* @param InputEvent - Evento que quitar del mapa.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos del tipo dado.
	*/
	void removeEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id = input::ANY_DEVICE) const;
	/**
	* @brief Elimina todos los eventos asociados a una accion.
	*
	* @param actionName - Accion cuyos eventos hay que eliminar.
	*/
	void removeEvents(const std::string& actionName) const;
	/**
	* @brief Elimina todos los eventos asociados a una accion y a un id.
	*
	* @param actionName - Accion cuyos eventos hay que eliminar.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos de la accion (llama a removeEvents(actionName)).
	*/
	void removeEventsFromID(const std::string& actionName, input::DeviceID id = input::ANY_DEVICE) const;

	/**
	* @brief Devuelve todos los eventos correspondientes a una accion.
	*
	* @param actionName - Accion a consultar.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => Devuelve todos los eventos de esa accion.
	*
	* @return std::vector<InputAction> - Vector de InputActions correspondientes.
	*/
	std::vector<input::InputEvent> getInputEvents(const std::string& actionName, input::DeviceID id = input::ANY_DEVICE) const;
	/**
	* @brief Devuelve todas las acciones.
	*
	* @return std::vector<std::string> - Nombres de acciones registradas.
	*/
	std::vector<std::string> getActions() const;

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
	 * @brief Engine es clase amiga.
	 */
	friend Engine;
	/**
	 * @brief Referencia al modulo que envuelve.
	 */
	PlatformModule* _platform = nullptr;
};

