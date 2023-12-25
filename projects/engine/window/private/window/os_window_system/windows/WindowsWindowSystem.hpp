#pragma once

#include <window/os_window_system/OSWindowSystem.hpp>

#if BITCRAFTE_PLATFORM_WINDOWS

#include <Windows.h>

// Prevent polluting CreateWindow all over the place
#undef CreateWindow



namespace bc {
namespace window {



class Window;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WindowsWindowSystem : public OSWindowSystem
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowsWindowSystem(
		WindowComponent											&	window_component,
		const WindowComponentCreateInfo							&	window_component_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~WindowsWindowSystem() override;
	virtual	void													Run() override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const WNDCLASSW												&	GetWindowClass();

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual UniquePtr<Window>										DoCreateWindow(
		const WindowCreateInfo									&	window_create_info
	) override;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															ProcessMessages();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Text16															window_class_name			= "BitCrafteEngine";
	WNDCLASSW														window_class				= {};
};



} // window
} // bc



#endif // BITCRAFTE_PLATFORM_WINDOWS