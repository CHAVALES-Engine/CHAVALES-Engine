#include "PlatformModule.h"

#include <SDL3/SDL.h>

#include <Debug.h>


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
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		Debug::error("SDL Couldn't be initialized.");
		return false;
	}
	// Creacion de ventana
	if ((_window = SDL_CreateWindow(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE) )== nullptr) {
		Debug::error("SDL Couldn't be Created.");
		return false;
	}

	SDL_PropertiesID _props = SDL_GetWindowProperties(_window);

	_windowHandle = (HWND)SDL_GetPointerProperty(_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	if (!_windowHandle) return false;

	return true;
}

const HWND PlatformModule::getWindowHandle() const
{
	return _windowHandle;
}

const bool PlatformModule::syncronize()
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

const int PlatformModule::getWindowWidth() const
{
	return SCREEN_WIDTH;
}

const int PlatformModule::getWindowHeight() const
{
	return SCREEN_HEIGHT;
}

bool PlatformModule::isKeyPressed(input::InputEvent inputAction, input::DeviceID device) const
{
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

input::InputMapper& PlatformModule::getInputMapper()
{
	// TODO: insert return statement here
}

const void PlatformModule::processEvent(const SDL_Event& event)
{
	/*switch (event.type) {
	case SDL_EVENT_GAMEPAD_ADDED: {
		uint32_t id = event.gdevice.which;
		SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
		if (gamepad) {
			_gamepadHandles[id] = gamepad;
			ControllerState st;
			st.connected = true;
			_controllerStates[id] = st;
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_REMOVED: {
		uint32_t id = event.gdevice.which;
		auto it = _gamepadHandles.find(id);
		if (it != _gamepadHandles.end()) {
			SDL_CloseGamepad(it->second);
			_gamepadHandles.erase(it);
		}
		_controllerStates.erase(id);
		break;
	}
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
		uint32_t id = event.gaxis.which;
		auto it = _controllerStates.find(id);
		if (it != _controllerStates.end()) {
			int axis = -1;
			if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
				axis = 0;
			else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
				axis = 1;

			if (axis >= 0 && axis < NUM_AXIS) {
				it->second.axis[axis] = cleanAxisValue(event.gaxis.value);
			}
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
	case SDL_EVENT_GAMEPAD_BUTTON_UP: {
		uint32_t id = event.gbutton.which;
		auto it = _controllerStates.find(id);
		if (it != _controllerStates.end()) {
			uint8_t buttonIndex = 255;
			switch (event.gbutton.button) {
			case SDL_GAMEPAD_BUTTON_SOUTH: buttonIndex = BUTTON_CROSS; break;
			case SDL_GAMEPAD_BUTTON_EAST: buttonIndex = BUTTON_CIRCLE; break;
			case SDL_GAMEPAD_BUTTON_WEST: buttonIndex = BUTTON_SQUARE; break;
			case SDL_GAMEPAD_BUTTON_NORTH: buttonIndex = BUTTON_TRIANGLE; break;
			case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: buttonIndex = BUTTON_LB; break;
			case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: buttonIndex = BUTTON_RB; break;
			case SDL_GAMEPAD_BUTTON_BACK: buttonIndex = BUTTON_BACK; break;
			case SDL_GAMEPAD_BUTTON_START: buttonIndex = BUTTON_START; break;
			case SDL_GAMEPAD_BUTTON_LEFT_STICK: buttonIndex = BUTTON_LEFT_STICK; break;
			case SDL_GAMEPAD_BUTTON_RIGHT_STICK: buttonIndex = BUTTON_RIGHT_STICK; break;
			case SDL_GAMEPAD_BUTTON_GUIDE: buttonIndex = BUTTON_GUIDE; break;
			}

			if (buttonIndex != 255) {
				if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
					it->second.buttonsMask |= (1ull << buttonIndex);
				}
				else {
					it->second.buttonsMask &= ~(1ull << buttonIndex);
				}
			}
		}
		break;
	}
	case SDL_EVENT_KEY_DOWN: {
		SDL_Keycode key = event.key.key;
		auto& keys = _controllerStates[KEYBOARD_CONTROLLER_ID].pressedKeys;
		_controllerStates[KEYBOARD_CONTROLLER_ID].addKey(static_cast<uint8_t>(key));
		break;
	}
	case SDL_EVENT_KEY_UP: {
		SDL_Keycode key = event.key.key;
		auto& keys = _controllerStates[KEYBOARD_CONTROLLER_ID].pressedKeys;
		_controllerStates[KEYBOARD_CONTROLLER_ID].removeKey(static_cast<uint8_t>(key));
		break;
	}
	default:
		break;
	}*/
}
