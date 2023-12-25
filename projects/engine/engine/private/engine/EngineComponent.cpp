
#include <engine/PreCompiledHeader.hpp>

#include <engine/EngineComponent.hpp>
#include <core/CoreComponent.hpp>
#include <window/WindowComponent.hpp>
#include <rhi/RHIComponent.hpp>

#include <core/diagnostic/logger/Logger.hpp>



// Include RHIs.
#if BITCRAFTE_RHI_VULKAN
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#elif BITCRAFTE_RHI_METAL
#include <rhi_metal/RHIMetalComponent.hpp>
#else
#warning "No RHI included, using dummy interface"
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

	window_component	= CreateWindowComponent( create_info );
	rhi_component		= CreateRHIComponent( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent::~EngineComponent()
{
	rhi_component		= nullptr;
	window_component	= nullptr;

	global_engine		= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::engine::EngineComponent::Run()
{
	window_component->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowComponent * bc::engine::EngineComponent::GetWindowComponent()
{
	return window_component.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIComponent * bc::engine::EngineComponent::GetRHIComponent()
{
	return rhi_component.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::window::WindowComponent> bc::engine::EngineComponent::CreateWindowComponent(
	const EngineComponentCreateInfo & create_info
)
{
	return MakeUniquePtr<window::WindowComponent>( create_info.window_create_info );
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

	bc::GetCore()->GetLogger()->LogWarning( U"Creating dummy RHI" );
	return MakeUniquePtr<rhi::RHIComponent>( create_info.rhi_create_info );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::engine::EngineComponent * bc::engine::GetEngine()
{
	return global_engine;
}
