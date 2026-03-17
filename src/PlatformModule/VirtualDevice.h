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
        bool isPressed(Key key) const;
        /**
        * @brief Devuelve el estado de un boton del raton.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool isPressed(MouseButton button) const;
        /**
        * @brief Devuelve el estado de un boton del pad.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool isPressed(GamepadButton button) const;
        // TODO
        /**
        * @brief Devuelve el estado de una tecla.
        *
        * @param key - Tecla a comprobar.
        *
        * @returns bool - Estado de la tecla.
        */
        bool isReleased(Key key) const;
        /**
        * @brief Devuelve el estado de un boton del raton.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool isReleased(MouseButton button) const;
        /**
        * @brief Devuelve el estado de un boton del pad.
        *
        * @param button - Boton a comprobar.
        *
        * @returns bool - Estado del boton.
        */
        bool isReleased(GamepadButton button) const;
        /**
        * @brief Devuelve el estado de un eje del raton.
        *
        * @param axis - Eje a comprobar.
        *
        * @returns float - Estado del eje.
        */
        float getAxis(MouseAxis axis) const;
        /**
        * @brief Devuelve el estado de un eje del pad.
        *
        * @param axis - Eje a comprobar.
        *
        * @returns float - Estado del eje.
        */
        float getAxis(GamepadAxis axis) const;
        /**
        * @brief Devuelve el estado del dispositivo.
        *
        * @returns bool - Estado del dispositivo.
        */
        bool isConnected() const;
        /**
         * @brief Devuelve el input de texto escrito.
         * @return std::string& - String de texto escrito.
         */
        const std::string& getTextInput() const;
        /**
         * @brief Borra el buffer del input de texto para cuando se deja de escribir.
         */
        void clearTextInput();
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
        void _setConnected(bool con);
    	/**
        * @brief Para settear un valor a un axis;
        * 
        * @param axis - Axis a establecer.
        * @param value - Cantidad accionada.
        */
        void _setAxis(input::InputAxis axis, float value);
        /**
        * @brief Para settear un valor a un boton.
        *
        * @param button - Boton a establecer.
        * @param value - Estado a establecer.
        */
        void _setButton(input::InputButtons button, bool value);
        /**
         * @brief Anyade texto al buffer de texto cuando se esta haciendo input de texto.
         * @param text - Texto que se anyade al buffer.
         */
        void _appendText(const std::string& text);
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
         * @brief Buffer de texto para cuando se esta haciento input de texto.
         */
        std::string _textBuffer;
        /**
        * @brief Estado del dispositivo.
        */
        bool _connected = false;
    };
}
