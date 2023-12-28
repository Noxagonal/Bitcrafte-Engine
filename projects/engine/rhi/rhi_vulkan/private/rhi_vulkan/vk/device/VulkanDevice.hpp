#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/vk/queue/VulkanQueue.hpp>
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
		VulkanPhysicalDevice							&	physical_device,
		const RHIComponentStartInfo						&	rhi_start_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanDevice();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice								&	GetVulkanPhysicalDevice() { return physical_device; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanQueue											&	GetPrimaryRenderQueue() { return primary_render_queue; }
	VulkanQueue											&	GetSecondaryRenderQueue() { return secondary_render_queue; }
	VulkanQueue											&	GetPrimaryComputeQueue() { return primary_compute_queue; }
	VulkanQueue											&	GetPrimaryTransferQueue() { return primary_transfer_queue; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline operator VkDevice() { return vk_device; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl										&	rhi_vulkan_impl;
	VulkanPhysicalDevice								&	physical_device;
	VkDevice												vk_device							= VK_NULL_HANDLE;

	List<Text>												enabled_extension_names;

	VulkanQueue												primary_render_queue;
	VulkanQueue												secondary_render_queue;
	VulkanQueue												primary_compute_queue;
	VulkanQueue												primary_transfer_queue;
};



} // rhi
} // bc
