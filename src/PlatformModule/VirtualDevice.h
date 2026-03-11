#pragma once
#include <string>

#include "InputDefs.h"

class PlatformModule;

namespace input
{
	// Clase para registrar el estado del mando
    class VirtualDevice {
    public:
        VirtualDevice();
        bool IsPressed(Key key) const;
        bool IsPressed(MouseButton button) const;
        bool IsPressed(GamepadButton button) const;
        float GetAxis(MouseAxis axis) const;
        float GetAxis(GamepadAxis axis) const;
        bool IsConnected() const;
    private:
        friend class PlatformModule;

        bool _keyState[Key::KEY_COUNT] = {};
        bool _mouseButtonState[MouseButton::MOUSE_BUTTON_COUNT] = {};
        float _mouseAxisState[MouseAxis::MOUSE_AXIS_COUNT] = {};
        bool _gamepadButtonState[GamepadButton::BUTTON_GP_COUNT] = {};
        float _gamepadAxisState[GamepadAxis::GAMEPAD_AXIS_COUNT] = {};
        bool _connected = false;
    };
}
