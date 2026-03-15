#pragma once
#include <cstdint>
#include <variant>

namespace input
{
	// Teclado
	enum Key : uint8_t
	{
		KEY_NONE         ,
		KEY_SPECIAL      ,   // tecla especial generica
		KEY_ESCAPE       ,
		KEY_TAB          ,
		KEY_BACKTAB      ,   // Shift+Tab
		KEY_BACKSPACE    ,
		KEY_ENTER        ,
		KEY_KP_ENTER     ,   // Enter del teclado numerico
		KEY_INSERT       ,
		KEY_DELETE       ,
		KEY_PAUSE        ,
		KEY_PRINT        ,
		KEY_SYSREQ       ,  // Alt+Print Screen
		KEY_CLEAR        ,
		KEY_HOME         ,
		KEY_END          ,
		KEY_LEFT         ,
		KEY_UP           ,
		KEY_RIGHT        ,
		KEY_DOWN         ,
		KEY_PAGEUP       ,
		KEY_PAGEDOWN     ,
		KEY_SHIFT        ,
		KEY_CTRL         ,
		KEY_META         ,  // Windows / Cmd
		KEY_ALT          ,
		KEY_CAPSLOCK     ,
		KEY_NUMLOCK      ,
		KEY_SCROLLLOCK   ,
		KEY_F1           ,
		KEY_F2           ,
		KEY_F3           ,
		KEY_F4           ,
		KEY_F5           ,
		KEY_F6           ,
		KEY_F7           ,
		KEY_F8           ,
		KEY_F9           ,
		KEY_F10          ,
		KEY_F11          ,
		KEY_F12          ,
		KEY_F13          ,
		KEY_F14          ,
		KEY_F15          ,
		KEY_F16          ,
		KEY_F17          ,
		KEY_F18          ,
		KEY_F19          ,
		KEY_F20          ,
		KEY_F21          ,
		KEY_F22          ,
		KEY_F23          ,
		KEY_F24          ,
		KEY_F25          ,
		KEY_SPACE        ,
		KEY_EXCLAM       ,  // !
		KEY_QUOTEDBL     ,  // "
		KEY_NUMBERSIGN   ,  // #
		KEY_DOLLAR       ,  // $
		KEY_PERCENT      ,  // %
		KEY_AMPERSAND    ,  // &
		KEY_APOSTROPHE   ,  // '
		KEY_PARENLEFT    ,  // (
		KEY_PARENRIGHT   ,  // )
		KEY_ASTERISK     ,  // *
		KEY_PLUS         ,  // +
		KEY_COMMA        ,  // ,
		KEY_MINUS        ,  // -
		KEY_PERIOD       ,  // .
		KEY_SLASH        ,  // /
		KEY_0            ,
		KEY_1            ,
		KEY_2            ,
		KEY_3            ,
		KEY_4            ,
		KEY_5            ,
		KEY_6            ,
		KEY_7            ,
		KEY_8            ,
		KEY_9            ,
		KEY_COLON        ,  // :
		KEY_SEMICOLON    ,  // ;
		KEY_LESS         ,  // <
		KEY_EQUAL        ,  // =
		KEY_GREATER      ,  // >
		KEY_QUESTION     ,  // ?
		KEY_AT           ,  // @
		KEY_A            ,
		KEY_B            ,
		KEY_C            ,
		KEY_D            ,
		KEY_E            ,
		KEY_F            ,
		KEY_G            ,
		KEY_H            ,
		KEY_I            ,
		KEY_J            ,
		KEY_K            ,
		KEY_L            ,
		KEY_M            ,
		KEY_N            ,
		KEY_O            ,
		KEY_P            ,
		KEY_Q            ,
		KEY_R            ,
		KEY_S            ,
		KEY_T            ,
		KEY_U            ,
		KEY_V            ,
		KEY_W            ,
		KEY_X            ,
		KEY_Y            ,
		KEY_Z            ,
		KEY_BRACKETLEFT  , // [
		KEY_BACKSLASH    , // '\'
		KEY_BRACKETRIGHT , // ]
		KEY_ASCIICIRCUM  , // ^
		KEY_UNDERSCORE   , // _
		KEY_QUOTELEFT    , // `
		KEY_BRACELEFT    , // {
		KEY_BAR          , // |
		KEY_BRACERIGHT   , // }
		KEY_ASCIITILDE   , // ~
		KEY_YEN          , // ¥
		KEY_SECTION      , // §
		KEY_MENU         ,
		KEY_HYPER        , // solo Linux/X11
		KEY_HELP         ,
		KEY_BACK         ,
		KEY_FORWARD      ,
		KEY_STOP         ,
		KEY_REFRESH      ,
		KEY_VOLUMEDOWN   ,
		KEY_VOLUMEMUTE   ,
		KEY_VOLUMEUP     ,
		KEY_MEDIAPLAY    ,
		KEY_MEDIASTOP    ,
		KEY_MEDIAPREVIOUS,
		KEY_MEDIANEXT    ,
		KEY_MEDIARECORD  ,
		KEY_HOMEPAGE     ,
		KEY_FAVORITES    ,
		KEY_SEARCH       ,
		KEY_STANDBY      , // suspender el sistema
		KEY_OPENURL      , // abrir navegador
		KEY_LAUNCHMAIL   ,
		KEY_LAUNCHMEDIA  ,
		KEY_LAUNCH0      ,
		KEY_LAUNCH1      ,
		KEY_LAUNCH2      ,
		KEY_LAUNCH3      ,
		KEY_LAUNCH4      ,
		KEY_LAUNCH5      ,
		KEY_LAUNCH6      ,
		KEY_LAUNCH7      ,
		KEY_LAUNCH8      ,
		KEY_LAUNCH9      ,
		KEY_LAUNCHA      ,
		KEY_LAUNCHB      ,
		KEY_LAUNCHC      ,
		KEY_LAUNCHD      ,
		KEY_LAUNCHE      ,
		KEY_LAUNCHF      ,
		KEY_GLOBE        , // Mac/iPad
		KEY_KEYBOARD     , // teclado en pantalla (iPad)
		KEY_JIS_EISU     , // 英数 (Mac)
		KEY_JIS_KANA     , // かな (Mac)
		KEY_KP_MULTIPLY  , // * numpad
		KEY_KP_DIVIDE    , // / numpad
		KEY_KP_SUBTRACT  , // - numpad
		KEY_KP_PERIOD    , // . numpad
		KEY_KP_ADD       , // + numpad
		KEY_KP_0         ,
		KEY_KP_1         ,
		KEY_KP_2         ,
		KEY_KP_3         ,
		KEY_KP_4         ,
		KEY_KP_5         ,
		KEY_KP_6         ,
		KEY_KP_7         ,
		KEY_KP_8         ,
		KEY_KP_9         ,
		KEY_UNKNOWN      ,
		KEY_COUNT
	};

