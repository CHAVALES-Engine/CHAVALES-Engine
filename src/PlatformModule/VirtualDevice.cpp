#include "VirtualDevice.h"
#include <Debug.h>

input::VirtualDevice::VirtualDevice()
{
	_connected = true;
}

void input::VirtualDevice::_setConnected(bool con)
{
	_connected = con;
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
			_gamepadAxisState[a] = value;
		}
		}, axis);
}

void input::VirtualDevice::_setButton(input::InputButtons button, bool value)
{
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			//Debug::out(_keyState[b]);
			_keyLastState[b] = _keyState[b];
			_keyState[b] = value;
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			_mouseButtonLastState[b] = _mouseButtonState[b];
			_mouseButtonState[b] = value;
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			_gamepadButtonLastState[b] = _gamepadButtonState[b];
			_gamepadButtonState[b] = value;
		}
		}, button);
}

void input::VirtualDevice::_appendText(const std::string& text)
{
	_textBuffer += text;
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
	return false;
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
