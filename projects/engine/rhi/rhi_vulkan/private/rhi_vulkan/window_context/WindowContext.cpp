
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/window_context/WindowContext.hpp>

#include <rhi_vulkan/vk/surface/VulkanWindowSurface.hpp>
#include <rhi_vulkan/vk/swapchain/VulkanSwapchain.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::WindowContext::WindowContext(
	RHIVulkanImpl&					rhi_vulkan_impl,
	window_manager::Window*			window,
	const WindowContextCreateInfo&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	window( window )
{
	window_surface	= MakeUniquePtr<VulkanWindowSurface>( rhi_vulkan_impl, window );
	swapchain		= MakeUniquePtr<VulkanSwapchain>( rhi_vulkan_impl, *window_surface, create_info.swapchain_create_info, nullptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::WindowContext::~WindowContext()
{
	swapchain			= nullptr;
	window_surface		= nullptr;
}
