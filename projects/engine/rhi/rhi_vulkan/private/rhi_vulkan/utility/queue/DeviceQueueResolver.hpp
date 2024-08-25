#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi_vulkan/vk/queue/VulkanQueue.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>

#include <core/containers/List.hpp>
#include <core/containers/Pair.hpp>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to automatically resolve requested queues from physical device.
///
/// This is helper class that finds device queues from physical device and collects them into an easier to use logical queues.
///
///	No matter how many queues the device actually has, this helper class will always return the number of requested queues back.
/// Some of those logical queues might be mapped to a same hardware queue as another logical queue if the device does not have
/// enough physical queues.
/// 
///	Queue families are automatically resolved, queues are requested from queue families with more specific support, eg. If we
/// request a queue that is used only to transfer, and physical device has 2 queue families where first supports render and
/// transfer and the second only transfer, the second queue family is used for this queue.
///
///	If physical device supports less queues than requested, heuristics are used to select which resolved queues should map to
/// which physical queues based on how many already share the same physical queue and the priority of the requested queue.
class DeviceQueueResolver
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DeviceQueueResolver() = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DeviceQueueResolver(
		RHIVulkanImpl&					rhi_vulkan_impl,
		const VulkanPhysicalDevice&		vk_physical_device,
		List<Pair<VkQueueFlags, f32>>	queue_types
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~DeviceQueueResolver();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetDeviceQueueCreateInfos() -> const List<VkDeviceQueueCreateInfo>&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetVulkanQueues( VkDevice device ) -> List<VulkanQueue>;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct QueueGetInfo
	{
		u32	queue_index;
		u32	queue_family_index;
		u32	based_on;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl&									rhi_vulkan_impl;
	const VulkanPhysicalDevice&						physical_device;
	VkInstance										vk_instance					= VK_NULL_HANDLE;

	List<Pair<VkQueueFlags, f32>>					queue_types;

	List<VkDeviceQueueCreateInfo>					queue_create_infos;
	List<QueueGetInfo>								queue_get_info;
	List<List<f32>>									queue_priorities;

	const VulkanPhysicalDevice::QueueFamilyInfo&	queue_family_properties;
};



} // rhi
} // bc
