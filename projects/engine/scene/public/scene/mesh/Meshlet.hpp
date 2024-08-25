#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>
#include <core/math/Vector.hpp>
#include <scene/mesh/Vertex.hpp>
#include <core/containers/Array.hpp>



namespace bc {
namespace scene {
namespace internal_ {

constexpr u32 MESHLET_MAX_INDICES = 64;

} // internal_



using MeshletIndexArray = Array<u32, internal_::MESHLET_MAX_INDICES>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Meshlet
{
	math::Vec4f32		mean_normal				= {};
	math::Vec4f32		center_point			= {};
	f32					mean_normal_cone_dot	= {};
	f32					radius					= {};
	u32					vertex_count			= {};
	MeshletIndexArray	vertex_index_list;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto MakeMeshlet(
	const List<Vertex>&		vertex_list,
	ListView<u32>			vertex_index_list
) -> Meshlet;



} // scene
} // bc
