#include "PlatformModule.h"

#include <SDL3/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


PlatformModule::PlatformModule()
{

}

PlatformModule::~PlatformModule()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

bool PlatformModule::Init()
{
	_window = SDL_CreateWindow("CHAVALES WINDOW", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (_window == nullptr) return false;

	_renderer = SDL_CreateRenderer(_window, "");
	if (_renderer == nullptr) return false;


	SDL_PropertiesID _props = SDL_GetWindowProperties(_window);

	_windowHandle = (HWND)SDL_GetPointerProperty(_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	if (!_windowHandle) return false;

	/*SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 250);
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);*/

	return true;
}

const HWND PlatformModule::getWindowHandle() const
{
	return _windowHandle;
}

const UINT64 PlatformModule::getSecSinceStart() const
{
	// Multiplicamos para pasarlo a segundos
	return SDL_GetTicks() * 0.0001;
}

const int PlatformModule::getWindowWidth() const
{
	return SCREEN_WIDTH;
}

const int PlatformModule::getWindowHeight() const
{
	return SCREEN_HEIGHT;
}
