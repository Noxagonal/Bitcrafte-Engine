
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/RHIVulkanComponent.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanComponent::RHIVulkanComponent(
	::bc::window_manager::WindowManagerComponent&	window_manager_component,
	const RHIComponentCreateInfo&					create_info
)
:
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
void bc::rhi::RHIVulkanComponent::Start( const RHIComponentStartInfo& rhi_start_info )
{
	rhi_vulkan_impl->Start( rhi_start_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIVulkanComponent::CreateRHIVulkanImpl(
	window_manager::WindowManagerComponent&		window_manager_component,
	const RHIComponentCreateInfo&				create_info
) -> UniquePtr<RHIVulkanImpl>
{
	return MakeUniquePtr<RHIVulkanImpl>( window_manager_component, create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIVulkanComponent::GetGraphicsCardList() -> List<Text>
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
auto bc::rhi::RHIVulkanComponent::GetPrimaryGraphicsCardIndex() -> i64
{
	return rhi_vulkan_impl->GetBestPhysicalDevice();
}
