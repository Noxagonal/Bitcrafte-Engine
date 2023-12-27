
#include <window_manager_win32/PreCompiledHeader.hpp>
#include <window_manager_win32/WindowManagerWin32Component.hpp>
#include <window_manager_win32/win32_manager/Win32Manager.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerWin32Component::WindowManagerWin32Component(
	const WindowManagerComponentCreateInfo & create_info
) :
	WindowManagerComponent( create_info )
{
	win32_manager = MakeUniquePtr<Win32Manager>( *this, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerWin32Component::~WindowManagerWin32Component()
{
	win32_manager = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WindowManagerWin32Component::Run()
{
	win32_manager->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::Window> bc::window_manager::WindowManagerWin32Component::CreateWindow(
	const WindowCreateInfo & window_create_info
)
{
	return win32_manager->CreateWindow( window_create_info );
	return {};
}
