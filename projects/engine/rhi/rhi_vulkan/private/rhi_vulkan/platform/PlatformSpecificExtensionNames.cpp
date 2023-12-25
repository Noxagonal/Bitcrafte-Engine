
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>
#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::Text> bc::rhi::GetPlatformSpecificInstanceExtensionNames()
{
	auto result = List<Text> {};
	#if BITCRAFTE_PLATFORM_WINDOWS
	result.PushBack( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
	#elif BITCRAFTE_PLATFORM_LINUX
	result.PushBack( VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME );
	#elif BITCRAFTE_PLATFORM_APPLE
	result.PushBack( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
	#elif BITCRAFTE_PLATFORM_IOS
	result.PushBack( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
	#elif BITCRAFTE_PLATFORM_ANDROID
	result.PushBack( VK_MVK_ANDROID_SURFACE_EXTENSION_NAME );
	#else
	#error please add platform support here
	#endif
	return result;
}
