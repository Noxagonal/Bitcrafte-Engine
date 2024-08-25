
#include <window_manager_wayland/PreCompiledHeader.hpp>

#include <window_manager_wayland/wayland_manager/WaylandManager.hpp>
#include <window_manager_wayland/window/WaylandWindow.hpp>

#include <wayland-client-protocol.h>
#include <xdg-shell-client-protocol.h>
#include "WaylandWindow.hpp"



namespace bc {
namespace window_manager {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void xdg_surface_configure(
	void*			data,
	xdg_surface*	xdg_surface,
	uint32_t		serial
)
{
	xdg_surface_ack_configure( xdg_surface, serial );
}

static const struct xdg_surface_listener xdg_surface_listener
{
	xdg_surface_configure
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void xdg_toplevel_configure(
	void*			data,
	xdg_toplevel*	xdg_toplevel,
	int32_t			width,
	int32_t			height,
	wl_array*		states
)
{
	// Handle toplevel configuration (resize, maximize, etc.)
	WaylandWindow * window = static_cast<WaylandWindow*>( data );
}

static void xdg_toplevel_close(
	void*			data,
	xdg_toplevel*	xdg_toplevel
)
{
	// Handle window close event
	WaylandWindow *window = static_cast<WaylandWindow *>(data);

	// Notify the WaylandManager or perform some cleanup if necessary.
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
	xdg_toplevel_configure,
	xdg_toplevel_close,
	// TODO: Add rest of xdg_toplevel_listener
};

} // internal_
} // window_manager
} // bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandWindow::WaylandWindow(
	WaylandManager&				wayland_manager,
	const WindowCreateInfo&		window_create_info
)
:
	Window( window_create_info ),
	wayland_manager( wayland_manager ),
	platform_handles( *wayland_manager.GetPlatformSpecificHandles() )
{
	CreateWindowSurface();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandWindow::~WaylandWindow()
{
	wayland_manager.NotifyWindowBeingDestroyed( this );

	DestroyWindowSurface();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::WaylandWindow::GetDecorated() const -> bool
{
	// TODO: Get window decorations

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::SetDecorated( bool is_decorated )
{
	// TODO: Set window decorations

//	if( is_decorated )
//	{
//		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
//	}
//	else
//	{
//		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
//	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::WaylandWindow::GetResizeable() const -> bool
{
	// TODO: Get window resizeable
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::SetResizeable( bool is_resizeable )
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::WaylandWindow::GetPlatformSpecificHandles() const -> const WindowManagerPlatformHandlesBase*
{
	return &platform_handles;
}

void bc::window_manager::WaylandWindow::CreateWindowSurface()
{
	platform_handles.surface = wl_compositor_create_surface( platform_handles.compositor );
	if( !platform_handles.surface )
	{
		DestroyWindowSurface();
		BAssert( 0, "Failed to create Wayland surface" );
		return;
	}

	platform_handles.surface_xdg = xdg_wm_base_get_xdg_surface( platform_handles.wm_base_xdg, platform_handles.surface );
	if( !platform_handles.surface_xdg )
	{
		DestroyWindowSurface();
		BAssert( 0, "Failed to create Wayland xdg surface" );
		return;
	}

	platform_handles.toplevel_xdg = xdg_surface_get_toplevel( platform_handles.surface_xdg );
	if( !platform_handles.toplevel_xdg )
	{
		DestroyWindowSurface();
		BAssert( 0, "Failed to create Wayland toplevel" );
		return;
	}

	xdg_surface_add_listener( platform_handles.surface_xdg, &internal_::xdg_surface_listener, this );
	xdg_toplevel_add_listener( platform_handles.toplevel_xdg, &internal_::xdg_toplevel_listener, this );

	wl_surface_commit( platform_handles.surface );
}

void bc::window_manager::WaylandWindow::DestroyWindowSurface()
{
	if( platform_handles.toplevel_xdg )	xdg_toplevel_destroy( platform_handles.toplevel_xdg );
	platform_handles.toplevel_xdg		= nullptr;

	if( platform_handles.surface_xdg )	xdg_surface_destroy( platform_handles.surface_xdg );
	platform_handles.surface_xdg		= nullptr;

	if( platform_handles.surface )		wl_surface_destroy( platform_handles.surface );
	platform_handles.surface			= nullptr;
}
