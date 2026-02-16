/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char* argv[]) {
	//sol::state lua;
	//lua.script_file("variables.lua");
	//// the type "sol::state" behaves 
	//// exactly like a table!
	//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
	//sol::table config = lua["config"];
	//assert(!isfullscreen);

    /*SDL_Window* window = SDL_CreateWindow("Example: 0", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;*/

	return 0;
}