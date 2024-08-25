
#include <window_manager_xlib/PreCompiledHeader.hpp>

#include <window_manager_xlib/xlib_manager/XLibManager.hpp>
#include <window_manager_xlib/window/XLibWindow.hpp>

#include <X11/Xlib.h>
#include <X11/Xatom.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::XLibWindow::XLibWindow(
	XLibManager&				xlib_manager,
	const WindowCreateInfo&		window_create_info
)
:
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
		platform_handles.default_visual,
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

	SetupProperties();
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
auto bc::window_manager::XLibWindow::GetDecorated() const -> bool
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetDecorated( bool is_decorated )
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::XLibWindow::GetResizeable() const -> bool
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetResizeable( bool is_resizeable )
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::XLibWindow::GetPlatformSpecificHandles() const -> const WindowManagerPlatformHandlesBase*
{
	return GetXLibPlatformHandles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::XLibWindow::GetXLibPlatformHandles() const -> const WindowManagerXLibPlatformHandles*
{
	return &platform_handles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::XLibWindow::SetupProperties()
{
	// Set property handles
	auto user_pointer_value = reinterpret_cast<u64>( this );
	auto user_pointer_value_array = Array<unsigned long, 2>( user_pointer_value & 0xFFFFFFFF, user_pointer_value >> 32 );
	XChangeProperty(
		platform_handles.display,
		platform_handles.window,
		platform_handles.window_user_pointer_atom,
		XA_CARDINAL,
		32,
		PropModeReplace,
		reinterpret_cast<const unsigned char*>( user_pointer_value_array.Data() ),
		static_cast<int>( user_pointer_value_array.Size() )
	);

	// Set window protocols
	auto window_protocols = List<::Atom> {
		platform_handles.window_protocol_close_atom,
		platform_handles.window_protocol_take_focus_atom,
		platform_handles.window_protocol_ping_atom
	};
	XChangeProperty(
		platform_handles.display,
		platform_handles.window,
		platform_handles.window_protocol_atom,
		XA_ATOM,
		32,
		PropModeReplace,
		reinterpret_cast<const unsigned char*>( window_protocols.Data() ),
		static_cast<int>( window_protocols.Size() )
	);
}
