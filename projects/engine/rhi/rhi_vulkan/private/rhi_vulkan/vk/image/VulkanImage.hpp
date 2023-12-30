#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/vk/image/VulkanImageCreateInfo.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIPoolMemoryHandle.hpp>



namespace bc {
namespace rhi {



class RHIVulkanImpl;
class RHIPoolMemoryHandle;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanImage
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanImage(
		RHIVulkanImpl						&	rhi_vulkan_impl,
		const VulkanImageCreateInfo			&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanImage();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline operator VkImage() { return vk_image; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkImage										vk_image								= VK_NULL_HANDLE;
	VkImageView									vk_image_view							= VK_NULL_HANDLE;

	RHIPoolMemoryHandle							image_memory_handle;

	RHIVulkanImpl							&	rhi_vulkan_impl;

	math::Vector2u								image_size								= {};
	List<math::Vector2u>						mip_levels;

	VkFormat									image_format							= {};
	VkImageAspectFlagBits						image_aspect_mask						= {};

	bool										is_sparse								= {};
	bool										is_host_visible							= {};
};



} // rhi
} // bc
