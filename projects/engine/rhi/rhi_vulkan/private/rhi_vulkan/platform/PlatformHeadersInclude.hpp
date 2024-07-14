
#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>

// Add appropriate platform specific includes.
#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/Windows.hpp>
#include <vulkan/vulkan_win32.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XCB
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_XLIB
#include <vulkan/vulkan_xlib.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
#include <vulkan/vulkan_wayland.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_MACOS
#include <vulkan/vulkan_macos.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_IOS
#include <vulkan/vulkan_ios.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_ANDROID
#include <vulkan/vulkan_android.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_HAIKU
#include <vulkan/vulkan_haiku.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_QNX
#include <vulkan/vulkan_photon.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_VIV
#include <vulkan/vulkan_vi.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_FUCHSIA
#include <vulkan/vulkan_fuchsia.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_DIRECTFB
#include <vulkan/vulkan_directfb.h>
#endif

#if BITCRAFTE_WINDOW_MANAGER_METAL
#include <vulkan/vulkan_metal.h>
#endif
