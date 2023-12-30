#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/linear_algebra/Vector.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class VulkanSwapchain;



struct VulkanImageCreateInfo
{
	VkImageType											image_type									= {};
	VkImageCreateFlags									image_create_flags							= {};
	VkFormat											image_format								= {};
	math::Vector2u										image_size									= {};
	uint32_t											image_array_layers							= {};
	VkSampleCountFlagBits								image_samples								= {};
	VkImageUsageFlagBits								image_usage									= {};
	VulkanSwapchain									*	image_usage_in_swapchain					= {};
	bool												is_host_visible								= {};
	bool												use_mip_maps								= {};
};



} // rhi
} // bc
