#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



struct VulkanBufferCreateInfo
{
	VkDeviceSize									buffer_size						= {};
	VkBufferCreateFlagBits							buffer_flags					= {};
	VkBufferUsageFlagBits							buffer_usage_flags				= {};
	VkFormat										buffer_format					= {};
	bool											create_buffer_view				= {};
	bool											is_host_visible					= {};
};



} // rhi
} // bc
