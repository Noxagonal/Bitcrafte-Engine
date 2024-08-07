#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/utility/concepts/CallableConcepts.hpp>
#include <core/containers/List.hpp>
#include <core/containers/UniquePtr.hpp>

#include <scene/mesh/Mesh.hpp>
#include <scene/mesh/LODMesh.hpp>



namespace bc {
namespace scene {

class LODMesh;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides runtime storage for meshes in RAM.
///
/// This class is used to store, copy and access mesh data. This is useful when mesh data is shared between objects, no more
/// than 1 copy is kept in RAM. eg. Static meshes.
///
/// Mesh data may also be duplicated in cases where mesh data must be unique per object that needs it. eg. Dynamic meshes that
/// are modified by the CPU.
class MeshStorage
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MeshStorage();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~MeshStorage();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<utility::CallableWithParameters<u32, LODMesh&>		CallbackType>
	void											CreateLODMesh(
		u32											lod_level_count,
		CallbackType							&&	lod_callback
	)
	{
		auto lod_mesh = CreateNewLODMesh( lod_level_count );
		for( u32 i = 0; i < lod_level_count; ++i )
		{
			auto & lod_level = lod_mesh.GetLODLevel( i );
			lod_callback( i, lod_level.mesh );
		}
		FinalizeLODMesh( lod_mesh );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LODMesh										&	CreateNewLODMesh(
		u32											lod_level_count
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void											DestroyLODMesh(
		LODMesh									&	lod_mesh
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void											FinalizeLODMesh(
		LODMesh									&	lod_mesh
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<UniquePtr<LODMesh>>						lod_mesh_list;
};



} // scene
} // bc
