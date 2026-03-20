#include "PlatformModule.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

#include <Debug.h>
#include <optional>

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
	//limpieza de input devices
	for (auto& [id, device] : _virtualDevices)
		delete device;
	_virtualDevices.clear();

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

	// Virtual device correspondiente al teclado.
	input::VirtualDevice* keyboard = new input::VirtualDevice();
	keyboard->_setConnected(true);
	_virtualDevices[input::KEYBOARD_ID] = keyboard;

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
		if (event.type == SDL_EVENT_QUIT)
			return true;
		else
			_processEvent(event);
	}
	return false;
}

int PlatformModule::getWindowWidth() const
{
	int w = 0;
	SDL_GetWindowSize(_window, &w, nullptr);
	return w;
}

int PlatformModule::getWindowHeight() const
{
	int h = 0;
	SDL_GetWindowSize(_window, nullptr, &h);
	return h;
}

bool PlatformModule::isDeviceConnected(input::DeviceID device)
{
	bool connected = false;
	auto it = _virtualDevices.find(device);
	if (it != _virtualDevices.end()) connected = it->second->isConnected();
	return (device != input::ANY_DEVICE) && connected;
}

bool PlatformModule::isKeyPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	// Usa std::visit para seleccionar y ejecutar una funcion de tipo de dato del inputEvent.
	// "func" es la funcion escogida segun el tipo de dato de inputEvent.
	auto func = [&](const input::VirtualDevice* vd) -> bool {
		return std::visit(input::overloaded{
			[&](input::Key k) { return vd->isPressed(k); },
			[&](input::GamepadButton b) { return vd->isPressed(b); },
			[&](input::MouseButton b) { return vd->isPressed(b); },
			[](auto&&) { Debug::error("[Input] inputEvent not allowed"); return false; }
			}, inputEvent); // Le pasamos ya el InputEvent para no tener que gestionarlo luego.
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE) {
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return func(it->second);
		Debug::error("[Input] device: ", device, " not found");
		return false;
	}
	// ANY_DEVICE - early exit en cuanto algun device tenga la tecla pulsada.
	for (const auto& [id, vd] : _virtualDevices)
		if (func(vd)) return true;
	return false;
}

bool PlatformModule::isKeyReleased(input::InputEvent inputEvent, input::DeviceID device) const
{
	// Usa std::visit para seleccionar y ejecutar una funcion de tipo de dato del inputEvent.
	// "func" es la funcion escogida segun el tipo de dato de inputEvent.
	auto func = [&](const input::VirtualDevice* vd) -> bool {
		return std::visit(input::overloaded{
			[&](input::Key k) {return vd->isPressed(k); },
			[&](input::GamepadButton b) {return vd->isPressed(b); },
			[&](input::MouseButton b) {return vd->isPressed(b); },
			[](auto&&) { Debug::error("[Input] inputEvent not allowed"); return false; }
			}, inputEvent);
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE)
	{
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return func(it->second);
		Debug::error("[Input] device: ", device, " not found");
		return false;
	}
	// ANY_DEVICE - early exit en cuanto algun device tenga la tecla pulsada.
	for (const auto& [id, vd] : _virtualDevices)
		if (func(vd)) return true;
	return false;
}

float PlatformModule::getAxis(input::InputEvent inputEvent, input::DeviceID device) const
{
	auto func = [&](const input::VirtualDevice* vd) -> float {
		return std::visit(input::overloaded{
			[&](input::MouseAxis a) {return vd->getAxis(a); },
			[&](input::GamepadAxis a) {return vd->getAxis(a); },
			[](auto&&) { Debug::error("[Input] inputEvent not allowed"); return 0.0f; }
			}, inputEvent);
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE)
	{
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return func(it->second);
		Debug::error("[Input] device: ", device, " not found");
		return 0.0f;
	}
	// ANY_DEVICE - Coge el mayor input de todos los device.
	float maxVal = 0.0f;
	for (const auto& [id, vd] : _virtualDevices){
		float value = func(vd);
		if (abs(value) > abs(maxVal)) maxVal = abs(value);
	}
	return maxVal;
}

