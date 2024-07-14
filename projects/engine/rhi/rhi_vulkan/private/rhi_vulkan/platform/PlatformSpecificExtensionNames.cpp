
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>
#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <window_manager/WindowManagerComponent.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::Text> bc::rhi::GetPlatformSpecificInstanceExtensionNames(
	bc::rhi::RHIVulkanImpl		&	rhi_vulkan_impl
)
{
	switch( rhi_vulkan_impl.GetWindowManagerComponent().GetPlatformSpecificHandles()->structure_type )
	{
		#if BITCRAFTE_WINDOW_MANAGER_WIN32
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32:
			return { VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_XLIB
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XLIB:
			return { VK_KHR_XLIB_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_XCB
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XCB:
			return { VK_KHR_XCB_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WAYLAND:
			return { VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_MIR
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_MIR:
			return { VK_KHR_MIR_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_ANDROID
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_ANDROID:
			return { VK_KHR_ANDROID_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_VIV
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_VIV:
			return { VK_NN_VI_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_FUCHSIA
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_FUCHSIA:
			return { VK_FUCHSIA_EXTERNAL_MEMORY_EXTENSION_NAME, VK_FUCHSIA_EXTERNAL_SEMAPHORE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_MACOS
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_MACOS:
			return { VK_MVK_MACOS_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_MVK
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_MVK:
			return { VK_MVK_MOLTENVK_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_DIRECTFB
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_DIRECTFB:
			return { VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME };
		#endif

		#if BITCRAFTE_WINDOW_MANAGER_IOS
		case bc::window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_IOS:
			return { VK_MVK_IOS_SURFACE_EXTENSION_NAME };
		#endif

	default:
		return {};
	}
}
