#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstdint>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/WinDef.hpp>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XLIB
#include <X11/Xlib.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XCB
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
	WINDOW_MANAGER_XLIB,
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
	HINSTANCE										hInstance								= {};
	HWND											hWnd									= {};
};
#endif

#if BITCRAFTE_WINDOW_MANAGER_XLIB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerXLibPlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerXLibPlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XLIB ) {}
	::Display									*	display									= {};
	int32_t											default_screen							= {};
	::Visual									*	default_visual							= {};
	::VisualID										default_visual_id						= {};
	::XIM											xim										= {};
	::XIC											xic										= {};
	::Window										window									= {};
	::Atom											window_user_pointer_atom				= {};
	::Atom											window_protocol_atom					= {};
	::Atom											window_protocol_close_atom				= {};
	::Atom											window_protocol_take_focus_atom			= {};
	::Atom											window_protocol_ping_atom				= {};
};
#endif

#if BITCRAFTE_WINDOW_MANAGER_XCB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowManagerXCBPlatformHandles : public WindowManagerPlatformHandlesBase
{
	inline WindowManagerXCBPlatformHandles() : WindowManagerPlatformHandlesBase( WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XCB ) {}
	xcb_connection_t							*	connection								= {};
	xcb_screen_t								*	screen									= {};
	xcb_window_t									window									= {};
	xcb_key_symbols_t							*	key_symbols								= {};
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
	wl_display									*	display									= {};
	wl_compositor								*	compositor								= {};
	xdg_wm_base									*	wm_base_xdg								= {};
	wl_surface									*	surface									= {};
	xdg_surface									*	surface_xdg								= {};
	xdg_toplevel								*	toplevel_xdg							= {};
};
#endif



} // window_manager
} // bc
