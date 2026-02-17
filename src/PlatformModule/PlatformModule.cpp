// PlatformModule.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include <SDL3/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// TODO: This is an example of a library function
void fnPlatformModule()
{
    SDL_Window* window = SDL_CreateWindow("Example: 0", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
   SDL_Renderer* renderer = SDL_CreateRenderer(window, "");
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
   SDL_RenderClear(renderer);
   SDL_RenderPresent(renderer);
   SDL_Delay(2000);
   SDL_DestroyWindow(window);
   SDL_DestroyRenderer(renderer);
   SDL_Quit();
   return EXIT_SUCCESS;
}
