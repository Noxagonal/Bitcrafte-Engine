#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkImageAspectFlagBits			CreateAspectMaskFromFormat(
	VkFormat					format
);



} // rhi
} // bc
