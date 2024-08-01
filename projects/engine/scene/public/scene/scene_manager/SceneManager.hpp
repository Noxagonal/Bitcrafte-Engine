#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>



namespace bc {
namespace scene {

class SceneDataManager;
class SceneTree;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Scene manager is the top level manager meant to handle everything happening in a scene.
///
/// This class provides a central location for scene management. It primarily acts as a container for multiple other managers
/// that specialize in their areas, like data management. Most tasks can be directly delegated to these sub-managers.
class SceneManager
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SceneManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~SceneManager();

private:

	UniquePtr<SceneDataManager>							scene_data_manager;
	List<UniquePtr<SceneTree>>							scene_tree_list;
};



} // scene
} // bc
