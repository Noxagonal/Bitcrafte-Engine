
#include <scene/PreCompiledHeader.hpp>
#include <scene/mesh/Vertex.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Vertex bc::scene::MakeVertex(
	math::Vec4f32			position,
	math::Vec4f32			normal,
	math::Vec4f32			color,
	math::Vec2f32			uv
) noexcept
{
	auto result = Vertex();
	result.position		= position;
	result.normal		= normal;
	result.color		= color;
	result.uv			= uv;
	return result;
}
