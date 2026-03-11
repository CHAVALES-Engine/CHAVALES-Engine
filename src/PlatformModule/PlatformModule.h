#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Plataforma
 */


#include <string>
#include <unordered_map>
#include <variant>
#include <Windows.h>

struct SDL_Window; 
union SDL_Event;

class VirtualDevice;
enum Key;
enum MouseButton;
enum MouseAxis;
enum GamepadButton;
enum GamepadAxis;
using InputAction = std::variant<
	Key,
	MouseButton,
	MouseAxis,
	GamepadButton,
	GamepadAxis>;

/**
* @brief Modulo de Plataforma
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
	const HWND getWindowHandle() const;
	/**
	* @brief Actualiza eventos
	*
	* @return bool - Booleano para saber si se ha cerrado la ventana
	*/
	const bool syncronize();
	/**
	* @brief Devuelve anchura de la ventana
	*/
	const int getWindowWidth() const;
	/**
	* @brief Devuelve altura de la ventana
	*/
	const int getWindowHeight() const;

	const bool isKeyPressed() const;

	const bool isKeyReleased() const;

	const float getAxis() const;

	const bool isActionPressed(std::string) const;

	const bool isActionReleased(std::string) const;

private:
	/**
	* @brief procesa un evento de sdl
	*/
	const void processEvent(const SDL_Event& event);
	std::unordered_map<uint8_t, VirtualDevice> _virtualDevices;
	/**
	* @brief Ventana de SDL
	*/
	SDL_Window* _window = nullptr;
	/**
	* @brief Manejador de la ventanda de Windows
	*/
	HWND _windowHandle = nullptr;
};
