
#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>

// Add appropriate platform specific includes.
#if BITCRAFTE_PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#elif BITCRAFTE_PLATFORM_LINUX
#include <vulkan/vulkan_wayland.h>
#elif BITCRAFTE_PLATFORM_APPLE
#include <vulkan/vulkan_macos.h>
#elif BITCRAFTE_PLATFORM_IOS
#include <vulkan/vulkan_ios.h>
#elif BITCRAFTE_PLATFORM_ANDROID
#include <vulkan/vulkan_android.h>
#else
#error "Please add platform support here"
#endif
