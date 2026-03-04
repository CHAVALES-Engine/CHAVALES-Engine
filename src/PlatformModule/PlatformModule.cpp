#include "PlatformModule.h"

#include <SDL3/SDL.h>

#include "../Core/Debug.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_NAME "ChavalesWindow"


PlatformModule::PlatformModule():
_window(nullptr), _windowHandle(nullptr)
{}

PlatformModule::~PlatformModule()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool PlatformModule::Init()
{
	// Inicializacion de SDL
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {	
		Debug::error("SDL Couldn't be initialized.");
		return false;
	}
	// Creacion de ventana
	if ((_window = SDL_CreateWindow(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE) )== nullptr) {
		Debug::error("SDL Couldn't be Created.");
		return false;
	}
	SDL_Init(SDL_INIT_EVENTS);

	SDL_PropertiesID _props = SDL_GetWindowProperties(_window);

	_windowHandle = (HWND)SDL_GetPointerProperty(_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	if (!_windowHandle) return false;

	return true;
}

const HWND PlatformModule::getWindowHandle() const
{
	return _windowHandle;
}

const bool PlatformModule::syncronize() const
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			return true;
		}
	}
	return false;
}

const int PlatformModule::getWindowWidth() const
{
	return SCREEN_WIDTH;
}

const int PlatformModule::getWindowHeight() const
{
	return SCREEN_HEIGHT;
}
