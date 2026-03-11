#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Plataforma
 */


#include <string>
#include <unordered_map>
#include <Windows.h>
#include "InputDefs.h"

struct SDL_Window;
union SDL_Event;
namespace input
{
	class InputMapper;
	class VirtualDevice;
}

/**
* @brief Modulo de Plataforma, encargado de gestionar la ventana y los eventos de input.
*/
class PlatformModule
{
public:
	/**
	* @brief Constructora vacia de PlatformModule
	*
	*/
	PlatformModule();
	/**
	* @brief Destructora de PlatformModule
	*
	* Elimina la ventana, el renderer y hace el Quit de SDL
	*/
	~PlatformModule();
	/**
	* @brief Inicializador de PlatformModule
	*
	*		Crea la ventana y el renderer
	* @return bool - Si se ha creado correctamente
	*/
	bool Init();
	/**
	* @brief Devuelve el handle de la ventana de Windows
	*
	* @return HWND - Hanlde de la ventana (internamente un id)
	*/
	HWND getWindowHandle() const;
	/**
	* @brief Actualiza eventos
	*
	* @return bool - Booleano para saber si se ha cerrado la ventana
	*/
	bool syncronize();
	/**
	* @brief Devuelve anchura de la ventana
	*/
	int getWindowWidth() const;
	/**
	* @brief Devuelve altura de la ventana
	*/
	int getWindowHeight() const;
	/*
	 * @brief Devuelve si una tecla esta pulsada
	 * @param inputAction - InputAction a comprobar
	 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	 */
	bool isKeyPressed(input::InputAction inputAction, input::DeviceID device = -1) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una tecla
	 * @param inputAction - InputAction a comprobar
	 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	 */
	bool isKeyReleased(input::InputAction inputAction, input::DeviceID device = -1) const;
	/*
	 * @brief Devuelve cuanto de accionado esta la accion a comprobar
	 * @param inputAction - InputAction a comprobar
	 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	 * @return float - Devuelve de -1 a 1
	 */
	float getAxis(input::InputAction inputAction, input::DeviceID device = -1) const;
	/*
	 * @brief Devuelve si se ha pulsado una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	 */
	bool isActionPressed(std::string actionName, input::DeviceID device = -1) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	 */
	bool isActionReleased(std::string actionName, input::DeviceID device = -1) const;
	/*
	 * @brief Getter del input mapper para registrar acciones
	 * @return input::InputMapper& - referencia al InputMapper
	 */
	input::InputMapper& getInputMapper();
private:
	/**
	* @brief procesa un evento de sdl
	*/
	const void processEvent(const SDL_Event& event);
	/**
	* @brief ´mapa de dispositivos virtuales
	*/
	std::unordered_map<uint8_t, input::VirtualDevice> _virtualDevices;
	/**
	* @brief Ventana de SDL
	*/
	SDL_Window* _window = nullptr;
	/**
	* @brief Manejador de la ventanda de Windows
	*/
	HWND _windowHandle = nullptr;
};
