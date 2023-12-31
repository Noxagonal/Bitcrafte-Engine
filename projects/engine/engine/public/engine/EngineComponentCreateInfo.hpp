#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window_manager/WindowManagerComponentCreateInfo.hpp>
#include <rhi/RHIComponentCreateInfo.hpp>



namespace bc {
namespace engine {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class EngineComponentCreateInfoRHISelection
{
	NONE			= 0,	///< Dummy device, may be used for testing.
	VULKAN,					///< Use Vulkan API.
	METAL,					///< Use Apple Metal API.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EngineComponentCreateInfo
{
	window_manager::WindowManagerComponentCreateInfo		window_manager_create_info			= {};

	EngineComponentCreateInfoRHISelection					rhi_selection					= EngineComponentCreateInfoRHISelection::NONE;
	rhi::RHIComponentCreateInfo								rhi_create_info					= {};
};



} // engine
} // bc
