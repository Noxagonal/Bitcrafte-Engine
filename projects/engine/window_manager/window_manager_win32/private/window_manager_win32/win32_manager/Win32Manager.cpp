
#include <window_manager_win32/PreCompiledHeader.hpp>
#include <window_manager_win32/win32_manager/Win32Manager.hpp>

#include <window_manager/window/Window.hpp>
#include <window_manager_win32/window/Win32Window.hpp>
#include <window_manager_win32/WindowManagerWin32Component.hpp>



namespace bc {
namespace window_manager {



LRESULT CALLBACK WndProc(
	HWND		window_handle,
	UINT		message,
	WPARAM		w_param,
	LPARAM		l_param
)
{
	auto win32_window = reinterpret_cast<bc::window_manager::Win32Window*>( GetWindowLongPtr( window_handle, GWLP_USERDATA ) );

	// Do limited message parsing until window has been created
	if( !win32_window )
	{
		switch( message )
		{
		case WM_DESTROY:
			// Handle manually.
			// PostQuitMessage( 0 );
			break;

		case WM_CLOSE:
			// Prevent closing.
			break;

		default:
			return DefWindowProcW( window_handle, message, w_param, l_param );
		}
		return 0;
	}


	auto GetModifierKeys = []() -> bc::window_manager::ModifierKeyFlags
		{
			// Check the state of modifier keys
			SHORT shift_key_state		= GetKeyState( VK_SHIFT );
			SHORT ctrl_key_state		= GetKeyState( VK_CONTROL );
			SHORT alt_key_state			= GetKeyState( VK_MENU );
			SHORT left_super_key_state	= GetKeyState( VK_LWIN );
			SHORT right_super_key_state	= GetKeyState( VK_RWIN );

			bc::window_manager::ModifierKeyFlags modifier_keys;
			modifier_keys |= ( shift_key_state & 0x8000 ) ? bc::window_manager::ModifierKeyFlags::SHIFT : bc::window_manager::ModifierKeyFlags {};
			modifier_keys |= ( ctrl_key_state & 0x8000 ) ? bc::window_manager::ModifierKeyFlags::CONTROL : bc::window_manager::ModifierKeyFlags {};
			modifier_keys |= ( alt_key_state & 0x8000 ) ? bc::window_manager::ModifierKeyFlags::ALT : bc::window_manager::ModifierKeyFlags {};
			modifier_keys |= ( ( left_super_key_state & 0x8000 ) || ( right_super_key_state & 0x8000 ) ) ? bc::window_manager::ModifierKeyFlags::SUPER : bc::window_manager::ModifierKeyFlags {};

			return modifier_keys;
		};

	auto MapVirtualKeyToKeyboardButton = []( WPARAM w_param, LPARAM l_param ) -> bc::window_manager::KeyboardButton
		{
			if( w_param == VK_SPACE )			return bc::window_manager::KeyboardButton::KEY_SPACE;
			if( w_param == VK_OEM_7 )			return bc::window_manager::KeyboardButton::KEY_APOSTROPHE;		// Double check.
			if( w_param == VK_OEM_PLUS )		return bc::window_manager::KeyboardButton::KEY_PLUS;			// Double check.
			if( w_param == VK_OEM_COMMA )		return bc::window_manager::KeyboardButton::KEY_COMMA;
			if( w_param == VK_OEM_MINUS )		return bc::window_manager::KeyboardButton::KEY_MINUS;
			if( w_param == VK_OEM_PERIOD )		return bc::window_manager::KeyboardButton::KEY_PERIOD;
			if( w_param == VK_OEM_2 )			return bc::window_manager::KeyboardButton::KEY_SLASH;			// Double check.
			if( w_param == 0x30 )				return bc::window_manager::KeyboardButton::KEY_0;
			if( w_param == 0x31 )				return bc::window_manager::KeyboardButton::KEY_1;
			if( w_param == 0x32 )				return bc::window_manager::KeyboardButton::KEY_2;
			if( w_param == 0x33 )				return bc::window_manager::KeyboardButton::KEY_3;
			if( w_param == 0x34 )				return bc::window_manager::KeyboardButton::KEY_4;
			if( w_param == 0x35 )				return bc::window_manager::KeyboardButton::KEY_5;
			if( w_param == 0x36 )				return bc::window_manager::KeyboardButton::KEY_6;
			if( w_param == 0x37 )				return bc::window_manager::KeyboardButton::KEY_7;
			if( w_param == 0x38 )				return bc::window_manager::KeyboardButton::KEY_8;
			if( w_param == 0x39 )				return bc::window_manager::KeyboardButton::KEY_9;
			if( w_param == VK_OEM_1 )			return bc::window_manager::KeyboardButton::KEY_SEMICOLON;		// Double check.
			if( w_param == 0x41 )				return bc::window_manager::KeyboardButton::KEY_A;
			if( w_param == 0x42 )				return bc::window_manager::KeyboardButton::KEY_B;
			if( w_param == 0x43 )				return bc::window_manager::KeyboardButton::KEY_C;
			if( w_param == 0x44 )				return bc::window_manager::KeyboardButton::KEY_D;
			if( w_param == 0x45 )				return bc::window_manager::KeyboardButton::KEY_E;
			if( w_param == 0x46 )				return bc::window_manager::KeyboardButton::KEY_F;
			if( w_param == 0x47 )				return bc::window_manager::KeyboardButton::KEY_G;
			if( w_param == 0x48 )				return bc::window_manager::KeyboardButton::KEY_H;
			if( w_param == 0x49 )				return bc::window_manager::KeyboardButton::KEY_I;
			if( w_param == 0x4A )				return bc::window_manager::KeyboardButton::KEY_J;
			if( w_param == 0x4B )				return bc::window_manager::KeyboardButton::KEY_K;
			if( w_param == 0x4C )				return bc::window_manager::KeyboardButton::KEY_L;
			if( w_param == 0x4D )				return bc::window_manager::KeyboardButton::KEY_M;
			if( w_param == 0x4E )				return bc::window_manager::KeyboardButton::KEY_N;
			if( w_param == 0x4F )				return bc::window_manager::KeyboardButton::KEY_O;
			if( w_param == 0x50 )				return bc::window_manager::KeyboardButton::KEY_P;
			if( w_param == 0x51 )				return bc::window_manager::KeyboardButton::KEY_Q;
			if( w_param == 0x52 )				return bc::window_manager::KeyboardButton::KEY_R;
			if( w_param == 0x53 )				return bc::window_manager::KeyboardButton::KEY_S;
			if( w_param == 0x54 )				return bc::window_manager::KeyboardButton::KEY_T;
			if( w_param == 0x55 )				return bc::window_manager::KeyboardButton::KEY_U;
			if( w_param == 0x56 )				return bc::window_manager::KeyboardButton::KEY_V;
			if( w_param == 0x57 )				return bc::window_manager::KeyboardButton::KEY_W;
			if( w_param == 0x58 )				return bc::window_manager::KeyboardButton::KEY_X;
			if( w_param == 0x59 )				return bc::window_manager::KeyboardButton::KEY_Y;
			if( w_param == 0x5A )				return bc::window_manager::KeyboardButton::KEY_Z;
			if( w_param == VK_OEM_4 )			return bc::window_manager::KeyboardButton::KEY_LEFT_BRACKET;
			if( w_param == VK_OEM_5 )			return bc::window_manager::KeyboardButton::KEY_BACKSLASH;
			if( w_param == VK_OEM_6 )			return bc::window_manager::KeyboardButton::KEY_RIGHT_BRACKET;
			if( w_param == VK_OEM_3 )			return bc::window_manager::KeyboardButton::KEY_GRAVE_ACCENT;
			if( w_param == VK_OEM_AX )			return bc::window_manager::KeyboardButton::KEY_AX;
			if( w_param == VK_OEM_102 )			return bc::window_manager::KeyboardButton::KEY_ANGLE_BRACKETS;

			if( w_param == VK_ESCAPE )			return bc::window_manager::KeyboardButton::KEY_ESCAPE;
			if( w_param == VK_RETURN )
			{
				if( ( l_param & 0x01000000 ) != 0 )
				{
					return bc::window_manager::KeyboardButton::KEY_NUMPAD_ENTER;
				}
				else
				{
					return bc::window_manager::KeyboardButton::KEY_ENTER;
				}
			}
			if( w_param == VK_TAB )				return bc::window_manager::KeyboardButton::KEY_TAB;
			if( w_param == VK_BACK )			return bc::window_manager::KeyboardButton::KEY_BACKSPACE;
			if( w_param == VK_INSERT )			return bc::window_manager::KeyboardButton::KEY_INSERT;
			if( w_param == VK_DELETE )			return bc::window_manager::KeyboardButton::KEY_DELETE;
			if( w_param == VK_RIGHT )			return bc::window_manager::KeyboardButton::KEY_RIGHT;
			if( w_param == VK_LEFT )			return bc::window_manager::KeyboardButton::KEY_LEFT;
			if( w_param == VK_DOWN )			return bc::window_manager::KeyboardButton::KEY_DOWN;
			if( w_param == VK_UP )				return bc::window_manager::KeyboardButton::KEY_UP;
			if( w_param == VK_PRIOR )			return bc::window_manager::KeyboardButton::KEY_PAGE_UP;
			if( w_param == VK_NEXT )			return bc::window_manager::KeyboardButton::KEY_PAGE_DOWN;
			if( w_param == VK_HOME )			return bc::window_manager::KeyboardButton::KEY_HOME;
			if( w_param == VK_END )				return bc::window_manager::KeyboardButton::KEY_END;
			if( w_param == VK_CAPITAL )			return bc::window_manager::KeyboardButton::KEY_CAPS_LOCK;
			if( w_param == VK_SCROLL )			return bc::window_manager::KeyboardButton::KEY_SCROLL_LOCK;
			if( w_param == VK_NUMLOCK )			return bc::window_manager::KeyboardButton::KEY_NUM_LOCK;
			if( w_param == VK_PRINT )			return bc::window_manager::KeyboardButton::KEY_PRINT_SCREEN;
			if( w_param == VK_PAUSE )			return bc::window_manager::KeyboardButton::KEY_PAUSE;
			if( w_param == VK_F1 )				return bc::window_manager::KeyboardButton::KEY_F1;
			if( w_param == VK_F2 )				return bc::window_manager::KeyboardButton::KEY_F2;
			if( w_param == VK_F3 )				return bc::window_manager::KeyboardButton::KEY_F3;
			if( w_param == VK_F4 )				return bc::window_manager::KeyboardButton::KEY_F4;
			if( w_param == VK_F5 )				return bc::window_manager::KeyboardButton::KEY_F5;
			if( w_param == VK_F6 )				return bc::window_manager::KeyboardButton::KEY_F6;
			if( w_param == VK_F7 )				return bc::window_manager::KeyboardButton::KEY_F7;
			if( w_param == VK_F8 )				return bc::window_manager::KeyboardButton::KEY_F8;
			if( w_param == VK_F9 )				return bc::window_manager::KeyboardButton::KEY_F9;
			if( w_param == VK_F10 )				return bc::window_manager::KeyboardButton::KEY_F10;
			if( w_param == VK_F11 )				return bc::window_manager::KeyboardButton::KEY_F11;
			if( w_param == VK_F12 )				return bc::window_manager::KeyboardButton::KEY_F12;
			if( w_param == VK_F13 )				return bc::window_manager::KeyboardButton::KEY_F13;
			if( w_param == VK_F14 )				return bc::window_manager::KeyboardButton::KEY_F14;
			if( w_param == VK_F15 )				return bc::window_manager::KeyboardButton::KEY_F15;
			if( w_param == VK_F16 )				return bc::window_manager::KeyboardButton::KEY_F16;
			if( w_param == VK_F17 )				return bc::window_manager::KeyboardButton::KEY_F17;
			if( w_param == VK_F18 )				return bc::window_manager::KeyboardButton::KEY_F18;
			if( w_param == VK_F19 )				return bc::window_manager::KeyboardButton::KEY_F19;
			if( w_param == VK_F20 )				return bc::window_manager::KeyboardButton::KEY_F20;
			if( w_param == VK_F21 )				return bc::window_manager::KeyboardButton::KEY_F21;
			if( w_param == VK_F22 )				return bc::window_manager::KeyboardButton::KEY_F22;
			if( w_param == VK_F23 )				return bc::window_manager::KeyboardButton::KEY_F23;
			if( w_param == VK_F24 )				return bc::window_manager::KeyboardButton::KEY_F24;
			if( w_param == VK_NUMPAD0 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_0;
			if( w_param == VK_NUMPAD1 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_1;
			if( w_param == VK_NUMPAD2 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_2;
			if( w_param == VK_NUMPAD3 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_3;
			if( w_param == VK_NUMPAD4 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_4;
			if( w_param == VK_NUMPAD5 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_5;
			if( w_param == VK_NUMPAD6 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_6;
			if( w_param == VK_NUMPAD7 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_7;
			if( w_param == VK_NUMPAD8 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_8;
			if( w_param == VK_NUMPAD9 )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_9;
			if( w_param == VK_DECIMAL )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_DECIMAL;
			if( w_param == VK_DIVIDE )			return bc::window_manager::KeyboardButton::KEY_NUMPAD_DIVIDE;
			if( w_param == VK_MULTIPLY )		return bc::window_manager::KeyboardButton::KEY_NUMPAD_MULTIPLY;
			if( w_param == VK_SUBTRACT )		return bc::window_manager::KeyboardButton::KEY_NUMPAD_SUBTRACT;
			if( w_param == VK_ADD )				return bc::window_manager::KeyboardButton::KEY_NUMPAD_ADD;
			if( w_param == VK_OEM_NEC_EQUAL )	return bc::window_manager::KeyboardButton::KEY_NUMPAD_EQUAL;
			if( w_param == VK_LSHIFT )			return bc::window_manager::KeyboardButton::KEY_LEFT_SHIFT;
			if( w_param == VK_LCONTROL )		return bc::window_manager::KeyboardButton::KEY_LEFT_CONTROL;
			if( w_param == VK_LMENU )			return bc::window_manager::KeyboardButton::KEY_LEFT_ALT;
			if( w_param == VK_LWIN )			return bc::window_manager::KeyboardButton::KEY_LEFT_SUPER;
			if( w_param == VK_RSHIFT )			return bc::window_manager::KeyboardButton::KEY_RIGHT_SHIFT;
			if( w_param == VK_RCONTROL )		return bc::window_manager::KeyboardButton::KEY_RIGHT_CONTROL;
			if( w_param == VK_RMENU )			return bc::window_manager::KeyboardButton::KEY_RIGHT_ALT;
			if( w_param == VK_RWIN )			return bc::window_manager::KeyboardButton::KEY_RIGHT_SUPER;
			if( w_param == VK_MENU )			return bc::window_manager::KeyboardButton::KEY_MENU;
			return {};
		};

	auto SetUpMouseLeaveTracking = [ window_handle ]()
		{
			// Set up tracking for mouse events
			TRACKMOUSEEVENT tme {};
			tme.cbSize = sizeof( TRACKMOUSEEVENT );
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = window_handle;
			TrackMouseEvent( &tme );
		};

	// Do full message processing
	switch( message )
	{
		// Startup.
	case WM_CREATE:
		SetUpMouseLeaveTracking();
		break;



		// Window events.
	case WM_DESTROY:
		// Handle manually.
		// PostQuitMessage( 0 );
		break;

	case WM_CLOSE:
		win32_window->events.CloseRequested.Signal();
		break;

	case WM_MOVE:
	{
		auto window_position = MAKEPOINTS( l_param );
		win32_window->events.PositionChanged.Signal( bc::math::Vector2i { static_cast<i64>( window_position.x ), static_cast<i64>( window_position.y ) } );
		break;
	}

	case WM_SIZE:
	{
		if( w_param == SIZE_MINIMIZED )
		{
			win32_window->runtime_info.current_windows_state = bc::window_manager::WindowState::MINIMIZED;
			win32_window->events.StateChanged.Signal( win32_window->runtime_info.current_windows_state );
		}
		else if( w_param == SIZE_MAXIMIZED )
		{
			win32_window->runtime_info.current_windows_state = bc::window_manager::WindowState::MAXIMIZED;
			win32_window->events.StateChanged.Signal( win32_window->runtime_info.current_windows_state );
		}
		else if( w_param == SIZE_RESTORED )
		{
			if( win32_window->runtime_info.current_windows_state == bc::window_manager::WindowState::MINIMIZED ||
				win32_window->runtime_info.current_windows_state == bc::window_manager::WindowState::MAXIMIZED )
			{
				if( win32_window->runtime_info.is_fullscreen )
				{
					win32_window->runtime_info.current_windows_state = bc::window_manager::WindowState::FULLSCREEN;
				}
				else
				{
					win32_window->runtime_info.current_windows_state = bc::window_manager::WindowState::WINDOWED;
				}
				win32_window->events.StateChanged.Signal( win32_window->runtime_info.current_windows_state );
			}
		}
		win32_window->events.SizeChanged.Signal( bc::math::Vector2i { static_cast<i64>( LOWORD( l_param ) ), static_cast<i64>( HIWORD( l_param ) ) } );
		break;
	}

	case WM_SETFOCUS:
		win32_window->events.FocusChanged.Signal( true );
		break;

	case WM_KILLFOCUS:
		win32_window->events.FocusChanged.Signal( false );
		break;

	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)w_param;
		UINT file_count = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );
		bc::List<bc::Text> result( file_count );
		for( UINT i = 0; i < file_count; ++i )
		{
			UINT buffer_size = DragQueryFile( hDrop, i, NULL, 0 );
			result[ i ].Reserve( buffer_size + 1 );
			result[ i ].Resize( buffer_size );

			DragQueryFile( hDrop, i, result[ i ].Data(), buffer_size + 1 );
		}
		DragFinish( hDrop );

		win32_window->events.FileDrop.Signal( result );
		break;
	}



	// Mouse events.
	case WM_LBUTTONDOWN:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_1, bc::window_manager::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_LBUTTONUP:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_1, bc::window_manager::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;

	case WM_RBUTTONDOWN:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_2, bc::window_manager::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_RBUTTONUP:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_2, bc::window_manager::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;

	case WM_MBUTTONDOWN:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_3, bc::window_manager::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_MBUTTONUP:
		win32_window->events.MouseButton.Signal( bc::window_manager::MouseButton::BUTTON_3, bc::window_manager::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;

	case WM_XBUTTONDOWN:
	{
		auto button_number = GET_XBUTTON_WPARAM( w_param );
		auto mouse_button = bc::window_manager::MouseButton {};
		if( button_number & XBUTTON1 ) mouse_button = bc::window_manager::MouseButton::BUTTON_4;
		if( button_number & XBUTTON2 ) mouse_button = bc::window_manager::MouseButton::BUTTON_5;
		win32_window->events.MouseButton.Signal( mouse_button, bc::window_manager::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;
	}

	case WM_XBUTTONUP:
	{
		auto button_number = GET_XBUTTON_WPARAM( w_param );
		auto mouse_button = bc::window_manager::MouseButton {};
		if( button_number & XBUTTON1 ) mouse_button = bc::window_manager::MouseButton::BUTTON_4;
		if( button_number & XBUTTON2 ) mouse_button = bc::window_manager::MouseButton::BUTTON_5;
		win32_window->events.MouseButton.Signal( mouse_button, bc::window_manager::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;
	}

	case WM_MOUSEMOVE:
	{
		if( !win32_window->runtime_info.is_mouse_over_window )
		{
			win32_window->runtime_info.is_mouse_over_window = true;
			win32_window->events.MouseEnter.Signal( true );
			SetUpMouseLeaveTracking();
		}
		auto mouse_pos = MAKEPOINTS( l_param );
		win32_window->events.MousePosition.Signal( bc::math::Vector2d { static_cast<f64>( mouse_pos.x ), static_cast<f64>( mouse_pos.y ) } );
		break;
	}

	case WM_MOUSELEAVE:
	{
		win32_window->runtime_info.is_mouse_over_window = false;
		win32_window->events.MouseEnter.Signal( false );
	}

	case WM_MOUSEWHEEL:
		win32_window->events.MouseScroll.Signal( bc::math::Vector2d { 0.0, static_cast<f64>( GET_WHEEL_DELTA_WPARAM( w_param ) ) } );
		break;

	case WM_MOUSEHWHEEL:
		win32_window->events.MouseScroll.Signal( bc::math::Vector2d { static_cast<f64>( GET_WHEEL_DELTA_WPARAM( w_param ), 0.0 ) } );
		break;



		// Keyboard events.
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		bc::window_manager::KeyboardButton keyboard_button = MapVirtualKeyToKeyboardButton( w_param, l_param );
		bc::window_manager::KeyboardButtonAction action = bc::window_manager::KeyboardButtonAction::PRESSED;
		i32 scancode = HIWORD( l_param ) & 0xFF;
		if( l_param & ( 1 << 24 ) ) scancode |= 0xE000;
		int repeat_count = LOWORD( l_param );
		if( repeat_count > 1 ) action = bc::window_manager::KeyboardButtonAction::REPEATED;
		win32_window->events.KeyboardKey.Signal( keyboard_button, scancode, action, GetModifierKeys() );
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		bc::window_manager::KeyboardButton keyboard_button = MapVirtualKeyToKeyboardButton( w_param, l_param );
		bc::window_manager::KeyboardButtonAction action = bc::window_manager::KeyboardButtonAction::RELEASED;
		i32 scancode = HIWORD( l_param ) & 0xFF;
		if( l_param & ( 1 << 24 ) ) scancode |= 0xE000;
		win32_window->events.KeyboardKey.Signal( keyboard_button, scancode, action, GetModifierKeys() );
		break;
	}



	// Text input.
	case WM_CHAR:
	{
		win32_window->events.KeyboardCharacter.Signal( static_cast<i32>( w_param ) );
		break;
	}
	case WM_UNICHAR:
	{
		if( w_param == UNICODE_NOCHAR )
		{
			// Return true to signal that we support unicode characters.
			return TRUE;
		}

		win32_window->events.KeyboardCharacter.Signal( static_cast<i32>( w_param ) );
		break;
	}

	default:
		return DefWindowProcW( window_handle, message, w_param, l_param );
	}
	return 0;
}



} // window_manager
} // bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Win32Manager::Win32Manager(
	WindowManagerWin32Component				&	window_manager_win32_component,
	const WindowManagerComponentCreateInfo	&	window_manager_component_create_info
) :
	window_manager_win32_component( window_manager_win32_component )
{
	// Register window class
	window_class					= { 0 };
	window_class.lpfnWndProc		= WndProc;
	window_class.hInstance			= GetModuleHandleW( NULL );
	window_class.hbrBackground		= (HBRUSH)( COLOR_BACKGROUND );
	window_class.lpszClassName		= reinterpret_cast<const wchar_t*>( window_class_name.ToCStr() );

	if( !RegisterClassW( &window_class ) )
	{
		MessageBoxW( NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK );
		BHardAssert( 0, "Window registration failed" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Win32Manager::~Win32Manager()
{
	BHardAssert( active_window_list.IsEmpty(), "Error while shutting down OS window system, there are still active windows" );

	UnregisterClassW( window_class.lpszClassName, window_class.hInstance );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Manager::Run()
{
	ProcessMessages();

	for( auto window : active_window_list )
	{
		window->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::Win32Manager::CreateWindow(
	const WindowCreateInfo  & window_create_info
)
{
	auto new_window = MakeUniquePtr<Win32Window>( *this, window_create_info );
	active_window_list.PushBack( new_window.Get() );
	window_manager_win32_component.events.OnWindowCreated.Signal( new_window.Get() );
	return new_window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Manager::NotifyWindowBeingDestroyed(
	bc::window_manager::Win32Window * window_ptr
)
{
	window_manager_win32_component.events.OnWindowBeingDestroyed.Signal( window_ptr );
	active_window_list.Erase( window_ptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Manager::ProcessMessages()
{
	MSG message {};
	while( PeekMessageW( &message, NULL, 0, 0, PM_REMOVE ) )
	{
		if( message.message == WM_QUIT )
		{
			break;
		}
		TranslateMessage( &message );
		DispatchMessageW( &message );
	}
}
