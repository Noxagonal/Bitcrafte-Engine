#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/linear_algebra/Vector.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vertex
{
	math::Vector4f		position;
	math::Vector4f		normal;
	math::Vector4f		color;
	math::Vector2f		UV;
};



} // scene
} // bc
