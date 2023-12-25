#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window/window/Window.hpp>

#if BITCRAFTE_PLATFORM_WINDOWS

#include <Windows.h>

// Prevent polluting CreateWindow all over the place
#undef CreateWindow




namespace bc {
namespace window {



class WindowsWindowSystem;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WindowsWindow : public Window
{
	struct Settings
	{
		bool is_decorated									= false;
		bool is_visible										= false;
		bool show_minimize_button							= false;
		bool show_maximize_button							= false;
		bool is_minimized									= false;
		bool is_maximized									= false;
		bool allow_drag_resize								= false;
		bool allow_file_drop								= false;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DWORD												MakeWindowStyle() const;
	};

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowsWindow(
		WindowComponent									&	window_component,
		WindowsWindowSystem								&	windows_window_system,
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~WindowsWindow() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool											GetDecorated();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											SetDecorated(
		bool												is_decorated
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool											GetResizeable();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											SetResizeable(
		bool												is_decorated
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											Update() override;

	bool													is_fullscreen				= false;
	bool													is_mouse_over_window		= false;
	WindowState												current_windows_state		= WindowState::WINDOWED;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Settings											MakeSettingsFromCreateInfo(
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowsWindowSystem									&	windows_window_system;

	HWND													window_handle				= NULL;
	DWORD													style						= {};

	Settings												settings					= {};
};



} // window
} // bc



#endif // BITCRAFTE_PLATFORM_WINDOWS