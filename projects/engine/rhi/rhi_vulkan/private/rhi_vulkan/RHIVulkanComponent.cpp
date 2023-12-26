
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::RHIVulkanComponent(
	const RHIComponentCreateInfo & create_info
) :
	RHIComponent( create_info )
{
	implementation = CreateRHIVulkanImpl( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::~RHIVulkanComponent()
{
	implementation = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::RHIVulkanImpl> bc::rhi::RHIVulkanComponent::CreateRHIVulkanImpl(
	const RHIComponentCreateInfo & create_info
)
{
	return MakeUniquePtr<RHIVulkanImpl>( create_info );
}
