#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window_manager/window/Window.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>



namespace bc {
namespace window_manager {



class XLibManager;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class XLibWindow : public Window
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	XLibWindow(
		XLibManager										&	xlib_manager,
		const WindowCreateInfo							&	window_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~XLibWindow() override;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													CleanupHandles();

public:

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
	virtual const WindowManagerPlatformHandlesBase		*	GetPlatformSpecificHandles() const override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const WindowManagerXLibPlatformHandles				*	GetXLibPlatformHandles() const;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													SetupProperties();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	XLibManager											&	xlib_manager;

	WindowManagerXLibPlatformHandles						platform_handles			= {};
};



} // window_manager
} // bc