#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/vk/queue/Queue.hpp>
#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class RHIVulkanImpl;
class VulkanPhysicalDevice;
struct RHIComponentStartInfo;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanDevice
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanDevice(
		RHIVulkanImpl									&	rhi_vulkan_impl,
		const VulkanPhysicalDevice						&	physical_device,
		const RHIComponentStartInfo						&	rhi_start_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanDevice();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Queue												&	GetPrimaryRenderQueue() { return primary_render_queue; }
	Queue												&	GetSecondaryRenderQueue() { return secondary_render_queue; }
	Queue												&	GetPrimaryComputeQueue() { return primary_compute_queue; }
	Queue												&	GetPrimaryTransferQueue() { return primary_transfer_queue; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline operator VkDevice() { return vk_device; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl										&	rhi_vulkan_impl;
	const VulkanPhysicalDevice							&	physical_device;
	VkDevice												vk_device							= VK_NULL_HANDLE;

	List<Text>												enabled_extension_names;

	Queue													primary_render_queue;
	Queue													secondary_render_queue;
	Queue													primary_compute_queue;
	Queue													primary_transfer_queue;
};



} // rhi
} // bc
