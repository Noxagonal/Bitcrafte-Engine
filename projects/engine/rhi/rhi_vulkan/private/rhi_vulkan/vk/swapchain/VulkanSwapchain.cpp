
#include <rhi_vulkan/PreCompiledHeader.hpp>

#include <rhi_vulkan/vk/swapchain/VulkanSwapchain.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>

#include <rhi_vulkan/vk/surface/VulkanWindowSurface.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>
#include <core/math/Vector.hpp>



bc::rhi::VulkanSwapchain::VulkanSwapchain(
	RHIVulkanImpl&						rhi_vulkan_impl,
	VulkanWindowSurface&				window_surface,
	const VulkanSwapchainCreateInfo&	swapchain_create_info,
	VulkanSwapchain*					old_swapchain
)
:
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	// Get new surface capabilities as window extent might have changed
	auto GetCurrentSurfaceCapabilities = [ &rhi_vulkan_impl, &window_surface ]()
		{
			auto new_surface_capabilities = VkSurfaceCapabilitiesKHR {};
			BAssertVkResult( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
				rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
				window_surface,
				&new_surface_capabilities
			) );

			return new_surface_capabilities;
		};

	auto GetSwapchainMinimumImageCount = [ &swapchain_create_info ]( const VkSurfaceCapabilitiesKHR & surface_capabilities )
		{
			auto image_count = u32 {};
			if( swapchain_create_info.use_v_sync )
			{
				image_count = 2;	// Vsync enabled, we only need 2 swapchain images
			}
			else
			{
				image_count = 3;	// Vsync disabled, we should use at least 3 images
			}

			if( surface_capabilities.maxImageCount != 0 )
			{
				image_count = std::min( image_count, surface_capabilities.maxImageCount );
			}
			image_count = std::max( image_count, surface_capabilities.minImageCount );
			if( image_count == 0 )
			{
				diagnostic::Throw( diagnostic::Exception( U"Failed to acquire Vulkan swapchain image count" ) );
			}
			return image_count;
		};

	auto GetPresentMode = [ &rhi_vulkan_impl, &window_surface, &swapchain_create_info ]()
		{
			auto present_mode = VkPresentModeKHR {};

			if( swapchain_create_info.use_v_sync )
			{
				// Using VSync we should use FIFO, this mode is required to be supported so we can rely on that and just use it without checking
				return VK_PRESENT_MODE_FIFO_KHR;
			}

			List<VkPresentModeKHR> surface_present_modes;
			{
				u32 present_mode_count = 0;
				BAssertVkResult( vkGetPhysicalDeviceSurfacePresentModesKHR(
					rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
					window_surface,
					&present_mode_count,
					nullptr
				) );
				surface_present_modes.Resize( present_mode_count );
				BAssertVkResult( vkGetPhysicalDeviceSurfacePresentModesKHR(
					rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice(),
					window_surface,
					&present_mode_count,
					surface_present_modes.Data()
				) );
			}

			// Not using VSync, we should try mailbox first and immediate second, fall back to FIFO if neither is supported
			if( surface_present_modes.Find( VK_PRESENT_MODE_MAILBOX_KHR ) )		return VK_PRESENT_MODE_MAILBOX_KHR;
			if( surface_present_modes.Find( VK_PRESENT_MODE_IMMEDIATE_KHR ) )	return VK_PRESENT_MODE_IMMEDIATE_KHR;
			return VK_PRESENT_MODE_FIFO_KHR;
		};

	auto CreateSwapchain = [ &rhi_vulkan_impl, &window_surface, &swapchain_create_info, &old_swapchain ](
		u32										swapchain_minimum_image_count,
		const VkSurfaceCapabilitiesKHR		&	surface_capabilities,
		const VkPresentModeKHR				&	present_mode
		) -> VkSwapchainKHR
		{
			auto old_vk_swapchain = VkSwapchainKHR {};
			if( old_swapchain )
			{
				old_vk_swapchain = old_swapchain->GetVkSwapchain();
			}

			VkSwapchainCreateInfoKHR create_info {};
			create_info.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			create_info.pNext					= nullptr;
			create_info.flags					= 0;
			create_info.surface					= window_surface;
			create_info.minImageCount			= swapchain_minimum_image_count;
			create_info.imageFormat				= window_surface.GetPreferredSurfaceFormat().format;
			create_info.imageColorSpace			= window_surface.GetPreferredSurfaceFormat().colorSpace;
			create_info.imageExtent				= surface_capabilities.currentExtent;
			create_info.imageArrayLayers		= 1; // Change this if we ever want to use VR.
			create_info.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			create_info.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
			create_info.queueFamilyIndexCount	= 0;
			create_info.pQueueFamilyIndices		= nullptr;
			create_info.preTransform			= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			create_info.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			create_info.presentMode				= present_mode;
			create_info.clipped					= VK_FALSE;
			create_info.oldSwapchain			= old_vk_swapchain;

			auto swapchain = VkSwapchainKHR {};
			BAssertVkResult( vkCreateSwapchainKHR(
				rhi_vulkan_impl.GetVulkanDevice(),
				&create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&swapchain
			) );

			return swapchain;
		};

	auto GetSwapchainImages = [ &rhi_vulkan_impl ]( VkSwapchainKHR vk_swapchain ) -> List<VkImage>
		{
			auto swapchain_images		= List<VkImage> {};
			auto swapchain_image_count	= u32 {};
			BAssertVkResult( vkGetSwapchainImagesKHR(
				rhi_vulkan_impl.GetVulkanDevice(),
				vk_swapchain,
				&swapchain_image_count,
				nullptr
			) );
			swapchain_images.Resize( swapchain_image_count );
			BAssertVkResult( vkGetSwapchainImagesKHR(
				rhi_vulkan_impl.GetVulkanDevice(),
				vk_swapchain,
				&swapchain_image_count,
				swapchain_images.Data()
			) );
			return swapchain_images;
		};

	auto surface_capabilities			= GetCurrentSurfaceCapabilities();
	auto swapchain_minimum_image_count	= GetSwapchainMinimumImageCount( surface_capabilities );
	auto swapchain_present_mode			= GetPresentMode();
	vk_swapchain						= CreateSwapchain( swapchain_minimum_image_count, surface_capabilities, swapchain_present_mode );
	swapchain_vk_image_list				= GetSwapchainImages( vk_swapchain );
}

bc::rhi::VulkanSwapchain::~VulkanSwapchain()
{
	vkDestroySwapchainKHR(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_swapchain,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
}
