#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/Vector.hpp>



namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowCreateInfo
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Position of the window on the screen.
	math::Vec2i32 position = { 0, 0 };

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Size of the window.
	math::Vec2i32 dimensions = { 640, 480 };

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should be window be decorated, ie. Have border, close, minimize, maximize buttons and visible icon.
	bool decorated = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should the window have minimize button.
	///
	/// @note
	/// This button will only be visible on decorated windows.
	bool has_minimize_button = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should the window have maximize button.
	///
	/// @note
	/// This button will only be visible on decorated windows.
	bool has_maximize_button = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should the window be initially minimized to the task bar.
	bool initially_minimized = false;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should the window be initially maximized to fill the screen.
	bool initially_maximized = false;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Allow resizing the window by dragging the edges of it.
	bool allow_drag_resize = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Should the window be initially visible.
	bool initially_visible = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Allow files to be dropped on a window.
	bool allow_file_drop = false;
};



} // window
} // bc
