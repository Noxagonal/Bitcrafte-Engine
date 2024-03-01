#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>
#include <core/math/Vector.hpp>
#include <scene/mesh/Vertex.hpp>
#include <core/containers/Array.hpp>



namespace bc {
namespace scene {
namespace internal {

constexpr u32 MESHLET_MAX_INDICES = 64;

} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Meshlet
{
	math::Vec4f32													mean_normal						= {};
	math::Vec4f32													center_point					= {};
	f32																mean_normal_cone_dot			= {};
	f32																radius							= {};
	u32																vertex_count					= {};
	Array<u32, internal::MESHLET_MAX_INDICES>						vertex_index_list;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Meshlet																MakeMeshlet(
	const List<Vertex>											&	vertex_list,
	ListView<u32>													vertex_index_list
);



} // scene
} // bc
