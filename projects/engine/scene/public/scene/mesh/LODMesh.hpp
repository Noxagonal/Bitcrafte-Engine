#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <scene/mesh/MeshData.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Represents a mesh on RAM.
///
/// This is a storage class meant to store multiple mesh LOD levels. This is the primary handle to accessing a mesh in engine.
class LODMesh
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LODMesh();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~LODMesh();

private:
};



} // scene
} // bc