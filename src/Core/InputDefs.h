#pragma once
#include <cstdint>
#include <variant>

namespace input
{
	// Teclado
	enum Key : uint8_t
	{
		KEY_NONE          = 0,
		KEY_SPECIAL       = 1,   // tecla especial generica
		KEY_ESCAPE        = 2,
		KEY_TAB           = 3,
		KEY_BACKTAB       = 4,   // Shift+Tab
		KEY_BACKSPACE     = 5,
		KEY_ENTER         = 6,
		KEY_KP_ENTER      = 7,   // Enter del teclado numerico
		KEY_INSERT        = 8,
		KEY_DELETE        = 9,
		KEY_PAUSE         = 10,
		KEY_PRINT         = 11,
		KEY_SYSREQ        = 12,  // Alt+Print Screen
		KEY_CLEAR         = 13,
		KEY_HOME          = 14,
		KEY_END           = 15,
		KEY_LEFT          = 16,
		KEY_UP            = 17,
		KEY_RIGHT         = 18,
		KEY_DOWN          = 19,
		KEY_PAGEUP        = 20,
		KEY_PAGEDOWN      = 21,
		KEY_SHIFT         = 22,
		KEY_CTRL          = 23,
		KEY_META          = 24,  // Windows / Cmd
		KEY_ALT           = 25,
		KEY_CAPSLOCK      = 26,
		KEY_NUMLOCK       = 27,
		KEY_SCROLLLOCK    = 28,
		KEY_F1            = 29,
		KEY_F2            = 30,
		KEY_F3            = 31,
		KEY_F4            = 32,
		KEY_F5            = 33,
		KEY_F6            = 34,
		KEY_F7            = 35,
		KEY_F8            = 36,
		KEY_F9            = 37,
		KEY_F10           = 38,
		KEY_F11           = 39,
		KEY_F12           = 40,
		KEY_F13           = 41,
		KEY_F14           = 42,
		KEY_F15           = 43,
		KEY_F16           = 44,
		KEY_F17           = 45,
		KEY_F18           = 46,
		KEY_F19           = 47,
		KEY_F20           = 48,
		KEY_F21           = 49,
		KEY_F22           = 50,
		KEY_F23           = 51,
		KEY_F24           = 52,
		KEY_F25           = 53,
		KEY_SPACE         = 54,
		KEY_EXCLAM        = 55,  // !
		KEY_QUOTEDBL      = 56,  // "
		KEY_NUMBERSIGN    = 57,  // #
		KEY_DOLLAR        = 58,  // $
		KEY_PERCENT       = 59,  // %
		KEY_AMPERSAND     = 60,  // &
		KEY_APOSTROPHE    = 61,  // '
		KEY_PARENLEFT     = 62,  // (
		KEY_PARENRIGHT    = 63,  // )
		KEY_ASTERISK      = 64,  // *
		KEY_PLUS          = 65,  // +
		KEY_COMMA         = 66,  // ,
		KEY_MINUS         = 67,  // -
		KEY_PERIOD        = 68,  // .
		KEY_SLASH         = 69,  // /
		KEY_0             = 70,
		KEY_1             = 71,
		KEY_2             = 72,
		KEY_3             = 73,
		KEY_4             = 74,
		KEY_5             = 75,
		KEY_6             = 76,
		KEY_7             = 77,
		KEY_8             = 78,
		KEY_9             = 79,
		KEY_COLON         = 80,  // :
		KEY_SEMICOLON     = 81,  // ;
		KEY_LESS          = 82,  // <
		KEY_EQUAL         = 83,  // =
		KEY_GREATER       = 84,  // >
		KEY_QUESTION      = 85,  // ?
		KEY_AT            = 86,  // @
		KEY_A             = 87,
		KEY_B             = 88,
		KEY_C             = 89,
		KEY_D             = 90,
		KEY_E             = 91,
		KEY_F             = 92,
		KEY_G             = 93,
		KEY_H             = 94,
		KEY_I             = 95,
		KEY_J             = 96,
		KEY_K             = 97,
		KEY_L             = 98,
		KEY_M             = 99,
		KEY_N             = 100,
		KEY_O             = 101,
		KEY_P             = 102,
		KEY_Q             = 103,
		KEY_R             = 104,
		KEY_S             = 105,
		KEY_T             = 106,
		KEY_U             = 107,
		KEY_V             = 108,
		KEY_W             = 109,
		KEY_X             = 110,
		KEY_Y             = 111,
		KEY_Z             = 112,
		KEY_BRACKETLEFT   = 113, // [
		KEY_BACKSLASH     = 114, // '\'
		KEY_BRACKETRIGHT  = 115, // ]
		KEY_ASCIICIRCUM   = 116, // ^
		KEY_UNDERSCORE    = 117, // _
		KEY_QUOTELEFT     = 118, // `
		KEY_BRACELEFT     = 119, // {
		KEY_BAR           = 120, // |
		KEY_BRACERIGHT    = 121, // }
		KEY_ASCIITILDE    = 122, // ~
		KEY_YEN           = 123, // ¥
		KEY_SECTION       = 124, // §
		KEY_MENU          = 125,
		KEY_HYPER         = 126, // solo Linux/X11
		KEY_HELP          = 127,
		KEY_BACK          = 128,
		KEY_FORWARD       = 129,
		KEY_STOP          = 130,
		KEY_REFRESH       = 131,
		KEY_VOLUMEDOWN    = 132,
		KEY_VOLUMEMUTE    = 133,
		KEY_VOLUMEUP      = 134,
		KEY_MEDIAPLAY     = 135,
		KEY_MEDIASTOP     = 136,
		KEY_MEDIAPREVIOUS = 137,
		KEY_MEDIANEXT     = 138,
		KEY_MEDIARECORD   = 139,
		KEY_HOMEPAGE      = 140,
		KEY_FAVORITES     = 141,
		KEY_SEARCH        = 142,
		KEY_STANDBY       = 143, // suspender el sistema
		KEY_OPENURL       = 144, // abrir navegador
		KEY_LAUNCHMAIL    = 145,
		KEY_LAUNCHMEDIA   = 146,
		KEY_LAUNCH0       = 147,
		KEY_LAUNCH1       = 148,
		KEY_LAUNCH2       = 149,
		KEY_LAUNCH3       = 150,
		KEY_LAUNCH4       = 151,
		KEY_LAUNCH5       = 152,
		KEY_LAUNCH6       = 153,
		KEY_LAUNCH7       = 154,
		KEY_LAUNCH8       = 155,
		KEY_LAUNCH9       = 156,
		KEY_LAUNCHA       = 157,
		KEY_LAUNCHB       = 158,
		KEY_LAUNCHC       = 159,
		KEY_LAUNCHD       = 160,
		KEY_LAUNCHE       = 161,
		KEY_LAUNCHF       = 162,
		KEY_GLOBE         = 163, // Mac/iPad
		KEY_KEYBOARD      = 164, // teclado en pantalla (iPad)
		KEY_JIS_EISU      = 165, // 英数 (Mac)
		KEY_JIS_KANA      = 166, // かな (Mac)
		KEY_KP_MULTIPLY   = 167, // * numpad
		KEY_KP_DIVIDE     = 168, // / numpad
		KEY_KP_SUBTRACT   = 169, // - numpad
		KEY_KP_PERIOD     = 170, // . numpad
		KEY_KP_ADD        = 171, // + numpad
		KEY_KP_0          = 172,
		KEY_KP_1          = 173,
		KEY_KP_2          = 174,
		KEY_KP_3          = 175,
		KEY_KP_4          = 176,
		KEY_KP_5          = 177,
		KEY_KP_6          = 178,
		KEY_KP_7          = 179,
		KEY_KP_8          = 180,
		KEY_KP_9          = 181,
		KEY_UNKNOWN       = 182,
		KEY_COUNT
	};

