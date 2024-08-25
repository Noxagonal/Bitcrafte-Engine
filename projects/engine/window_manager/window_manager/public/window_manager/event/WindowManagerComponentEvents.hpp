#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/event/Event.hpp>


namespace bc {
namespace window_manager {



class Window;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerComponentEvents
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Event that fires when window has just been created.
	Event<Window*> OnWindowCreated;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Event that fires just before window is going to be destroyed.
	Event<Window*> OnWindowBeingDestroyed;
};



} // window
} // bc
