#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager/window/WindowCreateInfo.hpp>
#include <window_manager/event/WindowEvents.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>



namespace bc {
namespace window_manager {



class WindowManagerComponent;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API Window
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct Settings
	{
		Settings(
			const WindowCreateInfo						&	window_create_info
		);

		bool is_decorated									= false;
		bool is_visible										= false;
		bool show_minimize_button							= false;
		bool show_maximize_button							= false;
		bool is_minimized									= false;
		bool is_maximized									= false;
		bool allow_drag_resize								= false;
		bool allow_file_drop								= false;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct RuntimeInfo
	{
		bool												is_fullscreen				= false;
		bool												is_mouse_over_window		= false;
		WindowState											current_windows_state		= WindowState::WINDOWED;
	};

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Window(
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~Window();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowEvents											events;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual inline bool										GetDecorated() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual inline void										SetDecorated(
		bool												is_decorated
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual inline bool										GetResizeable() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											SetResizeable(
		bool												is_resizeable
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const Settings								&	GetSettings() const { return settings; }
	inline const RuntimeInfo							&	GetRuntimeInfo() const { return runtime_info; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const WindowManagerPlatformHandlesBase		*	GetPlatformSpecificHandles() const = 0;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Settings												settings;
	RuntimeInfo												runtime_info;

};



} // window
} // bc
