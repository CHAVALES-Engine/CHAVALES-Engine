#include "VirtualDevice.h"

#include <unordered_map>


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
			_mouseAxisState[a] = value;
		else if constexpr (std::is_same_v<T, GamepadAxis>)
			_gamepadAxisState[a] = value;
		}, axis);
}

void input::VirtualDevice::_setButton(input::InputButtons button, bool value)
{
	std::visit([&](auto b) {
		using T = decltype(b);
		if constexpr (std::is_same_v<T, Key>)
			_keyState[b] = value;
		else if constexpr (std::is_same_v<T, MouseButton>)
			_mouseButtonState[b] = value;
		else if constexpr (std::is_same_v<T, GamepadButton>)
			_gamepadButtonState[b] = value;
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

bool input::VirtualDevice::isReleased(Key key) const
{
	return false;
}

bool input::VirtualDevice::isReleased(MouseButton button) const
{
	return false;
}

bool input::VirtualDevice::isReleased(GamepadButton button) const
{
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
