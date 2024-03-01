#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/Vector.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vertex
{
	math::Vec4f32											position;
	math::Vec4f32											normal;
	math::Vec4f32											color;
	math::Vec2f32											uv;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vertex														MakeVertex(
	math::Vec4f32											position,
	math::Vec4f32											normal,
	math::Vec4f32											color,
	math::Vec2f32											uv
) noexcept;



} // scene
} // bc
