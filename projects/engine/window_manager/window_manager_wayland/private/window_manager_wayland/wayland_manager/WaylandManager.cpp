
#include <window_manager_wayland/PreCompiledHeader.hpp>
#include <window_manager_wayland/wayland_manager/WaylandManager.hpp>

#include <window_manager/window/Window.hpp>
#include <window_manager_wayland/window/WaylandWindow.hpp>
#include <window_manager_wayland/WindowManagerWaylandComponent.hpp>

#include <poll.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

// TEMP!!!
#include <iostream>



namespace bc {
namespace window_manager {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registry listener callbacks to bind global objects
static void registry_handler(
	void					*	data,
	wl_registry				*	registry,
	uint32_t					id,
	const char				*	interface,
	uint32_t					version
)
{
	WaylandManager* wayland_manager = static_cast<WaylandManager*>( data );
	std::cout << "Interface: " << interface << ", Version: " << version << std::endl;

	if( strcmp( interface, wl_compositor_interface.name ) == 0 )
	{
		wayland_manager->platform_handles.compositor = static_cast<wl_compositor*>( wl_registry_bind( registry, id, &wl_compositor_interface, 1 ) );
	}
	else if( strcmp(interface, xdg_wm_base_interface.name ) == 0 )
	{
		wayland_manager->platform_handles.wm_base_xdg = static_cast<xdg_wm_base*>(wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void registry_remover(
	void					*	data,
	wl_registry				*	registry,
	uint32_t					id
)
{
	WaylandManager* wayland_manager = static_cast<WaylandManager*>( data );
	// Handle removal of global objects if necessary
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const struct wl_registry_listener registry_listener = {
	registry_handler,
	registry_remover
};



} // internal_
} // window_manager
} // bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandManager::WaylandManager(
	WindowManagerWaylandComponent			&	window_manager_wayland_component,
	const WindowManagerComponentCreateInfo	&	window_manager_component_create_info
) :
	window_manager_wayland_component( window_manager_wayland_component )
{
	platform_handles.display = wl_display_connect( nullptr );
	BEnsure( platform_handles.display, "Failed to connect to Wayland display" );

	auto registry = wl_display_get_registry( platform_handles.display );
	wl_registry_add_listener( registry, &internal_::registry_listener, this );
	wl_display_roundtrip( platform_handles.display );

	BEnsure( platform_handles.compositor, "Failed to get Wayland compositor" );
	BEnsure( platform_handles.wm_base_xdg, "Failed to get Wayland shell" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandManager::~WaylandManager()
{
	BHardAssert( active_window_list.IsEmpty(), "Error while shutting down OS window system, there are still active windows" );

	if( platform_handles.wm_base_xdg )	xdg_wm_base_destroy( platform_handles.wm_base_xdg );
	platform_handles.wm_base_xdg		= nullptr;

	if( platform_handles.compositor )	wl_compositor_destroy( platform_handles.compositor );
	platform_handles.compositor			= nullptr;

	if( platform_handles.display )		wl_display_disconnect( platform_handles.display );
	platform_handles.display			= nullptr;

	platform_handles					= {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandManager::Run()
{
	ProcessMessages();

	for( auto window : active_window_list )
	{
		window->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::WaylandManager::CreateWindow(
	const WindowCreateInfo  & window_create_info
)
{
	auto new_window = MakeUniquePtr<WaylandWindow>( *this, window_create_info );
	active_window_list.PushBack( new_window.Get() );
	ProcessMessages();
	window_manager_wayland_component.events.OnWindowCreated.Signal( new_window.Get() );
	return new_window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandManager::NotifyWindowBeingDestroyed(
	bc::window_manager::WaylandWindow * window_ptr
)
{
	window_manager_wayland_component.events.OnWindowBeingDestroyed.Signal( window_ptr );
	active_window_list.Erase( window_ptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandManager::ProcessMessages()
{
	struct pollfd fd = { wl_display_get_fd(platform_handles.display), POLLIN };

	while( wl_display_prepare_read( platform_handles.display ) != 0)
	{
		if( wl_display_dispatch_pending( platform_handles.display ) > 0 )
		{
			return;
		}
	}

	if(!FlushDisplay())
	{
		wl_display_cancel_read( platform_handles.display );
		BEnsure( false, "Failed to flush Wayland display" );
		return;
	}

	int ret = poll( &fd, 1, 0 ); // Poll with zero timeout to return immediately
	if( ret > 0 ) {
		if( fd.revents & POLLIN )
		{
			wl_display_read_events( platform_handles.display );
			wl_display_dispatch_pending( platform_handles.display );
		}
		else
		{
			wl_display_cancel_read( platform_handles.display );
		}
	}
	else
	{
		wl_display_cancel_read( platform_handles.display );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::WaylandManager::FlushDisplay()
{
	while( wl_display_flush( platform_handles.display ) == -1 )
	{
		if( errno != EAGAIN ) return false;

		struct pollfd fd = { wl_display_get_fd( platform_handles.display ), POLLOUT };

		while( poll( &fd, 1, -1 ) == -1 )
		{
			if( errno != EINTR && errno != EAGAIN ) return false;
		}
	}

	return true;
}
