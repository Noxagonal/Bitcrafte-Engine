#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi/color/ColorFormat.hpp>
#include <core/math/Vector.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GBufferCreateInfo
{
	ColorFormat		color_format	= {};
	math::Vec2u32	size			= {};
	u32				sample_count	= {};
};




} // rhi
} // bc
