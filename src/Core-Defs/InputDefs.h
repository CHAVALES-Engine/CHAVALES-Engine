#pragma once
#include <cstdint>
#include <variant>

namespace input
{
	/**
	 * @brief Para declarar estos arrays estamos usando X-Macros
	 * Las X-Macros son una lista usada para mantener sincronizados varios datos sintener que escribir varias veces lo mismo
	 * En este caso lo usamos para:
	 *      - Generar enums de los diferentes InputEvent que se pueden registrar.
	 *      - Asignar un nombre a cada InputEvent en un mapa.
	 *
	 */

#define INPUT_KEYS(X) \
        X(KEY_NONE,          0)   \
        X(KEY_SPECIAL,       1)   \
        X(KEY_ESCAPE,        2)   \
        X(KEY_TAB,           3)   \
        X(KEY_BACKTAB,       4)   \
        X(KEY_BACKSPACE,     5)   \
        X(KEY_ENTER,         6)   \
        X(KEY_KP_ENTER,      7)   \
        X(KEY_INSERT,        8)   \
        X(KEY_DELETE,        9)   \
        X(KEY_PAUSE,         10)  \
        X(KEY_PRINT,         11)  \
        X(KEY_SYSREQ,        12)  \
        X(KEY_CLEAR,         13)  \
        X(KEY_HOME,          14)  \
        X(KEY_END,           15)  \
        X(KEY_LEFT,          16)  \
        X(KEY_UP,            17)  \
        X(KEY_RIGHT,         18)  \
        X(KEY_DOWN,          19)  \
        X(KEY_PAGEUP,        20)  \
        X(KEY_PAGEDOWN,      21)  \
        X(KEY_SHIFT,         22)  \
        X(KEY_CTRL,          23)  \
        X(KEY_META,          24)  \
        X(KEY_ALT,           25)  \
        X(KEY_CAPSLOCK,      26)  \
        X(KEY_NUMLOCK,       27)  \
        X(KEY_SCROLLLOCK,    28)  \
        X(KEY_F1,            29)  \
        X(KEY_F2,            30)  \
        X(KEY_F3,            31)  \
        X(KEY_F4,            32)  \
        X(KEY_F5,            33)  \
        X(KEY_F6,            34)  \
        X(KEY_F7,            35)  \
        X(KEY_F8,            36)  \
        X(KEY_F9,            37)  \
        X(KEY_F10,           38)  \
        X(KEY_F11,           39)  \
        X(KEY_F12,           40)  \
        X(KEY_F13,           41)  \
        X(KEY_F14,           42)  \
        X(KEY_F15,           43)  \
        X(KEY_F16,           44)  \
        X(KEY_F17,           45)  \
        X(KEY_F18,           46)  \
        X(KEY_F19,           47)  \
        X(KEY_F20,           48)  \
        X(KEY_F21,           49)  \
        X(KEY_F22,           50)  \
        X(KEY_F23,           51)  \
        X(KEY_F24,           52)  \
        X(KEY_F25,           53)  \
        X(KEY_SPACE,         54)  \
        X(KEY_EXCLAM,        55)  \
        X(KEY_QUOTEDBL,      56)  \
        X(KEY_NUMBERSIGN,    57)  \
        X(KEY_DOLLAR,        58)  \
        X(KEY_PERCENT,       59)  \
        X(KEY_AMPERSAND,     60)  \
        X(KEY_APOSTROPHE,    61)  \
        X(KEY_PARENLEFT,     62)  \
        X(KEY_PARENRIGHT,    63)  \
        X(KEY_ASTERISK,      64)  \
        X(KEY_PLUS,          65)  \
        X(KEY_COMMA,         66)  \
        X(KEY_MINUS,         67)  \
        X(KEY_PERIOD,        68)  \
        X(KEY_SLASH,         69)  \
        X(KEY_0,             70)  \
        X(KEY_1,             71)  \
        X(KEY_2,             72)  \
        X(KEY_3,             73)  \
        X(KEY_4,             74)  \
        X(KEY_5,             75)  \
        X(KEY_6,             76)  \
        X(KEY_7,             77)  \
        X(KEY_8,             78)  \
        X(KEY_9,             79)  \
        X(KEY_COLON,         80)  \
        X(KEY_SEMICOLON,     81)  \
        X(KEY_LESS,          82)  \
        X(KEY_EQUAL,         83)  \
        X(KEY_GREATER,       84)  \
        X(KEY_QUESTION,      85)  \
        X(KEY_AT,            86)  \
        X(KEY_A,             87)  \
        X(KEY_B,             88)  \
        X(KEY_C,             89)  \
        X(KEY_D,             90)  \
        X(KEY_E,             91)  \
        X(KEY_F,             92)  \
        X(KEY_G,             93)  \
        X(KEY_H,             94)  \
        X(KEY_I,             95)  \
        X(KEY_J,             96)  \
        X(KEY_K,             97)  \
        X(KEY_L,             98)  \
        X(KEY_M,             99)  \
        X(KEY_N,             100) \
        X(KEY_O,             101) \
        X(KEY_P,             102) \
        X(KEY_Q,             103) \
        X(KEY_R,             104) \
        X(KEY_S,             105) \
        X(KEY_T,             106) \
        X(KEY_U,             107) \
        X(KEY_V,             108) \
        X(KEY_W,             109) \
        X(KEY_X,             110) \
        X(KEY_Y,             111) \
        X(KEY_Z,             112) \
        X(KEY_BRACKETLEFT,   113) \
        X(KEY_BACKSLASH,     114) \
        X(KEY_BRACKETRIGHT,  115) \
        X(KEY_ASCIICIRCUM,   116) \
        X(KEY_UNDERSCORE,    117) \
        X(KEY_QUOTELEFT,     118) \
        X(KEY_BRACELEFT,     119) \
        X(KEY_BAR,           120) \
        X(KEY_BRACERIGHT,    121) \
        X(KEY_ASCIITILDE,    122) \
        X(KEY_YEN,           123) \
        X(KEY_SECTION,       124) \
        X(KEY_MENU,          125) \
        X(KEY_HYPER,         126) \
        X(KEY_HELP,          127) \
        X(KEY_BACK,          128) \
        X(KEY_FORWARD,       129) \
        X(KEY_STOP,          130) \
        X(KEY_REFRESH,       131) \
        X(KEY_VOLUMEDOWN,    132) \
        X(KEY_VOLUMEMUTE,    133) \
        X(KEY_VOLUMEUP,      134) \
        X(KEY_MEDIAPLAY,     135) \
        X(KEY_MEDIASTOP,     136) \
        X(KEY_MEDIAPREVIOUS, 137) \
        X(KEY_MEDIANEXT,     138) \
        X(KEY_MEDIARECORD,   139) \
        X(KEY_HOMEPAGE,      140) \
        X(KEY_FAVORITES,     141) \
        X(KEY_SEARCH,        142) \
        X(KEY_STANDBY,       143) \
        X(KEY_OPENURL,       144) \
        X(KEY_LAUNCHMAIL,    145) \
        X(KEY_LAUNCHMEDIA,   146) \
        X(KEY_LAUNCH0,       147) \
        X(KEY_LAUNCH1,       148) \
        X(KEY_LAUNCH2,       149) \
        X(KEY_LAUNCH3,       150) \
        X(KEY_LAUNCH4,       151) \
        X(KEY_LAUNCH5,       152) \
        X(KEY_LAUNCH6,       153) \
        X(KEY_LAUNCH7,       154) \
        X(KEY_LAUNCH8,       155) \
        X(KEY_LAUNCH9,       156) \
        X(KEY_LAUNCHA,       157) \
        X(KEY_LAUNCHB,       158) \
        X(KEY_LAUNCHC,       159) \
        X(KEY_LAUNCHD,       160) \
        X(KEY_LAUNCHE,       161) \
        X(KEY_LAUNCHF,       162) \
        X(KEY_GLOBE,         163) \
        X(KEY_KEYBOARD,      164) \
        X(KEY_JIS_EISU,      165) \
        X(KEY_JIS_KANA,      166) \
        X(KEY_KP_MULTIPLY,   167) \
        X(KEY_KP_DIVIDE,     168) \
        X(KEY_KP_SUBTRACT,   169) \
        X(KEY_KP_PERIOD,     170) \
        X(KEY_KP_ADD,        171) \
        X(KEY_KP_0,          172) \
        X(KEY_KP_1,          173) \
        X(KEY_KP_2,          174) \
        X(KEY_KP_3,          175) \
        X(KEY_KP_4,          176) \
        X(KEY_KP_5,          177) \
        X(KEY_KP_6,          178) \
        X(KEY_KP_7,          179) \
        X(KEY_KP_8,          180) \
        X(KEY_KP_9,          181) \
        X(KEY_UNKNOWN,       182) \
        X(KEY_COUNT,         183)

