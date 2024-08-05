
#include <rhi/PreCompiledHeader.hpp>
#include <rhi/RHIComponent.hpp>



// Include RHI components.
#if BITCRAFTE_RHI_VULKAN
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#elif BITCRAFTE_RHI_METAL
#include <rhi_metal/RHIMetalComponent.hpp>
#else
#warning "No RHI component included, using dummy interface"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIComponent::RHIComponent(
	const RHIComponentCreateInfo & create_info
)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIComponent::~RHIComponent()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::RHIComponent> bc::rhi::CreateRHIComponent(
	window_manager::WindowManagerComponent			&	window_manager_component,
	const RHIComponentCreateInfo					&	create_info
)
{
	#if BITCRAFTE_RHI_VULKAN
	if( create_info.rhi_selection == RHISelection::VULKAN )
	{
		return MakeUniquePtr<rhi::RHIVulkanComponent>( window_manager_component, create_info );
	}
	#endif // BITCRAFTE_RHI_VULKAN

	#if BITCRAFTE_RHI_METAL
	if( create_info.rhi_selection == RHISelection::METAL )
	{
		return MakeUniquePtr<rhi::RHIVulkanComponent>( window_manager_component, create_info );
	}
	#endif // BITCRAFTE_RHI_METAL

	assert( false && "Couldn't create RHI component because no RHI component was compiled." );
	return {};
}
