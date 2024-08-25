
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/render_pipeline/stage/VulkanRenderPipelineStage.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPipelineStage::VulkanRenderPipelineStage(
	RHIVulkanImpl&							rhi_vulkan_impl,
	const RenderPipelineStageCreateInfo&	create_info
)
	:
	rhi_vulkan_impl( rhi_vulkan_impl )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPipelineStage::~VulkanRenderPipelineStage()
{
}
