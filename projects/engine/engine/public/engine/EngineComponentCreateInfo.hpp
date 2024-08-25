#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <window_manager/WindowManagerComponentCreateInfo.hpp>
#include <rhi/RHIComponentCreateInfo.hpp>



namespace bc {
namespace engine {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EngineComponentCreateInfo
{
	window_manager::WindowManagerComponentCreateInfo	window_manager_create_info	= {};

	rhi::RHIComponentCreateInfo							rhi_create_info				= {};
};



} // engine
} // bc
