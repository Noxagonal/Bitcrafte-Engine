
#include <window_manager/PreCompiledHeader.hpp>
#include <window_manager/window/Window.hpp>



namespace bc {
namespace window_manager {



bc::window_manager::Window::Settings::Settings(
	const bc::window_manager::WindowCreateInfo & window_create_info
)
{
	is_decorated			= window_create_info.decorated;
	show_minimize_button	= window_create_info.has_minimize_button;
	show_maximize_button	= window_create_info.has_maximize_button;
	is_minimized			= window_create_info.initially_minimized;
	is_maximized			= window_create_info.initially_maximized;
	allow_drag_resize		= window_create_info.allow_drag_resize;
	allow_file_drop			= window_create_info.allow_file_drop;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Window::Window(
	const WindowCreateInfo		&	window_create_info
) :
	settings( window_create_info )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Window::~Window()
{
}



} // window
} // bc
