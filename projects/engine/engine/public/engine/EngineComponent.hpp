#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/UniquePtr.hpp>

#include <engine/EngineComponentCreateInfo.hpp>



namespace bc {

namespace window { class WindowComponent; }

namespace engine {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API EngineComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EngineComponent(
		const EngineComponentCreateInfo							&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~EngineComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	window::WindowComponent										*	GetWindowComponent();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EngineComponentCreateInfo										create_info_copy;

	UniquePtr<window::WindowComponent>								window_component;
};



EngineComponent * GetEngine();



} // engine
} // bc