	// Raton
	enum MouseButton : uint8_t
	{
		MOUSE_BUTTON_LEFT    ,
		MOUSE_BUTTON_RIGHT   ,
		MOUSE_BUTTON_MIDDLE  ,
		MOUSE_BUTTON_X1      ,  // boton lateral trasero
		MOUSE_BUTTON_X2      ,  // boton lateral delantero
		MOUSE_BUTTON_COUNT
	};

	enum MouseAxis : uint8_t
	{
		MOUSE_AXIS_NONE		 = 0,
		MOUSE_AXIS_X         = 1,  // posicion absoluta X
		MOUSE_AXIS_Y         = 2,  // posicion absoluta Y
		MOUSE_AXIS_REL_X     = 3,  // movimiento relativo X (delta)
		MOUSE_AXIS_REL_Y     = 4,  // movimiento relativo Y (delta)
		MOUSE_AXIS_SCROLL_X  = 5,  // rueda horizontal
		MOUSE_AXIS_SCROLL_Y  = 6,  // rueda vertical
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

	using InputAxis = std::variant<
		MouseAxis,
		GamepadAxis>; 

	using InputButtons = std::variant<
		Key,
		MouseButton,
		GamepadButton>;

	using DeviceID = uint32_t;
	static constexpr DeviceID KEYBOARD_ID = 0;
	static constexpr DeviceID ANY_DEVICE = -1;
} // namespace Input