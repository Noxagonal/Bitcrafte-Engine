#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIMemoryPoolCreateInfo
{
	VkDeviceSize	linear_allocation_chunk_size		= {};
	VkDeviceSize	non_linear_allocation_chunk_size	= {};
};



} // rhi
} // bc
