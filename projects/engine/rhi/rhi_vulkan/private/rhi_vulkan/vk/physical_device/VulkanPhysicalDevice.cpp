
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>

#include <window_manager/WindowManagerComponent.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#undef ERROR
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <core/platform/windows/Windows.hpp>
#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
#include <wayland-client-core.h>
#else
#error "Please add window manager required headers here"
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

	#if BITCRAFTE_WINDOW_MANAGER_WIN32
	for( u64 i = 0; i < family_properties.Size(); ++i )
	{
		result[ i ] = !!vkGetPhysicalDeviceWin32PresentationSupportKHR( vk_physical_device, i );
	}

	#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
	auto platform_specific_handles = reinterpret_cast<const window_manager::WindowManagerWaylandPlatformHandles*>( rhi_vulkan_impl->GetWindowManagerComponent().GetPlatformSpecificHandles() );
	assert( platform_specific_handles );
	assert( platform_specific_handles->structure_type == window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WAYLAND );
	BAssert( platform_specific_handles->display, "Failed to get Wayland display, make sure desktop environment is running on Wayland" );

	auto wayland_display = platform_specific_handles->display;
	for( u64 i = 0; i < family_properties.Size(); ++i )
	{
		result[ i ] = !!vkGetPhysicalDeviceWaylandPresentationSupportKHR( vk_physical_device, i, wayland_display );
	}

	#else
	#error "Please add window manager specific queue presentation support query here"

	#endif

	return result;
}
