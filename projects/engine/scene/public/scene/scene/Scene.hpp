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
/// Multiple scenes can be loaded at runtime, but they cannot be mixed or combined into one, however they can render into render
/// target textures which can be used in other scenes. Multiple scenes is useful when you want to keep a clear separation
/// between world spaces. If you want to load chunks of the level in at runtime based on some condition, see EntityGroup.
///
/// @note
/// A world can handle multiple scenes at a time, however scenes are individual, they cannot see other scenes and cannot
/// directly communicate with other scenes, this simplifies rendering. A scene can only show a render target texture from
/// another scene. If you want to conditionally load chunks of the level in based on some conditions, see EntityGroup.
class Scene
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Scene();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~Scene();

private:
};



} // scene
} // bc
