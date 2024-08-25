#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/Vector.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class VulkanSwapchain;



struct VulkanImageCreateInfo
{
	VkImageCreateFlagBits	image_flags					= {};
	VkFormat				image_format				= {};
	math::Vec2u32			image_size					= {};
	u32						image_array_layers			= {};
	VkSampleCountFlagBits	image_samples				= {};
	VkImageUsageFlagBits	image_usage					= {};
	VulkanSwapchain*		image_usage_in_swapchain	= {};
	bool					is_host_visible				= {};
	bool					use_mip_maps				= {};
};



} // rhi
} // bc
