#include "VirtualDevice.h"
#include <Debug.h>
#include <algorithm>

input::VirtualDevice::VirtualDevice()
{
	_connected = true;
}

void input::VirtualDevice::_setConnected(bool con)
{
	_connected = con;
}

void input::VirtualDevice::_update()
{
	std::memcpy(_mouseButtonLastState, _mouseButtonState, sizeof(_mouseButtonState));
	std::memcpy(_keyLastState, _keyState, sizeof(_keyState));
	std::memcpy(_gamepadButtonLastState, _gamepadButtonState, sizeof(_gamepadButtonState));
}

void input::VirtualDevice::_setAxis(input::InputAxis axis, float value)
{
	std::visit([&](auto a) {
		using T = decltype(a);
		if constexpr (std::is_same_v<T, MouseAxis>)
		{
			_mouseAxisState[a] = value;
		}
		else if constexpr (std::is_same_v<T, GamepadAxis>)
		{
			// SDL usa int16 para los rangos de los joysticks.
			constexpr float MAX_VALUE = std::numeric_limits<int16_t>::max();
			// Escalamos para que el rango util sea -1.0 a 1.0 compensando deadzone.
			const float deadzoneFactor = _deadzone / MAX_VALUE;

			// Normalizar el valor a rango -1.0 a 1.0
			float normalized = value / MAX_VALUE; // usamos int16, 
			// Si esta dentro de la deadzone 0.
			if (normalized < _deadzone)
			{
				_gamepadAxisState[a] = 0.0f;
				return;
			}

			// Aplicamos la zona muerta.
			normalized = normalized > 0 ?
				(normalized - deadzoneFactor) / (1.0f - deadzoneFactor) :
				(normalized + deadzoneFactor) / (1.0f - deadzoneFactor);

			// Clampeamos a -1.0 a 1.0 por si acaso
			float clampValue = (normalized < -1.0f) ? -1.0f : (normalized > 1.0f) ? 1.0f : normalized;

			_gamepadAxisState[a] = clampValue;
		}
		}, axis);
}

void input::VirtualDevice::_setButton(input::InputButtons button, bool value)
{
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			_keyState[b] = value;
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			_mouseButtonState[b] = value;
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			_gamepadButtonState[b] = value;
		}
		}, button);
}

void input::VirtualDevice::_appendText(const std::string& text)
{
	_textBuffer += text;
}

float input::VirtualDevice::_getDeadzone()
{
	return _deadzone;
}

void input::VirtualDevice::_setDeadzone(float dz)
{
	if (dz > MAX_DEADZONE || dz < 0) return;

	_deadzone = dz;
}

bool input::VirtualDevice::isPressed(Key key) const
{
	return _connected && _keyState[key];
}

bool input::VirtualDevice::isPressed(MouseButton button) const
{
	return _connected && _mouseButtonState[button];
}

bool input::VirtualDevice::isPressed(GamepadButton button) const
{
	return _connected && _gamepadButtonState[button];
}

bool input::VirtualDevice::isPressed(InputButtons button) const
{
	bool result = false;
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			result = isPressed(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			result = isPressed(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			result = isPressed(b);
		}
		}, button);
	return result;
}

bool input::VirtualDevice::isReleased(Key key) const
{
	return _connected && !_keyState[key] && _keyLastState[key];
}

bool input::VirtualDevice::isReleased(MouseButton button) const
{
	return _connected && !_mouseButtonState[button] && _mouseButtonLastState[button];
}

bool input::VirtualDevice::isReleased(GamepadButton button) const
{
	return _connected && !_gamepadButtonState[button] && _gamepadButtonLastState[button];
}

bool input::VirtualDevice::isReleased(InputButtons button) const
{
	bool result = false;
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			result = isReleased(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			result = isReleased(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			result = isReleased(b);
		}
		}, button);
	return result;
}

bool input::VirtualDevice::isJustPressed(Key key) const
{
	return _connected && _keyState[key] && !_keyLastState[key];
}

bool input::VirtualDevice::isJustPressed(MouseButton button) const
{
	return _connected && _mouseButtonState[button] && !_mouseButtonLastState[button];
}

bool input::VirtualDevice::isJustPressed(GamepadButton button) const
{
	return _connected && _gamepadButtonState[button] && !_gamepadButtonLastState[button];
}

bool input::VirtualDevice::isJustPressed(InputButtons button) const
{
	bool result = false;
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			result = isJustPressed(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			result = isJustPressed(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			result = isJustPressed(b);
		}
		}, button);
	return result;
}

float input::VirtualDevice::getAxis(MouseAxis axis) const
{
	return _connected ? _mouseAxisState[axis] : 0.0f;
}

float input::VirtualDevice::getAxis(GamepadAxis axis) const
{
	return _connected ? _gamepadAxisState[axis] : 0.0f;
}

float input::VirtualDevice::getAxis(InputAxis axis) const
{
	float result = 0.0;
	std::visit([&](auto a) {
		using T = decltype(a);
		if constexpr (std::is_same_v<T, MouseAxis>)
		{
			result = getAxis(a);
		}
		else if constexpr (std::is_same_v<T, GamepadAxis>)
		{
			result = getAxis(a);
		}
		}, axis);
	return result;
}

bool input::VirtualDevice::isConnected() const
{
	return _connected;
}

const std::string& input::VirtualDevice::getTextInput() const
{
	return _textBuffer;
}

void input::VirtualDevice::clearTextInput()
{
	_textBuffer.clear();
}
