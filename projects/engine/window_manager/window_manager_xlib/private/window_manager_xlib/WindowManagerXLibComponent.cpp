
#include <window_manager_xlib/PreCompiledHeader.hpp>
#include <window_manager_xlib/WindowManagerXLibComponent.hpp>
#include <window_manager_xlib/xlib_manager/XLibManager.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerXLibComponent::WindowManagerXLibComponent(
	const WindowManagerComponentCreateInfo & create_info
) :
	WindowManagerComponent( create_info )
{
	xlib_manager = MakeUniquePtr<XLibManager>( *this, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerXLibComponent::~WindowManagerXLibComponent()
{
	xlib_manager = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WindowManagerXLibComponent::Run()
{
	xlib_manager->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::WindowManagerXLibComponent::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return xlib_manager->CreateWindow( window_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerPlatformHandlesBase * bc::window_manager::WindowManagerXLibComponent::GetPlatformSpecificHandles() const
{
	return xlib_manager->GetPlatformSpecificHandles();
}
