
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/surface/VulkanWindowSurface.hpp>

#include <rhi_vulkan/PreCompiledHeader.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>
#include <window_manager/window/Window.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanWindowSurface::VulkanWindowSurface(
	RHIVulkanImpl				&	rhi_vulkan_impl,
	window_manager::Window		*	window
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto CreatePlatformSpecificSurface = [ &rhi_vulkan_impl, window ]() -> VkSurfaceKHR
		{
			auto result = VkSurfaceKHR {};

			#if BITCRAFTE_WINDOW_MANAGER_WIN32

			auto win32_handles = reinterpret_cast<window_manager::WindowManagerWin32PlatformHandles*>( window->GetPlatformSpecificHandles() );
			assert( win32_handles );
			assert( win32_handles->structure_type == window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32 );

			auto surface_create_info = VkWin32SurfaceCreateInfoKHR {};
			surface_create_info.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surface_create_info.pNext		= nullptr;
			surface_create_info.flags		= 0;
			surface_create_info.hinstance	= win32_handles->hInstance;
			surface_create_info.hwnd		= win32_handles->hWnd;
			BAssertVkResult( vkCreateWin32SurfaceKHR(
				rhi_vulkan_impl.GetVulkanInstance(),
				&surface_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );

			#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND

			auto wayland_handles = reinterpret_cast<window_manager::WindowManagerWaylandPlatformHandles*>( window->GetPlatformSpecificHandles() );
			assert( wayland_handles );
			assert( wayland_handles->structure_type == window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32 );

			auto surface_create_info = VkWaylandSurfaceCreateInfoKHR {};
			surface_create_info.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surface_create_info.pNext		= nullptr;
			surface_create_info.flags		= 0;
			surface_create_info.display		= wayland_handles->display;
			surface_create_info.surface		= wayland_handles->surface;
			BAssertVkResult( vkCreateWaylandSurfaceKHR(
				rhi_vulkan_impl.GetVulkanInstance(),
				&surface_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );


			#else
			#error "Please add window manager specific vulkan window surface creation code here"
			#endif

			return result;
		};

	auto GetPhysicalDeviceSurfaceSupport = [ this, &rhi_vulkan_impl ]( VkSurfaceKHR surface ) -> VkBool32
		{
			VkBool32 surface_supported = VK_FALSE;
			BAssertVkResult( vkGetPhysicalDeviceSurfaceSupportKHR(
				rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
				rhi_vulkan_impl.GetVulkanDevice().GetPrimaryRenderQueue().GetQueueFamilyIndex(),
				surface,
				&surface_supported
			) );

			return surface_supported;
		};

	// Get physical device surface capabilities.
	auto GetPhysicalDeviceSurfaceCapabilities = [ &rhi_vulkan_impl ]( VkSurfaceKHR surface ) -> Capabilities
		{
			auto surface_info = VkPhysicalDeviceSurfaceInfo2KHR {};
			surface_info.sType		= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
			surface_info.pNext		= nullptr;
			surface_info.surface	= surface;


			// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_shared_presentable_image.html
			// This allows the application to use the image while the presention engine is accessing it,
			// in order to reduce the latency between rendering and presentation.
			auto shared_present_surface_capabilities = VkSharedPresentSurfaceCapabilitiesKHR {};
			shared_present_surface_capabilities.sType	= VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR;
			shared_present_surface_capabilities.pNext	= nullptr;

			auto surface_capabilities_2 = VkSurfaceCapabilities2KHR {};
			surface_capabilities_2.sType				= VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
			surface_capabilities_2.pNext				= &shared_present_surface_capabilities;

			BAssertVkResult( vkGetPhysicalDeviceSurfaceCapabilities2KHR(
				rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
				&surface_info,
				&surface_capabilities_2
			) );

			auto result = Capabilities {};
			result.vk_capabilities							= surface_capabilities_2.surfaceCapabilities;
			result.vk_shared_present_supported_usage_flags	= shared_present_surface_capabilities.sharedPresentSupportedUsageFlags;
			return result;
		};

	// Check if VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT is supported
	auto GetColorOutSupport = []( VkSurfaceCapabilitiesKHR capabilities ) -> VkBool32
		{
			VkImageUsageFlags required_image_support = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			return ( capabilities.supportedUsageFlags & required_image_support ) == required_image_support;
		};

	auto GetSurfaceFormats = [ &rhi_vulkan_impl ]( VkSurfaceKHR surface ) -> List<VkSurfaceFormatKHR>
		{
			List<VkSurfaceFormatKHR> result;
			u32 surface_format_count = 0;
			BAssertVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR(
				rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
				surface,
				&surface_format_count,
				nullptr
			) );
			result.Resize( surface_format_count );

			BAssertVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR(
				rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
				surface,
				&surface_format_count,
				result.Data()
			) );
			return result;
		};

	auto ChoosePreferredOutputFormat = []( List<VkSurfaceFormatKHR> surface_format_list ) -> VkSurfaceFormatKHR
		{
			if( surface_format_list.IsEmpty() ) return {};
			if( auto found = surface_format_list.FindIf( []( auto & f ) { return f.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32; } ) )
			{
				return *found;
			}
			if( auto found = surface_format_list.FindIf( []( auto & f ) { return f.format == VK_FORMAT_B8G8R8A8_UNORM; } ) )
			{
				return *found;
			}
			if( auto found = surface_format_list.FindIf( []( auto & f ) { return f.format == VK_FORMAT_R8G8B8A8_UNORM; } ) )
			{
				return *found;
			}
			return surface_format_list[ 0 ];
		};

	vk_surface							= CreatePlatformSpecificSurface();
	physical_device_surface_support		= GetPhysicalDeviceSurfaceSupport( vk_surface );
	capabilities						= GetPhysicalDeviceSurfaceCapabilities( vk_surface );
	supports_color_output				= GetColorOutSupport( capabilities.vk_capabilities );
	surface_format_list					= GetSurfaceFormats( vk_surface );
	preferred_surface_format			= ChoosePreferredOutputFormat( surface_format_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanWindowSurface::~VulkanWindowSurface()
{
	vkDestroySurfaceKHR( rhi_vulkan_impl.GetVulkanInstance(), vk_surface, rhi_vulkan_impl.GetMainThreadAllocationCallbacks() );
}
