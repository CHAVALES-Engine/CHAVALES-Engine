#include "pch.h"
#include "PlatformModule.h"

#include "framework.h"

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



	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 250);
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);

	return true;
}

SDL_Window* PlatformModule::getWindow() const
{
	return _window;
}

SDL_Renderer* PlatformModule::getRenderer() const
{
	return _renderer;
}

void PlatformModule::present() const
{
	SDL_RenderPresent(_renderer);
}
