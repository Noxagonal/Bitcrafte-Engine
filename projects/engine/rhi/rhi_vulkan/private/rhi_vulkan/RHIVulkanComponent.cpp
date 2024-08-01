
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::RHIVulkanComponent(
	window_manager::WindowManagerComponent		&	window_manager_component,
	const RHIComponentCreateInfo				&	create_info
) :
	RHIComponent( create_info )
{
	rhi_vulkan_impl = CreateRHIVulkanImpl( window_manager_component, create_info );
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
	window_manager::WindowManagerComponent		&	window_manager_component,
	const RHIComponentCreateInfo				&	create_info
)
{
	return MakeUniquePtr<RHIVulkanImpl>( window_manager_component, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::Text> bc::rhi::RHIVulkanComponent::GetGraphicsCardList()
{
	auto & physical_device_list = rhi_vulkan_impl->GetVulkanInstance().GetPhysicalDeviceList();
	auto result = List<Text>();
	result.Reserve( physical_device_list.Size() );
	for( auto & p : physical_device_list )
	{
		result.PushBack( p.GetProperties().properties.deviceName );
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::i64 bc::rhi::RHIVulkanComponent::GetPrimaryGraphicsCardIndex()
{
	return rhi_vulkan_impl->GetBestPhysicalDevice();
}
