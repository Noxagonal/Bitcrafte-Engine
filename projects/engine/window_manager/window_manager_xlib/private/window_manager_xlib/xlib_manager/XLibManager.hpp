#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager_xlib/window/XLibWindow.hpp>

#include <core/containers/Optional.hpp>
#include <core/containers/List.hpp>
#include <core/containers/UniquePtr.hpp>



namespace bc {
namespace window_manager {



struct WindowManagerComponentCreateInfo;
class WindowManagerXLibComponent;
class XLibManager;
class XLibWindow;

struct WindowCreateInfo;
class Window;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class XLibManager
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	XLibManager(
		WindowManagerXLibComponent						&	window_manager_xlib_component,
		const WindowManagerComponentCreateInfo			&	window_manager_component_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~XLibManager();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													CleanupHandles();

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<Window>										CreateWindow(
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													NotifyWindowBeingDestroyed(
		XLibWindow										*	window_ptr
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const WindowManagerXLibPlatformHandles		*	GetPlatformSpecificHandles() const { return &platform_handles; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													ProcessMessages();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													ProcessEvent(
	XEvent												&	event
	);

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO: Change this to "Result<type>", result should be a type that holds an optional return value, and an error message.
	// For now, the text field is only used for error messages.
	Optional<Text32>										PopulateX11Atoms();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowManagerXLibComponent							&	window_manager_xlib_component;

	List<XLibWindow*>										active_window_list;

	WindowManagerXLibPlatformHandles						platform_handles = {};
};



} // window_manager
} // bc
