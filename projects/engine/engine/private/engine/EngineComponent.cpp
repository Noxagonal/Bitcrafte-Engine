
#include <engine/PreCompiledHeader.hpp>

#include <engine/EngineComponent.hpp>
#include <core/CoreComponent.hpp>
#include <rhi/RHIComponent.hpp>
#include <window_manager/WindowManagerComponent.hpp>

#include <core/diagnostic/logger/Logger.hpp>



// Include RHI components.
#if BITCRAFTE_RHI_VULKAN
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#elif BITCRAFTE_RHI_METAL
#include <rhi_metal/RHIMetalComponent.hpp>
#else
#warning "No RHI component included, using dummy interface"
#endif



// Include window components.
#if BITCRAFTE_WINDOW_WIN32
#include <window_manager_win32/WindowManagerWin32Component.hpp>
#elif BITCRAFTE_WINDOW_WAYLAND
#include <window_manager_wayland/WindowManagerWaylandComponent.hpp>
#else
#warning "No Window component included, using dummy interface"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent * global_engine = nullptr;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent::EngineComponent(
	const EngineComponentCreateInfo & create_info
) :
	create_info_copy( create_info )
{
	global_engine = this;

	window_manager_component	= CreateWindowManagerComponent( create_info );
	rhi_component				= CreateRHIComponent( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent::~EngineComponent()
{
	rhi_component				= nullptr;
	window_manager_component	= nullptr;

	global_engine				= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::engine::EngineComponent::Run()
{
	window_manager_component->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WindowManagerComponent * bc::engine::EngineComponent::GetWindowManagerComponent()
{
	return window_manager_component.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIComponent * bc::engine::EngineComponent::GetRHIComponent()
{
	return rhi_component.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window_manager::WindowManagerComponent> bc::engine::EngineComponent::CreateWindowManagerComponent(
	const EngineComponentCreateInfo & create_info
)
{
	#if BITCRAFTE_WINDOW_WIN32
	return MakeUniquePtr<window_manager::WindowManagerWin32Component>( create_info.window_manager_create_info );

	#elif BITCRAFTE_WINDOW_WAYLAND
	return MakeUniquePtr<window_manager::WindowManagerWaylandComponent>( create_info.window_manager_create_info );

	#else
	bc::GetCore()->GetLogger()->LogWarning( U"Creating dummy window component" );
	return MakeUniquePtr<window_manager::WindowManagerComponent>( create_info.window_manager_create_info );

	#endif // Window manager selection
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::RHIComponent> bc::engine::EngineComponent::CreateRHIComponent(
	const EngineComponentCreateInfo & create_info
)
{
	#if BITCRAFTE_RHI_VULKAN
	if( create_info.rhi_selection == EngineComponentCreateInfoRHISelection::VULKAN )
	{
		return MakeUniquePtr<rhi::RHIVulkanComponent>( create_info.rhi_create_info );
	}
	#endif // BITCRAFTE_RHI_VULKAN

	#if BITCRAFTE_RHI_METAL
	if( create_info.rhi_selection == EngineComponentCreateInfoRHISelection::VULKAN )
	{
		return MakeUniquePtr<rhi::RHIVulkanComponent>( create_info.rhi_create_info );
	}
	#endif // BITCRAFTE_RHI_METAL

	bc::GetCore()->GetLogger()->LogWarning( U"Creating dummy RHI component" );
	return MakeUniquePtr<rhi::RHIComponent>( create_info.rhi_create_info );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent * bc::engine::GetEngine()
{
	return global_engine;
}
