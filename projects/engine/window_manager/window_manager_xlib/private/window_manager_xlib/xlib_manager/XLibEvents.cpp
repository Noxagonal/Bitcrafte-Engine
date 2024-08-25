
#include <window_manager_xlib/PreCompiledHeader.hpp>
#include <window_manager_xlib/xlib_manager/XLibEvents.hpp>

#include <window_manager_xlib/window/XLibWindow.hpp>

#include <X11/Xlib.h>
#include <X11/keysym.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::xlib::SetInputFocus( const XLibWindow& window )
{
	auto handles = window.GetXLibPlatformHandles();
	XSetInputFocus(
		handles->display,
		handles->window,
		RevertToParent,
		CurrentTime
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::xlib::ReplyToPing(
	const XEvent&		event,
	const XLibWindow&	window
)
{
	auto handles = window.GetXLibPlatformHandles();
	auto e = event;
	e.xclient.window = RootWindow( handles->display, handles->default_screen );
	XSendEvent(
		window.GetXLibPlatformHandles()->display,
		e.xclient.window,
		False,
		SubstructureRedirectMask | SubstructureNotifyMask,
		&e
	);
	XFlush( handles->display );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::xlib::GetModifierKeyFlags( uint32_t state ) -> ModifierKeyFlags
{
	auto modifier_key_flags = bc::window_manager::ModifierKeyFlags {};
	if( state & ShiftMask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::SHIFT;
	if( state & ControlMask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::CONTROL;
	if( state & LockMask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::CAPS_LOCK;
	if( state & Mod1Mask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::ALT;
	if( state & Mod2Mask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::NUM_LOCK;
	if( state & Mod4Mask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::SUPER;
	if( state & Mod5Mask ) modifier_key_flags |= bc::window_manager::ModifierKeyFlags::ALT_GR;
	return modifier_key_flags;
}

auto bc::window_manager::xlib::GetMouseButton( uint32_t button ) -> MouseButton
{
	switch (button)
	{
	case Button1:	return bc::window_manager::MouseButton::BUTTON_1;
	case Button2:	return bc::window_manager::MouseButton::BUTTON_3;
	case Button3:	return bc::window_manager::MouseButton::BUTTON_2;
	case Button4:	return bc::window_manager::MouseButton::BUTTON_4;
	case Button5:	return bc::window_manager::MouseButton::BUTTON_5;
	}
	return {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::xlib::GetKeyboardButton( KeySym keysym ) -> KeyboardButton
{
	switch( keysym )
	{
	case XK_space:			return KeyboardButton::KEY_SPACE;				// Space bar
	case XK_apostrophe:		return KeyboardButton::KEY_APOSTROPHE;			// Apostrophe
	case XK_plus:
	case XK_equal:			return KeyboardButton::KEY_EQUAL_OR_PLUS;		// Equal or plus
	case XK_comma:			return KeyboardButton::KEY_COMMA;				// Comma
	case XK_minus:			return KeyboardButton::KEY_MINUS;				// Minus
	case XK_period:			return KeyboardButton::KEY_PERIOD;				// Period
	case XK_slash:			return KeyboardButton::KEY_SLASH;				// Slash
	case XK_0:				return KeyboardButton::KEY_0;					// Top row 0
	case XK_1:				return KeyboardButton::KEY_1;					// Top row 1
	case XK_2:				return KeyboardButton::KEY_2;					// Top row 2
	case XK_3:				return KeyboardButton::KEY_3;					// Top row 3
	case XK_4:				return KeyboardButton::KEY_4;					// Top row 4
	case XK_5:				return KeyboardButton::KEY_5;					// Top row 5
	case XK_6:				return KeyboardButton::KEY_6;					// Top row 6
	case XK_7:				return KeyboardButton::KEY_7;					// Top row 7
	case XK_8:				return KeyboardButton::KEY_8;					// Top row 8
	case XK_9:				return KeyboardButton::KEY_9;					// Top row 9
	case XK_semicolon:		return KeyboardButton::KEY_SEMICOLON;			// Semicolon
	case XK_a:				return KeyboardButton::KEY_A;					// A
	case XK_b:				return KeyboardButton::KEY_B;					// B
	case XK_c:				return KeyboardButton::KEY_C;					// C
	case XK_d:				return KeyboardButton::KEY_D;					// D
	case XK_e:				return KeyboardButton::KEY_E;					// E
	case XK_f:				return KeyboardButton::KEY_F;					// F
	case XK_g:				return KeyboardButton::KEY_G;					// G
	case XK_h:				return KeyboardButton::KEY_H;					// H
	case XK_i:				return KeyboardButton::KEY_I;					// I
	case XK_j:				return KeyboardButton::KEY_J;					// J
	case XK_k:				return KeyboardButton::KEY_K;					// K
	case XK_l:				return KeyboardButton::KEY_L;					// L
	case XK_m:				return KeyboardButton::KEY_M;					// M
	case XK_n:				return KeyboardButton::KEY_N;					// N
	case XK_o:				return KeyboardButton::KEY_O;					// O
	case XK_p:				return KeyboardButton::KEY_P;					// P
	case XK_q:				return KeyboardButton::KEY_Q;					// Q
	case XK_r:				return KeyboardButton::KEY_R;					// R
	case XK_s:				return KeyboardButton::KEY_S;					// S
	case XK_t:				return KeyboardButton::KEY_T;					// T
	case XK_u:				return KeyboardButton::KEY_U;					// U
	case XK_v:				return KeyboardButton::KEY_V;					// V
	case XK_w:				return KeyboardButton::KEY_W;					// W
	case XK_x:				return KeyboardButton::KEY_X;					// X
	case XK_y:				return KeyboardButton::KEY_Y;					// Y
	case XK_z:				return KeyboardButton::KEY_Z;					// Z
	case XK_bracketleft:	return KeyboardButton::KEY_LEFT_BRACKET;		// Left bracket
	case XK_backslash:		return KeyboardButton::KEY_BACKSLASH;			// Backslash
	case XK_bracketright:	return KeyboardButton::KEY_RIGHT_BRACKET;		// Right bracket
	case XK_grave:			return KeyboardButton::KEY_GRAVE_ACCENT;		// Grave accent
	case XK_less:
	case XK_greater:		return KeyboardButton::KEY_ANGLE_BRACKETS;		// Angle brackets
	case XK_Escape:			return KeyboardButton::KEY_ESCAPE;				// Escape
	case XK_Return:			return KeyboardButton::KEY_ENTER;				// Enter
	case XK_Tab:			return KeyboardButton::KEY_TAB;					// Tab
	case XK_BackSpace:		return KeyboardButton::KEY_BACKSPACE;			// Backspace
	case XK_Insert:			return KeyboardButton::KEY_INSERT;				// Insert
	case XK_Delete:			return KeyboardButton::KEY_DELETE;				// Delete
	case XK_Right:			return KeyboardButton::KEY_RIGHT;				// Right arrow
	case XK_Left:			return KeyboardButton::KEY_LEFT;				// Left arrow
	case XK_Down:			return KeyboardButton::KEY_DOWN;				// Down arrow
	case XK_Up:				return KeyboardButton::KEY_UP;					// Up arrow
	case XK_Page_Up:		return KeyboardButton::KEY_PAGE_UP;				// Page up
	case XK_Page_Down:		return KeyboardButton::KEY_PAGE_DOWN;			// Page down
	case XK_Home:			return KeyboardButton::KEY_HOME;				// Home
	case XK_End:			return KeyboardButton::KEY_END;					// End
	case XK_Caps_Lock:		return KeyboardButton::KEY_CAPS_LOCK;			// Caps lock
	case XK_Scroll_Lock:	return KeyboardButton::KEY_SCROLL_LOCK;			// Scroll lock
	case XK_Num_Lock:		return KeyboardButton::KEY_NUM_LOCK;			// Num lock
	case XK_Print:			return KeyboardButton::KEY_PRINT_SCREEN;		// Print screen
	case XK_Pause:			return KeyboardButton::KEY_PAUSE;				// Pause
	case XK_F1:				return KeyboardButton::KEY_F1;					// F1
	case XK_F2:				return KeyboardButton::KEY_F2;					// F2
	case XK_F3:				return KeyboardButton::KEY_F3;					// F3
	case XK_F4:				return KeyboardButton::KEY_F4;					// F4
	case XK_F5:				return KeyboardButton::KEY_F5;					// F5
	case XK_F6:				return KeyboardButton::KEY_F6;					// F6
	case XK_F7:				return KeyboardButton::KEY_F7;					// F7
	case XK_F8:				return KeyboardButton::KEY_F8;					// F8
	case XK_F9:				return KeyboardButton::KEY_F9;					// F9
	case XK_F10:			return KeyboardButton::KEY_F10;					// F10
	case XK_F11:			return KeyboardButton::KEY_F11;					// F11
	case XK_F12:			return KeyboardButton::KEY_F12;					// F12
	case XK_F13:			return KeyboardButton::KEY_F13;					// F13
	case XK_F14:			return KeyboardButton::KEY_F14;					// F14
	case XK_F15:			return KeyboardButton::KEY_F15;					// F15
	case XK_F16:			return KeyboardButton::KEY_F16;					// F16
	case XK_F17:			return KeyboardButton::KEY_F17;					// F17
	case XK_F18:			return KeyboardButton::KEY_F18;					// F18
	case XK_F19:			return KeyboardButton::KEY_F19;					// F19
	case XK_F20:			return KeyboardButton::KEY_F20;					// F20
	case XK_F21:			return KeyboardButton::KEY_F21;					// F21
	case XK_F22:			return KeyboardButton::KEY_F22;					// F22
	case XK_F23:			return KeyboardButton::KEY_F23;					// F23
	case XK_F24:			return KeyboardButton::KEY_F24;					// F24
	case XK_KP_0:			return KeyboardButton::KEY_NUMPAD_0;			// Numpad 0
	case XK_KP_1:			return KeyboardButton::KEY_NUMPAD_1;			// Numpad 1
	case XK_KP_2:			return KeyboardButton::KEY_NUMPAD_2;			// Numpad 2
	case XK_KP_3:			return KeyboardButton::KEY_NUMPAD_3;			// Numpad 3
	case XK_KP_4:			return KeyboardButton::KEY_NUMPAD_4;			// Numpad 4
	case XK_KP_5:			return KeyboardButton::KEY_NUMPAD_5;			// Numpad 5
	case XK_KP_6:			return KeyboardButton::KEY_NUMPAD_6;			// Numpad 6
	case XK_KP_7:			return KeyboardButton::KEY_NUMPAD_7;			// Numpad 7
	case XK_KP_8:			return KeyboardButton::KEY_NUMPAD_8;			// Numpad 8
	case XK_KP_9:			return KeyboardButton::KEY_NUMPAD_9;			// Numpad 9
	case XK_KP_Decimal:
	case XK_KP_Separator:	return KeyboardButton::KEY_NUMPAD_SEPARATOR;	// Numpad decimal
	case XK_KP_Divide:		return KeyboardButton::KEY_NUMPAD_DIVIDE;		// Numpad divide
	case XK_KP_Multiply:	return KeyboardButton::KEY_NUMPAD_MULTIPLY;		// Numpad multiply
	case XK_KP_Subtract:	return KeyboardButton::KEY_NUMPAD_SUBTRACT;		// Numpad subtract
	case XK_KP_Add:			return KeyboardButton::KEY_NUMPAD_ADD;			// Numpad add
	case XK_KP_Enter:		return KeyboardButton::KEY_NUMPAD_ENTER;		// Numpad enter
	case XK_KP_Equal:		return KeyboardButton::KEY_NUMPAD_EQUAL;		// Numpad equal
	case XK_Shift_L:		return KeyboardButton::KEY_LEFT_SHIFT;			// Left shift
	case XK_Control_L:		return KeyboardButton::KEY_LEFT_CONTROL;		// Left control
	case XK_Meta_L:
	case XK_Alt_L:			return KeyboardButton::KEY_LEFT_ALT;			// Left alt
	case XK_Super_L:		return KeyboardButton::KEY_LEFT_SUPER;			// Left super
	case XK_Shift_R:		return KeyboardButton::KEY_RIGHT_SHIFT;			// Right shift
	case XK_Control_R:		return KeyboardButton::KEY_RIGHT_CONTROL;		// Right control
	case XK_Mode_switch: // Mapped to Alt_R on many keyboards
    case XK_ISO_Level3_Shift: // AltGr on some keyboards
    case XK_Meta_R:
	case XK_Alt_R:			return KeyboardButton::KEY_RIGHT_ALT;			// Right alt
	case XK_Super_R:		return KeyboardButton::KEY_RIGHT_SUPER;			// Right super
	case XK_Menu:			return KeyboardButton::KEY_MENU;				// Menu
	default: 				return KeyboardButton::KEY_UNKNOWN;
	}
}