bool PlatformModule::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		if (isKeyPressed(event, device))
			return true;
	}
	return false;
}

bool PlatformModule::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		if (isKeyReleased(event, device))
			return true;
	}
	return false;
}

void PlatformModule::startTextInput() const
{
	SDL_StartTextInput(_window);
}

void PlatformModule::stopTextInput() const
{
	SDL_StopTextInput(_window);
}

std::string PlatformModule::getTextInput(input::DeviceID device) const
{
	auto it = _virtualDevices.find(device);
	if (it != _virtualDevices.end())
	{
		return it->second->getTextInput();
	}
	return {};
}

const input::InputMapper* PlatformModule::getInputMapper() const
{
	return _inputMapper;
}

input::InputButtons PlatformModule::_castButton(const SDL_Event& event) const
{
	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
		switch (event.key.scancode)
		{
		case SDL_SCANCODE_ESCAPE:       return input::KEY_ESCAPE;
		case SDL_SCANCODE_TAB:          return input::KEY_TAB;
		case SDL_SCANCODE_BACKSPACE:    return input::KEY_BACKSPACE;
		case SDL_SCANCODE_RETURN:       return input::KEY_ENTER;
		case SDL_SCANCODE_KP_ENTER:     return input::KEY_KP_ENTER;
		case SDL_SCANCODE_INSERT:       return input::KEY_INSERT;
		case SDL_SCANCODE_DELETE:       return input::KEY_DELETE;
		case SDL_SCANCODE_PAUSE:        return input::KEY_PAUSE;
		case SDL_SCANCODE_PRINTSCREEN:  return input::KEY_PRINT;
		case SDL_SCANCODE_CLEAR:        return input::KEY_CLEAR;
		case SDL_SCANCODE_HOME:         return input::KEY_HOME;
		case SDL_SCANCODE_END:          return input::KEY_END;
		case SDL_SCANCODE_LEFT:         return input::KEY_LEFT;
		case SDL_SCANCODE_UP:           return input::KEY_UP;
		case SDL_SCANCODE_RIGHT:        return input::KEY_RIGHT;
		case SDL_SCANCODE_DOWN:         return input::KEY_DOWN;
		case SDL_SCANCODE_PAGEUP:       return input::KEY_PAGEUP;
		case SDL_SCANCODE_PAGEDOWN:     return input::KEY_PAGEDOWN;
		case SDL_SCANCODE_LSHIFT:
		case SDL_SCANCODE_RSHIFT:       return input::KEY_SHIFT;
		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_RCTRL:        return input::KEY_CTRL;
		case SDL_SCANCODE_LALT:
		case SDL_SCANCODE_RALT:         return input::KEY_ALT;
		case SDL_SCANCODE_LGUI:
		case SDL_SCANCODE_RGUI:         return input::KEY_META;
		case SDL_SCANCODE_CAPSLOCK:     return input::KEY_CAPSLOCK;
		case SDL_SCANCODE_NUMLOCKCLEAR: return input::KEY_NUMLOCK;
		case SDL_SCANCODE_SCROLLLOCK:   return input::KEY_SCROLLLOCK;
		case SDL_SCANCODE_F1:           return input::KEY_F1;
		case SDL_SCANCODE_F2:           return input::KEY_F2;
		case SDL_SCANCODE_F3:           return input::KEY_F3;
		case SDL_SCANCODE_F4:           return input::KEY_F4;
		case SDL_SCANCODE_F5:           return input::KEY_F5;
		case SDL_SCANCODE_F6:           return input::KEY_F6;
		case SDL_SCANCODE_F7:           return input::KEY_F7;
		case SDL_SCANCODE_F8:           return input::KEY_F8;
		case SDL_SCANCODE_F9:           return input::KEY_F9;
		case SDL_SCANCODE_F10:          return input::KEY_F10;
		case SDL_SCANCODE_F11:          return input::KEY_F11;
		case SDL_SCANCODE_F12:          return input::KEY_F12;
		case SDL_SCANCODE_SPACE:        return input::KEY_SPACE;
		case SDL_SCANCODE_A:            return input::KEY_A;
		case SDL_SCANCODE_B:            return input::KEY_B;
		case SDL_SCANCODE_C:            return input::KEY_C;
		case SDL_SCANCODE_D:            return input::KEY_D;
		case SDL_SCANCODE_E:            return input::KEY_E;
		case SDL_SCANCODE_F:            return input::KEY_F;
		case SDL_SCANCODE_G:            return input::KEY_G;
		case SDL_SCANCODE_H:            return input::KEY_H;
		case SDL_SCANCODE_I:            return input::KEY_I;
		case SDL_SCANCODE_J:            return input::KEY_J;
		case SDL_SCANCODE_K:            return input::KEY_K;
		case SDL_SCANCODE_L:            return input::KEY_L;
		case SDL_SCANCODE_M:            return input::KEY_M;
		case SDL_SCANCODE_N:            return input::KEY_N;
		case SDL_SCANCODE_O:            return input::KEY_O;
		case SDL_SCANCODE_P:            return input::KEY_P;
		case SDL_SCANCODE_Q:            return input::KEY_Q;
		case SDL_SCANCODE_R:            return input::KEY_R;
		case SDL_SCANCODE_S:            return input::KEY_S;
		case SDL_SCANCODE_T:            return input::KEY_T;
		case SDL_SCANCODE_U:            return input::KEY_U;
		case SDL_SCANCODE_V:            return input::KEY_V;
		case SDL_SCANCODE_W:            return input::KEY_W;
		case SDL_SCANCODE_X:            return input::KEY_X;
		case SDL_SCANCODE_Y:            return input::KEY_Y;
		case SDL_SCANCODE_Z:            return input::KEY_Z;
		case SDL_SCANCODE_0:            return input::KEY_0;
		case SDL_SCANCODE_1:            return input::KEY_1;
		case SDL_SCANCODE_2:            return input::KEY_2;
		case SDL_SCANCODE_3:            return input::KEY_3;
		case SDL_SCANCODE_4:            return input::KEY_4;
		case SDL_SCANCODE_5:            return input::KEY_5;
		case SDL_SCANCODE_6:            return input::KEY_6;
		case SDL_SCANCODE_7:            return input::KEY_7;
		case SDL_SCANCODE_8:            return input::KEY_8;
		case SDL_SCANCODE_9:            return input::KEY_9;
		case SDL_SCANCODE_MINUS:        return input::KEY_MINUS;
		case SDL_SCANCODE_EQUALS:       return input::KEY_EQUAL;
		case SDL_SCANCODE_LEFTBRACKET:  return input::KEY_BRACKETLEFT;
		case SDL_SCANCODE_RIGHTBRACKET: return input::KEY_BRACKETRIGHT;
		case SDL_SCANCODE_BACKSLASH:    return input::KEY_BACKSLASH;
		case SDL_SCANCODE_SEMICOLON:    return input::KEY_SEMICOLON;
		case SDL_SCANCODE_APOSTROPHE:   return input::KEY_APOSTROPHE;
		case SDL_SCANCODE_GRAVE:        return input::KEY_QUOTELEFT;
		case SDL_SCANCODE_COMMA:        return input::KEY_COMMA;
		case SDL_SCANCODE_PERIOD:       return input::KEY_PERIOD;
		case SDL_SCANCODE_SLASH:        return input::KEY_SLASH;
		case SDL_SCANCODE_KP_MULTIPLY:  return input::KEY_KP_MULTIPLY;
		case SDL_SCANCODE_KP_DIVIDE:    return input::KEY_KP_DIVIDE;
		case SDL_SCANCODE_KP_MINUS:     return input::KEY_KP_SUBTRACT;
		case SDL_SCANCODE_KP_PLUS:      return input::KEY_KP_ADD;
		case SDL_SCANCODE_KP_PERIOD:    return input::KEY_KP_PERIOD;
		case SDL_SCANCODE_KP_0:         return input::KEY_KP_0;
		case SDL_SCANCODE_KP_1:         return input::KEY_KP_1;
		case SDL_SCANCODE_KP_2:         return input::KEY_KP_2;
		case SDL_SCANCODE_KP_3:         return input::KEY_KP_3;
		case SDL_SCANCODE_KP_4:         return input::KEY_KP_4;
		case SDL_SCANCODE_KP_5:         return input::KEY_KP_5;
		case SDL_SCANCODE_KP_6:         return input::KEY_KP_6;
		case SDL_SCANCODE_KP_7:         return input::KEY_KP_7;
		case SDL_SCANCODE_KP_8:         return input::KEY_KP_8;
		case SDL_SCANCODE_KP_9:         return input::KEY_KP_9;
		case SDL_SCANCODE_MENU:         return input::KEY_MENU;
		case SDL_SCANCODE_HELP:         return input::KEY_HELP;
		case SDL_SCANCODE_VOLUMEUP:     return input::KEY_VOLUMEUP;
		case SDL_SCANCODE_VOLUMEDOWN:   return input::KEY_VOLUMEDOWN;
		case SDL_SCANCODE_MUTE:         return input::KEY_VOLUMEMUTE;
		case SDL_SCANCODE_MEDIA_PLAY:   return input::KEY_MEDIAPLAY;
		case SDL_SCANCODE_MEDIA_STOP:   return input::KEY_MEDIASTOP;
		case SDL_SCANCODE_MEDIA_NEXT_TRACK: return input::KEY_MEDIANEXT;
		case SDL_SCANCODE_MEDIA_PREVIOUS_TRACK: return input::KEY_MEDIAPREVIOUS;
		default:                        return input::KEY_UNKNOWN;
		}

	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:   return input::MOUSE_BUTTON_LEFT;
		case SDL_BUTTON_RIGHT:  return input::MOUSE_BUTTON_RIGHT;
		case SDL_BUTTON_MIDDLE: return input::MOUSE_BUTTON_MIDDLE;
		case SDL_BUTTON_X1:     return input::MOUSE_BUTTON_X1;
		case SDL_BUTTON_X2:     return input::MOUSE_BUTTON_X2;
		default:                return input::KEY_UNKNOWN;
		}

	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
		switch (event.gbutton.button)
		{
		case SDL_GAMEPAD_BUTTON_SOUTH:           return input::BUTTON_GP_CROSS;
		case SDL_GAMEPAD_BUTTON_EAST:            return input::BUTTON_GP_CIRCLE;
		case SDL_GAMEPAD_BUTTON_WEST:            return input::BUTTON_GP_SQUARE;
		case SDL_GAMEPAD_BUTTON_NORTH:           return input::BUTTON_GP_TRIANGLE;
		case SDL_GAMEPAD_BUTTON_DPAD_UP:         return input::BUTTON_GP_UP;
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:      return input::BUTTON_GP_RIGHT;
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN:       return input::BUTTON_GP_DOWN;
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT:       return input::BUTTON_GP_LEFT;
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:   return input::BUTTON_GP_LB;
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:  return input::BUTTON_GP_RB;
		case SDL_GAMEPAD_BUTTON_LEFT_STICK:      return input::BUTTON_GP_LEFT_STICK;
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK:     return input::BUTTON_GP_RIGHT_STICK;
		case SDL_GAMEPAD_BUTTON_START:           return input::BUTTON_GP_START;
		case SDL_GAMEPAD_BUTTON_BACK:            return input::BUTTON_GP_SELECT;
		case SDL_GAMEPAD_BUTTON_GUIDE:           return input::BUTTON_GP_HOME;
		case SDL_GAMEPAD_BUTTON_TOUCHPAD:        return input::BUTTON_GP_TOUCHPAD;
		default:                                 return input::KEY_UNKNOWN;
		}
	default:
		return input::KEY_UNKNOWN;
	}
}

