#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/render_pipeline/RenderPipelineCreateInfo.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RenderPipeline
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RenderPipeline(
		const RenderPipelineCreateInfo								&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~RenderPipeline();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};




} // rhi
} // bc
