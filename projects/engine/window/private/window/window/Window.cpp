
#include <window/PreCompiledHeader.hpp>
#include <window/window/Window.hpp>

#include <window/WindowComponent.hpp>



namespace bc {
namespace window {



window::Window::Window(
	WindowComponent				&	window_component,
	const WindowCreateInfo		&	window_create_info
) :
	window_component( window_component ),
	window_create_info_copy( window_create_info )
{}

Window::~Window()
{
	window_component.NotifyWindowDestroyed( this );
}



} // window
} // bc
