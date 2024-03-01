#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

// TODO: AABB
// #include <core/math/AABB.hpp>

#include <scene/mesh/Mesh.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Represents a complete mesh with all LOD levels on RAM.
///
/// This is a storage class meant to store multiple mesh LOD levels. This is the primary handle to accessing a mesh in engine.
class LODMesh
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct LODLevel
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Mesh												mesh;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// @brief
		/// Viewport coverage, once we fall under this coverage, we switch to the next lod level.
		f32													viewport_coverage				= 0.0f;

		// TODO: LODLevel extra information that could be implemented in the future. Below are a few examples.
		//BlendMode											blend_mode						= BlendMode::NONE;		// Blending mode to the next level.
		//MaterialInfo										material_info					= {};					// Material override info for this level.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LODMesh();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~LODMesh();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// LOD level list.
	///
	/// Index 0 is the highest quality LOD level.
	List<LODLevel>											lod_list;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	math::AABB3f64											aabb							= {};
};



} // scene
} // bc
