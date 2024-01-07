#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace scene {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Entity is an object that can be placed in a scene to be rendered or act on the scene or surrounding objects in some way.
///
/// Entity is basically just a container that holds transformation info used in 3D space, and a collection of components. Entity
/// can be derived from to get more specialized functionality, however, whenever possible, consider using components to create
/// extra functionality for an entity.
class Entity
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Entity();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~Entity();

private:
};



} // scene
} // bc
