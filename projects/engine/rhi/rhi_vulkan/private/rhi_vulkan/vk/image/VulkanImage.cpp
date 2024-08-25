
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/image/VulkanImage.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <rhi_vulkan/vk/swapchain/VulkanSwapchain.hpp>

#include <rhi_vulkan/utility/format/FormatHelpers.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanImage::VulkanImage(
	RHIVulkanImpl&					rhi_vulkan_impl,
	const VulkanImageCreateInfo&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto IsImageSparse = [ &create_info ]()
		{
			return
				create_info.image_flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT ||
				create_info.image_flags & VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT ||
				create_info.image_flags & VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
		};

	auto GetImageSize = [ &rhi_vulkan_impl, &create_info ]() -> math::Vec2u32
		{
			auto result = create_info.image_size;
			auto max_image_extent = rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice().GetProperties().properties.limits.maxImageDimension2D;
			result = { std::min( result.x, max_image_extent ), std::min( result.y, max_image_extent ) };
			return result;
		};

	auto CalculateMipLevels = [ &create_info ]( math::Vec2u32 full_size ) -> List<math::Vec2u32>
		{
			auto result = List<math::Vec2u32> {};
			result.Reserve( 16 );

			auto current = full_size;
			result.PushBack( current );
			if( !create_info.use_mip_maps ) return result;
			while( current.x != 1 && current.y != 1 )
			{
				current.x = std::max( 1ULL, current.x / 2ULL );
				current.y = std::max( 1ULL, current.y / 2ULL );
				result.PushBack( current );
			}
			return result;
		};

	auto CreateImage = [ &create_info, &rhi_vulkan_impl ](
		math::Vec2u32	size,
		u32				mip_level_count,
		VkImageTiling	image_tiling_info
		)
		{
			void * image_create_info_pNext		= nullptr;
			auto swapchain_usage_create_info	= VkImageSwapchainCreateInfoKHR {};
			if( create_info.image_usage_in_swapchain )
			{
				swapchain_usage_create_info.sType		= VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR;
				swapchain_usage_create_info.pNext		= nullptr;
				swapchain_usage_create_info.swapchain	= *create_info.image_usage_in_swapchain;
				image_create_info_pNext = &swapchain_usage_create_info;
			}

			auto image_create_info = VkImageCreateInfo {};
			image_create_info.sType					= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			image_create_info.pNext					= image_create_info_pNext;
			image_create_info.flags					= create_info.image_flags;
			image_create_info.imageType				= VK_IMAGE_TYPE_2D;
			image_create_info.format				= create_info.image_format;
			image_create_info.extent				= { u32( size.x ), u32( size.y ), 1 };
			image_create_info.mipLevels				= mip_level_count;
			image_create_info.arrayLayers			= create_info.image_array_layers;
			image_create_info.samples				= create_info.image_samples;
			image_create_info.tiling				= image_tiling_info;
			image_create_info.usage					= create_info.image_usage;
			image_create_info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
			image_create_info.queueFamilyIndexCount	= 0;
			image_create_info.pQueueFamilyIndices	= nullptr;
			image_create_info.initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED;

			auto result = VkImage {};
			BAssertVkResult( vkCreateImage(
				rhi_vulkan_impl.GetVulkanDevice(),
				&image_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );
			return result;
		};

	auto AllocateMemory = [ &rhi_vulkan_impl ](
		VkImage			image,
		VkImageTiling	image_tiling_info,
		bool			is_host_visible
		)
		{
			auto & memory_pool = rhi_vulkan_impl.GetRHIMemoryPool();
			auto result = memory_pool.AllocateImageMemory(
				image,
				image_tiling_info,
				is_host_visible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);
			return result;
		};

	auto BindImageMemory = [ &rhi_vulkan_impl ](
		VkImage					image,
		RHIPoolMemoryHandle	&	memory_handle
		)
		{
			auto bind_info = VkBindImageMemoryInfo {};
			bind_info.sType			= VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
			bind_info.pNext			= nullptr;
			bind_info.image			= image;
			bind_info.memory		= memory_handle.GetVulkanMemory();
			bind_info.memoryOffset	= memory_handle.GetOffset();
			vkBindImageMemory2(
				rhi_vulkan_impl.GetVulkanDevice(),
				1,
				&bind_info
			);
		};

	auto CreateImageView = [ &create_info, &rhi_vulkan_impl ](
		VkImage						image,
		List<math::Vec2u32>		&	mip_levels,
		VkImageAspectFlagBits		image_aspect_mask
		)
		{
			auto view_type = VK_IMAGE_VIEW_TYPE_2D;
			if( create_info.image_array_layers > 1 )
			{
				view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			}

			auto image_view_create_info = VkImageViewCreateInfo {};
			image_view_create_info.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_create_info.pNext				= nullptr;
			image_view_create_info.flags				= 0;
			image_view_create_info.image				= image;
			image_view_create_info.viewType				= view_type;
			image_view_create_info.format				= create_info.image_format;
			image_view_create_info.components.r			= VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.g			= VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.b			= VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.a			= VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.subresourceRange.aspectMask			= image_aspect_mask;
			image_view_create_info.subresourceRange.layerCount			= create_info.image_array_layers;
			image_view_create_info.subresourceRange.baseArrayLayer		= 0;
			image_view_create_info.subresourceRange.levelCount			= u32( mip_levels.Size() );
			image_view_create_info.subresourceRange.baseMipLevel		= 0;
			auto result = VkImageView {};
			BAssertVkResult( vkCreateImageView(
				rhi_vulkan_impl.GetVulkanDevice(),
				&image_view_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );
			return result;
		};

	image_format	= create_info.image_format;

	is_sparse		= IsImageSparse();
	image_size		= GetImageSize();
	mip_levels		= CalculateMipLevels( image_size );
	is_host_visible	= create_info.is_host_visible;

	auto image_tiling_info = is_host_visible ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;

	vk_image = CreateImage(
		image_size,
		u32( mip_levels.Size() ),
		image_tiling_info
	);

	image_memory_handle = AllocateMemory(
		vk_image,
		image_tiling_info,
		is_host_visible
	);

	image_aspect_mask = CreateAspectMaskFromFormat( create_info.image_format );

	BindImageMemory( vk_image, image_memory_handle );

	vk_image_view = CreateImageView( vk_image, mip_levels, image_aspect_mask );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanImage::~VulkanImage()
{
	vkDestroyImageView(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_image_view,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	vkDestroyImage(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_image,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	image_memory_handle = nullptr;
}
