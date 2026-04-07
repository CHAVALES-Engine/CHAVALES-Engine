#pragma once


#include <string>
#include <unordered_map>
#include <Windows.h>
#include <InputDefs.h>
#include "InputMapper.h"

// Fordard declarations
struct SDL_Window;
union SDL_Event;
struct SDL_Gamepad;
struct SDL_Surface;
namespace input
{
	class VirtualDevice;
}

/*
 * @brief Define las funciones del Modulo de Plataforma:
 *	- Funciones de personalizacion de la ventana.
 *	- Funciones para gestionar el input.
 */
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
	/**
	 * @brief Bloquea y oculta el cursor.
	 * @param enabled - Habilitado o no.
	 */
	void setRelativeMouseMode(bool enabled) const;
	/**
	 * @brief Comprueba si un dispositivo esta conectado.
	 * @param device - id del dispositivo a comprobar.
	 * @return bool - True o false si esta conectado o no.
	 */
	bool isDeviceConnected(input::DeviceID device);
	/*
	 * @brief Devuelve si una tecla esta pulsada
	 * @param inputEvent - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha pulsado en cualquier dispositivo.
	 */
	bool isKeyPressed(input::InputEvent inputEvent, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una tecla
	 * @param inputEvent - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha dejado de pulsar en cualquier dispositivo.
	 */
	bool isKeyReleased(input::InputEvent inputEvent, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve cuanto de accionado esta la accion a comprobar
	 * @param inputEvent - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => la media de los ejes de los dispositivos.
	 * @return float - Devuelve de -1 a 1
	 */
	float getAxis(input::InputEvent inputEvent, input::DeviceID device = input::ANY_DEVICE) const;
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
	 * @param blockKeyboard - Booleano que indica si se debe bloquear el input de teclado mientras se escibe.
	 */
	void startTextInput(bool blockKeyboard = true);
	/*
	 * @brief Indica a la ventana que deje de tomar input de texto.
	 */
	void stopTextInput();
	/*
	 * @brief Devuelve el texto introducido por el dispositivo.
	 * @param device - id del dispositivo a registrar. ANY_DEVICE por defecto => la suma del input de todos los dispositivos.
	 * @return std::string - Input del texto.
	 */
	std::string getTextInput(input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Borra el buffer del input escrito.
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => la suma del input de todos los dispositivos.
	 */
	void clearTextInput(input::DeviceID device = input::ANY_DEVICE);
	/*
	 * @brief Getter del input mapper para registrar acciones
	 * @return input::InputMapper& - referencia al InputMapper
	 */
	input::InputMapper* getInputMapper() const;
	/**
	* @brief Cambia el tamanyo de la ventana.
	* @param w - Width.
	* @param w - Height.
	*/
	void setWindowSize(int w, int h);
	/**
	* @brief Cambia el tamanyo de la ventana.
	* @param path - Ruta del icono.
	* @return bool - si se ha podido cambiar.
	*/
	bool setWindowIcon(std::string path);
	/**
	* @brief Cambia el tamanyo de la ventana.
	* @param name - Nombre de la ventana.
	*/
	void setWindowName(std::string name);

private:

	/**
	* @brief procesa un evento de sdl
	*/
	void _processEvent(const SDL_Event& event);
	/**
	 * @brief Comprueba si un input event esta permitido mientras se escribe (escape, enter, KP_enter).
	 * @param inputEvent - Evento a comprobar.
	 * @return bool - Si el evento esta permitido en el modo escribir.
	 */
	bool _isTextInputAllowed(input::InputEvent inputEvent) const;
	/**
	 * @brief Castea un axis de SDL a nuestro propio sistema.
	 * @param event - Evento a castear.
	 * @return input::InputAxis - Evento casteado.
	 */
	input::InputAxis _castAxis(const SDL_Event& event) const;
	/**
	 * @brief Castea un button de SDL a nuestro propio sistema.
	 * @param event - Evento a castear.
	 * @return input::InputButtons - Evento casteado.
	 */
	input::InputButtons _castButton(const SDL_Event& event) const;
	/**
	 * @brief Indica si bloquear el teclado mientras se escribe.
	 */
	bool _blockKeyboard = true;
	/**
	 * @brief Se esta escribiendo.
	 */
	bool _textInputActive = false;
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
	/**
	* @brief Surface para el icono.
	*/
	SDL_Surface* _icon = nullptr;
};
