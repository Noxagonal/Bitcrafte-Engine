#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <stdint.h>



namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class MouseButtonAction : int32_t
{
	RELEASED				= 0,	///< Button was lift up.
	PRESSED					= 1,	///< Button was pressed down.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class KeyboardButtonAction : int32_t
{
	RELEASED				= 0,	///< Button was lift up.
	PRESSED					= 1,	///< Button was pressed down.
	REPEATED				= 2,	///< Button was held down long and is being repeated by the OS, this is used in text input when user wants to insert same character multiple times.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class MouseButton : int32_t
{
	BUTTON_1				= 0,		///< Left mouse button
	BUTTON_2				= 1,		///< Right mouse button
	BUTTON_3				= 2,		///< Middle mouse button
	BUTTON_4				= 3,		///< Forward side button
	BUTTON_5				= 4,		///< Backward side button
	BUTTON_LAST				= BUTTON_5,	///< (Extra mouse button)
	BUTTON_LEFT				= BUTTON_1,	///< Left mouse button
	BUTTON_RIGHT			= BUTTON_2,	///< Right mouse button
	BUTTON_MIDDLE			= BUTTON_3,	///< Middle mouse button
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class ModifierKeyFlags : int32_t
{
	SHIFT					= 0x0001,	///< Shift key, either left or right
	CONTROL					= 0x0002,	///< Ctrl key, either left or right
	ALT						= 0x0004,	///< Alt key, either left or right
	SUPER					= 0x0008	///< Windows key, either left or right
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline ModifierKeyFlags		operator|(
	ModifierKeyFlags		f1,
	ModifierKeyFlags		f2
)
{
	return ModifierKeyFlags( std::to_underlying( f1 ) | std::to_underlying( f2 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline ModifierKeyFlags		operator|=(
	ModifierKeyFlags	&	f1,
	ModifierKeyFlags		f2
)
{
	return f1 = f1 | f2;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline ModifierKeyFlags		operator&(
	ModifierKeyFlags		f1,
	ModifierKeyFlags		f2
)
{
	return ModifierKeyFlags( std::to_underlying( f1 ) & std::to_underlying( f2 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline ModifierKeyFlags		operator&=(
	ModifierKeyFlags	&	f1,
	ModifierKeyFlags		f2
)
{
	return f1 = f1 & f2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class KeyboardButton : int32_t
{
	KEY_UNKNOWN				= -1,	///< Unrecognized keyboard button

	KEY_SPACE				= 32,	///< Space bar
	KEY_APOSTROPHE			= 39,	///< <tt>'</tt>
	KEY_PLUS				= 43,	///< <tt>+</tt>
	KEY_COMMA				= 44,	///< <tt>,</tt>
	KEY_MINUS				= 45,	///< <tt>-</tt>
	KEY_PERIOD				= 46,	///< <tt>.</tt>
	KEY_SLASH				= 47,	///< <tt>/</tt> forward slash
	KEY_0					= 48,	///< Top row <tt>0</tt>
	KEY_1					= 49,	///< Top row <tt>1</tt>
	KEY_2					= 50,	///< Top row <tt>2</tt>
	KEY_3					= 51,	///< Top row <tt>3</tt>
	KEY_4					= 52,	///< Top row <tt>4</tt>
	KEY_5					= 53,	///< Top row <tt>5</tt>
	KEY_6					= 54,	///< Top row <tt>6</tt>
	KEY_7					= 55,	///< Top row <tt>7</tt>
	KEY_8					= 56,	///< Top row <tt>8</tt>
	KEY_9					= 57,	///< Top row <tt>9</tt>
	KEY_SEMICOLON			= 59,	///< <tt>;</tt>
	KEY_A					= 65,
	KEY_B					= 66,
	KEY_C					= 67,
	KEY_D					= 68,
	KEY_E					= 69,
	KEY_F					= 70,
	KEY_G					= 71,
	KEY_H					= 72,
	KEY_I					= 73,
	KEY_J					= 74,
	KEY_K					= 75,
	KEY_L					= 76,
	KEY_M					= 77,
	KEY_N					= 78,
	KEY_O					= 79,
	KEY_P					= 80,
	KEY_Q					= 81,
	KEY_R					= 82,
	KEY_S					= 83,
	KEY_T					= 84,
	KEY_U					= 85,
	KEY_V					= 86,
	KEY_W					= 87,
	KEY_X					= 88,
	KEY_Y					= 89,
	KEY_Z					= 90,
	KEY_LEFT_BRACKET		= 91,	///< US keyboard: <tt>[</tt>
	KEY_BACKSLASH			= 92,	///< US keyboard: <tt>\\</tt>
	KEY_RIGHT_BRACKET		= 93,	///< US keyboard: <tt>]</tt>
	KEY_GRAVE_ACCENT		= 96,	///< US keyboard: <tt>`</tt>
	KEY_AX					= 161,	///< Japanese AX keyboard: 'AX' key.
	KEY_ANGLE_BRACKETS		= 162,	///< <tt><></tt> or <tt>\|</tt> on RT 102-key kbd.

	KEY_ESCAPE				= 256,	///< <tt>Esc</tt>
	KEY_ENTER				= 257,	///< <tt>Enter</tt>, Underneath backspace
	KEY_TAB					= 258,	///< <tt>Tab</tt>
	KEY_BACKSPACE			= 259,	///< <tt>Backspace</tt>
	KEY_INSERT				= 260,	///< <tt>Insert</tt>
	KEY_DELETE				= 261,	///< <tt>Delete</tt>
	KEY_RIGHT				= 262,	///< Right arrow key
	KEY_LEFT				= 263,	///< Left arrow key
	KEY_DOWN				= 264,	///< Down arrow key
	KEY_UP					= 265,	///< Up arrow key
	KEY_PAGE_UP				= 266,	///< <tt>Page up</tt>
	KEY_PAGE_DOWN			= 267,	///< <tt>Page down</tt>
	KEY_HOME				= 268,	///< <tt>Home</tt>
	KEY_END					= 269,	///< <tt>End</tt>
	KEY_CAPS_LOCK			= 280,	///< <tt>Caps lock</tt>
	KEY_SCROLL_LOCK			= 281,	///< <tt>Scroll lock</tt>
	KEY_NUM_LOCK			= 282,	///< <tt>Num lock</tt>
	KEY_PRINT_SCREEN		= 283,	///< <tt>Print screen</tt>
	KEY_PAUSE				= 284,	///< <tt>Pause</tt>
	KEY_F1					= 290,
	KEY_F2					= 291,
	KEY_F3					= 292,
	KEY_F4					= 293,
	KEY_F5					= 294,
	KEY_F6					= 295,
	KEY_F7					= 296,
	KEY_F8					= 297,
	KEY_F9					= 298,
	KEY_F10					= 299,
	KEY_F11					= 300,
	KEY_F12					= 301,
	KEY_F13					= 302,
	KEY_F14					= 303,
	KEY_F15					= 304,
	KEY_F16					= 305,
	KEY_F17					= 306,
	KEY_F18					= 307,
	KEY_F19					= 308,
	KEY_F20					= 309,
	KEY_F21					= 310,
	KEY_F22					= 311,
	KEY_F23					= 312,
	KEY_F24					= 313,
	KEY_NUMPAD_0			= 320,
	KEY_NUMPAD_1			= 321,
	KEY_NUMPAD_2			= 322,
	KEY_NUMPAD_3			= 323,
	KEY_NUMPAD_4			= 324,
	KEY_NUMPAD_5			= 325,
	KEY_NUMPAD_6			= 326,
	KEY_NUMPAD_7			= 327,
	KEY_NUMPAD_8			= 328,
	KEY_NUMPAD_9			= 329,
	KEY_NUMPAD_DECIMAL		= 330,	///< Numpad <tt>.</tt> or <tt>,</tt> depending on region
	KEY_NUMPAD_DIVIDE		= 331,	///< Numpad <tt>/</tt>
	KEY_NUMPAD_MULTIPLY		= 332,	///< Numpad <tt>*</tt>
	KEY_NUMPAD_SUBTRACT		= 333,	///< Numpad <tt>-</tt>
	KEY_NUMPAD_ADD			= 334,	///< Numpad <tt>+</tt>
	KEY_NUMPAD_ENTER		= 335,	///< Numpad <tt>Enter</tt>
	KEY_NUMPAD_EQUAL		= 336,	///< Numpad <tt>=</tt> (often missing)
	KEY_LEFT_SHIFT			= 340,	///< Left <tt>Shift</tt>
	KEY_LEFT_CONTROL		= 341,	///< Left <tt>Ctrl</tt>
	KEY_LEFT_ALT			= 342,	///< Left <tt>Alt</tt>
	KEY_LEFT_SUPER			= 343,	///< Left Super/Windows key 
	KEY_RIGHT_SHIFT			= 344,	///< Right <tt>Shift</tt>
	KEY_RIGHT_CONTROL		= 345,	///< Right <tt>Ctrl</tt>
	KEY_RIGHT_ALT			= 346,	///< Right <tt>Alt</tt>
	KEY_RIGHT_SUPER			= 347,	///< Right Super/Windows key
	KEY_MENU				= 348,	///< Menu

	KEY_LAST				= KEY_MENU,	///< Used to get the number of total key entries.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class WindowState : int32_t
{
	WINDOWED				= 0,	///< Window is in normal windowed mode.
	FULLSCREEN				= 1,	///< Window is in fullscreen mode.
	MINIMIZED				= 2,	///< Window is minimized to task bar.
	MAXIMIZED				= 3,	///< Window is maximized to take the entire screen without being in fullscreen mode.
};



} // window
} // bc
