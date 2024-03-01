
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>

#include <core/memory/raw/RawMemory.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi/RHIComponentStartInfo.hpp>

#include <window_manager/WindowManagerComponent.hpp>
#include <rhi_vulkan/window_context/WindowContext.hpp>



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
bc::rhi::RHIVulkanImpl::RHIVulkanImpl(
	window_manager::WindowManagerComponent		&	window_manager_component,
	const RHIComponentCreateInfo				&	create_info
) :
	window_manager_component( window_manager_component )
{
	main_thread_allocation_callbacks.pUserData				= this;
	main_thread_allocation_callbacks.pfnAllocation			= VulkanMemoryAllocationFunction;
	main_thread_allocation_callbacks.pfnReallocation		= VulkanMemoryReallocationFunction;
	main_thread_allocation_callbacks.pfnFree				= VulkanMemoryFreeFunction;
	main_thread_allocation_callbacks.pfnInternalAllocation	= VulkanMemoryInternalAllocationNotification;
	main_thread_allocation_callbacks.pfnInternalFree		= VulkanMemoryInternalFreeNotification;

	// Fill out application info
	application_info.application_name			= create_info.application_name;
	application_info.application_version		= create_info.application_version;

	// Fill out debug settings
	debug_settings.debug_enabled				= create_info.enable_debug;
	debug_settings.minimum_debug_level			= create_info.minimum_debug_level;

	vulkan_instance		= MakeUniquePtr<VulkanInstance>( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIVulkanImpl::~RHIVulkanImpl()
{
	vulkan_device		= nullptr;
	vulkan_instance		= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIVulkanImpl::Start(
	const RHIComponentStartInfo & rhi_start_info
)
{
	auto & selected_physical_device = vulkan_instance->GetPhysicalDeviceList()[ rhi_start_info.use_device ];
	vulkan_device = MakeUniquePtr<VulkanDevice>( *this, selected_physical_device, rhi_start_info );

	auto memory_pool_create_info = RHIMemoryPoolCreateInfo {};
	// Hard coded for now, would be better to query a better size from the system.
	memory_pool_create_info.linear_allocation_chunk_size		= 1024 * 1024 * 128;
	memory_pool_create_info.non_linear_allocation_chunk_size	= 1024 * 1024 * 512;
	memory_pool = MakeUniquePtr<RHIMemoryPool>( *this, memory_pool_create_info );

	window_manager_component.events.OnWindowCreated.RegisterObserver( &OnWindowCreated );
	window_manager_component.events.OnWindowBeingDestroyed.RegisterObserver( &OnWindowBeingDestroyed );

	OnWindowCreated.RegisterCallback( [ this ]( window_manager::Window * window )
		{
			auto window_context_create_info = WindowContextCreateInfo {};
			// TODO: Hard-coded value for now. Eventually this should be get from global settings.
			// Also this option is only the initial value, the user should be able change this at runtime.
			// Internally this means recreating the swapchain.
			// This might also be the wrong place for swapchain considerations. Initial swapchain should be
			// pretty self contained, besides reading some initial global settings.
			// Window should decide the swapchain settings but I'm not sure where to put these settings yet.
			window_context_create_info.swapchain_create_info.use_v_sync = true;

			window_context_list.PushBack( MakeUniquePtr<WindowContext>( *this, window, window_context_create_info ) );
		}
	);
	OnWindowBeingDestroyed.RegisterCallback( [ this ]( window_manager::Window * window )
		{
			auto it = std::find_if( window_context_list.begin(), window_context_list.end(), [ window ]( auto & window_context )
				{
					return window_context->GetWindow() == window;
				}
			);
			if( it == window_context_list.end() ) return;
			window_context_list.Erase( it );
		}
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::i64 bc::rhi::RHIVulkanImpl::GetBestPhysicalDevice()
{
	auto & physical_device_list = vulkan_instance->GetPhysicalDeviceList();

	auto device_scores = [ &physical_device_list ]() -> List<u64>
		{
			List<u64> result( physical_device_list.Size() );
			for( size_t i=0; i < physical_device_list.Size(); ++i )
			{
				auto & pd = physical_device_list[ i ];
				auto & s = result[ i ];
				auto & p = pd.GetProperties();
				s += 1000; // some intial score
				s += u64( p.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) * 16000;
				s += u64( p.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ) * 5000;
				s += u64( p.properties.limits.maxImageDimension2D );
				s += u64( p.properties.limits.maxPerStageDescriptorUniformBuffers ) * 20;
				s += u64( p.properties.limits.maxPerStageDescriptorSampledImages ) * 40;
				s += u64( p.properties.limits.maxVertexInputBindings ) * 10;
				s += u64( p.properties.limits.maxComputeWorkGroupInvocations );
				s += u64( p.properties.limits.maxSamplerAnisotropy ) * 200;

				// Check if physical device can present
				auto can_present						= false;
				auto queue_family_can_present_list		= pd.GetQueueFamilyProperties().can_present;
				for( auto p : queue_family_can_present_list )
				{
					if( p )
					{
						can_present = true;
						break;
					}
				}

				// If the physical device cannot present anything we won't even consider it
				if( !can_present )
				{
					s = 0;
				}
			}
			return result;
		}();

	auto best_physical_device_index		= i64{ -1 };
	auto best_score_so_far				= u64 {};
	for( size_t i = 0; i < physical_device_list.Size(); ++i )
	{
		if( device_scores[ i ] > best_score_so_far )
		{
			best_score_so_far			= device_scores[ i ];
			best_physical_device_index	= i;
		}
	}

	return best_physical_device_index;
}
