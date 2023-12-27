#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {

namespace window_manager { class Window; }

namespace rhi {

class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WindowContext
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowContext(
		RHIVulkanImpl										&	rhi_vulkan_impl,
		window_manager::Window								*	window
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~WindowContext();

	inline window_manager::Window							*	GetWindow() { return window; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl											&	rhi_vulkan_impl;
	window_manager::Window									*	window;
};



} // rhi
} // bc
