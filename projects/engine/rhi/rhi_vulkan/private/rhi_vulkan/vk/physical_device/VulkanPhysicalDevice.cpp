
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>

#include <window_manager/WindowManagerComponent.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#undef ERROR
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/WinDef.hpp>
#endif

#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
#include <wayland-client-core.h>
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::VulkanPhysicalDevice(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	VkPhysicalDevice		vk_physical_device
) :
	rhi_vulkan_impl( &rhi_vulkan_impl ),
	vk_physical_device( vk_physical_device )
{
	vk_memory_properties.sType		= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	vk_memory_properties.pNext		= nullptr;
	vkGetPhysicalDeviceMemoryProperties2( vk_physical_device, &vk_memory_properties );

	vk_properties.sType				= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	vk_properties.pNext				= nullptr;
	vkGetPhysicalDeviceProperties2( vk_physical_device, &vk_properties );

	vk_features.sType				= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	vk_features.pNext				= nullptr;
	vkGetPhysicalDeviceFeatures2( vk_physical_device, &vk_features );

	auto family_count = u32 {};
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &family_count, nullptr );
	queue_family_properties.queue_family_properties.Resize( family_count );
	for( auto & p : queue_family_properties.queue_family_properties )
	{
		p.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
	}
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &family_count, queue_family_properties.queue_family_properties.Data() );
	queue_family_properties.can_present = GetPhysicalDeviceQueuePresentationSupport( queue_family_properties.queue_family_properties );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bool> bc::rhi::VulkanPhysicalDevice::GetPhysicalDeviceQueuePresentationSupport(
	ListView<VkQueueFamilyProperties2>		family_properties
) const
{
	auto result = List<bool>( family_properties.Size() );

	auto * platform_specific_handles_base = rhi_vulkan_impl->GetWindowManagerComponent().GetPlatformSpecificHandles();
	assert( platform_specific_handles_base );

	switch (platform_specific_handles_base->structure_type)
	{

	#if BITCRAFTE_WINDOW_MANAGER_WIN32
	case window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32:
	{
		for( u64 i = 0; i < family_properties.Size(); ++i )
		{
			result[ i ] = !!vkGetPhysicalDeviceWin32PresentationSupportKHR( vk_physical_device, i );
		}
		break;
	}
	#endif

	#if BITCRAFTE_WINDOW_MANAGER_XCB
	case window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_XCB:
	{
		auto platform_specific_handles = static_cast<const window_manager::WindowManagerXCBPlatformHandles*>( platform_specific_handles_base );
		BAssert( platform_specific_handles->xcb_connection, "Failed to aquire X11 connection, make sure desktop environment is running X11" );

		auto xcb_connection = platform_specific_handles->xcb_connection;
		for( u64 i = 0; i < family_properties.Size(); ++i )
		{
			result[ i ] = !!vkGetPhysicalDeviceXcbPresentationSupportKHR( vk_physical_device, i, xcb_connection, platform_specific_handles->xcb_screen->root_visual );
		}
		break;
	}
	#endif // BITCRAFTE_WINDOW_MANAGER_XCB

	#if BITCRAFTE_WINDOW_MANAGER_WAYLAND
	case window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WAYLAND:
	{
		auto platform_specific_handles = static_cast<const window_manager::WindowManagerWaylandPlatformHandles*>( platform_specific_handles_base );
		BAssert( platform_specific_handles->display, "Failed to get Wayland display, make sure desktop environment is running on Wayland" );

		auto wayland_display = platform_specific_handles->display;
		for( u64 i = 0; i < family_properties.Size(); ++i )
		{
			result[ i ] = !!vkGetPhysicalDeviceWaylandPresentationSupportKHR( vk_physical_device, i, wayland_display );
		}
		break;
	}
	#endif // BITCRAFTE_WINDOW_MANAGER_WAYLAND

	default:
		BEnsure( 0, "Please add window manager specific queue presentation support query here" );
		break;
	}

	return result;
}
