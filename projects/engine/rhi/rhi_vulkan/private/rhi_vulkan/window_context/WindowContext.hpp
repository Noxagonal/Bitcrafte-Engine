#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/window_context/WindowContextCreateInfo.hpp>



namespace bc {

namespace window_manager { class Window; }

namespace rhi {



class RHIVulkanImpl;
class VulkanWindowSurface;
class VulkanSwapchain;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WindowContext
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowContext(
		RHIVulkanImpl										&	rhi_vulkan_impl,
		window_manager::Window								*	window,
		const WindowContextCreateInfo						&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~WindowContext();

	inline window_manager::Window							*	GetWindow() { return window; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl											&	rhi_vulkan_impl;
	window_manager::Window									*	window;

	UniquePtr<VulkanWindowSurface>								window_surface;
	UniquePtr<VulkanSwapchain>									swapchain;
};



} // rhi
} // bc
