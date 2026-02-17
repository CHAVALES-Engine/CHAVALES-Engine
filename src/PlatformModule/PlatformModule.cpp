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
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool PlatformModule::Init()
{
	window = SDL_CreateWindow("CHAVALES WINDOW", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == nullptr) return false;

	renderer = SDL_CreateRenderer(window, "");
	if (renderer == nullptr) return false;



	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(200000);

	return true;
}
