
#include <window_manager_xlib/PreCompiledHeader.hpp>
#include <window_manager_xlib/xlib_manager/XLibManager.hpp>

#include <window_manager/window/Window.hpp>
#include <window_manager_xlib/window/XLibWindow.hpp>
#include <window_manager_xlib/WindowManagerXLibComponent.hpp>
#include <window_manager_xlib/xlib_manager/XLibEvents.hpp>

#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/Xatom.h>

#include <iostream>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XLibManager::XLibManager(
	WindowManagerXLibComponent				&	window_manager_xlib_component,
	const WindowManagerComponentCreateInfo	&	window_manager_component_create_info
) :
	window_manager_xlib_component( window_manager_xlib_component )
{
	// Open xlib display
	platform_handles.display = XOpenDisplay( nullptr );
	if( platform_handles.display == nullptr )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to open X11 display" );
	}

	// Get the default screen
	platform_handles.default_screen = DefaultScreen( platform_handles.display );
	if( platform_handles.default_screen < 0 )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get default screen" );
	}

	// Get the default visual
	platform_handles.default_visual = DefaultVisual( platform_handles.display, platform_handles.default_screen );
	if( platform_handles.default_visual == nullptr )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get default screen visual" );
	}

	// Get the default visual ID
	platform_handles.default_visual_id = XVisualIDFromVisual( platform_handles.default_visual );
	if( platform_handles.default_visual_id == 0 )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get default screen visual ID" );
	}

	// Populate XLib atoms
	if( auto result = PopulateX11Atoms() )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to populate XLib atoms" );
	}

	// Create an X input method (global)
	platform_handles.xim = XOpenIM( platform_handles.display, nullptr, nullptr, nullptr );
	if( !platform_handles.xim )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get XIM" );
	}

	// Create an input context (global)
	platform_handles.xic = XCreateIC( platform_handles.xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, nullptr );
	if( !platform_handles.xic )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get XIC" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XLibManager::~XLibManager()
{
	BHardAssert( active_window_list.IsEmpty(), "Error while shutting down OS window system, there are still active windows" );

	CleanupHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibManager::CleanupHandles()
{
	XFlush( platform_handles.display );

	if( platform_handles.xic )			XDestroyIC( platform_handles.xic );
	platform_handles.xic 				= nullptr;

	if( platform_handles.xim )			XCloseIM( platform_handles.xim );
	platform_handles.xim 				= nullptr;

	if( platform_handles.display )		XCloseDisplay( platform_handles.display );
	platform_handles.display 			= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibManager::Run()
{
	ProcessMessages();

	for( auto window : active_window_list )
	{
		window->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::XLibManager::CreateWindow(
	const WindowCreateInfo  & window_create_info
)
{
	auto new_window = MakeUniquePtr<XLibWindow>( *this, window_create_info );
	active_window_list.PushBack( new_window.Get() );
	auto platform_handles = static_cast<const bc::window_manager::WindowManagerXLibPlatformHandles*>( new_window->GetPlatformSpecificHandles() );
	window_manager_xlib_component.events.OnWindowCreated.Signal( new_window.Get() );
	return new_window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibManager::NotifyWindowBeingDestroyed(
	bc::window_manager::XLibWindow * window_ptr
)
{
	XFlush( platform_handles.display );

	window_manager_xlib_component.events.OnWindowBeingDestroyed.Signal( window_ptr );
	active_window_list.Erase( window_ptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibManager::ProcessMessages()
{
	auto event = XEvent {};
	while( XPending( platform_handles.display ) > 0 )
	{
		XNextEvent( platform_handles.display, &event );
		ProcessEvent( event );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibManager::ProcessEvent(
	XEvent & event
)
{
	std::cout << "Event type: " << event.type << std::endl;

	auto GetWindowPointer = [ this ]( auto window ) -> bc::window_manager::XLibWindow*
	{
		return xlib::GetProperty<bc::window_manager::XLibWindow*>(
			platform_handles.display,
			window,
			platform_handles.window_user_pointer_atom,
			XA_CARDINAL,
			xlib::PropertyFormat::F8
		);
	};

	switch( event.type )
	{
		case KeyPress:
		{
			auto & e = event.xkey;
			auto window = GetWindowPointer( e.window );

			auto key = bc::window_manager::KeyboardButton::KEY_UNKNOWN;

			auto modifier_key_flags = xlib::GetModifierKeyFlags( e.state );

			// Get key code
			auto key_code_symbol = XLookupKeysym( &e, 0 );
			auto key_code = bc::window_manager::xlib::GetKeyboardButton( key_code_symbol );

			// TODO: Get scancode.
			auto scancode = i32 {};

			// TODO: Handle repeat.
			auto keyboard_button_action = bc::window_manager::KeyboardButtonAction::PRESSED;

			window->events.KeyboardKey.Signal(
				key_code,
				scancode,
				keyboard_button_action,
				modifier_key_flags
			);

			// Handle text character input.
			static_assert( sizeof( wchar_t ) == 4 );
			if( XFilterEvent( &event, e.window ) ) return;

			auto buffer = Array<wchar_t, 32> {};
			auto keysym_output = KeySym {};
			auto status = Status {};
			int xmb_lookup_length = XwcLookupString(
				platform_handles.xic,
				&e,
				buffer.Data(),
				buffer.Size() - 1,
				&keysym_output,
				&status
			);
			buffer[ xmb_lookup_length ] = '\0';

			// TODO: if( status == XBufferOverflow )
			// TODO: Consider using XwcLookupString.

			std::wcout
				// << L"Keysym: " << keysym << L"\n"
				<< L"Modifier key flags: " << std::to_underlying( modifier_key_flags ) << L"\n"
				// << L"Col: " << keysym_column << L"\n"
				<< L"Keysym output: " << keysym_output << L"\n"
				<< L"Status: "<< status << std::endl;

			if( status != XLookupChars && status != XLookupBoth ) break;

			// TODO: Convert keysym to character that can be displayed on screen
			auto character = c32( buffer[ 0 ] );

			std::wcout << L"Character: \"" << buffer[ 0 ] << L"\"" << std::endl;

			if( character )
			{
				window->events.KeyboardCharacter.Signal( character );
			}
		}
		return;

		case KeyRelease:
		{
			// TODO: Map keys.
		}
		return;

		case ButtonPress:
		{
			auto & e = event.xbutton;
			auto window = GetWindowPointer( e.window );
			window->events.MouseButton.Signal(
				xlib::GetMouseButton( e.button ),
				bc::window_manager::MouseButtonAction::PRESSED,
				xlib::GetModifierKeyFlags( e.state )
			);
		}
		return;

		case ButtonRelease:
		{
			auto & e = event.xbutton;
			auto window = GetWindowPointer( e.window );
			window->events.MouseButton.Signal(
				xlib::GetMouseButton( e.button ),
				bc::window_manager::MouseButtonAction::RELEASED,
				xlib::GetModifierKeyFlags( e.state )
			);
		}
		return;

		case MotionNotify:
		{
			// TODO
		}
		return;

		case EnterNotify:
		{
			auto & e = event.xcrossing;
			auto window = GetWindowPointer( e.window );
			window->events.MouseEnter.Signal( true );
		}
		return;

		case LeaveNotify:
		{
			auto & e = event.xcrossing;
			auto window = GetWindowPointer( e.window );
			window->events.MouseEnter.Signal( false );
		}
		return;

		case FocusIn:
		{
			auto & e = event.xfocus;
			auto window = GetWindowPointer( e.window );
			window->events.FocusChanged.Signal( true );
		}
		return;

		case FocusOut:
		{
			auto & e = event.xfocus;
			auto window = GetWindowPointer( e.window );
			window->events.FocusChanged.Signal( false );
		}
		return;

		case KeymapNotify:
		{
			// TODO
		}
		return;

		case Expose:
		{
			// TODO
		}
		return;

		case GraphicsExpose:
		{
			// TODO
		}
		return;

		case NoExpose:
		{
			// TODO
		}
		return;

		case VisibilityNotify:
		{
			// TODO
		}
		return;

		case CreateNotify:
		{
			// TODO
		}
		return;

		case DestroyNotify:
		{
			// TODO
		}
		return;

		case UnmapNotify:
		{
			// TODO
		}
		return;

		case MapNotify:
		{
			// TODO
		}
		return;

		case MapRequest:
		{
			// TODO
		}
		return;

		case ReparentNotify:
		{
			// TODO
		}
		return;

		case ConfigureNotify:
		{
			// TODO
		}
		return;

		case ConfigureRequest:
		{
			// TODO
		}
		return;

		case GravityNotify:
		{
			// TODO
		}
		return;

		case ResizeRequest:
		{
			// TODO
		}
		return;

		case CirculateNotify:
		{
			// TODO
		}
		return;

		case CirculateRequest:
		{
			// TODO
		}
		return;

		case PropertyNotify:
		{
			// TODO
		}
		return;

		case SelectionClear:
		{
			// TODO
		}
		return;

		case SelectionRequest:
		{
			// TODO
		}
		return;

		case SelectionNotify:
		{
			// TODO
		}
		return;

		case ColormapNotify:
		{
			// TODO
		}
		return;

		case ClientMessage:
		{
			auto & e = event.xclient;
			//if( e->window != platform_handles.screen->root ) break;
			auto window = GetWindowPointer( e.window );
			if( e.data.l[ 0 ] == platform_handles.window_protocol_close_atom )
			{
				window->events.CloseRequested.Signal();
				return;
			}
			if( e.data.l[ 0 ] == platform_handles.window_protocol_take_focus_atom )
			{
				xlib::SetInputFocus( *window );
				return;
			}
			if( e.data.l[ 0 ] == platform_handles.window_protocol_ping_atom )
			{
				// Does not work, might not need.
				//xlib::ReplyToPing( e, *window );
				return;
			}
		}
		return;

		case MappingNotify:
		{
			// TODO
		}
		return;

		case GenericEvent:
		{
			// TODO
		}
		return;

		default:
		{
			// TODO
		}
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::Optional<bc::Text32> bc::window_manager::XLibManager::PopulateX11Atoms()
{
	platform_handles.window_user_pointer_atom = xlib::GetAtom( platform_handles.display, "_BITCRAFTE_XLIB_WINDOW_POINTER", true );
	if( platform_handles.window_user_pointer_atom == None ) return { "Failed to get _BITCRAFTE_XLIB_WINDOW_POINTER atom" };

	// Protocol atoms
	platform_handles.window_protocol_atom = xlib::GetAtom( platform_handles.display, "WM_PROTOCOLS", false );
	if( platform_handles.window_protocol_atom == None ) return { "Failed to get WM_PROTOCOLS atom" };

	platform_handles.window_protocol_close_atom = xlib::GetAtom( platform_handles.display, "WM_DELETE_WINDOW", false );
	if( platform_handles.window_protocol_close_atom == None ) return { "Failed to get WM_DELETE_WINDOW atom" };

	platform_handles.window_protocol_take_focus_atom = xlib::GetAtom( platform_handles.display, "WM_TAKE_FOCUS", false );
	if( platform_handles.window_protocol_take_focus_atom == None ) return { "Failed to get WM_TAKE_FOCUS atom" };

	platform_handles.window_protocol_ping_atom = xlib::GetAtom( platform_handles.display, "_NET_WM_PING", false );
	if( platform_handles.window_protocol_ping_atom == None ) return { "Failed to get _NET_WM_PING atom" };

	return {};
}
