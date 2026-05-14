/**
* @file
*/

#pragma once

#include <string>
#include <unordered_map>
#include <Windows.h>
#include <InputDefs.h>
#include <functional>
#include "InputMapper.h"
#include "Color.h"
#include "EngineAPI.h"

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
class ENGINE_API PlatformModule
{
	using EventCallback = bool(*)(const SDL_Event* event);
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
	* @brief Devuelve la ventana de SDL.
	* @return SDL_Window - ventana
	*/
	SDL_Window* getSDLWindow();
	/**
	* @brief Actualiza eventos.
	* @return bool - Booleano para saber si se ha cerrado la ventana.
	*/
	bool pollEvents();
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
	 * Settea la sensibilidad del cursor
	 * @param sensitivity - sensibilidad a settear.
	 */
	void setMouseSensitivity(float sensitivity);
	/**
	 * @brief Comprueba si un dispositivo esta conectado.
	 * @param device - id del dispositivo a comprobar.
	 * @return bool - True o false si esta conectado o no.
	 */
	bool isDeviceConnected(input::DeviceID device);

	/**
	 * @brief Devuelve si una tecla esta pulsada
	 * @param inputEvent - InputEvent a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha pulsado en cualquier dispositivo.
	 */
	bool isKeyPressed(input::InputEvent inputEvent, input::DeviceID device = input::ANY_DEVICE) const;

	/**
	* @brief Devuelve si una tecla se acaba de pulsar.
	* @param key - Tecla a comprobar.
	* @returns bool - Estado de la tecla.
	*/
	bool isJustPressed(input::InputEvent inputEvent, input::DeviceID device = input::ANY_DEVICE) const;

	/**
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
	 * @brief Devuelve si se ha pulsado una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha pulsado en cualquier dispositivo.
	 */
	bool isActionJustPressed(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve si se ha dejado de pulsar una accion
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha dejado de pulsar en cualquier dispositivo.
	 */
	bool isActionReleased(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	 * @brief Devuelve la media de los ejes registrados a esa accion(input::ANY_DEVICE) o la media de los ejes del device pedido.
	 * @param actionName - accion a comprobar
	 * @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => si se ha dejado de pulsar en cualquier dispositivo.
	 */
	float getActionAxis(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
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
	bool setIcon(std::string path);
	/**
	* @brief Cambia el tamanyo de la ventana.
	* @param name - Nombre de la ventana.
	*/
	void setWindowName(std::string name);
	/**
	* @brief Registra una funcion para llamarla en el syncronice().
	* @param callback - Funcion a registrar.
	*/
	void registerEventObserver(EventCallback callback);
	/**
	* @brief Vibra el mando con los parametros dados.
	* Si ANY_DEVICE hace vibrar a todos.
	* @param id - Id del mando que tiene que vibrar.
	* @param lowFreq - Intensidad de la frecuencia menor.
	* @param highFreq - Intensidad de la frecuencia mayor.
	* @param dur - Duracion en ms de la vibracion.
	*/
	void setGamepadVibration(input::DeviceID id, float lowFreq, float highFreq, uint32_t dur);
	/**
	* @brief Cambia el color del mando.
	* Si ANY_DEVICE cambia el de todos.
	* @param color - Color para poner.
	*/
	void setGamepadColor(input::DeviceID id, core::Color color);
	/**
	* @brief Cambia el color del mando.
	* Si ANY_DEVICE cambia el de todos.
	* @param r - Valor de rojo.
	* @param g - Valor de verde.
	* @param b - Valor de azul.
	*/
	void setGamepadColor(input::DeviceID id, uint8_t r, uint8_t g, uint8_t b);

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
	 * @brief Sensibilidad del raton.
	 */
	float _mouseSensitivity = 10.0f;
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
	/**
	* @brief Funcion para registrar y llamar.
	*/
	EventCallback _eventObserver;

};
