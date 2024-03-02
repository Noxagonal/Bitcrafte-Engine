#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace scene {



// Planning:
//
// In the engine there is only one world, which is mostly in the background for the game developer, and new worlds cannot be
// created by the game developer. It is mostly just for organizational purposes, eg. When saving a game, the entire world gets
// saved.
//
// The world can hold multiple scenes but scenes cannot see each other or otherwise communicate. Scenes may be primary or
// secondary. Primary scene is always loaded in when loading a game, secondary scenes may be loaded from another scene.
//
// The World contains a list of active RenderPipelines in memory. A RenderPipeline can be used to render different scenes into
// render targets by the world. A world will initiate a scene render based on which scenes are set as primary scenes, usually
// the game or editor scene. A scene can request a secondary scene to be loaded or unloaded alongside it or sometime during the
// game loop, and it can also ask to render from it, but otherwise they cannot interact with each other.
//
// A render target texture for example, contains information of which scene it should render and by using which RenderPipeline.
// The scene may be the same scene as destination output image is drawn for, or it may point to another scene. Regardless when a
// render target texture is drawn, it requests a scene to be loaded if it isn't already. Render target visibility is calculated
// automatically, if the render target is visible, its contents are drawn.
//
// A world also outputs a render target texture, which can be used to output to window surface. For multi-window situations like
// in the editor, a world can be drawn multiple times, each time a world is drawn, the primary scene is drawn, which will be the
// editor scene as it has the highest priority.
//
// Game view in Editor will be a render target texture. Editor will have different panels and based on window settings, editor
// scene can be drawn differently per window. An editor scene should keep track of each open window and their content, as well
// as the number of render target textures needed to draw all 3D views on all open windows.
//
// Render target texture size will determine the render size and changing that will either require a recreation of the
// RenderPipeline because of the internal_ G-buffers. A RenderPipeline would have an internal_ maximum size, and target output
// size.
//
// I think I covered most of this by now, and I'll probably figure out more details as I go but I think this is the direction I
// want to go.



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SceneComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SceneComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~SceneComponent();

private:

};



} // scene
} // bc
