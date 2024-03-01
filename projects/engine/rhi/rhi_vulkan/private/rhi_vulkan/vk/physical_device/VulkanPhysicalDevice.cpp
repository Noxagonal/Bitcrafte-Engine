
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#undef ERROR
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>

#if BITCRAFTE_WINDOW_MANAGER_WIN32
#include <Windows.h>
#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND
todo;
#else
#error "Please add window manager required headers here"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::VulkanPhysicalDevice(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	VkPhysicalDevice		vk_physical_device
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
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

	auto DetermineWhichQueuesCanPresent = [ this, vk_physical_device ]( const List<VkQueueFamilyProperties2> & queue_family_properties ) -> List<bool>
		{
			auto result = List<bool>( queue_family_properties.Size() );

			#if BITCRAFTE_WINDOW_MANAGER_WIN32

			for( size_t i = 0; i < queue_family_properties.Size(); ++i )
			{
				result[ i ] = !!vkGetPhysicalDeviceWin32PresentationSupportKHR( vk_physical_device, i );
			}

			#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND

			vkGetPhysicalDeviceWaylandPresentationSupportKHR( todo );

			#else

			#error "Please add window manager specific queue presentation support query here"

			#endif

			return result;
		};

	auto family_count = u32 {};
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &family_count, nullptr );
	queue_family_properties.queue_family_properties.Resize( family_count );
	for( auto & p : queue_family_properties.queue_family_properties )
	{
		p.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
	}
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &family_count, queue_family_properties.queue_family_properties.Data() );
	queue_family_properties.can_present = DetermineWhichQueuesCanPresent( queue_family_properties.queue_family_properties );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}
