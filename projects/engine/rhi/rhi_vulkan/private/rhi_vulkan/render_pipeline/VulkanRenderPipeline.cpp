
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/render_pipeline/VulkanRenderPipeline.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPipeline::VulkanRenderPipeline(
	RHIVulkanImpl											&	rhi_vulkan_impl,
	const RenderPipelineCreateInfo							&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	RenderPipeline( create_info )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPipeline::~VulkanRenderPipeline()
{}
