#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/AABB.hpp>

#include <scene/mesh/Vertex.hpp>
#include <scene/mesh/Meshlet.hpp>
#include <core/containers/List.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Represents a mesh in RAM.
///
/// Represents a single mesh stored in RAM. Contains Vertex and Index lists, or optionally to vertex indices, meshlet data.
/// This is typically the lowest level class before submitting mesh data to the GPU.
class Mesh
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class Type : u8
	{
		NONE					= 0,
		VERTICES,
		MESHLET,				// TODO: Meshlets are not implemented yet!
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct mesh using vertices and meshlets.
	///
	/// Use this constructor to create a mesh that uses meshlets and newer GPU mesh shaders. Mesh shaders are more modern way of
	/// handling mesh data in the GPU that replaces the traditional GPU vertex, geometry and tessellation shader stages. This may
	/// perform better than the traditional pipeline, however, it is not as straight forward.
	///
	/// @param vertex_list
	/// List of vertices that are copied over to internal vertex list.
	///
	/// @param meshlet_list
	/// List of meshlets that are copied over to internal meshlet list.
	Mesh(
		const List<Vertex>									&	vertex_list,
		const List<Meshlet>									&	meshlet_list
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct mesh using vertices and meshlets.
	///
	/// Use this constructor to create a mesh that uses traditional vertex indices. This option is much more simple to use than
	/// meshlets but may not perform as good.
	///
	/// @param vertex_list
	/// List of vertices that are copied over to internal vertex list.
	///
	/// @param index_list
	/// List of vertex indices that are copied over to internal index list.
	Mesh(
		const List<Vertex>									&	vertex_list,
		const List<u32>										&	index_list
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~Mesh();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<Vertex>												vertex_list;
	List<Meshlet>												meshlet_list;
	List<u32>													index_list;
	Type														type					= Type::NONE;

	math::AABB3f64												aabb					= {};
};



} // scene
} // bc
