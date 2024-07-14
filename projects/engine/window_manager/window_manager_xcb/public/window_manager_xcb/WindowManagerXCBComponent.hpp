#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/UniquePtr.hpp>

#include <window_manager/WindowManagerComponent.hpp>



namespace bc {
namespace window_manager {



class XCBManager;
class Window;
struct WindowCreateInfo;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WindowManagerXCBComponent : public WindowManagerComponent
{
	friend class Window;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowManagerXCBComponent(
		const WindowManagerComponentCreateInfo				&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~WindowManagerXCBComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void												Run() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual UniquePtr<Window>									CreateWindow(
		const WindowCreateInfo								&	window_create_info
	) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const WindowManagerPlatformHandlesBase			*	GetPlatformSpecificHandles() const override;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<XCBManager>										xcb_manager;
};



} // window
} // bc