	 // Raton - Botones
#define INPUT_MOUSE_BUTTONS(X) \
        X(MOUSE_BUTTON_LEFT,   0) \
        X(MOUSE_BUTTON_RIGHT,  1) \
        X(MOUSE_BUTTON_MIDDLE, 2) \
        X(MOUSE_BUTTON_X1,     3) \
        X(MOUSE_BUTTON_X2,     4) \
        X(MOUSE_BUTTON_COUNT,  5)

	// Raton - Ejes
#define INPUT_MOUSE_AXES(X) \
        X(MOUSE_AXIS_NONE,     0) \
        X(MOUSE_AXIS_X,        1) \
        X(MOUSE_AXIS_Y,        2) \
        X(MOUSE_AXIS_REL_X,    3) \
        X(MOUSE_AXIS_REL_Y,    4) \
        X(MOUSE_AXIS_SCROLL_X, 5) \
        X(MOUSE_AXIS_SCROLL_Y, 6) \
        X(MOUSE_AXIS_COUNT,    7)

	// Gamepad - Botones
#define INPUT_GAMEPAD_BUTTONS(X) \
        X(BUTTON_GP_CROSS,        0)  \
        X(BUTTON_GP_CIRCLE,       1)  \
        X(BUTTON_GP_SQUARE,       2)  \
        X(BUTTON_GP_TRIANGLE,     3)  \
        X(BUTTON_GP_UP,           4)  \
        X(BUTTON_GP_RIGHT,        5)  \
        X(BUTTON_GP_DOWN,         6)  \
        X(BUTTON_GP_LEFT,         7)  \
        X(BUTTON_GP_LB,           8)  \
        X(BUTTON_GP_RB,           9)  \
        X(BUTTON_GP_LEFT_STICK,   10) \
        X(BUTTON_GP_RIGHT_STICK,  11) \
        X(BUTTON_GP_START,        12) \
        X(BUTTON_GP_SELECT,       13) \
        X(BUTTON_GP_HOME,         14) \
        X(BUTTON_GP_TOUCHPAD,     15) \
        X(BUTTON_GP_1,            16) \
        X(BUTTON_GP_2,            17) \
        X(BUTTON_GP_3,            18) \
        X(BUTTON_GP_4,            19) \
        X(BUTTON_GP_5,            20) \
        X(BUTTON_GP_6,            21) \
        X(BUTTON_GP_7,            22) \
        X(BUTTON_GP_8,            23) \
        X(BUTTON_GP_9,            24) \
        X(BUTTON_GP_10,           25) \
        X(BUTTON_GP_COUNT,        26)

