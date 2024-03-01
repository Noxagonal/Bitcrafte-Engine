
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/renderpass/RenderPass.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPass::VulkanRenderPass(
	RHIVulkanImpl						&	rhi_vulkan_impl,
	const VulkanRenderPassCreateInfo	&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	// TODO

	//auto use_multisampling = create_info.multisamples != VK_SAMPLE_COUNT_1_BIT;
	//
	//auto attachment_create_info_list = List<VkAttachmentDescription2>();
	//for( u64 i = 0; i < attachment_list.Size(); i++ )
	//{
	//	// TODO: Determined by attachment
	//	auto flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	//
	//	attachment_create_info_list[ i ].sType				= VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2;
	//	attachment_create_info_list[ i ].pNext				= nullptr;
	//	attachment_create_info_list[ i ].flags				= flags;
	//	attachment_create_info_list[ i ].format				= determined_by_attachment;
	//	attachment_create_info_list[ i ].samples			= determined_by_attachment;
	//	attachment_create_info_list[ i ].loadOp				= determined_later;
	//	attachment_create_info_list[ i ].storeOp			= determined_later;
	//	attachment_create_info_list[ i ].stencilLoadOp		= determined_later;
	//	attachment_create_info_list[ i ].stencilStoreOp		= determined_later;
	//	attachment_create_info_list[ i ].initialLayout		= determined_by_attachment;
	//	attachment_create_info_list[ i ].finalLayout		= determined_by_attachment;
	//}
	//
	//auto subpass_create_info_list = List<VkSubpassDescription2>();
	//auto subpass_create_info_input_attachment_reference_list			= List<List<VkAttachmentReference2>> {};
	//auto subpass_create_info_color_attachment_reference_list			= List<List<VkAttachmentReference2>> {};
	//auto subpass_create_info_resolve_attachment_reference_list			= List<List<VkAttachmentReference2>> {};
	//auto subpass_create_info_depth_stencil_attachment_reference_list	= List<VkAttachmentReference2> {};
	//auto subpass_create_info_preserve_attachment_reference_list			= List<List<u32>> {};
	//for( u64 i = 0; i < subpass_list.Size(); i++ )
	//{
	//	for( u64 r = 0; r < subpass_input_references[ i ].Size(); r++ )
	//	{
	//		subpass_create_info_input_attachment_reference_list[ i ][ r ].sType			= VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
	//		subpass_create_info_input_attachment_reference_list[ i ][ r ].pNext			= nullptr;
	//		subpass_create_info_input_attachment_reference_list[ i ][ r ].attachment	= determined_by_subpass_reference;
	//		subpass_create_info_input_attachment_reference_list[ i ][ r ].layout		= determined_by_subpass_reference;
	//		subpass_create_info_input_attachment_reference_list[ i ][ r ].aspectMask	= determined_by_attachment_format;
	//	}
	//
	//	for( u64 r = 0; r < subpass_color_references[ i ].Size(); r++ )
	//	{
	//		subpass_create_info_color_attachment_reference_list[ i ][ r ].sType			= VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
	//		subpass_create_info_color_attachment_reference_list[ i ][ r ].pNext			= nullptr;
	//		subpass_create_info_color_attachment_reference_list[ i ][ r ].attachment	= determined_by_subpass_reference;
	//		subpass_create_info_color_attachment_reference_list[ i ][ r ].layout		= determined_by_subpass_reference;
	//		subpass_create_info_color_attachment_reference_list[ i ][ r ].aspectMask	= determined_by_attachment_format;
	//	}
	//
	//	for( u64 r = 0; r < subpass_resolve_references[ i ].Size(); r++ )
	//	{
	//		subpass_create_info_resolve_attachment_reference_list[ i ][ r ].sType		= VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
	//		subpass_create_info_resolve_attachment_reference_list[ i ][ r ].pNext		= nullptr;
	//		subpass_create_info_resolve_attachment_reference_list[ i ][ r ].attachment	= determined_by_subpass_reference;
	//		subpass_create_info_resolve_attachment_reference_list[ i ][ r ].layout		= determined_by_subpass_reference;
	//		subpass_create_info_resolve_attachment_reference_list[ i ][ r ].aspectMask	= determined_by_attachment_format;
	//	}
	//
	//	subpass_create_info_depth_stencil_attachment_reference_list[ i ].sType			= VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;
	//	subpass_create_info_depth_stencil_attachment_reference_list[ i ].pNext			= nullptr;
	//	subpass_create_info_depth_stencil_attachment_reference_list[ i ].attachment		= determined_by_subpass_reference;
	//	subpass_create_info_depth_stencil_attachment_reference_list[ i ].layout			= determined_by_subpass_reference;
	//	subpass_create_info_depth_stencil_attachment_reference_list[ i ].aspectMask		= determined_by_attachment_format;
	//
	//	for( u64 r = 0; r < subpass_preserve_references[ i ].Size(); r++ )
	//	{
	//		subpass_create_info_preserve_attachment_reference_list[ i ][ r ] = determined_by_subpass_reference;
	//	}
	//
	//	subpass_create_info_list[ i ].sType						= VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2;
	//	subpass_create_info_list[ i ].pNext						= nullptr;
	//	subpass_create_info_list[ i ].flags						= 0;
	//	subpass_create_info_list[ i ].pipelineBindPoint			= determined_by_subpass;
	//	subpass_create_info_list[ i ].viewMask					= 0;
	//	subpass_create_info_list[ i ].inputAttachmentCount		= u32( subpass_create_info_input_attachment_reference_list[ i ].Size() );
	//	subpass_create_info_list[ i ].pInputAttachments			= subpass_create_info_input_attachment_reference_list[ i ].Data();
	//	subpass_create_info_list[ i ].colorAttachmentCount		= u32( subpass_create_info_color_attachment_reference_list[ i ].Size() );
	//	subpass_create_info_list[ i ].pColorAttachments			= subpass_create_info_color_attachment_reference_list[ i ].Data();
	//	subpass_create_info_list[ i ].pResolveAttachments		= subpass_create_info_resolve_attachment_reference_list[ i ].Data();
	//	subpass_create_info_list[ i ].pDepthStencilAttachment	= &subpass_create_info_depth_stencil_attachment_reference_list[ i ];
	//	subpass_create_info_list[ i ].preserveAttachmentCount	= u32( subpass_create_info_preserve_attachment_reference_list[ i ].Size() );
	//	subpass_create_info_list[ i ].pPreserveAttachments		= subpass_create_info_preserve_attachment_reference_list[ i ].Data();
	//}
	//
	//auto subpass_dependency_create_info_list		= List<VkSubpassDependency2>();
	//for( u64 i = 0; i < subpass_dependencies.Size(); i++ )
	//{
	//	subpass_dependency_create_info_list[ i ].sType				= VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2;
	//	subpass_dependency_create_info_list[ i ].pNext				= nullptr;
	//	subpass_dependency_create_info_list[ i ].srcSubpass			= determined_by_subpass;
	//	subpass_dependency_create_info_list[ i ].dstSubpass			= determined_by_subpass;
	//	subpass_dependency_create_info_list[ i ].srcStageMask		= determined_by_subpass;
	//	subpass_dependency_create_info_list[ i ].dstStageMask		= determined_by_subpass;
	//	subpass_dependency_create_info_list[ i ].srcAccessMask		= determined_by_subpass;
	//	subpass_dependency_create_info_list[ i ].dstAccessMask		= determined_by_subpass;
	//	// Set to VK_DEPENDENCY_VIEW_LOCAL_BIT for multi-view renders and VK_DEPENDENCY_DEVICE_GROUP_BIT for multi-gpu devices.
	//	subpass_dependency_create_info_list[ i ].dependencyFlags	= VK_DEPENDENCY_BY_REGION_BIT;
	//	// Reconsider this for multi-view renderpasses.
	//	subpass_dependency_create_info_list[ i ].viewOffset			= 0;
	//}
	//
	//auto render_pass_create_info = VkRenderPassCreateInfo2 {};
	//render_pass_create_info.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2;
	//render_pass_create_info.pNext					= nullptr;
	//render_pass_create_info.flags					= 0;
	//render_pass_create_info.attachmentCount			= u32( attachment_create_info_list.Size() );
	//render_pass_create_info.pAttachments			= attachment_create_info_list.Data();
	//render_pass_create_info.subpassCount			= u32( subpass_create_info_list.Size() );
	//render_pass_create_info.pSubpasses				= subpass_create_info_list.Data();
	//render_pass_create_info.dependencyCount			= u32( subpass_dependency_create_info_list.Size() );
	//render_pass_create_info.pDependencies			= subpass_dependency_create_info_list.Data();
	//render_pass_create_info.correlatedViewMaskCount	= 0;
	//render_pass_create_info.pCorrelatedViewMasks	= nullptr;
	//
	//BAssertVkResult( vkCreateRenderPass2(
	//	rhi_vulkan_impl.GetVulkanDevice(),
	//	&render_pass_create_info,
	//	rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
	//	&vk_render_pass
	//) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_render_pass,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
}
