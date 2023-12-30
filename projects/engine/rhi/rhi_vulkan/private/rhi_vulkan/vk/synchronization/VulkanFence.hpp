#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanFence
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanFence(
		RHIVulkanImpl							&	rhi_vulkan_impl,
		bool										initially_signaled
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanFence(
		const VulkanFence						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanFence(
		VulkanFence								&&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanFence();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanFence									&	operator=(
		const VulkanFence						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanFence									&	operator=(
		VulkanFence								&&	other
	) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void											Clear();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void											MoveOther(
		VulkanFence								&&	other
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkFence											vk_fence								= VK_NULL_HANDLE;

	RHIVulkanImpl								&	rhi_vulkan_impl;
};



} // rhi
} // bc
