
#include <window/PreCompiledHeader.hpp>
#include <window/os_window_system/windows/WindowsWindowSystem.hpp>
#include <window/os_window_system/windows/WindowsWindow.hpp>



#if BITCRAFTE_PLATFORM_WINDOWS



LRESULT CALLBACK WndProc(
	HWND		window_handle,
	UINT		message,
	WPARAM		w_param,
	LPARAM		l_param
)
{
	auto windows_window = reinterpret_cast<bc::window::WindowsWindow*>( GetWindowLongPtr( window_handle, GWLP_USERDATA ) );

	// Do limited message parsing until window has been created
	if( !windows_window )
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


	auto GetModifierKeys = []() -> bc::window::ModifierKeyFlags
		{
			// Check the state of modifier keys
			SHORT shift_key_state		= GetKeyState( VK_SHIFT );
			SHORT ctrl_key_state		= GetKeyState( VK_CONTROL );
			SHORT alt_key_state			= GetKeyState( VK_MENU );
			SHORT left_super_key_state	= GetKeyState( VK_LWIN );
			SHORT right_super_key_state	= GetKeyState( VK_RWIN );

			bc::window::ModifierKeyFlags modifier_keys;
			modifier_keys |= ( shift_key_state & 0x8000 ) ? bc::window::ModifierKeyFlags::SHIFT : bc::window::ModifierKeyFlags {};
			modifier_keys |= ( ctrl_key_state & 0x8000 ) ? bc::window::ModifierKeyFlags::CONTROL : bc::window::ModifierKeyFlags {};
			modifier_keys |= ( alt_key_state & 0x8000 ) ? bc::window::ModifierKeyFlags::ALT : bc::window::ModifierKeyFlags {};
			modifier_keys |= ( ( left_super_key_state & 0x8000 ) || ( right_super_key_state & 0x8000 ) ) ? bc::window::ModifierKeyFlags::SUPER : bc::window::ModifierKeyFlags {};

			return modifier_keys;
		};

	auto MapVirtualKeyToKeyboardButton = []( WPARAM w_param, LPARAM l_param ) -> bc::window::KeyboardButton
		{
			if( w_param == VK_SPACE )			return bc::window::KeyboardButton::KEY_SPACE;
			if( w_param == VK_OEM_7 )			return bc::window::KeyboardButton::KEY_APOSTROPHE;		// Double check.
			if( w_param == VK_OEM_PLUS )		return bc::window::KeyboardButton::KEY_PLUS;			// Double check.
			if( w_param == VK_OEM_COMMA )		return bc::window::KeyboardButton::KEY_COMMA;
			if( w_param == VK_OEM_MINUS )		return bc::window::KeyboardButton::KEY_MINUS;
			if( w_param == VK_OEM_PERIOD )		return bc::window::KeyboardButton::KEY_PERIOD;
			if( w_param == VK_OEM_2 )			return bc::window::KeyboardButton::KEY_SLASH;			// Double check.
			if( w_param == 0x30 )				return bc::window::KeyboardButton::KEY_0;
			if( w_param == 0x31 )				return bc::window::KeyboardButton::KEY_1;
			if( w_param == 0x32 )				return bc::window::KeyboardButton::KEY_2;
			if( w_param == 0x33 )				return bc::window::KeyboardButton::KEY_3;
			if( w_param == 0x34 )				return bc::window::KeyboardButton::KEY_4;
			if( w_param == 0x35 )				return bc::window::KeyboardButton::KEY_5;
			if( w_param == 0x36 )				return bc::window::KeyboardButton::KEY_6;
			if( w_param == 0x37 )				return bc::window::KeyboardButton::KEY_7;
			if( w_param == 0x38 )				return bc::window::KeyboardButton::KEY_8;
			if( w_param == 0x39 )				return bc::window::KeyboardButton::KEY_9;
			if( w_param == VK_OEM_1 )			return bc::window::KeyboardButton::KEY_SEMICOLON;		// Double check.
			if( w_param == 0x41 )				return bc::window::KeyboardButton::KEY_A;
			if( w_param == 0x42 )				return bc::window::KeyboardButton::KEY_B;
			if( w_param == 0x43 )				return bc::window::KeyboardButton::KEY_C;
			if( w_param == 0x44 )				return bc::window::KeyboardButton::KEY_D;
			if( w_param == 0x45 )				return bc::window::KeyboardButton::KEY_E;
			if( w_param == 0x46 )				return bc::window::KeyboardButton::KEY_F;
			if( w_param == 0x47 )				return bc::window::KeyboardButton::KEY_G;
			if( w_param == 0x48 )				return bc::window::KeyboardButton::KEY_H;
			if( w_param == 0x49 )				return bc::window::KeyboardButton::KEY_I;
			if( w_param == 0x4A )				return bc::window::KeyboardButton::KEY_J;
			if( w_param == 0x4B )				return bc::window::KeyboardButton::KEY_K;
			if( w_param == 0x4C )				return bc::window::KeyboardButton::KEY_L;
			if( w_param == 0x4D )				return bc::window::KeyboardButton::KEY_M;
			if( w_param == 0x4E )				return bc::window::KeyboardButton::KEY_N;
			if( w_param == 0x4F )				return bc::window::KeyboardButton::KEY_O;
			if( w_param == 0x50 )				return bc::window::KeyboardButton::KEY_P;
			if( w_param == 0x51 )				return bc::window::KeyboardButton::KEY_Q;
			if( w_param == 0x52 )				return bc::window::KeyboardButton::KEY_R;
			if( w_param == 0x53 )				return bc::window::KeyboardButton::KEY_S;
			if( w_param == 0x54 )				return bc::window::KeyboardButton::KEY_T;
			if( w_param == 0x55 )				return bc::window::KeyboardButton::KEY_U;
			if( w_param == 0x56 )				return bc::window::KeyboardButton::KEY_V;
			if( w_param == 0x57 )				return bc::window::KeyboardButton::KEY_W;
			if( w_param == 0x58 )				return bc::window::KeyboardButton::KEY_X;
			if( w_param == 0x59 )				return bc::window::KeyboardButton::KEY_Y;
			if( w_param == 0x5A )				return bc::window::KeyboardButton::KEY_Z;
			if( w_param == VK_OEM_4 )			return bc::window::KeyboardButton::KEY_LEFT_BRACKET;
			if( w_param == VK_OEM_5 )			return bc::window::KeyboardButton::KEY_BACKSLASH;
			if( w_param == VK_OEM_6 )			return bc::window::KeyboardButton::KEY_RIGHT_BRACKET;
			if( w_param == VK_OEM_3 )			return bc::window::KeyboardButton::KEY_GRAVE_ACCENT;
			if( w_param == VK_OEM_AX )			return bc::window::KeyboardButton::KEY_AX;
			if( w_param == VK_OEM_102 )			return bc::window::KeyboardButton::KEY_ANGLE_BRACKETS;

			if( w_param == VK_ESCAPE )			return bc::window::KeyboardButton::KEY_ESCAPE;
			if( w_param == VK_RETURN )
			{
				if( ( l_param & 0x01000000 ) != 0 )
				{
					return bc::window::KeyboardButton::KEY_NUMPAD_ENTER;
				}
				else
				{
					return bc::window::KeyboardButton::KEY_ENTER;
				}
			}
			if( w_param == VK_TAB )				return bc::window::KeyboardButton::KEY_TAB;
			if( w_param == VK_BACK )			return bc::window::KeyboardButton::KEY_BACKSPACE;
			if( w_param == VK_INSERT )			return bc::window::KeyboardButton::KEY_INSERT;
			if( w_param == VK_DELETE )			return bc::window::KeyboardButton::KEY_DELETE;
			if( w_param == VK_RIGHT )			return bc::window::KeyboardButton::KEY_RIGHT;
			if( w_param == VK_LEFT )			return bc::window::KeyboardButton::KEY_LEFT;
			if( w_param == VK_DOWN )			return bc::window::KeyboardButton::KEY_DOWN;
			if( w_param == VK_UP )				return bc::window::KeyboardButton::KEY_UP;
			if( w_param == VK_PRIOR )			return bc::window::KeyboardButton::KEY_PAGE_UP;
			if( w_param == VK_NEXT )			return bc::window::KeyboardButton::KEY_PAGE_DOWN;
			if( w_param == VK_HOME )			return bc::window::KeyboardButton::KEY_HOME;
			if( w_param == VK_END )				return bc::window::KeyboardButton::KEY_END;
			if( w_param == VK_CAPITAL )			return bc::window::KeyboardButton::KEY_CAPS_LOCK;
			if( w_param == VK_SCROLL )			return bc::window::KeyboardButton::KEY_SCROLL_LOCK;
			if( w_param == VK_NUMLOCK )			return bc::window::KeyboardButton::KEY_NUM_LOCK;
			if( w_param == VK_PRINT )			return bc::window::KeyboardButton::KEY_PRINT_SCREEN;
			if( w_param == VK_PAUSE )			return bc::window::KeyboardButton::KEY_PAUSE;
			if( w_param == VK_F1 )				return bc::window::KeyboardButton::KEY_F1;
			if( w_param == VK_F2 )				return bc::window::KeyboardButton::KEY_F2;
			if( w_param == VK_F3 )				return bc::window::KeyboardButton::KEY_F3;
			if( w_param == VK_F4 )				return bc::window::KeyboardButton::KEY_F4;
			if( w_param == VK_F5 )				return bc::window::KeyboardButton::KEY_F5;
			if( w_param == VK_F6 )				return bc::window::KeyboardButton::KEY_F6;
			if( w_param == VK_F7 )				return bc::window::KeyboardButton::KEY_F7;
			if( w_param == VK_F8 )				return bc::window::KeyboardButton::KEY_F8;
			if( w_param == VK_F9 )				return bc::window::KeyboardButton::KEY_F9;
			if( w_param == VK_F10 )				return bc::window::KeyboardButton::KEY_F10;
			if( w_param == VK_F11 )				return bc::window::KeyboardButton::KEY_F11;
			if( w_param == VK_F12 )				return bc::window::KeyboardButton::KEY_F12;
			if( w_param == VK_F13 )				return bc::window::KeyboardButton::KEY_F13;
			if( w_param == VK_F14 )				return bc::window::KeyboardButton::KEY_F14;
			if( w_param == VK_F15 )				return bc::window::KeyboardButton::KEY_F15;
			if( w_param == VK_F16 )				return bc::window::KeyboardButton::KEY_F16;
			if( w_param == VK_F17 )				return bc::window::KeyboardButton::KEY_F17;
			if( w_param == VK_F18 )				return bc::window::KeyboardButton::KEY_F18;
			if( w_param == VK_F19 )				return bc::window::KeyboardButton::KEY_F19;
			if( w_param == VK_F20 )				return bc::window::KeyboardButton::KEY_F20;
			if( w_param == VK_F21 )				return bc::window::KeyboardButton::KEY_F21;
			if( w_param == VK_F22 )				return bc::window::KeyboardButton::KEY_F22;
			if( w_param == VK_F23 )				return bc::window::KeyboardButton::KEY_F23;
			if( w_param == VK_F24 )				return bc::window::KeyboardButton::KEY_F24;
			if( w_param == VK_NUMPAD0 )			return bc::window::KeyboardButton::KEY_NUMPAD_0;
			if( w_param == VK_NUMPAD1 )			return bc::window::KeyboardButton::KEY_NUMPAD_1;
			if( w_param == VK_NUMPAD2 )			return bc::window::KeyboardButton::KEY_NUMPAD_2;
			if( w_param == VK_NUMPAD3 )			return bc::window::KeyboardButton::KEY_NUMPAD_3;
			if( w_param == VK_NUMPAD4 )			return bc::window::KeyboardButton::KEY_NUMPAD_4;
			if( w_param == VK_NUMPAD5 )			return bc::window::KeyboardButton::KEY_NUMPAD_5;
			if( w_param == VK_NUMPAD6 )			return bc::window::KeyboardButton::KEY_NUMPAD_6;
			if( w_param == VK_NUMPAD7 )			return bc::window::KeyboardButton::KEY_NUMPAD_7;
			if( w_param == VK_NUMPAD8 )			return bc::window::KeyboardButton::KEY_NUMPAD_8;
			if( w_param == VK_NUMPAD9 )			return bc::window::KeyboardButton::KEY_NUMPAD_9;
			if( w_param == VK_DECIMAL )			return bc::window::KeyboardButton::KEY_NUMPAD_DECIMAL;
			if( w_param == VK_DIVIDE )			return bc::window::KeyboardButton::KEY_NUMPAD_DIVIDE;
			if( w_param == VK_MULTIPLY )		return bc::window::KeyboardButton::KEY_NUMPAD_MULTIPLY;
			if( w_param == VK_SUBTRACT )		return bc::window::KeyboardButton::KEY_NUMPAD_SUBTRACT;
			if( w_param == VK_ADD )				return bc::window::KeyboardButton::KEY_NUMPAD_ADD;
			if( w_param == VK_OEM_NEC_EQUAL )	return bc::window::KeyboardButton::KEY_NUMPAD_EQUAL;
			if( w_param == VK_LSHIFT )			return bc::window::KeyboardButton::KEY_LEFT_SHIFT;
			if( w_param == VK_LCONTROL )		return bc::window::KeyboardButton::KEY_LEFT_CONTROL;
			if( w_param == VK_LMENU )			return bc::window::KeyboardButton::KEY_LEFT_ALT;
			if( w_param == VK_LWIN )			return bc::window::KeyboardButton::KEY_LEFT_SUPER;
			if( w_param == VK_RSHIFT )			return bc::window::KeyboardButton::KEY_RIGHT_SHIFT;
			if( w_param == VK_RCONTROL )		return bc::window::KeyboardButton::KEY_RIGHT_CONTROL;
			if( w_param == VK_RMENU )			return bc::window::KeyboardButton::KEY_RIGHT_ALT;
			if( w_param == VK_RWIN )			return bc::window::KeyboardButton::KEY_RIGHT_SUPER;
			if( w_param == VK_MENU )			return bc::window::KeyboardButton::KEY_MENU;
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
		windows_window->events.CloseRequested.Signal();
		break;

	case WM_MOVE:
	{
		auto window_position = MAKEPOINTS( l_param );
		windows_window->events.PositionChanged.Signal( bc::math::Vector2i { static_cast<int64_t>( window_position.x ), static_cast<int64_t>( window_position.y ) } );
		break;
	}

	case WM_SIZE:
	{
		if( w_param == SIZE_MINIMIZED )
		{
			windows_window->current_windows_state = bc::window::WindowState::MINIMIZED;
			windows_window->events.StateChanged.Signal( windows_window->current_windows_state );
		}
		else if( w_param == SIZE_MAXIMIZED )
		{
			windows_window->current_windows_state = bc::window::WindowState::MAXIMIZED;
			windows_window->events.StateChanged.Signal( windows_window->current_windows_state );
		}
		else if( w_param == SIZE_RESTORED )
		{
			if( windows_window->current_windows_state == bc::window::WindowState::MINIMIZED ||
				windows_window->current_windows_state == bc::window::WindowState::MAXIMIZED )
			{
				if( windows_window->is_fullscreen )
				{
					windows_window->current_windows_state = bc::window::WindowState::FULLSCREEN;
				}
				else
				{
					windows_window->current_windows_state = bc::window::WindowState::WINDOWED;
				}
				windows_window->events.StateChanged.Signal( windows_window->current_windows_state );
			}
		}
		windows_window->events.SizeChanged.Signal( bc::math::Vector2i { static_cast<int64_t>( LOWORD( l_param ) ), static_cast<int64_t>( HIWORD( l_param ) ) } );
		break;
	}

	case WM_SETFOCUS:
		windows_window->events.FocusChanged.Signal( true );
		break;

	case WM_KILLFOCUS:
		windows_window->events.FocusChanged.Signal( false );
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

		windows_window->events.FileDrop.Signal( result );
		break;
	}



	// Mouse events.
	case WM_LBUTTONDOWN:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_1, bc::window::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_LBUTTONUP:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_1, bc::window::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;
		
	case WM_RBUTTONDOWN:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_2, bc::window::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_RBUTTONUP:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_2, bc::window::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;
		
	case WM_MBUTTONDOWN:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_3, bc::window::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;

	case WM_MBUTTONUP:
		windows_window->events.MouseButton.Signal( bc::window::MouseButton::BUTTON_3, bc::window::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;
		
	case WM_XBUTTONDOWN:
	{
		auto button_number = GET_XBUTTON_WPARAM( w_param );
		auto mouse_button = bc::window::MouseButton {};
		if( button_number & XBUTTON1 ) mouse_button = bc::window::MouseButton::BUTTON_4;
		if( button_number & XBUTTON2 ) mouse_button = bc::window::MouseButton::BUTTON_5;
		windows_window->events.MouseButton.Signal( mouse_button, bc::window::MouseButtonAction::PRESSED, GetModifierKeys() );
		break;
	}

	case WM_XBUTTONUP:
	{
		auto button_number = GET_XBUTTON_WPARAM( w_param );
		auto mouse_button = bc::window::MouseButton {};
		if( button_number & XBUTTON1 ) mouse_button = bc::window::MouseButton::BUTTON_4;
		if( button_number & XBUTTON2 ) mouse_button = bc::window::MouseButton::BUTTON_5;
		windows_window->events.MouseButton.Signal( mouse_button, bc::window::MouseButtonAction::RELEASED, GetModifierKeys() );
		break;
	}

	case WM_MOUSEMOVE:
	{
		if( !windows_window->is_mouse_over_window )
		{
			windows_window->is_mouse_over_window = true;
			windows_window->events.MouseEnter.Signal( true );
			SetUpMouseLeaveTracking();
		}
		auto mouse_pos = MAKEPOINTS( l_param );
		windows_window->events.MousePosition.Signal( bc::math::Vector2d { static_cast<double>( mouse_pos.x ), static_cast<double>( mouse_pos.y ) } );
		break;
	}

	case WM_MOUSELEAVE:
	{
		windows_window->is_mouse_over_window = false;
		windows_window->events.MouseEnter.Signal( false );
	}

	case WM_MOUSEWHEEL:
		windows_window->events.MouseScroll.Signal( bc::math::Vector2d { 0.0, static_cast<double>( GET_WHEEL_DELTA_WPARAM( w_param ) ) } );
		break;

	case WM_MOUSEHWHEEL:
		windows_window->events.MouseScroll.Signal( bc::math::Vector2d { static_cast<double>( GET_WHEEL_DELTA_WPARAM( w_param ), 0.0 ) } );
		break;



	// Keyboard events.
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		bc::window::KeyboardButton keyboard_button = MapVirtualKeyToKeyboardButton( w_param, l_param );
		bc::window::KeyboardButtonAction action = bc::window::KeyboardButtonAction::PRESSED;
		int32_t scancode = HIWORD( l_param ) & 0xFF;
		if( l_param & ( 1 << 24 ) ) scancode |= 0xE000;
		int repeat_count = LOWORD( l_param );
		if( repeat_count > 1 ) action = bc::window::KeyboardButtonAction::REPEATED;
		windows_window->events.KeyboardKey.Signal( keyboard_button, scancode, action, GetModifierKeys() );
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		bc::window::KeyboardButton keyboard_button = MapVirtualKeyToKeyboardButton( w_param, l_param );
		bc::window::KeyboardButtonAction action = bc::window::KeyboardButtonAction::RELEASED;
		int32_t scancode = HIWORD( l_param ) & 0xFF;
		if( l_param & ( 1 << 24 ) ) scancode |= 0xE000;
		windows_window->events.KeyboardKey.Signal( keyboard_button, scancode, action, GetModifierKeys() );
		break;
	}



	// Text input.
	case WM_CHAR:
	{
		windows_window->events.KeyboardCharacter.Signal( static_cast<int32_t>( w_param ) );
		break;
	}
	case WM_UNICHAR:
	{
		if( w_param == UNICODE_NOCHAR )
		{
			// Return true to signal that we support unicode characters.
			return TRUE;
		}

		windows_window->events.KeyboardCharacter.Signal( static_cast<int32_t>( w_param ) );
		break;
	}

	default:
		return DefWindowProcW( window_handle, message, w_param, l_param );
	}
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowsWindowSystem::WindowsWindowSystem(
	WindowComponent							&	window_component,
	const WindowComponentCreateInfo			&	window_component_create_info
) :
	OSWindowSystem(
		window_component,
		window_component_create_info
	)
{
	// Register window class
	window_class					= { 0 };
	window_class.lpfnWndProc		= WndProc;
	window_class.hInstance			= GetModuleHandle( NULL );
	window_class.hbrBackground		= (HBRUSH)( COLOR_BACKGROUND );
	window_class.lpszClassName		= reinterpret_cast<const wchar_t*>( window_class_name.ToCStr() );

	if( !RegisterClassW( &window_class ) )
	{
		MessageBoxW( NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK );
		BHardAssert( 0, "Window registration failed" );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowsWindowSystem::~WindowsWindowSystem()
{
	UnregisterClassW( window_class.lpszClassName, window_class.hInstance );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowsWindowSystem::Run()
{
	ProcessMessages();
	OSWindowSystem::Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const WNDCLASSW & bc::window::WindowsWindowSystem::GetWindowClass()
{
	return window_class;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window::Window> bc::window::WindowsWindowSystem::DoCreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return MakeUniquePtr<WindowsWindow>( window_component, *this, window_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowsWindowSystem::ProcessMessages()
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



#endif // BITCRAFTE_PLATFORM_WINDOWS