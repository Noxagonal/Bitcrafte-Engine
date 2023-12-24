
#include <window/PreCompiledHeader.hpp>
#include <window/WindowComponent.hpp>
#include <window/os_window_system/OSWindowSystem.hpp>

#include <window/window/Window.hpp>

#if BITCRAFTE_PLATFORM_WINDOWS
#include <window/os_window_system/windows/WindowsWindowSystem.hpp>
#elif BITCRAFTE_PLATFORM_LINUX
#include <window/os_window_system/wayland/WaylandWindowSystem.hpp>
#else
#error "Please add platform support here"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowComponent::WindowComponent(
	const WindowComponentCreateInfo & create_info
) : create_info_copy( create_info )
{
	#if BITCRAFTE_PLATFORM_WINDOWS
	os_window_system = MakeUniquePtr<WindowsWindowSystem>( *this, create_info_copy );
	#elif BITCRAFTE_PLATFORM_LINUX
	os_window_system = MakeUniquePtr<WaylandWindowSystem>( *this, create_info_copy );
	#else
	#error "Please add platform support here"
	#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowComponent::~WindowComponent()
{
	os_window_system = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowComponent::Run()
{
	os_window_system->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window::Window> bc::window::WindowComponent::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return os_window_system->CreateWindow( window_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowComponent::NotifyWindowDestroyed(
	Window * window
)
{
	os_window_system->NotifyWindowDestroyed( window );
}