input::InputAxis PlatformModule::_castAxis(const SDL_Event& event) const
{
	switch (event.type)
	{
	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		switch (event.gaxis.axis)
		{
		case SDL_GAMEPAD_AXIS_LEFTX:         return input::GAMEPAD_AXIS_LEFT_X;
		case SDL_GAMEPAD_AXIS_LEFTY:         return input::GAMEPAD_AXIS_LEFT_Y;
		case SDL_GAMEPAD_AXIS_RIGHTX:        return input::GAMEPAD_AXIS_RIGHT_X;
		case SDL_GAMEPAD_AXIS_RIGHTY:        return input::GAMEPAD_AXIS_RIGHT_Y;
		case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:  return input::GAMEPAD_AXIS_LEFT_TRIGGER;
		case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return input::GAMEPAD_AXIS_RIGHT_TRIGGER;
		default:                             return input::MOUSE_AXIS_NONE;
		}

	default:
		return input::MOUSE_AXIS_NONE;
	}
}

void PlatformModule::_processEvent(const SDL_Event& event)
{
	switch (event.type) {
		// GESTION DE MANDOS
	case SDL_EVENT_GAMEPAD_ADDED: {
		uint32_t id = event.gdevice.which;
		SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
		if (gamepad) {
			_devicesID[id] = gamepad;
			input::VirtualDevice* virtualDevice = new input::VirtualDevice();
			virtualDevice->_setConnected(true);
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
			it->second->_setAxis(_castAxis(event), event.gaxis.value);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
		uint32_t id = event.gbutton.which;
		auto it = _virtualDevices.find(id);
		if (it != _virtualDevices.end() && event.key.repeat >= 1) {
			it->second->_setButton(_castButton(event), true);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_UP: {
		uint32_t id = event.gbutton.which;
		auto it = _virtualDevices.find(id);
		if (it != _virtualDevices.end() &&  event.key.repeat >= 1) {
			it->second->_setButton(_castButton(event), false);
		}
		break;
	}
									// TECLADO Y RATON
	case SDL_EVENT_MOUSE_MOTION: {
		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end()) {
			it->second->_setAxis(input::MOUSE_AXIS_X, event.motion.x);
			it->second->_setAxis(input::MOUSE_AXIS_Y, event.motion.y);
			it->second->_setAxis(input::MOUSE_AXIS_REL_X, event.motion.xrel);
			it->second->_setAxis(input::MOUSE_AXIS_REL_Y, event.motion.yrel);
		}
		break;
	}
	case SDL_EVENT_MOUSE_WHEEL:
	{

		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end()) {
			if (event.wheel.x != 0)
				it->second->_setAxis(input::MOUSE_AXIS_SCROLL_X, event.wheel.x);
			if (event.wheel.y != 0)
				it->second->_setAxis(input::MOUSE_AXIS_SCROLL_Y, event.wheel.y);
		}
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_KEY_DOWN: {
		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end() && (it->second->isReleased(_castButton(event)) && event.key.repeat)) {
			it->second->_setButton(_castButton(event), true);
		}
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_KEY_UP: {
		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end() && (!it->second->isJustPressed(_castButton(event)) && event.key.repeat)) {
			it->second->_setButton(_castButton(event), false);
		}
		break;
	}
	case SDL_EVENT_TEXT_INPUT: {
		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end())
			it->second->_appendText(event.text.text);
		break;
	}
	default:
		break;
	}
}

