#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window_manager/WindowManagerComponentCreateInfo.hpp>
#include <window_manager/event/WindowManagerComponentEvents.hpp>
#include <window_manager/window/Window.hpp>

#include <core/containers/UniquePtr.hpp>


namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API WindowManagerComponent
{
	friend class Window;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowManagerComponent(
		const WindowManagerComponentCreateInfo				&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~WindowManagerComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void												Run() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual UniquePtr<Window>									CreateWindow(
		const WindowCreateInfo								&	window_create_info
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const void 										*	GetPlatformSpecificHandles() const = 0;

	WindowManagerComponentEvents								events;

private:
};



} // window
} // bc
