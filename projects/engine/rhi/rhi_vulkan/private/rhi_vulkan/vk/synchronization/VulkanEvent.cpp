
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/synchronization/VulkanEvent.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanEvent::VulkanEvent(
	RHIVulkanImpl		&	rhi_vulkan_impl
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto create_info = VkEventCreateInfo {};
	create_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	create_info.pNext	= nullptr;
	create_info.flags	= 0;
	BAssertVkResult( vkCreateEvent(
		rhi_vulkan_impl.GetVulkanDevice(),
		&create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_event
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanEvent::~VulkanEvent()
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanEvent & bc::rhi::VulkanEvent::operator=(
	VulkanEvent && other
) noexcept
{
	MoveOther( std::move( other ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanEvent::Clear()
{
	if( vk_event == VK_NULL_HANDLE ) return;
	vkDestroyEvent(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_event,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	vk_event = VK_NULL_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanEvent::MoveOther(
	VulkanEvent && other
)
{
	std::swap( vk_event, other.vk_event );
}
