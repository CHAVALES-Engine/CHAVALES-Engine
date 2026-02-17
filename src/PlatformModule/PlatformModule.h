#pragma once

struct SDL_Window;
struct SDL_Renderer;

class PlatformModule
{
public:

	PlatformModule();
	~PlatformModule();

	bool Init();

private:

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

};
