
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/synchronization/VulkanSemaphore.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanSemaphore::VulkanSemaphore(
	RHIVulkanImpl&	rhi_vulkan_impl,
	bool			crate_timeline_semaphore
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto type_create_info = VkSemaphoreTypeCreateInfo {};
	type_create_info.sType			= VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	type_create_info.pNext			= nullptr;
	type_create_info.semaphoreType	= crate_timeline_semaphore ? VK_SEMAPHORE_TYPE_TIMELINE : VK_SEMAPHORE_TYPE_BINARY;
	type_create_info.initialValue	= 0;

	auto create_info = VkSemaphoreCreateInfo {};
	create_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	create_info.pNext	= &type_create_info;
	create_info.flags	= 0;
	BAssertVkResult( vkCreateSemaphore(
		rhi_vulkan_impl.GetVulkanDevice(),
		&create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_semaphore
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanSemaphore::~VulkanSemaphore()
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::VulkanSemaphore::operator=( VulkanSemaphore&& other ) noexcept -> VulkanSemaphore&
{
	Move( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanSemaphore::Clear()
{
	if( vk_semaphore == VK_NULL_HANDLE ) return;
	vkDestroySemaphore(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_semaphore,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	vk_semaphore = VK_NULL_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::VulkanSemaphore::Move( VulkanSemaphore& other )
{
	std::swap( vk_semaphore, other.vk_semaphore );
}
