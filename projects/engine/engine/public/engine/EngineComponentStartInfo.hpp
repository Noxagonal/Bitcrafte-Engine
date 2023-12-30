#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/RHIComponentStartInfo.hpp>



namespace bc {
namespace engine {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EngineComponentStartInfo
{
	rhi::RHIComponentStartInfo									rhi_start_info				= {};
};



} // engine
} // bc