	// Gamepad - Ejes
#define INPUT_GAMEPAD_AXES(X) \
        X(GAMEPAD_AXIS_LEFT_X,        0) \
        X(GAMEPAD_AXIS_LEFT_Y,        1) \
        X(GAMEPAD_AXIS_RIGHT_X,       2) \
        X(GAMEPAD_AXIS_RIGHT_Y,       3) \
        X(GAMEPAD_AXIS_LEFT_TRIGGER,  4) \
        X(GAMEPAD_AXIS_RIGHT_TRIGGER, 5) \
        X(GAMEPAD_AXIS_COUNT,         6)


	// Declaracion de enums (generada por las X-Macros)
	enum Key : uint8_t
	{
#define X(name, value) name = value,
		INPUT_KEYS(X)
#undef X
	};

	enum MouseButton : uint8_t
	{
#define X(name, value) name = value,
		INPUT_MOUSE_BUTTONS(X)
#undef X
	};

	enum MouseAxis : uint8_t
	{
#define X(name, value) name = value,
		INPUT_MOUSE_AXES(X)
#undef X
	};

	enum GamepadButton : uint8_t
	{
#define X(name, value) name = value,
		INPUT_GAMEPAD_BUTTONS(X)
#undef X
	};

	enum GamepadAxis : uint8_t
	{
#define X(name, value) name = value,
		INPUT_GAMEPAD_AXES(X)
#undef X
	};

