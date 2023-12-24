#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/UniquePtr.hpp>

#include <window/WindowComponentCreateInfo.hpp>
#include <window/window/WindowCreateInfo.hpp>



namespace bc {
namespace window {



class OSWindowSystem;
class Window;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API WindowComponent
{
	friend class Window;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowComponent(
		const WindowComponentCreateInfo						&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~WindowComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void														Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<Window>											CreateWindow(
		const WindowCreateInfo								&	window_create_info
	);

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void														NotifyWindowDestroyed(
		Window												*	window
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowComponentCreateInfo									create_info_copy;

	UniquePtr<OSWindowSystem>									os_window_system;
};



} // window
} // bc
