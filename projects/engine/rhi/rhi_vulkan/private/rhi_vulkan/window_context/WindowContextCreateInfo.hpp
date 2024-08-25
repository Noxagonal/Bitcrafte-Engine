#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/vk/swapchain/VulkanSwapchainCreateInfo.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WindowContextCreateInfo
{
	VulkanSwapchainCreateInfo	swapchain_create_info	= {};
};



} // rhi
} // bc
