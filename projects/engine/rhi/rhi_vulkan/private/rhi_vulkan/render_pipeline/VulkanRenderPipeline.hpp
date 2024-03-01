#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/render_pipeline/RenderPipeline.hpp>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanRenderPipeline : public RenderPipeline
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanRenderPipeline(
		RHIVulkanImpl											&	rhi_vulkan_impl,
		const RenderPipelineCreateInfo							&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~VulkanRenderPipeline();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl												&	rhi_vulkan_impl;
};




} // rhi
} // bc