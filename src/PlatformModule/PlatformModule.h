#pragma once
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */

struct SDL_Window;
struct SDL_Renderer;

class PlatformModule
{
public:

	PlatformModule();
	~PlatformModule();

	bool Init();

	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;

	void present() const;
private:

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

};
