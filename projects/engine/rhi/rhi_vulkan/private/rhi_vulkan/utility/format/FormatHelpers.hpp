#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi/color/ColorFormat.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkImageAspectFlagBits			CreateAspectMaskFromFormat(
	VkFormat					format
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkFormat						GetVulkanFormatFromRHIColorFormat(
	ColorFormat					color_format
);



} // rhi
} // bc
