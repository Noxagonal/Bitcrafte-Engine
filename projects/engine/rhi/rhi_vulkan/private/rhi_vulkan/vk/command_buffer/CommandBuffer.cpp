
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/command_buffer/CommandBuffer.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/command_buffer/CommandPool.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanCommandBuffer::VulkanCommandBuffer(
	RHIVulkanImpl			&	rhi_vulkan_impl,
	VulkanCommandPool		&	vulkan_command_pool,
	bool						is_secondary
) :
	rhi_vulkan_impl( &rhi_vulkan_impl ),
	vulkan_command_pool( &vulkan_command_pool )
{
	auto level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	if( is_secondary ) level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

	auto allocate_info = VkCommandBufferAllocateInfo {};
	allocate_info.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.pNext					= nullptr;
	allocate_info.commandPool			= vulkan_command_pool;
	allocate_info.level					= level;
	allocate_info.commandBufferCount	= 1;

	BAssertVkResult( vkAllocateCommandBuffers(
		rhi_vulkan_impl.GetVulkanDevice(),
		&allocate_info,
		&vk_command_buffer
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanCommandBuffer::~VulkanCommandBuffer()
{
	if( vk_command_buffer == VK_NULL_HANDLE ) return;

	vkFreeCommandBuffers(
		rhi_vulkan_impl->GetVulkanDevice(),
		*vulkan_command_pool,
		1,
		&vk_command_buffer
	);
}
