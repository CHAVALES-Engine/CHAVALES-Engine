#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Plataforma:
 *	- Funciones de personalizacion de la ventana.
 *	- Funciones para gestionar el input.
 */


#include <string>
#include <unordered_map>
#include <Windows.h>
#include "InputDefs.h"
#include "InputMapper.h"

// Fordard declarations
struct SDL_Window;
union SDL_Event;
struct SDL_Gamepad;
namespace input
{
	class VirtualDevice;
}

class PlatformModule
{
public:
	/**
	* @brief Constructora vacia de PlatformModule.
	*/
	PlatformModule();
	/**
	* @brief Destructora de PlatformModule.
	* Elimina la ventana, el renderer y hace el Quit de SDL.
	*/
	~PlatformModule();
	/**
	* @brief Inicializador de PlatformModule, crea la ventana.
	* @return bool - Si se ha creado correctamente.
	*/
	bool Init();
	/**
	* @brief Devuelve el handle de la ventana de Windows.
	* @return HWND - Hanlde de la ventana (internamente un id)
	*/
	HWND getWindowHandle() const;
	/**
	* @brief Actualiza eventos.
	* @return bool - Booleano para saber si se ha cerrado la ventana.
	*/
	bool syncronize();
	/**
	* @brief Devuelve anchura de la ventana.
	*/
	int getWindowWidth() const;
	/**
	* @brief Devuelve altura de la ventana.
	*/
	int getWindowHeight() const;
	/*
	 * @brief Devuelve si una tecla esta pulsada
	 * @param inputAction - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha pulsado en cualquier dispositivo.
	 */
	bool isKeyPressed(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una tecla
	 * @param inputAction - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha dejado de pulsar en cualquier dispositivo.
	 */
	bool isKeyReleased(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve cuanto de accionado esta la accion a comprobar
	 * @param inputAction - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => el eje de cualquier dispositivo.
	 * @return float - Devuelve de -1 a 1
	 */
	float getAxis(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve si se ha pulsado una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha pulsado en cualquier dispositivo.
	 */
	bool isActionPressed(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha dejado de pulsar en cualquier dispositivo.
	 */
	bool isActionReleased(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Indica a la ventana que tome input de texto.
	 */
	void startTextInput() const;
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
	 * @brief Getter del input mapper para registrar acciones
	 * @return input::InputMapper& - referencia al InputMapper
	 */
	input::InputMapper* getInputMapper();
private:
	/**
	* @brief procesa un evento de sdl
	*/
	void _processEvent(const SDL_Event& event);
	/**
	 * @brief Castea un axis de SDL a nuestro propio sistema.
	 * @param event - Evento a castear.
	 * @return input::InputAxis - Evento casteado.
	 */
	input::InputAxis _castAxis(const SDL_Event& event);
	/**
	 * @brief Castea un button de SDL a nuestro propio sistema.
	 * @param event - Evento a castear.
	 * @return input::InputButtons - Evento casteado.
	 */
	input::InputButtons _castButton(const SDL_Event& event);
	/**
	* @brief mapa de dispositivos virtuales
	*/
	std::unordered_map<uint32_t, input::VirtualDevice*> _virtualDevices;
	/**
	* @brief mapa de ids
	*/
	std::unordered_map<uint32_t, SDL_Gamepad*> _devicesID;
	/**
	 */
	input::InputMapper* _inputMapper;
	/**
	* @brief Ventana de SDL
	*/
	SDL_Window* _window = nullptr;
	/**
	* @brief Manejador de la ventanda de Windows
	*/
	HWND _windowHandle = nullptr;
};
