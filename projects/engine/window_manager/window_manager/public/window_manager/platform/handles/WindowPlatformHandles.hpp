#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstdint>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/Windows.hpp>
#endif


namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class WindowManagerPlatformHandlesStructureType : u32
{
	WINDOW_MANAGER_NONE				= 0,
	WINDOW_MANAGER_WIN32,
	WINDOW_MANAGER_WAYLAND,
	// Add platform structure types here.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerPlatformHandlesBase
{
	inline WindowManagerPlatformHandlesBase(
		WindowManagerPlatformHandlesStructureType	structure_type
	) :
		structure_type( structure_type )
	{}
	WindowManagerPlatformHandlesStructureType		structure_type;
};
static_assert( sizeof( WindowManagerPlatformHandlesStructureType ) == 4 );



#if BITCRAFTE_WINDOW_MANAGER_WIN32
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerWin32PlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerWin32PlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32 ) {}
	HINSTANCE										hInstance				= NULL;
	HWND											hWnd					= NULL;
};
#endif

#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerWaylandPlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerWaylandPlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WAYLAND ) {}
	TODO;
};
#endif



} // window_manager
} // bc
