/**
* @file
*/

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
		/**
		* @brief Devuelve el estado de un boton variante.
		*
		* @param button - Boton variante a comprobar.
		*
		* @returns bool - Estado del boton variante.
		*/
		bool isPressed(InputButtons button) const;


		/**
		* @brief Devuelve si una tecla se acaba de soltar.
		*
		* @param key - Tecla a comprobar.
		*
		* @returns bool - Estado de la tecla.
		*/
		bool isReleased(Key key) const;
		/**
		* @brief Devuelve si un boton se acaba de soltar.
		*
		* @param button - Boton a comprobar.
		*
		* @returns bool - Estado del boton.
		*/
		bool isReleased(MouseButton button) const;
		/**
		* @brief Devuelve si un boton se acaba de soltar.
		*
		* @param button - Boton a comprobar.
		*
		* @returns bool - Estado del boton.
		*/
		bool isReleased(GamepadButton button) const;
		/**
		* @brief Devuelve si un boton variante se acaba de pulsar.
		*
		* @param key - boton variante a comprobar.
		*
		* @returns bool - Estado de la boton variante.
		*/
		bool isReleased(InputButtons button) const;


		/**
		* @brief Devuelve si una tecla se acaba de pulsar.
		*
		* @param key - Tecla a comprobar.
		*
		* @returns bool - Estado de la tecla.
		*/
		bool isJustPressed(Key key) const;
		/**
		* @brief Devuelve si un boton se acaba de pulsar.
		*
		* @param button - Boton a comprobar.
		*
		* @returns bool - Estado del boton.
		*/
		bool isJustPressed(MouseButton button) const;
		/**
		* @brief Devuelve si un boton se acaba de pulsar.
		*
		* @param button - Boton a comprobar.
		*
		* @returns bool - Estado del boton.
		*/
		bool isJustPressed(GamepadButton button) const;
		/**
		* @brief Devuelve si un boton variante se acaba de pulsar.
		*
		* @param button - Boton variante a comprobar.
		*
		* @returns bool - Estado del boton variante.
		*/
		bool isJustPressed(InputButtons button) const;


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
		* @brief Devuelve el estado de un eje variante.
		*
		* @param axis - Eje variante a comprobar.
		*
		* @returns float - Estado del eje variante.
		*/
		float getAxis(InputAxis axis) const;

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
		* @brief Update de VirtualDevice.
		*/
		void _update();
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
		 * @brief Aniade texto al buffer de texto cuando se esta haciendo input de texto.
		 * @param text - Texto que se aniade al buffer.
		 */
		void _appendText(const std::string& text);
		/**
		 * @brief Devuelve el valor de la deadzone.
		 * @returns float - Valor del deadzone actual.
		 */
		float _getDeadzone();
		/**
		 * @brief Cambia la deadzone para los joysticks por el valor dado.
		 * @param dz - Valor a cambiar.
		 */
		void _setDeadzone(float dz);

		/**
		 * @brief Elimina el texto guardado cuando se esta escribiendo.
		 */
		void _backspaceText();

		//------Arrays de teclas:
		/**
		* @brief Array de tamanyo KEY_COUNT con los estados de la teclas.
		*/
		bool _keyState[Key::KEY_COUNT] = {};
		/**
		* @brief Array con el anterior estado de cada tecla.
		*/
		bool _keyLastState[Key::KEY_COUNT] = {};

		//------Arrays de botones de raton:
		/**
		* @brief Array de tamanyo MOUSE_BUTTON_COUNT con los estados de los botones del raton.
		*/
		bool _mouseButtonState[MouseButton::MOUSE_BUTTON_COUNT] = {};
		/**
		* @brief Array con el anterior estado de cada boton del raton.
		*/
		bool _mouseButtonLastState[MouseButton::MOUSE_BUTTON_COUNT] = {};

		//------Array de ejes de raton.
		/**
		* @brief Array de tamanyo MOUSE_AXIS_COUNT con el estado de los ejes del raton.
		*/
		float _mouseAxisState[MouseAxis::MOUSE_AXIS_COUNT] = {};

		//------Arrays de botones de gamepad:
		/**
		* @brief Array de tamanyo BUTTON_GP_COUNT con los estados de los botones del pad.
		*/
		bool _gamepadButtonState[GamepadButton::BUTTON_GP_COUNT] = {};
		/**
		* @brief Array con el anterior estado de cada boton del gamepad.
		*/
		bool _gamepadButtonLastState[GamepadButton::BUTTON_GP_COUNT] = {};

		//-----Array de ejes de gamepad:
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
		/**
		 * @brief Deadzone para los joysticks.
		 */
		float _deadzone = DEFAULT_DEADZONE;
		/**
		* @brief Clase PlatformModule amiga para poder acceder a metodos privados.
		*/
		friend class PlatformModule;
	};
}