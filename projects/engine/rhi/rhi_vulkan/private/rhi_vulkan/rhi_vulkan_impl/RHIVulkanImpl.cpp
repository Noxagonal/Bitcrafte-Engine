
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <core/memory/raw/RawMemory.hpp>
#include <rhi_vulkan/instance/VulkanInstance.hpp>




void* VKAPI_PTR VulkanMemoryAllocationFunction(
	void*										pUserData,
	size_t										size,
	size_t										alignment,
	VkSystemAllocationScope						allocationScope
)
{
	return bc::memory::internal::AllocateRawMemory_Runtime( size, alignment );
}

void* VKAPI_PTR VulkanMemoryReallocationFunction(
	void*										pUserData,
	void*										pOriginal,
	size_t										size,
	size_t										alignment,
	VkSystemAllocationScope						allocationScope
)
{
	return bc::memory::internal::ReallocateRawMemory_Runtime( pOriginal, size );
}

void VKAPI_PTR VulkanMemoryFreeFunction(
	void*										pUserData,
	void*										pMemory
)
{
	bc::memory::internal::FreeRawMemory_Runtime( pMemory );
}

void VKAPI_PTR VulkanMemoryInternalAllocationNotification(
	void*										pUserData,
	size_t										size,
	VkInternalAllocationType					allocationType,
	VkSystemAllocationScope						allocationScope
)
{}

void VKAPI_PTR VulkanMemoryInternalFreeNotification(
	void*										pUserData,
	size_t										size,
	VkInternalAllocationType					allocationType,
	VkSystemAllocationScope						allocationScope
)
{}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanImpl::ApplicationInfo::ApplicationInfo(
	const RHIComponentCreateInfo & create_info
)
{
	application_name			= create_info.application_name;
	application_version			= create_info.application_version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanImpl::DebugSettings::DebugSettings(
	const RHIComponentCreateInfo & create_info
)
{
	debug_enabled				= create_info.enable_debug;
	minimum_debug_level			= create_info.minimum_debug_level;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanImpl::RHIVulkanImpl(
	const RHIComponentCreateInfo & create_info
) :
	application_info( create_info ),
	debug_settings( create_info )
{
	main_thread_allocation_callbacks.pUserData				= this;
	main_thread_allocation_callbacks.pfnAllocation			= VulkanMemoryAllocationFunction;
	main_thread_allocation_callbacks.pfnReallocation		= VulkanMemoryReallocationFunction;
	main_thread_allocation_callbacks.pfnFree				= VulkanMemoryFreeFunction;
	main_thread_allocation_callbacks.pfnInternalAllocation	= VulkanMemoryInternalAllocationNotification;
	main_thread_allocation_callbacks.pfnInternalFree		= VulkanMemoryInternalFreeNotification;

	vulkan_instance		= MakeUniquePtr<VulkanInstance>( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanImpl::~RHIVulkanImpl()
{
	vulkan_instance = nullptr;
}
