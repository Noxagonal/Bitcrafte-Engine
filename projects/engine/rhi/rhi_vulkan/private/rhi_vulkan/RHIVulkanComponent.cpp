
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::RHIVulkanComponent(
	const RHIComponentCreateInfo & create_info
) :
	RHIComponent( create_info )
{
	rhi_vulkan_impl = CreateRHIVulkanImpl( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::~RHIVulkanComponent()
{
	rhi_vulkan_impl = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIVulkanComponent::Start(
	const RHIComponentStartInfo & rhi_start_info
)
{
	rhi_vulkan_impl->Start( rhi_start_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::RHIVulkanImpl> bc::rhi::RHIVulkanComponent::CreateRHIVulkanImpl(
	const RHIComponentCreateInfo & create_info
)
{
	return MakeUniquePtr<RHIVulkanImpl>( create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::Text> bc::rhi::RHIVulkanComponent::GetGraphicsCardList()
{
	auto physical_device_list = rhi_vulkan_impl->GetVulkanInstance().GetPhysicalDeviceList();
	auto result = List<Text>();
	result.Reserve( physical_device_list.Size() );
	for( auto & p : physical_device_list )
	{
		result.PushBack( p.GetProperties().properties.properties.deviceName );
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int64_t bc::rhi::RHIVulkanComponent::GetPrimaryGraphicsCardIndex()
{
	return rhi_vulkan_impl->GetBestPhysicalDevice();
}
