
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/window_context/WindowContext.hpp>

#include <rhi_vulkan/vk/surface/VulkanWindowSurface.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::WindowContext::WindowContext(
	RHIVulkanImpl				&	rhi_vulkan_impl,
	window_manager::Window		*	window
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	window( window )
{
	window_surface = MakeUniquePtr<VulkanWindowSurface>( rhi_vulkan_impl, window );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::WindowContext::~WindowContext()
{
	window_surface = nullptr;
}
