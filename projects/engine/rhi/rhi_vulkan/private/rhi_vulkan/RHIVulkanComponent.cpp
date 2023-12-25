
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#include <rhi_vulkan/vulkan_implementation/VulkanImplementation.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::RHIVulkanComponent(
	const RHIComponentCreateInfo & create_info
) :
	RHIComponent( create_info )
{
	implementation = CreateVulkanImplementation( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::~RHIVulkanComponent()
{
	implementation = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::VulkanImplementation> bc::rhi::RHIVulkanComponent::CreateVulkanImplementation(
	const RHIComponentCreateInfo & create_info
)
{
	return MakeUniquePtr<VulkanImplementation>( create_info );
}