    /**
     * @brief Deadzone para los joystics.
     */
    const float MAX_DEADZONE = 1.0f;
    /**
     * @brief Deadzone para los joystics por defecto.
     */
    const float DEFAULT_DEADZONE = 0.25f;

	/**
	 * @brief Permite pasar multiples lambdas a std::visit combinandolas en un unico callable.
	 * Hereda de cada lambda y expone todos sus operator() en el mismo scope,
	 * dejando al compilador elegir el correcto segun el tipo activo del variant.
	 * @tparam Ts - Tipos de las lambdas a combinar.
	 */
	template<class... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };

	/**
	 * @brief Deduction guide para overloaded.
	 * Permite escribir overloaded{lambda1, lambda2} sin especificar los tipos manualmente,
	 * ya que los tipos de las lambdas no tienen nombre accesible.
	 * @tparam Ts - Tipos de las lambdas a combinar.
	 */
	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;

	// Aliases y constantes
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

	using DeviceID = int;
	static constexpr DeviceID KEYBOARD_ID = 0;
	static constexpr DeviceID ANY_DEVICE = static_cast<DeviceID>(-1);


} // namespace Input
	// toString — array estatico generado por las X-Macros
	// O(0) en runtime

	inline std::string_view toString(input::Key k)
	{
		static constexpr std::string_view names[] = {
			#define X(name, value) #name,
			INPUT_KEYS(X)
			#undef X
		};
		if (static_cast<uint8_t>(k) >= static_cast<uint8_t>(input::KEY_COUNT))
			return "KEY_INVALID";
		return names[k];
	}

	inline std::string_view toString(input::MouseButton b)
	{
		static constexpr std::string_view names[] = {
			#define X(name, value) #name,
			INPUT_MOUSE_BUTTONS(X)
			#undef X
		};
		if (static_cast<uint8_t>(b) >= static_cast<uint8_t>(input::MOUSE_BUTTON_COUNT))
			return "MOUSE_BUTTON_INVALID";
		return names[b];
	}

	inline std::string_view toString(input::MouseAxis a)
	{
		static constexpr std::string_view names[] = {
			#define X(name, value) #name,
			INPUT_MOUSE_AXES(X)
			#undef X
		};
		if (static_cast<uint8_t>(a) >= static_cast<uint8_t>(input::MOUSE_AXIS_COUNT))
			return "MOUSE_AXIS_INVALID";
		return names[a];
	}

	inline std::string_view toString(input::GamepadButton b)
	{
		static constexpr std::string_view names[] = {
			#define X(name, value) #name,
			INPUT_GAMEPAD_BUTTONS(X)
			#undef X
		};
		if (static_cast<uint8_t>(b) >= static_cast<uint8_t>(input::BUTTON_GP_COUNT))
			return "GAMEPAD_BUTTON_INVALID";
		return names[b];
	}

	inline std::string_view toString(input::GamepadAxis a)
	{
		static constexpr std::string_view names[] = {
			#define X(name, value) #name,
			INPUT_GAMEPAD_AXES(X)
			#undef X
		};
		if (static_cast<uint8_t>(a) >= static_cast<uint8_t>(input::GAMEPAD_AXIS_COUNT))
			return "GAMEPAD_AXIS_INVALID";
		return names[a];
	}

	// toString para InputEvent (variant)
	inline std::string_view toString(const input::InputEvent& e)
	{
		return std::visit([](auto v) { return toString(v); }, e);
	}