
#include <window_manager_xcb/PreCompiledHeader.hpp>
#include <window_manager_xcb/WindowManagerXCBComponent.hpp>
#include <window_manager_xcb/xcb_manager/XCBManager.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerXCBComponent::WindowManagerXCBComponent(
	const WindowManagerComponentCreateInfo & create_info
) :
	WindowManagerComponent( create_info )
{
	xcb_manager = MakeUniquePtr<XCBManager>( *this, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerXCBComponent::~WindowManagerXCBComponent()
{
	xcb_manager = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WindowManagerXCBComponent::Run()
{
	xcb_manager->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::WindowManagerXCBComponent::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return xcb_manager->CreateWindow( window_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::window_manager::WindowManagerPlatformHandlesBase * bc::window_manager::WindowManagerXCBComponent::GetPlatformSpecificHandles() const
{
	return xcb_manager->GetPlatformSpecificHandles();
}
