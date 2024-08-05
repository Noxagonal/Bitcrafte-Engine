
#include <window_manager/PreCompiledHeader.hpp>
#include <window_manager/WindowManagerComponent.hpp>
#include <window_manager/window/Window.hpp>



// Include window components.
#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <window_manager_win32/WindowManagerWin32Component.hpp>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XLIB
#include <window_manager_xlib/WindowManagerXLibComponent.hpp>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XCB
#include <window_manager_xcb/WindowManagerXCBComponent.hpp>
#endif

#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
#include <window_manager_wayland/WindowManagerWaylandComponent.hpp>
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerComponent::WindowManagerComponent(
	const WindowManagerComponentCreateInfo & create_info
)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerComponent::~WindowManagerComponent()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WindowManagerComponent::Run()
{
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::WindowManagerComponent> bc::window_manager::CreateWindowManagerComponent(
	const WindowManagerComponentCreateInfo & create_info
)
{
	#if BITCRAFTE_WINDOW_MANAGER_WIN32
	return MakeUniquePtr<window_manager::WindowManagerWin32Component>( create_info );

	#elif BITCRAFTE_WINDOW_MANAGER_XLIB && BITCRAFTE_WINDOW_MANAGER_WAYLAND
	// TODO: Detect which window manager to use based on what the user is currently using. For now, we'll just use Xlib.
	return MakeUniquePtr<window_manager::WindowManagerXLibComponent>( create_info );

	#elif BITCRAFTE_WINDOW_MANAGER_XLIB
	return MakeUniquePtr<window_manager::WindowManagerXLibComponent>( create_info );

	#elif BITCRAFTE_WINDOW_MANAGER_XCB
	return MakeUniquePtr<window_manager::WindowManagerXCBComponent>( create_info );

	#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
	return MakeUniquePtr<window_manager::WindowManagerWaylandComponent>( create_info );

	#else
	#error "No window manager available"
	#endif // Window manager selection
}
