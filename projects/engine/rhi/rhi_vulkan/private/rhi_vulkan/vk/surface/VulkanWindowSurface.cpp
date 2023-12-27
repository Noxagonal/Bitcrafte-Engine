
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/surface/VulkanWindowSurface.hpp>

#include <rhi_vulkan/PreCompiledHeader.hpp>

#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>
#include <window_manager/window/Window.hpp>
#include <window_manager/platform/handles/WindowPlatformHandles.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanWindowSurface::VulkanWindowSurface(
	RHIVulkanImpl				&	rhi_vulkan_impl,
	window_manager::Window		*	window
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	#if BITCRAFTE_WINDOW_MANAGER_WIN32

	auto win32_handles = reinterpret_cast<window_manager::WindowManagerWin32PlatformHandles*>( window->GetPlatformSpecificHandles() );
	assert( win32_handles );
	assert( win32_handles->structure_type == window_manager::WindowManagerPlatformHandlesStructureType::WINDOW_MANAGER_WIN32 );

	auto surface_create_info = VkWin32SurfaceCreateInfoKHR {};
	surface_create_info.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_create_info.pNext		= nullptr;
	surface_create_info.flags		= 0;
	surface_create_info.hinstance	= win32_handles->hInstance;
	surface_create_info.hwnd		= win32_handles->hWnd;
	vkCreateWin32SurfaceKHR( rhi_vulkan_impl.GetVulkanInstance(), &surface_create_info, rhi_vulkan_impl.GetMainThreadAllocationCallbacks(), &vk_surface );

	#elif BITCRAFTE_WINDOW_MANAGER_WAYLAND

	TODO;

	#else
	#error "Please add window manager specific vulkan window surface creation code here"
	#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanWindowSurface::~VulkanWindowSurface()
{
	vkDestroySurfaceKHR( rhi_vulkan_impl.GetVulkanInstance(), vk_surface, rhi_vulkan_impl.GetMainThreadAllocationCallbacks() );
}
