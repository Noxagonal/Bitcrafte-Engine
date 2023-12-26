
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
bc::rhi::VulkanPhysicalDevice::MemoryPropertiesInfo::MemoryPropertiesInfo(
	VkPhysicalDevice vk_physical_device
)
{
	memory_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	vkGetPhysicalDeviceMemoryProperties2( vk_physical_device, &memory_properties );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::PropertiesInfo::PropertiesInfo(
	VkPhysicalDevice vk_physical_device
)
{
	properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	vkGetPhysicalDeviceProperties2( vk_physical_device, &properties );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::QueueFamilyInfo::QueueFamilyInfo(
	VkQueueFamilyProperties2	queue_family_properties,
	bool						can_present
) :
	queue_family_properties( queue_family_properties ),
	can_present( can_present )
{}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::VulkanPhysicalDevice(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	VkPhysicalDevice		vk_physical_device
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	vk_physical_device( vk_physical_device ),
	memory_properties( vk_physical_device ),
	properties( vk_physical_device )
{
	queue_family_property_list = FetchQueueFamilyPropertyList();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::rhi::VulkanPhysicalDevice::QueueFamilyInfo> bc::rhi::VulkanPhysicalDevice::FetchQueueFamilyPropertyList()
{
	auto DetermineWhichQueuesCanPresent = [ this ]( const List<VkQueueFamilyProperties2> & queue_family_properties ) -> List<bool>
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

	auto count = uint32_t {};
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &count, nullptr );
	auto vk_queue_family_properties = List<VkQueueFamilyProperties2>( count );
	for( auto & p : vk_queue_family_properties )
	{
		p.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
	}
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &count, vk_queue_family_properties.Data() );
	auto presentation_support = DetermineWhichQueuesCanPresent( vk_queue_family_properties );

	List<QueueFamilyInfo> result;
	result.Reserve( vk_queue_family_properties.Size() );
	for( size_t i = 0; i < vk_queue_family_properties.Size(); ++i )
	{
		result.EmplaceBack( vk_queue_family_properties[ i ], presentation_support[ i ] );
	}
	return result;
}
