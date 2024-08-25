#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi/render_pipeline/stage/RenderPipelineStageCreateInfo.hpp>
#include <core/containers/List.hpp>



namespace bc {
namespace rhi {



class ShaderInterface;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RenderPipelineCreateInfo
{
	// TODO: Replace this with Handle type when Handle has been implemented.
	ShaderInterface*						shader_interface			= {};
	List<RenderPipelineStageCreateInfo>		stage_create_info_list;
};



} // rhi
} // bc
