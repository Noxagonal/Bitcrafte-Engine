#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RenderPipelineStageCreateInfo
{
	List<u32>	g_buffer_input_index_list;
	List<u32>	g_buffer_output_index_list;
};




} // rhi
} // bc
