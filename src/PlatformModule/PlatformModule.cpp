#include "PlatformModule.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_surface.h>

#include <Debug.h>
#include <optional>
#include <algorithm>

#include "VirtualDevice.h"
#include "InputMapper.h"

#include "GameConfigurator.h"
#include <checkMLNew.h>

#include "Network.h"


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
	delete _inputMapper;

	SDL_DestroySurface(_icon); // Elimina el surface para no dejar leaks.

	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool PlatformModule::Init()
{
	// Inicializacion de SDL
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		Debug::error("[Paltform] SDL No se ha podido inicializar.");
		return false;
	}
	// Creacion de ventana
	if ((_window = SDL_CreateWindow(core::GameConfigurator::instance()._windowName.c_str(),
		core::GameConfigurator::instance()._windowWidth,
		core::GameConfigurator::instance()._windowHeight,
		SDL_WINDOW_RESIZABLE)) == nullptr)
	{
		Debug::error("[Platform] SDL No se ha podido crear.");
		return false;
	}

	// Icono
	setIcon(core::GameConfigurator::instance()._iconRoot);

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

SDL_Window* PlatformModule::getSDLWindow()
{
	return _window;
}

bool PlatformModule::pollEvents()
{
	// limpia los ejes relativos
	auto it = _virtualDevices.find(input::KEYBOARD_ID);
	if (it != _virtualDevices.end()) {
		it->second->_setAxis(input::MOUSE_AXIS_REL_X, 0);
		it->second->_setAxis(input::MOUSE_AXIS_REL_Y, 0);
		it->second->_setAxis(input::MOUSE_AXIS_SCROLL_X, 0);
		it->second->_setAxis(input::MOUSE_AXIS_SCROLL_Y, 0);
	}

	// Update de los virtual devices
	for (auto& vd : _virtualDevices)
		vd.second->_update();

	// poll events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			return true;
		else
			_processEvent(event);
		if (_eventObserver)
		{
			_eventObserver(&event);
		}
	}

	if (_network.getConnectionState() != ConnectionState::IDLE)
	{
		_network.update();
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

void PlatformModule::setRelativeMouseMode(bool enabled) const
{
	SDL_SetWindowRelativeMouseMode(_window, enabled);
}

void PlatformModule::setMouseSensitivity(float sensitivity = 10.0)
{
	_mouseSensitivity = sensitivity;
}

bool PlatformModule::isDeviceConnected(input::DeviceID device) const
{
	bool connected = false;
	auto it = _virtualDevices.find(device);
	if (it != _virtualDevices.end()) connected = it->second->isConnected();
	return (device != input::ANY_DEVICE) && connected;
}

bool PlatformModule::isKeyPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _isKeyPressed(inputEvent, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isKeyPressedWithDevice(input::InputEvent inputEvent) const
{
	return _isKeyPressed(inputEvent, input::ANY_DEVICE);
}

bool PlatformModule::isJustPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _isJustPressed(inputEvent, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isJustPressedWithDevice(input::InputEvent inputEvent) const
{
	return _isJustPressed(inputEvent, input::ANY_DEVICE);
}

bool PlatformModule::isKeyReleased(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _isKeyReleased(inputEvent, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isKeyReleasedWithDevice(input::InputEvent inputEvent) const
{
	return _isKeyReleased(inputEvent, input::ANY_DEVICE);
}

float PlatformModule::getAxis(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _getAxis(inputEvent, device).first;
}

std::pair<float, input::DeviceID> PlatformModule::getAxisWithDevice(input::InputEvent inputEvent) const
{
	return _getAxis(inputEvent, input::ANY_DEVICE);
}

bool PlatformModule::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	return _isActionPressed(actionName, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isActionPressedWithDevice(const std::string& actionName) const
{
	return _isActionPressed(actionName, input::ANY_DEVICE);
}

bool PlatformModule::isActionJustPressed(const std::string& actionName, input::DeviceID device) const
{
	return _isActionJustPressed(actionName, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isActionJustPressedWithDevice(const std::string& actionName) const
{
	return _isActionJustPressed(actionName, input::ANY_DEVICE);
}

bool PlatformModule::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return _isActionReleased(actionName, device).first;
}

std::pair<bool, input::DeviceID> PlatformModule::isActionReleasedWithDevice(const std::string& actionName) const
{
	return _isActionReleased(actionName, input::ANY_DEVICE);
}

float PlatformModule::getActionAxis(const std::string& actionName, input::DeviceID device) const
{
	return _getActionAxis(actionName, device).first;
}

std::pair<float, input::DeviceID> PlatformModule::getActionAxisWithDevice(const std::string& actionName) const
{
	return _getActionAxis(actionName, input::ANY_DEVICE);
}

void PlatformModule::startTextInput(bool blockKeyboard)
{
	_blockKeyboard = blockKeyboard;
	_textInputActive = true;
	SDL_StartTextInput(_window);
}

void PlatformModule::stopTextInput()
{
	_textInputActive = false;
	SDL_StopTextInput(_window);
}

std::string PlatformModule::getTextInput(input::DeviceID device) const
{
	input::DeviceID id = (device == input::ANY_DEVICE) ? input::KEYBOARD_ID : device;
	auto it = _virtualDevices.find(id);
	if (it != _virtualDevices.end())
		return it->second->getTextInput();
	return {};
}

void PlatformModule::clearTextInput(input::DeviceID device)
{
	input::DeviceID id = (device == input::ANY_DEVICE) ? input::KEYBOARD_ID : device;
	auto it = _virtualDevices.find(id);
	if (it != _virtualDevices.end())
		it->second->clearTextInput();
}

input::InputMapper* PlatformModule::getInputMapper() const
{
	return _inputMapper;
}

void PlatformModule::setWindowSize(int w, int h)
{
	SDL_SetWindowSize(_window, w, h);
}

bool PlatformModule::setIcon(const std::string& path)
{
	if (_icon != nullptr)
	{
		SDL_DestroySurface(_icon);
		_icon = nullptr;
	}
	_icon = SDL_LoadSurface(path.c_str());
	if (!_icon)
	{
		Debug::error("[Platform] Window icon en la ruta \"", path, "\" no existe.");
		return false;
	}
	// Icono de la ventana
	if (!SDL_SetWindowIcon(_window, _icon))
	{
		Debug::error("[Platform] No se ha podido asignar el window icon : \"", path, "\" a la ventana.");
		return false;
	}
	// Icono de la taskbar

	Debug::out("[Platform] Window icon cambiado: \"", path, "\".");
	return true;
}

void PlatformModule::setWindowName(const std::string& name)
{
	SDL_SetWindowTitle(_window, name.c_str());
}

void PlatformModule::setWindowResizable(bool enabled)
{
	_windowResizable = enabled;
	_applyWindowStyleRestrictions();
}

void PlatformModule::setWindowMaximizable(bool enabled)
{
	_windowMaximizable = enabled;
	_applyWindowStyleRestrictions();
}

bool PlatformModule::setFullscreen(bool enabled) const
{
	if (_window == nullptr)
		return false;
	if (!SDL_SetWindowFullscreen(_window, enabled))
	{
		Debug::error("[Platform Module] No se pudo cambiar a fullscreen: ", SDL_GetError());
		return false;
	}
	_applyWindowStyleRestrictions();
	return true;
}

bool PlatformModule::isFullscreen() const
{
	if (_window == nullptr)
		return false;
	return (SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN) != 0;
}

void PlatformModule::registerEventObserver(EventCallback callback)
{
	_eventObserver = callback;
}

void PlatformModule::_applyWindowStyleRestrictions() const
{
	if (_window != nullptr)
	{
		SDL_SetWindowResizable(_window, _windowResizable);
	}

	if (_windowHandle != nullptr)
	{
		LONG_PTR style = GetWindowLongPtr(_windowHandle, GWL_STYLE);
		if (_windowMaximizable)
			style |= WS_MAXIMIZEBOX;
		else
			style &= ~WS_MAXIMIZEBOX;

		SetWindowLongPtr(_windowHandle, GWL_STYLE, style);
		SetWindowPos(_windowHandle, nullptr, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void PlatformModule::setGamepadVibration(input::DeviceID id, float lowFreq, float highFreq, uint32_t dur)
{
	// Clampeamos los valores dados a entre 0.0 y 1.0 y los convertimos a la unidad que pide SDL.
	uint16_t clampLow = static_cast<uint16_t>(std::clamp(lowFreq, 0.0f, 1.0f) * (std::numeric_limits<uint16_t>::max)());
	uint16_t clampHigh = static_cast<uint16_t>(std::clamp(highFreq, 0.0f, 1.0f) * (std::numeric_limits<uint16_t>::max)());

	if (id == input::ANY_DEVICE) // Si todos los mandos pues todos los mandos vibran.
	{
		for (auto it : _devicesID)
		{
			if (!SDL_RumbleGamepad(it.second, clampLow, clampHigh, dur))
			{
				Debug::error("[Platform] No se ha podido vibrar el gamepad con id\"", it.first, "\".");
			}
		}
	}
	else
	{
		auto it = _devicesID.find(id);
		if (it != _devicesID.end())
		{
			if (!SDL_RumbleGamepad(it->second, clampLow, clampHigh, dur))
			{
				Debug::error("[Platform] No se ha podido vibrar el gamepad con id\"", it->first, "\".");
			}
		}
	}
}

void PlatformModule::setGamepadColor(input::DeviceID id, const core::Color& color)
{
	// Clampeamos los valores dados a entre 0.0 y 1.0 y los convertimos a la unidad que pide SDL.
	uint8_t clampR = static_cast<uint8_t>(std::clamp(color.getRed(), 0.0f, 1.0f) * (std::numeric_limits<uint8_t>::max)());
	uint8_t clampG = static_cast<uint8_t>(std::clamp(color.getGreen(), 0.0f, 1.0f) * (std::numeric_limits<uint8_t>::max)());
	uint8_t clampB = static_cast<uint8_t>(std::clamp(color.getBlue(), 0.0f, 1.0f) * (std::numeric_limits<uint8_t>::max)());

	setGamepadColor(id, clampR, clampG, clampB);
}

void PlatformModule::setGamepadColor(input::DeviceID id, uint8_t r, uint8_t g, uint8_t b)
{
	if (id == input::ANY_DEVICE) // Si todos los mandos pues todos los mandos cambian de color.
	{
		for (auto it : _devicesID)
		{
			if (!SDL_SetGamepadLED(it.second, r, g, b))
			{
				Debug::error("[Platform] No se ha podido cambiar el color del led del gamepad con id\"", it.first, "\".");
			}
		}
	}
	else
	{
		auto it = _devicesID.find(id);
		if (it != _devicesID.end())
		{
			if (!SDL_SetGamepadLED(it->second, r, g, b))
			{
				Debug::error("[Platform] No se ha podido cambiar el color del led del gamepad con id\"", it->first, "\".");
			}
		}
	}
}

#pragma region Network

bool PlatformModule::networkInit()
{
	return _network.Init();
}

void PlatformModule::networkShutdown()
{
	_network.shutdown();
}

bool PlatformModule::networkHost(uint16_t port)
{
	return _network.hostSession(port);;
}

bool PlatformModule::networkJoin(const std::string& ip, uint16_t port)
{
	return _network.joinSession(ip, port);
}

void PlatformModule::networkDisconnect()
{
	_network.disconnect();
}

template<typename T>
inline void PlatformModule::networkSend(uint8_t type, const T& payload)
{
	_network.send(type, payload);
}

ObserverID PlatformModule::networkAddObserver(uint8_t type, Network::PacketCallback cb)
{
	return _network.addObserver(type, cb);
}

void PlatformModule::networkUnsubscribe(uint8_t type, ObserverID id)
{
	_network.unsubscribe(type, id);
}

void PlatformModule::networkClearObservers()
{
	_network.clearObservers();
}

ConnectionState PlatformModule::networkGetConnectionState()
{
	return _network.getConnectionState();;
}

bool PlatformModule::networkIsConnected()
{
	return _network.isConnected();
}

std::string PlatformModule::networkGetLocalIp()
{
	return _network.getLocalIP();
}

NetworkRole PlatformModule::networkGetRole()
{
	return _network.getRole();
}

#pragma endregion

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

std::pair<bool, input::DeviceID> PlatformModule::_isKeyPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	if (_textInputActive && !_isTextInputAllowed(inputEvent))
		return { false, device };
	// Usa std::visit para seleccionar y ejecutar una funcion de tipo de dato del inputEvent.
	// "func" es la funcion escogida segun el tipo de dato de inputEvent.
	auto func = [&](const input::VirtualDevice* vd) -> bool
		{
			return std::visit(input::overloaded{
				[&](input::Key k) { return vd->isPressed(k); },
				[&](input::GamepadButton b) { return vd->isPressed(b); },
				[&](input::MouseButton b) { return vd->isPressed(b); },
				[](auto&&) { Debug::error("[Input] inputEvent no permitido"); return false; }
				}, inputEvent); // Le pasamos ya el InputEvent para no tener que gestionarlo luego.
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE)
	{
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return { func(it->second), device };
		Debug::error("[Input] device: ", device, " no encontrado");
		return { false, device };
	}

	// ANY_DEVICE - early exit en cuanto algun device tenga la tecla pulsada.
	for (const auto& [id, vd] : _virtualDevices)
		if (func(vd))
		{
			return { true, id }; // El device id ha sido pulsado.
		}
	return { false, device };
}

std::pair<bool, input::DeviceID> PlatformModule::_isJustPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	if (_textInputActive && !_isTextInputAllowed(inputEvent))
		return { false, device };
	// Usa std::visit para seleccionar y ejecutar una funcion de tipo de dato del inputEvent.
	// "func" es la funcion escogida segun el tipo de dato de inputEvent.
	auto func = [&](const input::VirtualDevice* vd) -> bool {
		return std::visit(input::overloaded{
			[&](input::Key k) { return vd->isJustPressed(k); },
			[&](input::GamepadButton b) { return vd->isJustPressed(b); },
			[&](input::MouseButton b) { return vd->isJustPressed(b); },
			[](auto&& inputEvent) { Debug::error("[Input] inputEvent no permitido ", toString(inputEvent)); return false; }
			}, inputEvent); // Le pasamos ya el InputEvent para no tener que gestionarlo luego.
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE) {
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return { func(it->second), device };
		Debug::error("[Input] device: ", device, " no encontrado");
		return { false, device };
	}

	// ANY_DEVICE - early exit en cuanto algun device tenga la tecla pulsada.
	for (const auto& [id, vd] : _virtualDevices)
		if (func(vd))
		{
			return { true, id };
		}
	return { false, input::ANY_DEVICE };
}

std::pair<bool, input::DeviceID> PlatformModule::_isKeyReleased(input::InputEvent inputEvent, input::DeviceID device) const
{
	if (_textInputActive && !_isTextInputAllowed(inputEvent))
		return { false, device };
	// Usa std::visit para seleccionar y ejecutar una funcion de tipo de dato del inputEvent.
	// "func" es la funcion escogida segun el tipo de dato de inputEvent.
	auto func = [&](const input::VirtualDevice* vd) -> bool {
		return std::visit(input::overloaded{
			[&](input::Key k) {return vd->isReleased(k); },
			[&](input::GamepadButton b) {return vd->isReleased(b); },
			[&](input::MouseButton b) {return vd->isReleased(b); },
			[](auto&& inputEvent) { Debug::error("[Input] inputEvent no permitido ", toString(inputEvent)); return false; }
			}, inputEvent);
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE)
	{
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return { func(it->second), device };
		Debug::error("[Input] device: ", device, " no encontrado.");
		return { false, device };
	}
	// ANY_DEVICE - early exit en cuanto algun device tenga la tecla pulsada.
	for (const auto& [id, vd] : _virtualDevices)
		if (func(vd))
		{
			return { true, id }; // El device id ha sido pulsado.
		}
	return { false, input::ANY_DEVICE };
}

std::pair<float, input::DeviceID> PlatformModule::_getAxis(input::InputEvent inputEvent, input::DeviceID device) const
{
	auto func = [&](const input::VirtualDevice* vd) -> float {
		return std::visit(input::overloaded{
			[&](input::MouseAxis a) {return vd->getAxis(a); },
			[&](input::GamepadAxis a) {return vd->getAxis(a); },
			[](auto&& inputEvent) { Debug::error("[Input] inputEvent no permitido ", toString(inputEvent)); return 0.0f; }
			}, inputEvent);
		};
	// Si el device es uno concreto llama a su funcion.
	if (device != input::ANY_DEVICE)
	{
		auto it = _virtualDevices.find(device);
		if (it != _virtualDevices.end()) return { func(it->second), device };
		Debug::error("[Input] device: ", device, " no encontrado");
		return { 0.0f, device };
	}
	// ANY_DEVICE - Coge el mayor input de todos los device.
	input::DeviceID deviceID = device;
	float maxVal = 0.0f;
	for (const auto& [id, vd] : _virtualDevices) {
		float value = func(vd);
		if (abs(value) > abs(maxVal))
		{
			maxVal = value;
			deviceID = id;
		}
	}

	return { maxVal, deviceID };
}

std::pair<bool, input::DeviceID> PlatformModule::_isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		auto pair = _isKeyPressed(event, device);
		if (pair.first)
			return pair;
	}
	return { false, device };
}

std::pair<bool, input::DeviceID> PlatformModule::_isActionJustPressed(const std::string& actionName, input::DeviceID device) const
{
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		auto pair = _isJustPressed(event, device);
		if (pair.first)
			return pair;
	}
	return { false, device };
}

std::pair<bool, input::DeviceID> PlatformModule::_isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		auto pair = _isKeyReleased(event, device);
		if (pair.first)
			return pair;
	}
	return { false, device };
}

std::pair<float, input::DeviceID> PlatformModule::_getActionAxis(const std::string& actionName, input::DeviceID device) const
{
	std::pair<float, input::DeviceID> result;
	for (input::InputEvent event : _inputMapper->getInputEvents(actionName, device)) {
		auto pair = _getAxis(event, device);
		if (abs(pair.first) > abs(result.first))
		{
			result = pair;
		}
	}
	return result;
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
			input::VirtualDevice* virtualDevice = new input::VirtualDevice();
			virtualDevice->_setConnected(true);
			// Miramos si hay ids disponibles en la cola de ids liberadas.
			if (_releasedDevicedsID.empty())
			{
				uint32_t newID = _virtualDevices.size();
				_traductionMap[id] = newID;
				_devicesID[newID] = gamepad;
				_virtualDevices[newID] = virtualDevice;
				Debug::warning("[Platform] Nuevo Gamepad con id de SDL: ", id, " y nuestra id: ", newID);
			}
			else
			{
				uint32_t releasedId = _releasedDevicedsID.front();
				_releasedDevicedsID.pop();
				_traductionMap[id] = releasedId;
				_devicesID[releasedId] = gamepad;
				_virtualDevices[releasedId] = virtualDevice;
				Debug::warning("[Platform] Gamepad con id reutilizado: ", releasedId);
			}
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_REMOVED:
	{
		uint32_t id = event.gdevice.which;
		auto tra = _traductionMap.find(id);
		auto it = _devicesID.find(tra->first);
		if (it != _devicesID.end())
		{
			SDL_RumbleGamepad(it->second, 0, 0, 0); // Quitar cualquier posible vibracion por si acaso.
			SDL_CloseGamepad(it->second);
			_devicesID.erase(it);
		}
		auto vit = _virtualDevices.find(tra->second);
		if (vit != _virtualDevices.end())
		{
			_releasedDevicedsID.push(vit->first); // Nos guardamos el id del device liberado.
			delete vit->second;
			_virtualDevices.erase(vit);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
		uint32_t id = event.gaxis.which;
		auto trad = _traductionMap.find(id); // Dado el id de SDl buscamos en las traducciones el id nuestro.
		auto it = _virtualDevices.find(trad->second);
		if (it != _virtualDevices.end()) {
			it->second->_setAxis(_castAxis(event), event.gaxis.value);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
		uint32_t id = event.gbutton.which;
		auto trad = _traductionMap.find(id); // Dado el id de SDl buscamos en las traducciones el id nuestro.
		auto it = _virtualDevices.find(trad->second);
		if (it != _virtualDevices.end()) {
			it->second->_setButton(_castButton(event), true);
		}
		break;
	}
	case SDL_EVENT_GAMEPAD_BUTTON_UP: {
		uint32_t id = event.gbutton.which;
		auto trad = _traductionMap.find(id); // Dado el id de SDl buscamos en las traducciones el id nuestro.
		auto it = _virtualDevices.find(trad->second);
		if (it != _virtualDevices.end()) {
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

			float relX = std::clamp(event.motion.xrel / _mouseSensitivity, -1.0f, 1.0f);
			float relY = std::clamp(event.motion.yrel / _mouseSensitivity, -1.0f, 1.0f);

			it->second->_setAxis(input::MOUSE_AXIS_REL_X, relX);
			it->second->_setAxis(input::MOUSE_AXIS_REL_Y, relY);
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
		if (it != _virtualDevices.end() &&
			(!it->second->isPressed(_castButton(event))))
		{
			it->second->_setButton(_castButton(event), true);
		}
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_KEY_UP: {
		auto it = _virtualDevices.find(input::KEYBOARD_ID);
		if (it != _virtualDevices.end() &&
			(it->second->isPressed(_castButton(event))))
		{
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

bool PlatformModule::_isTextInputAllowed(input::InputEvent inputEvent) const
{
	if (!_blockKeyboard) return true;
	// Teclas permitidas
	static const std::vector<input::Key> allowed = {
		input::KEY_DELETE,
		input::KEY_ESCAPE,
		input::KEY_ENTER,
		input::KEY_KP_ENTER
	};
	return std::visit(input::overloaded{
		[&](input::Key k) {
			return std::find(allowed.begin(), allowed.end(), k) != allowed.end();
		},
		[](auto&&) { return true; }
		}, inputEvent);
}
