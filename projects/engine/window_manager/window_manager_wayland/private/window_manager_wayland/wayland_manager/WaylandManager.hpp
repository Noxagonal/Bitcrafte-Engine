#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager_wayland/window/WaylandWindow.hpp>

#include <core/containers/List.hpp>
#include <core/containers/UniquePtr.hpp>



struct wl_registry;



namespace bc {
namespace window_manager {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void registry_handler(
	void					*	data,
	wl_registry				*	registry,
	uint32_t					id,
	const char				*	interface,
	uint32_t					version
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void registry_remover(
	void					*	data,
	wl_registry				*	registry,
	uint32_t					id
);

} // internal_



struct WindowManagerComponentCreateInfo;
class WindowManagerWaylandComponent;
class WaylandWindow;

struct WindowCreateInfo;
class Window;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WaylandManager
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	friend void ::bc::window_manager::internal_::registry_handler(
		void					*	data,
		wl_registry				*	registry,
		uint32_t					id,
		const char				*	interface,
		uint32_t					version
	);

	friend void ::bc::window_manager::internal_::registry_remover(
		void					*	data,
		wl_registry				*	registry,
		uint32_t					id
	);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WaylandManager(
		WindowManagerWaylandComponent							&	window_manager_wayland_component,
		const WindowManagerComponentCreateInfo					&	window_manager_component_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~WaylandManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<Window>												CreateWindow(
		const WindowCreateInfo									&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															NotifyWindowBeingDestroyed(
		WaylandWindow											*	window_ptr
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const WindowManagerWaylandPlatformHandles					*	GetPlatformSpecificHandles() const;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															ProcessMessages();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool															FlushDisplay();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowManagerWaylandComponent								&	window_manager_wayland_component;

	List<WaylandWindow*>											active_window_list;

	WindowManagerWaylandPlatformHandles								platform_handles;
};



} // window_manager
} // bc
