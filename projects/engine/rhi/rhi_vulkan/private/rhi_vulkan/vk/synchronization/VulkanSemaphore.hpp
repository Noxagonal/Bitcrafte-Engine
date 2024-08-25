#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanSemaphore
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanSemaphore(
		RHIVulkanImpl&	rhi_vulkan_impl,
		bool			create_timeline_semaphore
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanSemaphore( const VulkanSemaphore& other ) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanSemaphore( VulkanSemaphore&& other ) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanSemaphore();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( const VulkanSemaphore& other ) -> VulkanSemaphore& = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( VulkanSemaphore&& other ) noexcept -> VulkanSemaphore&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Move( VulkanSemaphore& other );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkSemaphore		vk_semaphore		= VK_NULL_HANDLE;

	RHIVulkanImpl&	rhi_vulkan_impl;
};



} // rhi
} // bc
