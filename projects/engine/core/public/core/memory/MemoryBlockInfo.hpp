#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/data_types/FundamentalTypes.hpp>



namespace bc {
namespace memory {



struct MemoryBlockInfo
{
	u64	size		= 0;
	u32	alignment	= 0;
};



} // namespace memory
} // namespace bc
