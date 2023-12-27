
#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>

// Add appropriate platform specific includes.
#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <Windows.h>
#undef ERROR
#include <vulkan/vulkan_win32.h>
#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
#include <vulkan/vulkan_wayland.h>
#else
#error "Please add platform support here"
#endif
