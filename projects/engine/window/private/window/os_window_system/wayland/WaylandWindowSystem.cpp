
#include <window/PreCompiledHeader.hpp>
#include <window/os_window_system/wayland/WaylandWindowSystem.hpp>



#if BITCRAFTE_PLATFORM_LINUX



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WaylandWindowSystem::WaylandWindowSystem(
	const WindowComponentCreateInfo & window_component_create_info
) : OSWindowSystem( window_component_create_info )
{}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WaylandWindowSystem::~WaylandWindowSystem()
{}



#endif // BITCRAFTE_PLATFORM_LINUX
