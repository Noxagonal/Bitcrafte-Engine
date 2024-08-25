#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VulkanRenderPassCreateInfo
{
	VkSampleCountFlags	multisamples	= {};
};



} // rhi
} // bc
