
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/synchronization/VulkanFence.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanFence::VulkanFence(
	RHIVulkanImpl&	rhi_vulkan_impl,
	bool			initially_signaled
)
:
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto create_info = VkFenceCreateInfo {};
	create_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	create_info.pNext	= nullptr;
	create_info.flags	= initially_signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
	BAssertVkResult( vkCreateFence(
		rhi_vulkan_impl.GetVulkanDevice(),
		&create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_fence
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanFence::~VulkanFence()
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::VulkanFence::operator=( VulkanFence&& other ) noexcept -> VulkanFence&
{
	Move( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanFence::Clear()
{
	if( vk_fence == VK_NULL_HANDLE ) return;
	vkDestroyFence(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_fence,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	vk_fence = VK_NULL_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanFence::Move( VulkanFence& other )
{
	std::swap( vk_fence, other.vk_fence );
}
