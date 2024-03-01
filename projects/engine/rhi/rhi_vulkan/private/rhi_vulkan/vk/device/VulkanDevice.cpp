
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <rhi_vulkan/utility/queue/DeviceQueueResolver.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanDevice::VulkanDevice(
	RHIVulkanImpl						&	rhi_vulkan_impl,
	VulkanPhysicalDevice				&	physical_device,
	const RHIComponentStartInfo			&	rhi_start_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	physical_device( physical_device )
{
	enabled_extension_names.PushBack( VK_KHR_SWAPCHAIN_EXTENSION_NAME );



	auto queue_resolver = DeviceQueueResolver(
		rhi_vulkan_impl,
		physical_device,
		{
			{ VK_QUEUE_GRAPHICS_BIT, 1.0 },	// Primary render queue.
			{ VK_QUEUE_GRAPHICS_BIT, 0.2 },	// Secondary render queue.
			{ VK_QUEUE_COMPUTE_BIT, 1.0 },	// Primary compute queue.
			{ VK_QUEUE_TRANSFER_BIT, 1.0 }	// Primary transfer queue.
		}
	);
	auto device_queue_create_info_list = queue_resolver.GetDeviceQueueCreateInfos();



	auto enabled_extension_names_c_str = List<const char*>();
	for( auto & e : enabled_extension_names )
	{
		enabled_extension_names_c_str.PushBack( e.ToCStr() );
	}



	VkPhysicalDeviceVulkan12Features enabled_12_features {};
	enabled_12_features.sType		= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	enabled_12_features.pNext		= nullptr;
	// Add required Vulkan 1.2 features here.

	VkPhysicalDeviceVulkan11Features enabled_11_features {};
	enabled_11_features.sType		= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	enabled_11_features.pNext		= &enabled_12_features;
	// Add required Vulkan 1.1 features here.

	auto features = VkPhysicalDeviceFeatures {};
	// Add required Vulkan base features here.



	auto device_create_info = VkDeviceCreateInfo {};
	device_create_info.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext					= &enabled_11_features;
	device_create_info.flags					= 0;
	device_create_info.queueCreateInfoCount		= u32( device_queue_create_info_list.Size() );
	device_create_info.pQueueCreateInfos		= device_queue_create_info_list.Data();
	device_create_info.enabledLayerCount		= 0;
	device_create_info.ppEnabledLayerNames		= nullptr;
	device_create_info.enabledExtensionCount	= u32( enabled_extension_names_c_str.Size() );
	device_create_info.ppEnabledExtensionNames	= enabled_extension_names_c_str.Data();
	device_create_info.pEnabledFeatures			= &features;

	BAssertVkResult( vkCreateDevice(
		physical_device,
		&device_create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_device
	) );

	auto logical_queues			= queue_resolver.GetVulkanQueues( vk_device );
	primary_render_queue		= logical_queues[ 0 ];
	secondary_render_queue		= logical_queues[ 1 ];
	primary_compute_queue		= logical_queues[ 2 ];
	primary_transfer_queue		= logical_queues[ 3 ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice( vk_device, rhi_vulkan_impl.GetMainThreadAllocationCallbacks() );
}
