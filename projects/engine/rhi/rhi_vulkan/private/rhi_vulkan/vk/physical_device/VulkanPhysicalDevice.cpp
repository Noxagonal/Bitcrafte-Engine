
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#undef ERROR
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::VulkanPhysicalDevice(
	RHIVulkanImpl		&	vulkan_implementation,
	VkPhysicalDevice		vk_physical_device
) :
	vulkan_implementation( vulkan_implementation ),
	vk_physical_device( vk_physical_device )
{
	memory_properties				= FetchMemoryProperties();
	properties						= FetchProperties();
	queue_family_property_list		= FetchQueueFamilyPropertyList();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkPhysicalDeviceMemoryProperties2 bc::rhi::VulkanPhysicalDevice::FetchMemoryProperties() const
{
	auto result = VkPhysicalDeviceMemoryProperties2 {};
	result.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	vkGetPhysicalDeviceMemoryProperties2( vk_physical_device, &result );
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkPhysicalDeviceProperties2 bc::rhi::VulkanPhysicalDevice::FetchProperties() const
{
	auto result = VkPhysicalDeviceProperties2 {};
	result.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	vkGetPhysicalDeviceProperties2( vk_physical_device, &result );
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<VkQueueFamilyProperties2> bc::rhi::VulkanPhysicalDevice::FetchQueueFamilyPropertyList()
{
	auto count = uint32_t {};
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &count, nullptr );
	auto result = List<VkQueueFamilyProperties2> {};
	result.Resize( count );
	for( auto & p : result )
	{
		p.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
	}
	vkGetPhysicalDeviceQueueFamilyProperties2( vk_physical_device, &count, result.Data() );
	return result;
}
