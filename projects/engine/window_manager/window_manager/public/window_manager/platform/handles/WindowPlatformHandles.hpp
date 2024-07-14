#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstdint>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/WinDef.hpp>
#endif
#if BITCRAFTE_WINDOW_MANAGER_XCB
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#endif
#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
struct wl_display;
struct wl_compositor;
struct xdg_wm_base;
struct wl_surface;
struct xdg_surface;
struct xdg_toplevel;
#endif


namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class WindowManagerPlatformHandlesStructureType : u32
{
	WINDOW_MANAGER_NONE				= 0,
	WINDOW_MANAGER_WIN32,
	WINDOW_MANAGER_XCB,
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
	HINSTANCE										hInstance								= NULL;
	HWND											hWnd									= NULL;
};
#endif

#if BITCRAFTE_WINDOW_MANAGER_XCB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerXCBPlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerXCBPlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XCB ) {}
	Display										*	xlib_display							= nullptr;
	xcb_connection_t							*	xcb_connection							= nullptr;
	xcb_screen_t								*	xcb_screen								= nullptr;
	xcb_window_t									xcb_window								= 0;
	xcb_key_symbols_t							*	xcb_key_symbols							= {};
	XIM												xlib_xim								= {};
	XIC												xlib_xic								= {};
	xcb_atom_t										window_user_pointer_atom				= {};
	xcb_atom_t										window_protocol_atom					= {};
	xcb_atom_t										window_protocol_close_atom				= {};
	xcb_atom_t										window_protocol_take_focus_atom			= {};
	xcb_atom_t										window_protocol_ping_atom				= {};
};
#endif

#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerWaylandPlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerWaylandPlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WAYLAND ) {}
	wl_display									*	display									= nullptr;
	wl_compositor								*	compositor								= nullptr;
	xdg_wm_base									*	wm_base_xdg								= nullptr;
	wl_surface									*	surface									= nullptr;
	xdg_surface									*	surface_xdg								= nullptr;
	xdg_toplevel								*	toplevel_xdg							= nullptr;
};
#endif



} // window_manager
} // bc
