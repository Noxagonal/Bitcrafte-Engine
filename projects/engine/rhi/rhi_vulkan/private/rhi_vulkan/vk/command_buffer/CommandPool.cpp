
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/command_buffer/CommandPool.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/queue/VulkanQueue.hpp>
#include <rhi_vulkan/vk/command_buffer/CommandBuffer.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanCommandPool::VulkanCommandPool(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	VulkanQueue			&	queue,
	bool					is_transient,
	bool					is_resettable
) :
	rhi_vulkan_impl( &rhi_vulkan_impl )
{
	auto flags = VkCommandPoolCreateFlags {};
	if( is_transient )		flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	if( is_resettable )		flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	auto create_info = VkCommandPoolCreateInfo {};
	create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	create_info.pNext				= nullptr;
	create_info.flags				= flags;
	create_info.queueFamilyIndex	= queue.GetQueueFamilyIndex();

	BAssertVkResult( vkCreateCommandPool(
		rhi_vulkan_impl.GetVulkanDevice(),
		&create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_command_pool
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanCommandPool::~VulkanCommandPool()
{
	if( vk_command_pool == VK_NULL_HANDLE ) return;

	vkDestroyCommandPool(
		rhi_vulkan_impl->GetVulkanDevice(),
		vk_command_pool,
		rhi_vulkan_impl->GetMainThreadAllocationCallbacks()
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::UniquePtr<bc::rhi::VulkanCommandBuffer> bc::rhi::VulkanCommandPool::AllocateCommandBuffer(
	bool is_secondary
)
{
	return MakeUniquePtr<VulkanCommandBuffer>( *rhi_vulkan_impl, *this, is_secondary );
}
