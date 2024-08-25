
#include <window_manager/PreCompiledHeader.hpp>
#include <window_manager/window/Window.hpp>



namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Window::Window( const WindowCreateInfo& window_create_info )
:
	settings( CreateSettingsFromWindowCreateInfo( window_create_info ) )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Window::~Window()
{
}

auto bc::window_manager::Window::CreateSettingsFromWindowCreateInfo( const WindowCreateInfo& window_create_info ) -> Settings
{
	static_assert( sizeof( WindowCreateInfo ) == 24, "If this is not true, this function needs to be updated" );

	Settings settings;
	settings.is_decorated					= window_create_info.decorated;
	settings.is_visible						= window_create_info.initially_visible;
	settings.show_minimize_button			= window_create_info.has_minimize_button;
	settings.show_maximize_button			= window_create_info.has_maximize_button;
	settings.is_minimized					= window_create_info.initially_minimized;
	settings.is_maximized					= window_create_info.initially_maximized;
	settings.allow_drag_resize				= window_create_info.allow_drag_resize;
	settings.allow_file_drop				= window_create_info.allow_file_drop;
	return settings;
}



} // window
} // bc
