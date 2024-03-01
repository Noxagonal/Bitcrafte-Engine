
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/render_pipeline/g_buffer/VulkanGBuffer.hpp>

#include <rhi_vulkan/utility/format/FormatHelpers.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanGBuffer::VulkanGBuffer(
	RHIVulkanImpl											&	rhi_vulkan_impl,
	const GBufferCreateInfo									&	create_info
) :
	GBuffer( create_info ),
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	// Set image_flags to VK_IMAGE_CREATE_ALIAS_BIT if we ever optimize GBuffer so that multiple RenderPipeline stages can use
	// the same memory as this.
	
	auto image_format = GetVulkanFormatFromRHIColorFormat( create_info.color_format );
	auto image_aspect = CreateAspectMaskFromFormat( image_format );

	auto image_usage_flags = VkImageUsageFlagBits(
		VK_IMAGE_USAGE_SAMPLED_BIT |
		( ( image_aspect & VK_IMAGE_ASPECT_COLOR_BIT ) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0 ) |
		( ( image_aspect & VK_IMAGE_ASPECT_DEPTH_BIT ) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0 ) |
		( ( image_aspect & VK_IMAGE_ASPECT_STENCIL_BIT ) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0 )
	);

	BAssert( std::bitset<sizeof( decltype( create_info.sample_count ) ) * 8>( create_info.sample_count ).count() == 1, U"Cannot create G-buffer, invalid sample count" );

	auto image_create_info = VulkanImageCreateInfo {};
	image_create_info.image_flags					= VkImageCreateFlagBits {};
	image_create_info.image_format					= image_format;
	image_create_info.image_size					= create_info.size;
	image_create_info.image_array_layers			= 1;
	image_create_info.image_samples					= VkSampleCountFlagBits( create_info.sample_count );
	image_create_info.image_usage					= image_usage_flags;
	image_create_info.image_usage_in_swapchain		= nullptr;
	image_create_info.is_host_visible				= false;
	image_create_info.use_mip_maps					= false;
	image.Emplace( rhi_vulkan_impl, image_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanGBuffer::~VulkanGBuffer()
{
	image.Clear();
}
