
#include <window/PreCompiledHeader.hpp>
#include <window/os_window_system/OSWindowSystem.hpp>
#include <window/window/Window.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::OSWindowSystem::OSWindowSystem(
	WindowComponent							&	window_component,
	const WindowComponentCreateInfo			&	window_component_create_info
) :
	window_component( window_component ),
	window_component_create_info_copy( window_component_create_info )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::OSWindowSystem::~OSWindowSystem()
{
	BHardAssert( active_window_list.IsEmpty(), "Error while shutting down OS window system, there are still active windows" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::OSWindowSystem::Run()
{
	for( auto window : active_window_list )
	{
		window->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window::Window> bc::window::OSWindowSystem::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	auto window = DoCreateWindow( window_create_info );
	active_window_list.PushBack( window.Get() );
	return window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::OSWindowSystem::NotifyWindowDestroyed(
	bc::window::Window * window_ptr
)
{
	active_window_list.Erase( window_ptr );
}
