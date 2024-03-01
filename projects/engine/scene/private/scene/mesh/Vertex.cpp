
#include <scene/PreCompiledHeader.hpp>
#include <scene/mesh/Vertex.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Vertex bc::scene::MakeVertex(
	math::Vector4f			position,
	math::Vector4f			normal,
	math::Vector4f			color,
	math::Vector2f			uv
) noexcept
{
	auto result = Vertex();
	result.position		= position;
	result.normal		= normal;
	result.color		= color;
	result.uv			= uv;
	return result;
}
