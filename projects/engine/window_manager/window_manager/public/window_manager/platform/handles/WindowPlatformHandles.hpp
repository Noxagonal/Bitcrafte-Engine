#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstdint>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/Windows.hpp>
#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
struct wl_display;
struct wl_surface;
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
static_assert( sizeof( WindowManagerPlatformHandlesStructureType ) == 4 );

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
static_assert( sizeof( WindowManagerPlatformHandlesBase ) == 4 );
static_assert( offsetof( WindowManagerPlatformHandlesBase, structure_type ) == 0 );



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
	wl_display									*	display					= nullptr;
	wl_surface									*	surface					= nullptr;
};
#endif



} // window_manager
} // bc