	// Raton
	enum MouseButton : uint8_t
	{
		MOUSE_BUTTON_LEFT    = 0,
		MOUSE_BUTTON_RIGHT   = 1,
		MOUSE_BUTTON_MIDDLE  = 2,
		MOUSE_BUTTON_X1      = 3,  // boton lateral trasero
		MOUSE_BUTTON_X2      = 4,  // boton lateral delantero
		MOUSE_BUTTON_COUNT
	};

	enum MouseAxis : uint8_t
	{
		MOUSE_AXIS_X         = 0,  // posicion absoluta X
		MOUSE_AXIS_Y         = 1,  // posicion absoluta Y
		MOUSE_AXIS_REL_X     = 2,  // movimiento relativo X (delta)
		MOUSE_AXIS_REL_Y     = 3,  // movimiento relativo Y (delta)
		MOUSE_AXIS_SCROLL_X  = 4,  // rueda horizontal
		MOUSE_AXIS_SCROLL_Y  = 5,  // rueda vertical
		MOUSE_AXIS_COUNT
	};

	// Gamepad
	enum GamepadButton : uint8_t
	{
		BUTTON_GP_CROSS        = 0,
		BUTTON_GP_CIRCLE       = 1,
		BUTTON_GP_SQUARE       = 2,
		BUTTON_GP_TRIANGLE     = 3,
		BUTTON_GP_UP           = 4,  // D-pad
		BUTTON_GP_RIGHT        = 5,  // D-pad
		BUTTON_GP_DOWN         = 6,  // D-pad
		BUTTON_GP_LEFT         = 7,  // D-pad
		BUTTON_GP_LB           = 8,  // bumper izquierdo
		BUTTON_GP_RB           = 9,  // bumper derecho
		BUTTON_GP_LEFT_STICK   = 10, // click stick izquierdo
		BUTTON_GP_RIGHT_STICK  = 11, // click stick derecho
		BUTTON_GP_START        = 12,
		BUTTON_GP_SELECT       = 13,
		BUTTON_GP_HOME         = 14, // boton central / guia
		BUTTON_GP_TOUCHPAD     = 15, // solo DualShock / DualSense
		BUTTON_GP_1            = 16, // botones extra genericos
		BUTTON_GP_2            = 17,
		BUTTON_GP_3            = 18,
		BUTTON_GP_4            = 19,
		BUTTON_GP_5            = 20,
		BUTTON_GP_6            = 21,
		BUTTON_GP_7            = 22,
		BUTTON_GP_8            = 23,
		BUTTON_GP_9            = 24,
		BUTTON_GP_10           = 25,
		BUTTON_GP_COUNT
	};

	enum GamepadAxis : uint8_t
	{
		GAMEPAD_AXIS_LEFT_X        = 0,  // stick izquierdo, horizontal  [-1, 1]
		GAMEPAD_AXIS_LEFT_Y        = 1,  // stick izquierdo, vertical    [-1, 1]
		GAMEPAD_AXIS_RIGHT_X       = 2,  // stick derecho, horizontal    [-1, 1]
		GAMEPAD_AXIS_RIGHT_Y       = 3,  // stick derecho, vertical      [-1, 1]
		GAMEPAD_AXIS_LEFT_TRIGGER  = 4,  // gatillo izquierdo            [ 0, 1]
		GAMEPAD_AXIS_RIGHT_TRIGGER = 5,  // gatillo derecho              [ 0, 1]
		GAMEPAD_AXIS_COUNT
	};

	// Input action
	using InputEvent = std::variant<
		Key, 
		MouseButton, 
		MouseAxis, 
		GamepadButton, 
		GamepadAxis>;

	using DeviceID = uint8_t;
	static constexpr DeviceID KEYBOARD_ID = 0;
	static constexpr DeviceID ANY_DEVICE = -1;
} // namespace Input