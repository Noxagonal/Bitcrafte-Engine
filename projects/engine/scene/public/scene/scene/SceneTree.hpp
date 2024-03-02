#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides a 3D space for entities and entity groups. Also known as a level or map.
///
/// Provides the space where entities or entity groups are loaded into. This is typically the game world where the player is
/// loaded into, but can also be used for UI, or distant landscapes, or separate rooms where someone is talking to a camera.
/// Multiple scene trees can be loaded at runtime, but they cannot be mixed or combined into one, however they can render into
/// render target textures which can be used in other scene trees. Multiple scene trees is useful when you want to keep a clear
/// separation between world spaces. If you want to load chunks of the level in at runtime based on some condition, see
/// EntityGroup.
///
/// @note
/// A world can handle multiple scene trees at a time, however scene trees are individual, they cannot see other scene trees and
/// cannot directly communicate with other scene trees, this simplifies rendering. A scene tree can only show a render target
/// texture from another scene tree. If you want to conditionally load chunks of the level in based on some conditions, see
/// EntityGroup.
class SceneTree
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SceneTree();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~SceneTree();

private:
};



} // scene
} // bc
