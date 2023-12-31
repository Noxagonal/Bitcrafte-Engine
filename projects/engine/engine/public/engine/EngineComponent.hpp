#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/UniquePtr.hpp>

#include <engine/EngineComponentCreateInfo.hpp>
#include <engine/EngineComponentStartInfo.hpp>



namespace bc {

namespace window_manager { class WindowManagerComponent; }
namespace rhi { class RHIComponent; }

namespace engine {

struct EngineComponentStartInfo;



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
	void															Start(
		const EngineComponentStartInfo							&	engine_start_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void															Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	window_manager::WindowManagerComponent						*	GetWindowManagerComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	rhi::RHIComponent											*	GetRHIComponent();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<window_manager::WindowManagerComponent>				CreateWindowManagerComponent(
		const EngineComponentCreateInfo							&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<rhi::RHIComponent>									CreateRHIComponent(
		const EngineComponentCreateInfo							&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EngineComponentCreateInfo										create_info_copy;

	UniquePtr<window_manager::WindowManagerComponent>				window_manager_component;
	UniquePtr<rhi::RHIComponent>									rhi_component;
};



EngineComponent * GetEngine();



} // engine
} // bc
