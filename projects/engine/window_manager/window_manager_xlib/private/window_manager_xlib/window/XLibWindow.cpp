
#include <window_manager_xlib/PreCompiledHeader.hpp>

#include <window_manager_xlib/xlib_manager/XLibManager.hpp>
#include <window_manager_xlib/window/XLibWindow.hpp>

#include <X11/Xlib.h>
#include <X11/Xatom.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XLibWindow::XLibWindow(
	XLibManager				&	xlib_manager,
	const WindowCreateInfo	&	window_create_info
) :
	::bc::window_manager::Window( window_create_info ),
	xlib_manager( xlib_manager ),
	platform_handles( *xlib_manager.GetPlatformSpecificHandles() )
{
	auto window_attributes = XSetWindowAttributes {};
	window_attributes.background_pixel = BlackPixel( platform_handles.display, platform_handles.default_screen );
	window_attributes.border_pixel = WhitePixel( platform_handles.display, platform_handles.default_screen );
	window_attributes.event_mask =
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		EnterWindowMask |
		LeaveWindowMask |
		PointerMotionMask |
		PointerMotionHintMask |
		Button1MotionMask |
		Button2MotionMask |
		Button3MotionMask |
		Button4MotionMask |
		Button5MotionMask |
		ButtonMotionMask |
		KeymapStateMask |
		ExposureMask |
		VisibilityChangeMask |
		StructureNotifyMask |
		ResizeRedirectMask |
		SubstructureNotifyMask |
		SubstructureRedirectMask |FocusChangeMask |
		PropertyChangeMask |
		ColormapChangeMask |
		OwnerGrabButtonMask;

	platform_handles.window = XCreateWindow(
		platform_handles.display,
		RootWindow( platform_handles.display, platform_handles.default_screen ),
		window_create_info.position.x,
		window_create_info.position.y,
		window_create_info.dimensions.x,
		window_create_info.dimensions.y,
		0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWBackPixel | CWBorderPixel | CWEventMask,
		&window_attributes
	);
	if( !platform_handles.window )
	{
		CleanupHandles();
		diagnostic::Throw( "Failed to create X11 window" );
	}

	XMapWindow( platform_handles.display, platform_handles.window );
	XFlush( platform_handles.display );

	SetupPropertyHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XLibWindow::~XLibWindow()
{
	xlib_manager.NotifyWindowBeingDestroyed( this );

	CleanupHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::CleanupHandles()
{
	if( platform_handles.window )
	{
		XUnmapWindow( platform_handles.display, platform_handles.window );
		XDestroyWindow( platform_handles.display, platform_handles.window );
		XFlush( platform_handles.display );
		platform_handles.window = None;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::XLibWindow::GetDecorated()
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetDecorated(
	bool is_decorated
)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::XLibWindow::GetResizeable()
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetResizeable(
	bool is_decorated
)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerPlatformHandlesBase * bc::window_manager::XLibWindow::GetPlatformSpecificHandles() const
{
	return GetXLibPlatformHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerXLibPlatformHandles * bc::window_manager::XLibWindow::GetXLibPlatformHandles() const
{
	return &platform_handles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetupPropertyHandles()
{
	// TODO: Make sure that property sizes are automatically calculated properly.

	// Window user pointer property
	x11_property_handles.window_user_pointer	= xlib::PropertyHandle<XLibWindow*>(
		platform_handles.display,
		platform_handles.window,
		platform_handles.window_user_pointer_atom,
		XA_CARDINAL,
		xlib::PropertyFormat::F8
	);

	// Window close property
	x11_property_handles.window_protocols		= xlib::PropertyHandle<List<::Atom>>(
		platform_handles.display,
		platform_handles.window,
		platform_handles.window_protocol_atom,
		XA_ATOM,
		xlib::PropertyFormat::F32
	);

	// Window title property
	x11_property_handles.window_title			= xlib::PropertyHandle<Text>(
		platform_handles.display,
		platform_handles.window,
		XA_WM_NAME,
		XA_STRING,
		xlib::PropertyFormat::F8
	);

	// Window icon name property
	x11_property_handles.window_icon_name		= xlib::PropertyHandle<Text>(
		platform_handles.display,
		platform_handles.window,
		XA_WM_ICON_NAME,
		XA_STRING,
		xlib::PropertyFormat::F8
	);

	// Window size hints
	x11_property_handles.window_size_hints		= xlib::PropertyHandle<xlib::XSizeHints>(
		platform_handles.display,
		platform_handles.window,
		XA_WM_NORMAL_HINTS,
		XA_WM_SIZE_HINTS,
		xlib::PropertyFormat::F32
	);

	x11_property_handles.window_user_pointer.Change( this );
	x11_property_handles.window_protocols.Change(
		{
			platform_handles.window_protocol_close_atom,
			platform_handles.window_protocol_take_focus_atom,
			//platform_handles.window_protocol_ping_atom, // Does not work, might not need.
		}
	);
	x11_property_handles.window_title.Change( "Testing..." );
	//x11_property_handles.window_icon_name.Change( platform_handles.window_icon_name_atom );
	//x11_property_handles.window_size_hints.Change( platform_handles.window_size_hints_atom );
}
