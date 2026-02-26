#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Plataforma
 */

#if _WIN64
#include <Windows.h>
#include <windef.h>
#endif // _WIN64


struct SDL_Window;
struct SDL_Renderer;

/**
* @brief Modulo de Plataforma
*
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

private:
	/**
	* @brief Ventana de SDL
	*
	*/
	SDL_Window* _window = nullptr;
	/**
	* @brief Renderer de SDL
	*
	*/
	SDL_Renderer* _renderer = nullptr;
	/**
	* @brief Manejador de la ventanda de Windows
	*
	*/
	HWND _windowHandle = nullptr;
};
