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
			Debug::out( _keyState[b]);
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
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			return isPressed(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			return isPressed(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			return isPressed(b);
		}
		}, button);
	return false;
}

bool input::VirtualDevice::isReleased(Key key) const
{
	return _connected && !_keyState && _keyLastState;
}

bool input::VirtualDevice::isReleased(MouseButton button) const
{
	return _connected && !_mouseButtonState && _mouseButtonLastState;
}

bool input::VirtualDevice::isReleased(GamepadButton button) const
{
	return _connected && !_gamepadButtonState && _gamepadButtonLastState;
}

bool input::VirtualDevice::isReleased(InputButtons button) const
{
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			return isReleased(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			return isReleased(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			return isReleased(b);
		}
		}, button);
	return false;
}

bool input::VirtualDevice::isJustPressed(Key key) const
{
	return _connected && _keyState && !_keyLastState;
}

bool input::VirtualDevice::isJustPressed(MouseButton button) const
{
	return _connected && _mouseButtonState && !_mouseButtonLastState;
}

bool input::VirtualDevice::isJustPressed(GamepadButton button) const
{
	return _connected && _gamepadButtonState && !_gamepadButtonLastState;
}

bool input::VirtualDevice::isJustPressed(InputButtons button) const
{
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
		{
			return isJustPressed(b);
		}
		else if constexpr (std::is_same_v<T, MouseButton>)
		{
			return isJustPressed(b);
		}
		else if constexpr (std::is_same_v<T, GamepadButton>)
		{
			return isJustPressed(b);
		}
		}, button);
	return false;
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
	std::visit([&](auto a) {
		using T = decltype(a);
		if constexpr (std::is_same_v<T, MouseAxis>)
		{
			return getAxis(a);
		}
		else if constexpr (std::is_same_v<T, GamepadAxis>)
		{
			return getAxis(a);
		}
		}, axis);
	return 0.0f;
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
