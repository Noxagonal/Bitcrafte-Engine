
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/framebuffer/VulkanFramebuffer.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanFramebuffer::VulkanFramebuffer(
	RHIVulkanImpl&		rhi_vulkan_impl,
	VulkanRenderPass&	vulkan_render_pass
)
:
	rhi_vulkan_impl( rhi_vulkan_impl ),
	vulkan_render_pass( vulkan_render_pass )
{
	// TODO

	//auto create_info = VkFramebufferCreateInfo {};
	//create_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//create_info.pNext			= nullptr;
	//create_info.flags			= 0;
	//create_info.renderPass		= vulkan_render_pass;
	//create_info.attachmentCount	= todo;
	//create_info.pAttachments	= todo;
	//create_info.width			= todo;
	//create_info.height			= todo;
	//create_info.layers			= todo;
	//
	//vkCreateFramebuffer(
	//	rhi_vulkan_impl.GetVulkanDevice(),
	//	&create_info,
	//	rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
	//	&vk_framebuffer
	//);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanFramebuffer::~VulkanFramebuffer()
{
	vkDestroyFramebuffer(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_framebuffer,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
}
