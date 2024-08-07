#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class RHIVulkanImpl;
class VulkanQueue;
class VulkanCommandBuffer;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanCommandPool
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanCommandPool(
		RHIVulkanImpl								&	rhi_vulkan_impl,
		VulkanQueue									&	queue,
		bool											is_transient,
		bool											is_resettable
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanCommandPool(
		const VulkanCommandPool						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanCommandPool(
		VulkanCommandPool							&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanCommandPool();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanCommandPool								&	operator=(
		const VulkanCommandPool						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanCommandPool								&	operator=(
		VulkanCommandPool							&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<VulkanCommandBuffer>						AllocateCommandBuffer(
		bool											is_secondary
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline operator VkCommandPool() { return vk_command_pool; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkCommandPool										vk_command_pool							= VK_NULL_HANDLE;

	RHIVulkanImpl									*	rhi_vulkan_impl;
};



} // rhi
} // bc
