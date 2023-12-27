
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>

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
bc::rhi::RHIVulkanImpl::ApplicationInfo::ApplicationInfo(
	const RHIComponentCreateInfo		&	create_info
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
	window_manager::WindowManagerComponent		&	window_manager_component,
	const RHIComponentCreateInfo				&	create_info
) :
	window_manager_component( window_manager_component ),
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

	window_manager_component.events.OnWindowCreated.RegisterObserver( &OnWindowCreated );
	window_manager_component.events.OnWindowBeingDestroyed.RegisterObserver( &OnWindowBeingDestroyed );

	OnWindowCreated.RegisterCallback( [ this ]( window_manager::Window * window )
		{
			window_context_list.PushBack( MakeUniquePtr<WindowContext>( *this, window ) );
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
int64_t bc::rhi::RHIVulkanImpl::GetBestPhysicalDevice() const
{
	auto & physical_device_list = vulkan_instance->GetPhysicalDeviceList();

	auto device_scores = [ &physical_device_list ]() -> List<uint64_t>
		{
			List<uint64_t> result( physical_device_list.Size() );
			for( size_t i=0; i < physical_device_list.Size(); ++i )
			{
				auto & pd = physical_device_list[ i ];
				auto & s = result[ i ];
				auto & p = pd.GetProperties();
				s += 1000; // some intial score
				s += uint64_t( p.properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) * 16000;
				s += uint64_t( p.properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ) * 5000;
				s += uint64_t( p.properties.properties.limits.maxImageDimension2D );
				s += uint64_t( p.properties.properties.limits.maxPerStageDescriptorUniformBuffers ) * 20;
				s += uint64_t( p.properties.properties.limits.maxPerStageDescriptorSampledImages ) * 40;
				s += uint64_t( p.properties.properties.limits.maxVertexInputBindings ) * 10;
				s += uint64_t( p.properties.properties.limits.maxComputeWorkGroupInvocations );
				s += uint64_t( p.properties.properties.limits.maxSamplerAnisotropy ) * 200;

				// Check if physical device can present
				auto can_present			= false;
				auto queue_family_list		= pd.GetQueueFamilyPropertyList();
				for( auto & f : queue_family_list )
				{
					if( f.can_present )
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

	auto best_physical_device_index		= int64_t { -1 };
	auto best_score_so_far				= uint64_t {};
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
