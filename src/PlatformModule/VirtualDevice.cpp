#include "VirtualDevice.h"


input::VirtualDevice::VirtualDevice()
{
	_connected = true;
}

void input::VirtualDevice::setConnected(bool con)
{
	_connected = con;
}

void input::VirtualDevice::setAxis(input::InputAxis axis, float value)
{
	/*axis | match{
	[](MouseAxis x) { _mouseAxisState[x] = value; },
	[](GamepadAxis x) { _gamepadAxisState[x] = value; }
	};*/
}

void input::VirtualDevice::setButton(input::InputButtons button, bool value)
{/*
	button | match{
	[](Key x) { _keyState[x] = value; },
	[](MouseButton x) { _mouseButtonState[x] = value; },
	[](GamepadButton x) { _gamepadButtonState[x] = value; }
	};*/
}

bool input::VirtualDevice::IsPressed(Key key) const
{
	return _connected && _keyState[key];
}

bool input::VirtualDevice::IsPressed(MouseButton button) const
{
	return _connected && _mouseButtonState[button];
}

bool input::VirtualDevice::IsPressed(GamepadButton button) const
{
	return _connected && _gamepadButtonState[button];
}

float input::VirtualDevice::GetAxis(MouseAxis axis) const
{
	return _connected ? _mouseAxisState[axis] : 0.0f;
}

float input::VirtualDevice::GetAxis(GamepadAxis axis) const
{
	return _connected ? _gamepadAxisState[axis] : 0.0f;
}

bool input::VirtualDevice::IsConnected() const
{
	return _connected;
}
