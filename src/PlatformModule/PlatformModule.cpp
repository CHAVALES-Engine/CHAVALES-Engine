#include "PlatformModule.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

#include <Debug.h>
#include "VirtualDevice.h"
#include "InputMapper.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_NAME "ChavalesWindow"


PlatformModule::PlatformModule() :
	_window(nullptr), _windowHandle(nullptr)
{
	_inputMapper = new input::InputMapper();
}

PlatformModule::~PlatformModule()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool PlatformModule::Init()
{
	// Inicializacion de SDL
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		Debug::error("SDL Couldn't be initialized.");
		return false;
	}
	// Creacion de ventana
	if ((_window = SDL_CreateWindow(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE)) == nullptr) {
		Debug::error("SDL Couldn't be Created.");
		return false;
	}

	SDL_PropertiesID _props = SDL_GetWindowProperties(_window);

	_windowHandle = (HWND)SDL_GetPointerProperty(_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	if (!_windowHandle) return false;

	return true;
}

HWND PlatformModule::getWindowHandle() const
{
	return _windowHandle;
}

bool PlatformModule::syncronize()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			return true;
		}
		else
		{
			processEvent(event);
		}
	}
	return false;
}

int PlatformModule::getWindowWidth() const
{
	return SCREEN_WIDTH;
}

int PlatformModule::getWindowHeight() const
{
	return SCREEN_HEIGHT;
}

bool PlatformModule::isKeyPressed(input::InputEvent inputAction, input::DeviceID device) const
{
	return false;
}

bool PlatformModule::isKeyReleased(input::InputEvent inputAction, input::DeviceID device) const
{
	return false;
}

float PlatformModule::getAxis(input::InputEvent inputAction, input::DeviceID device) const
{
	return 0.0f;
}

bool PlatformModule::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	return false;
}

bool PlatformModule::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return false;
}

input::InputMapper* PlatformModule::getInputMapper()
{
	// TODO: insert return statement here
	return _inputMapper;
}

input::InputAxis PlatformModule::castAxis(const SDL_Event& event)
{
	return input::MOUSE_AXIS_NONE;
}

input::InputButtons PlatformModule::castButton(const SDL_Event& event)
{
	return input::KEY_NONE;
}

const void PlatformModule::processEvent(const SDL_Event& event)
{/*
	switch (event.type) {
	case SDL_EVENT_GAMEPAD_ADDED: {
		uint32_t id = event.gdevice.which;
		SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
		if (gamepad) {
			_devicesID[id] = gamepad;
			input::VirtualDevice virtualDevice;
			virtualDevice.setConnected(true);
			_virtualDevices[id] = virtualDevice;
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_REMOVED: {
		uint32_t id = event.gdevice.which;
		auto it = _devicesID.find(id);
		if (it != _devicesID.end()) {
			SDL_CloseGamepad(it->second);
			_devicesID.erase(it);
		}
		_virtualDevices.erase(id);
		break;
	}
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
		uint32_t id = event.gaxis.which;
		auto it = _virtualDevices.find(id);
		if (it != _virtualDevices.end()) {
			it->second.setAxis(castAxis(event), event.gaxis.value);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
	case SDL_EVENT_KEY_DOWN: {
		uint32_t id = event.gbutton.which;
		auto it = _virtualDevices.find(id);
		if (it != _virtualDevices.end()) {
			it->second.setAxis(castButton(event), true);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_UP: {
	case SDL_EVENT_KEY_UP: {
		uint32_t id = event.gbutton.which;
		auto it = _virtualDevices.find(id);
		if (it != _virtualDevices.end()) {
			it->second.setAxis(castButton(event), false);
		}
		break;
	}
	default:
		break;
	}*/
}

