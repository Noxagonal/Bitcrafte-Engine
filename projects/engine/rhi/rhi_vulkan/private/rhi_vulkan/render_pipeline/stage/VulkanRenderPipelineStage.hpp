#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi/render_pipeline/stage/RenderPipelineStageCreateInfo.hpp>

#include <rhi_vulkan/vk/image/VulkanImage.hpp>

#include <core/containers/Optional.hpp>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanRenderPipelineStage
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanRenderPipelineStage(
		RHIVulkanImpl&							rhi_vulkan_impl,
		const RenderPipelineStageCreateInfo&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanRenderPipelineStage();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl&	rhi_vulkan_impl;
};




} // rhi
} // bc
