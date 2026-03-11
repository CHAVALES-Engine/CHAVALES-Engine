#pragma once
#include <string>

#include "InputDefs.h"

class PlatformModule;


namespace input
{
    /**
    * @brief Clase para registrar el estado del mando.
    */
    class VirtualDevice
    {
    public:
        
        /**
        * @brief Devuelve el estado de una tecla.
        * 
        * @param key - Tecla a comprobar.
        * 
        * @returns bool - Estado de la tecla.
        */
        bool IsPressed(Key key) const;
        /**
        * @brief Devuelve el estado de un boton del raton.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool IsPressed(MouseButton button) const;
        /**
        * @brief Devuelve el estado de un boton del pad.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool IsPressed(GamepadButton button) const;
        /**
        * @brief Devuelve el estado de un eje del raton.
        *
        * @param axis - Eje a comprobar.
        *
        * @returns float - Estado del eje.
        */
        float GetAxis(MouseAxis axis) const;
        /**
        * @brief Devuelve el estado de un eje del pad.
        *
        * @param axis - Eje a comprobar.
        *
        * @returns float - Estado del eje.
        */
        float GetAxis(GamepadAxis axis) const;
        /**
        * @brief Devuelve el estado del dispositivo.
        *
        * @returns bool - Estado del dispositivo.
        */
        bool IsConnected() const;

    private:

        /**
        * @brief Construcora de VirtualDevice.
        */
        VirtualDevice();
        /**
        * @brief Para establecer si el dispositivo esta conectado o no.
        * 
        * @param con - Estado a establecer.
        */
        void setConnected(bool con);

        /**
        * @brief Clase PlatformModule amiga para poder acceder a metodos privados.
        */
        friend class PlatformModule;

        /**
        * @brief Array de tamanyo KEY_COUNT con los estados de la teclas.
        */
        bool _keyState[Key::KEY_COUNT] = {};
        /**
        * @brief Array de tamanyo MOUSE_BUTTON_COUNT con los estados de los botones del raton.
        */
        bool _mouseButtonState[MouseButton::MOUSE_BUTTON_COUNT] = {};
        /**
        * @brief Array de tamanyo MOUSE_AXIS_COUNT con el estado de los ejes del raton.
        */
        float _mouseAxisState[MouseAxis::MOUSE_AXIS_COUNT] = {};
        /**
        * @brief Array de tamanyo BUTTON_GP_COUNT con los estados de los botones del pad.
        */
        bool _gamepadButtonState[GamepadButton::BUTTON_GP_COUNT] = {};
        /**
        * @brief Array de tamanyo GAMEPAD_AXIS_COUNT con el estado de los ejes del pad.
        */
        float _gamepadAxisState[GamepadAxis::GAMEPAD_AXIS_COUNT] = {};
        /**
        * @brief Estado del dispositivo.
        */
        bool _connected = false;
    };
}
