
#include <window_manager_xcb/PreCompiledHeader.hpp>
#include <window_manager_xcb/xcb_manager/XCBManager.hpp>

#include <window_manager/window/Window.hpp>
#include <window_manager_xcb/window/XCBWindow.hpp>
#include <window_manager_xcb/WindowManagerXCBComponent.hpp>
#include <window_manager_xcb/xcb_manager/XCBEvents.hpp>

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <X11/Xlocale.h>

#include <iostream>
#include "XCBManager.hpp"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XCBManager::XCBManager(
	WindowManagerXCBComponent				&	window_manager_xcb_component,
	const WindowManagerComponentCreateInfo	&	window_manager_component_create_info
) :
	window_manager_xcb_component( window_manager_xcb_component )
{
	// Open xlib display
	platform_handles.xlib_display = XOpenDisplay( nullptr );
	if( platform_handles.xlib_display == nullptr )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to open X11 display" );
	}

	// Get the XCB connection from the Xlib display
	//platform_handles.xcb_connection = XGetXCBConnection( platform_handles.xlib_display );
	//if( xcb_connection_has_error( platform_handles.xcb_connection ) )
	//{
	//	CleanupHandles();
	//	diagnostic::Throw( "Failed to get XCB connection" );
	//}

	// Populate XCB atoms
	if( auto result = PopulateX11Atoms() )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to populate XCB atoms" );
	}

	// Get XCB screen
	//auto setup = xcb_get_setup( platform_handles.xcb_connection );
	//if( !setup )
	//{
	//	CleanupHandles();
	//	diagnostic::Throw( "Failed to get XCB setup" );
	//}
	//auto iter = xcb_setup_roots_iterator( setup );
	//if( iter.rem <= 0 )
	//{
	//	CleanupHandles();
	//	diagnostic::Throw( "Failed to get XCB screen" );
	//}
	//platform_handles.xcb_screen = iter.data;
	//if( !platform_handles.xcb_screen )
	//{
	//	CleanupHandles();
	//	diagnostic::Throw( "Failed to get XCB screen" );
	//}

	// Allocate key symbols
	//platform_handles.xcb_key_symbols = xcb_key_symbols_alloc( platform_handles.xcb_connection );
	//if( !platform_handles.xcb_key_symbols )
	//{
	//	CleanupHandles();
	//	diagnostic::Throw( "Failed to get XCB key symbols" );
	//}

	// Create an X input method (global)
	platform_handles.xlib_xim = XOpenIM( platform_handles.xlib_display, nullptr, nullptr, nullptr );
	if( !platform_handles.xlib_xim )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get XIM" );
	}

	// Create an input context (global)
	platform_handles.xlib_xic = XCreateIC( platform_handles.xlib_xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, nullptr );
	if( !platform_handles.xlib_xic )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to get XIC" );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XCBManager::~XCBManager()
{
	BHardAssert( active_window_list.IsEmpty(), "Error while shutting down OS window system, there are still active windows" );

	CleanupHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBManager::CleanupHandles()
{
	xcb_flush( platform_handles.xcb_connection );

	if( platform_handles.xlib_xic )			XDestroyIC( platform_handles.xlib_xic );
	platform_handles.xlib_xic 				= nullptr;

	if( platform_handles.xlib_xim )			XCloseIM( platform_handles.xlib_xim );
	platform_handles.xlib_xim 				= nullptr;

	if( platform_handles.xcb_key_symbols )	xcb_key_symbols_free( platform_handles.xcb_key_symbols );
	platform_handles.xcb_key_symbols 		= nullptr;

	if( platform_handles.xlib_display )		XCloseDisplay( platform_handles.xlib_display );
	platform_handles.xlib_display 			= nullptr;
	platform_handles.xcb_connection 		= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBManager::Run()
{
	ProcessMessages();

	for( auto window : active_window_list )
	{
		window->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::XCBManager::CreateWindow(
	const WindowCreateInfo  & window_create_info
)
{
	auto new_window = MakeUniquePtr<XCBWindow>( *this, window_create_info );
	active_window_list.PushBack( new_window.Get() );
	auto platform_handles = static_cast<const bc::window_manager::WindowManagerXCBPlatformHandles*>( new_window->GetPlatformSpecificHandles() );
	window_manager_xcb_component.events.OnWindowCreated.Signal( new_window.Get() );
	return new_window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBManager::NotifyWindowBeingDestroyed(
	bc::window_manager::XCBWindow * window_ptr
)
{
	window_manager_xcb_component.events.OnWindowBeingDestroyed.Signal( window_ptr );
	active_window_list.Erase( window_ptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBManager::ProcessMessages()
{
	xcb_generic_event_t * event = nullptr;
	while( ( event = xcb_poll_for_event( platform_handles.xcb_connection ) ) )
	{
		ProcessXCBEvent( event );
		free( event );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBManager::ProcessXCBEvent(
	xcb_generic_event_t * event
)
{
	// std::cout << "XCB Event: " << static_cast<int>( event->response_type ) << std::endl;

	auto GetXCBWindow = [ this ]( auto window ) -> bc::window_manager::XCBWindow*
	{
		return xcb::GetProperty<bc::window_manager::XCBWindow*>(
			platform_handles.xcb_connection,
			window,
			platform_handles.window_user_pointer_atom,
			XCB_ATOM_CARDINAL,
			xcb::PropertyFormat::F32
		);
	};

	switch( event->response_type & ~0x80 )
	{
		case XCB_KEY_PRESS:
		{
			auto e = reinterpret_cast<xcb_key_press_event_t*>( event );
			auto window = GetXCBWindow( e->event );

			auto key = bc::window_manager::KeyboardButton::KEY_UNKNOWN;

			auto modifier_key_flags = xcb::GetModifierKeyFlags( e->state );

			// Get key code
			auto key_code_symbol = xcb_key_press_lookup_keysym( platform_handles.xcb_key_symbols, e, 0 );
			auto key_code = bc::window_manager::xcb::GetKeyboardButton( key_code_symbol );

			// Get key symbols
			//auto keysym_column = i32 {};
			//if( std::to_underlying( modifier_key_flags & bc::window_manager::ModifierKeyFlags::SHIFT ) ) keysym_column += 1;
			//if( std::to_underlying( modifier_key_flags & bc::window_manager::ModifierKeyFlags::ALT_GR ) ) keysym_column += 2;
			//auto keysym = xcb_key_press_lookup_keysym( platform_handles.xcb_key_symbols, e, keysym_column );

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
			auto buffer = Array<wchar_t, 16> {};
			auto keysym_output = KeySym {};
			auto status = Status {};
			auto x_event = XEvent {};
			x_event.xkey.type = KeyPress;
			x_event.xkey.serial = e->sequence;
			x_event.xkey.send_event = False; // Test this
			x_event.xkey.display = platform_handles.xlib_display;
			x_event.xkey.window = e->event;
			x_event.xkey.root = e->root;
			x_event.xkey.subwindow = e->child;
			x_event.xkey.time = e->time;
			x_event.xkey.x = e->event_x;
			x_event.xkey.y = e->event_y;
			x_event.xkey.x_root = e->root_x;
			x_event.xkey.y_root = e->root_y;
			x_event.xkey.state = e->state;
			x_event.xkey.keycode = e->detail;
			x_event.xkey.same_screen = e->same_screen;

			if( XFilterEvent( &x_event, x_event.xkey.window ) ) break;

			int xmb_lookup_length = XwcLookupString(
				platform_handles.xlib_xic,
				&x_event.xkey,
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
		break;

		case XCB_KEY_RELEASE:
		{
			auto e = reinterpret_cast<const xcb_key_release_event_t*>( event );
			// TODO: Map keys.
		}
		break;

		case XCB_BUTTON_PRESS:
		{
			auto e = reinterpret_cast<const xcb_button_press_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.MouseButton.Signal(
				xcb::GetMouseButton( e->detail ),
				bc::window_manager::MouseButtonAction::PRESSED,
				xcb::GetModifierKeyFlags( e->state )
			);
		}
		break;

		case XCB_BUTTON_RELEASE:
		{
			auto e = reinterpret_cast<const xcb_button_release_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.MouseButton.Signal(
				xcb::GetMouseButton( e->detail ),
				bc::window_manager::MouseButtonAction::RELEASED,
				xcb::GetModifierKeyFlags( e->state )
			);
		}
		break;

		case XCB_MOTION_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_motion_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_ENTER_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_enter_notify_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.MouseEnter.Signal( true );
		}
		break;

		case XCB_LEAVE_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_leave_notify_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.MouseEnter.Signal( false );
		}
		break;

		case XCB_FOCUS_IN:
		{
			auto e = reinterpret_cast<const xcb_focus_in_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.FocusChanged.Signal( true );
		}
		break;

		case XCB_FOCUS_OUT:
		{
			auto e = reinterpret_cast<const xcb_focus_out_event_t*>( event );
			auto window = GetXCBWindow( e->event );
			window->events.FocusChanged.Signal( false );
		}
		break;

		case XCB_KEYMAP_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_keymap_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_EXPOSE:
		{
			auto e = reinterpret_cast<const xcb_expose_event_t*>( event );
			// TODO
		}
		break;

		case XCB_GRAPHICS_EXPOSURE:
		{
			auto e = reinterpret_cast<const xcb_graphics_exposure_event_t*>( event );
			// TODO
		}
		break;

		case XCB_NO_EXPOSURE:
		{
			auto e = reinterpret_cast<const xcb_no_exposure_event_t*>( event );
			// TODO
		}
		break;

		case XCB_VISIBILITY_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_visibility_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CREATE_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_create_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_DESTROY_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_destroy_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_UNMAP_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_unmap_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_MAP_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_map_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_MAP_REQUEST:
		{
			auto e = reinterpret_cast<const xcb_map_request_event_t*>( event );
			// TODO
		}
		break;

		case XCB_REPARENT_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_reparent_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CONFIGURE_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_configure_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CONFIGURE_REQUEST:
		{
			auto e = reinterpret_cast<const xcb_configure_request_event_t*>( event );
			// TODO
		}
		break;

		case XCB_GRAVITY_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_gravity_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_RESIZE_REQUEST:
		{
			auto e = reinterpret_cast<const xcb_resize_request_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CIRCULATE_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_circulate_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CIRCULATE_REQUEST:
		{
			auto e = reinterpret_cast<const xcb_circulate_request_event_t*>( event );
			// TODO
		}
		break;

		case XCB_PROPERTY_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_property_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_SELECTION_CLEAR:
		{
			auto e = reinterpret_cast<const xcb_selection_clear_event_t*>( event );
			// TODO
		}
		break;

		case XCB_SELECTION_REQUEST:
		{
			auto e = reinterpret_cast<const xcb_selection_request_event_t*>( event );
			// TODO
		}
		break;

		case XCB_SELECTION_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_selection_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_COLORMAP_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_colormap_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_CLIENT_MESSAGE:
		{
			auto e = reinterpret_cast<const xcb_client_message_event_t*>( event );
			//if( e->window != platform_handles.screen->root ) break;
			auto window = GetXCBWindow( e->window );
			if( e->data.data32[ 0 ] == platform_handles.window_protocol_close_atom )
			{
				window->events.CloseRequested.Signal();
				break;
			}
			if( e->data.data32[ 0 ] == platform_handles.window_protocol_take_focus_atom )
			{
				xcb::SetInputFocus( *window );
				break;
			}
			if( e->data.data32[ 0 ] == platform_handles.window_protocol_ping_atom )
			{
				// Does not work, might not need.
				xcb::ReplyToPing( e, *window );
				break;
			}
		}
		break;

		case XCB_MAPPING_NOTIFY:
		{
			auto e = reinterpret_cast<const xcb_mapping_notify_event_t*>( event );
			// TODO
		}
		break;

		case XCB_GE_GENERIC:
		{
			auto e = reinterpret_cast<const xcb_ge_generic_event_t*>( event );
			// TODO
		}
		break;

		default:
		{
			// TODO
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::Optional<bc::Text32> bc::window_manager::XCBManager::PopulateX11Atoms()
{
	platform_handles.window_user_pointer_atom = xcb::GetAtom( platform_handles.xcb_connection, "_BITCRAFTE_XCB_WINDOW_POINTER", true );
	if( platform_handles.window_user_pointer_atom == XCB_ATOM_NONE ) return { "Failed to get _BITCRAFTE_XCB_WINDOW_POINTER atom" };

	// Protocol atoms
	platform_handles.window_protocol_atom = xcb::GetAtom( platform_handles.xcb_connection, "WM_PROTOCOLS", false );
	if( platform_handles.window_protocol_atom == XCB_ATOM_NONE ) return { "Failed to get WM_PROTOCOLS atom" };

	platform_handles.window_protocol_close_atom = xcb::GetAtom( platform_handles.xcb_connection, "WM_DELETE_WINDOW", false );
	if( platform_handles.window_protocol_close_atom == XCB_ATOM_NONE ) return { "Failed to get WM_DELETE_WINDOW atom" };

	platform_handles.window_protocol_take_focus_atom = xcb::GetAtom( platform_handles.xcb_connection, "WM_TAKE_FOCUS", false );
	if( platform_handles.window_protocol_take_focus_atom == XCB_ATOM_NONE ) return { "Failed to get WM_TAKE_FOCUS atom" };

	platform_handles.window_protocol_ping_atom = xcb::GetAtom( platform_handles.xcb_connection, "_NET_WM_PING", false );
	if( platform_handles.window_protocol_ping_atom == XCB_ATOM_NONE ) return { "Failed to get _NET_WM_PING atom" };

	return {};
}
