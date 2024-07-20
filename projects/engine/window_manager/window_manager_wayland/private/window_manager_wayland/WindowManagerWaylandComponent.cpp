
#include <window_manager_wayland/PreCompiledHeader.hpp>
#include <window_manager_wayland/WindowManagerWaylandComponent.hpp>
#include <window_manager_wayland/wayland_manager/WaylandManager.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerWaylandComponent::WindowManagerWaylandComponent(
	const WindowManagerComponentCreateInfo & create_info
) :
	WindowManagerComponent( create_info )
{
	wayland_manager = MakeUniquePtr<WaylandManager>( *this, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerWaylandComponent::~WindowManagerWaylandComponent()
{
	wayland_manager = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WindowManagerWaylandComponent::Run()
{
	wayland_manager->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::WindowManagerWaylandComponent::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return wayland_manager->CreateWindow( window_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerPlatformHandlesBase * bc::window_manager::WindowManagerWaylandComponent::GetPlatformSpecificHandles() const
{
	return wayland_manager->GetPlatformSpecificHandles();
}
