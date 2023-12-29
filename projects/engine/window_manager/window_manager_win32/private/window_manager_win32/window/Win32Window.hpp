#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window_manager/window/Window.hpp>

#include <Windows.h>
// Prevent polluting CreateWindow all over the place
#undef CreateWindow

#include <window_manager/platform/handles/WindowPlatformHandles.hpp>




namespace bc {
namespace window_manager {



class Win32Manager;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Win32Window : public Window
{
public:

	friend LRESULT CALLBACK WndProc(
		HWND		window_handle,
		UINT		message,
		WPARAM		w_param,
		LPARAM		l_param
	);

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DWORD												MakeWindowStyle() const;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct RuntimeInfo
	{
		bool												is_fullscreen				= false;
		bool												is_mouse_over_window		= false;
		WindowState											current_windows_state		= WindowState::WINDOWED;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32Window(
		Win32Manager									&	win32_manager,
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~Win32Window() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool											GetDecorated() override;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											SetDecorated(
		bool												is_decorated
	) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool											GetResizeable() override;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void											SetResizeable(
		bool												is_decorated
	) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													Update();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const RuntimeInfo							&	GetRuntimeInfo() { return runtime_info; }
	inline const Settings								&	GetSettings() { return settings; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void										*	GetPlatformSpecificHandles();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32Manager										&	win32_manager;

	RuntimeInfo												runtime_info;
	Settings												settings;

	WindowManagerWin32PlatformHandles						platform_handles			= {};

	DWORD													style						= {};
};



} // window_manager
} // bc