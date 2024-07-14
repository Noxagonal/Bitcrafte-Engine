
#include <window_manager_xcb/PreCompiledHeader.hpp>

#include <window_manager_xcb/xcb_manager/XCBManager.hpp>
#include <window_manager_xcb/window/XCBWindow.hpp>

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include "XCBWindow.hpp"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XCBWindow::XCBWindow(
	XCBManager				&	xcb_manager,
	const WindowCreateInfo	&	window_create_info
) :
	Window( window_create_info ),
	xcb_manager( xcb_manager ),
	platform_handles( *xcb_manager.GetPlatformSpecificHandles() )
{
	platform_handles.xcb_window = xcb_generate_id( platform_handles.xcb_connection );

	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[ 32 ] = {};
	values[ 0 ] = platform_handles.xcb_screen->black_pixel;
	values[ 1 ] =
	XCB_EVENT_MASK_KEY_PRESS |
	XCB_EVENT_MASK_KEY_RELEASE |
	XCB_EVENT_MASK_BUTTON_PRESS |
	XCB_EVENT_MASK_BUTTON_RELEASE |
	XCB_EVENT_MASK_ENTER_WINDOW |
	XCB_EVENT_MASK_LEAVE_WINDOW |
	XCB_EVENT_MASK_POINTER_MOTION |
	XCB_EVENT_MASK_POINTER_MOTION_HINT |
	XCB_EVENT_MASK_BUTTON_1_MOTION |
	XCB_EVENT_MASK_BUTTON_2_MOTION |
	XCB_EVENT_MASK_BUTTON_3_MOTION |
	XCB_EVENT_MASK_BUTTON_4_MOTION |
	XCB_EVENT_MASK_BUTTON_5_MOTION |
	XCB_EVENT_MASK_BUTTON_MOTION |
	XCB_EVENT_MASK_KEYMAP_STATE |
	XCB_EVENT_MASK_EXPOSURE |
	XCB_EVENT_MASK_VISIBILITY_CHANGE |
	XCB_EVENT_MASK_STRUCTURE_NOTIFY |
	XCB_EVENT_MASK_RESIZE_REDIRECT |
	XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
	XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
	XCB_EVENT_MASK_FOCUS_CHANGE |
	XCB_EVENT_MASK_PROPERTY_CHANGE |
	XCB_EVENT_MASK_COLOR_MAP_CHANGE |
	XCB_EVENT_MASK_OWNER_GRAB_BUTTON;

	xcb_create_window(
		platform_handles.xcb_connection,
		XCB_COPY_FROM_PARENT,
		platform_handles.xcb_window,
		platform_handles.xcb_screen->root,
		int16_t( window_create_info.position.x ),
		int16_t( window_create_info.position.y ),
		uint16_t( window_create_info.dimensions.x ),
		uint16_t( window_create_info.dimensions.y ),
		0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		platform_handles.xcb_screen->root_visual,
		mask,
		&values
	);
	xcb_map_window( platform_handles.xcb_connection, platform_handles.xcb_window );
	xcb_flush( platform_handles.xcb_connection );

	SetupXCBPropertyHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XCBWindow::~XCBWindow()
{
	xcb_manager.NotifyWindowBeingDestroyed( this );

	if( platform_handles.xcb_window != XCB_NONE )
	{
		xcb_unmap_window( platform_handles.xcb_connection, platform_handles.xcb_window );
		xcb_flush( platform_handles.xcb_connection );
		xcb_destroy_window( platform_handles.xcb_connection, platform_handles.xcb_window );
		xcb_flush( platform_handles.xcb_connection );
		platform_handles.xcb_window = XCB_NONE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::XCBWindow::GetDecorated()
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBWindow::SetDecorated(
	bool is_decorated
)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::XCBWindow::GetResizeable()
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBWindow::SetResizeable(
	bool is_decorated
)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBWindow::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerPlatformHandlesBase * bc::window_manager::XCBWindow::GetPlatformSpecificHandles() const
{
	return GetXCBPlatformHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerXCBPlatformHandles * bc::window_manager::XCBWindow::GetXCBPlatformHandles() const
{
	return &platform_handles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XCBWindow::SetupXCBPropertyHandles()
{
	// TODO: Make sure that property sizes are automatically calculated properly.

	// Window user pointer property
	xcb_property_handles.window_user_pointer	= xcb::PropertyHandle<XCBWindow*>(
		platform_handles.xcb_connection,
		platform_handles.xcb_window,
		platform_handles.window_user_pointer_atom,
		XCB_ATOM_CARDINAL,
		xcb::PropertyFormat::F32
	);

	// Window close property
	xcb_property_handles.window_protocols		= xcb::PropertyHandle<List<xcb_atom_t>>(
		platform_handles.xcb_connection,
		platform_handles.xcb_window,
		platform_handles.window_protocol_atom,
		XCB_ATOM_ATOM,
		xcb::PropertyFormat::F32
	);

	// Window title property
	xcb_property_handles.window_title			= xcb::PropertyHandle<Text>(
		platform_handles.xcb_connection,
		platform_handles.xcb_window,
		XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING,
		xcb::PropertyFormat::F8
	);

	// Window icon name property
	xcb_property_handles.window_icon_name		= xcb::PropertyHandle<Text>(
		platform_handles.xcb_connection,
		platform_handles.xcb_window,
		XCB_ATOM_WM_ICON_NAME,
		XCB_ATOM_STRING,
		xcb::PropertyFormat::F8
	);

	// Window size hints
	xcb_property_handles.window_size_hints		= xcb::PropertyHandle<xcb::XSizeHints>(
		platform_handles.xcb_connection,
		platform_handles.xcb_window,
		XCB_ATOM_WM_NORMAL_HINTS,
		XCB_ATOM_WM_SIZE_HINTS,
		xcb::PropertyFormat::F32
	);

	xcb_property_handles.window_user_pointer.Change( this );
	xcb_property_handles.window_protocols.Change(
		{
			platform_handles.window_protocol_close_atom,
			platform_handles.window_protocol_take_focus_atom,
			//platform_handles.window_protocol_ping_atom, // Does not work, might not need.
		}
	);
	xcb_property_handles.window_title.Change( "Testing..." );
	//xcb_property_handles.window_icon_name.Change( platform_handles.window_icon_name_atom );
	//xcb_property_handles.window_size_hints.Change( platform_handles.window_size_hints_atom );
}
