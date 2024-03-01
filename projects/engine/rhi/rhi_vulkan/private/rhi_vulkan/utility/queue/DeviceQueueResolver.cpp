
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/utility/queue/DeviceQueueResolver.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::DeviceQueueResolver::DeviceQueueResolver(
	RHIVulkanImpl								&	rhi_vulkan_impl,
	const VulkanPhysicalDevice					&	physical_device,
	bc::List<bc::Pair<VkQueueFlags, f32>>			queue_types
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	physical_device( physical_device ),
	vk_instance( rhi_vulkan_impl.GetVulkanInstance() ),
	queue_types( queue_types ),
	queue_family_properties( physical_device.GetQueueFamilyProperties() )
{
	BHardAssert( queue_types.Size() > 0, "No queue types requested" );

	// available queues for the family, we can substract from this to test if queues are available or not
	auto available_queues		= List<u32>( queue_family_properties.queue_family_properties.Size() );
	// family allocations, outside List represents the queue family, inside List represents indices to the queue_types
	auto family_allocations		= List<List<u32>>( queue_family_properties.queue_family_properties.Size() );
	for( u64 i = 0; i < queue_family_properties.queue_family_properties.Size(); ++i )
	{
		available_queues[ i ]	= queue_family_properties.queue_family_properties[ i ].queueFamilyProperties.queueCount;
	}
	// queueGetInfo tells how to get the queues from the device after device creation
	queue_get_info.Resize( this->queue_types.Size() );
	for( auto & i : queue_get_info )
	{
		i.queue_index			= UINT32_MAX;
		i.queue_family_index	= UINT32_MAX;
		i.based_on				= UINT32_MAX;
	}

	// find queue families for queue types and also check for available queue count in that family
	for( u32 q = 0; q < this->queue_types.Size(); ++q )
	{
		u32 family_candidate = UINT32_MAX;
		for( u32 f = 0; f < queue_family_properties.queue_family_properties.Size(); ++f )
		{
			if( queue_family_properties.queue_family_properties[ f ].queueFamilyProperties.queueFlags & this->queue_types[ q ].first )
			{
				if( available_queues[ f ] )
				{
					if( family_candidate == UINT32_MAX )
					{
						// found initial candidate
						family_candidate = f;
					}
					else if( available_queues[ family_candidate ] &&
						queue_family_properties.queue_family_properties[ f ].queueFamilyProperties.queueFlags <
						queue_family_properties.queue_family_properties[ family_candidate ].queueFamilyProperties.queueFlags )
					{
						// found a better candidate family.
						// A family with less flags is usually more specialized. Also lower value flags are preferred over high value flags
						// as high flag values seem more specialized for certain tasks, like encoding or decoding video.
						family_candidate = f;
					}
				}
			}
		}
		// "allocate" queue from candidate if found
		if( family_candidate != UINT32_MAX )
		{
			--available_queues[ family_candidate ];
			queue_get_info[ q ].queue_family_index	= family_candidate;
			queue_get_info[ q ].queue_index			= u32( family_allocations[ family_candidate ].Size() );
			family_allocations[ family_candidate ].PushBack( q );
		}
	}

	// Check that everyone got a queue family
	List<bool> queue_allocated_test( this->queue_types.Size() );
	for( u64 a = 0; a < family_allocations.Size(); ++a )
	{
		for( u64 b = 0; b < family_allocations[ a ].Size(); ++b )
		{
			queue_allocated_test[ family_allocations[ a ][ b ] ] = true;
		}
	}

	// Those that didn't get an allocation will share the allocation with someone else with similar properties and fewer existing dependants
	List<u32> queue_dependants( this->queue_types.Size() );
	for( u32 a = 0; a < queue_allocated_test.Size(); ++a )
	{
		if( !queue_allocated_test[ a ] )
		{
			u32 candidate			= UINT32_MAX;
			f32 candidate_priority	= 0.0f;
			for( u32 t = 0; t < this->queue_types.Size(); ++t )
			{
				if( !( a == t ) && queue_allocated_test[ t ] )
				{
					if( queue_family_properties.queue_family_properties[ queue_get_info[ t ].queue_family_index ].queueFamilyProperties.queueFlags & this->queue_types[ a ].first )
					{
						if( candidate == UINT32_MAX )
						{
							// initial candidate
							candidate			= t;
							candidate_priority	= this->queue_types[ t ].second;
						}
						else if(
							( std::abs( this->queue_types[ t ].second - this->queue_types[ a ].second ) + ( queue_dependants[ t ] / 15.0 ) ) <
							( std::abs( this->queue_types[ t ].second - candidate_priority ) + ( queue_dependants[ candidate ] / 15.0 ) ) )
						{
							// found better candidate
							candidate			= t;
							candidate_priority	= this->queue_types[ t ].second;
						}
					}
				}
			}
			if( candidate != UINT32_MAX )
			{
				queue_get_info[ a ].based_on = candidate;
				++queue_dependants[ candidate ];
			}
		}
	}

	// build queue create infos
	queue_priorities.Resize( queue_family_properties.queue_family_properties.Size() );
	queue_create_infos.Reserve( queue_family_properties.queue_family_properties.Size() );
	for( u64 i = 0; i < queue_family_properties.queue_family_properties.Size(); ++i )
	{
		queue_priorities[ i ].Resize( family_allocations[ i ].Size() );
		for( u64 p = 0; p < family_allocations[ i ].Size(); ++p )
		{
			queue_priorities[ i ][ p ] = this->queue_types[ family_allocations[ i ][ p ] ].second;
		}
		if( family_allocations[ i ].Size() )
		{
			VkDeviceQueueCreateInfo ci {};
			ci.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			ci.pNext				= nullptr;
			ci.flags				= 0;
			ci.queueFamilyIndex		= u32( i );
			ci.queueCount			= u32( family_allocations[ i ].Size() );
			ci.pQueuePriorities		= queue_priorities[ i ].Data();
			queue_create_infos.PushBack( ci );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::DeviceQueueResolver::~DeviceQueueResolver()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::List<VkDeviceQueueCreateInfo> & bc::rhi::DeviceQueueResolver::GetDeviceQueueCreateInfos()
{
	return queue_create_infos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::rhi::VulkanQueue> bc::rhi::DeviceQueueResolver::GetVulkanQueues(
	VkDevice device
)
{
	List<VulkanQueue> ret( queue_get_info.Size() );
	for( u32 i=0; i < ret.Size(); ++i )
	{
		ret[ i ].queue_mutex				= nullptr;
		ret[ i ].queue						= VK_NULL_HANDLE;
		ret[ i ].queue_family_index			= UINT32_MAX;
		ret[ i ].supports_presentation		= VK_FALSE;
		ret[ i ].queue_family_properties	= {};
		ret[ i ].based_on					= UINT32_MAX;
	}

	// get the original queues first
	for( u64 i=0; i < queue_get_info.Size(); ++i )
	{
		if( queue_get_info[ i ].based_on == UINT32_MAX )
		{
			if( queue_get_info[ i ].queue_family_index != UINT32_MAX )
			{
				vkGetDeviceQueue( device, queue_get_info[ i ].queue_family_index, queue_get_info[ i ].queue_index, &ret[ i ].queue );
				ret[ i ].queue_family_index			= queue_get_info[ i ].queue_family_index;
				ret[ i ].supports_presentation		= queue_family_properties.can_present[ ret[ i ].queue_family_index ];
				ret[ i ].queue_family_properties	= queue_family_properties.queue_family_properties[ queue_get_info[ i ].queue_family_index ].queueFamilyProperties;
				ret[ i ].queue_mutex				= std::make_shared<std::mutex>();
			}
		}
	}

	// get the information for the shared queue after we know the originals
	for( u64 i=0; i < queue_get_info.Size(); ++i )
	{
		if( queue_get_info[ i ].based_on != UINT32_MAX )
		{
			auto based_on						= queue_get_info[ i ].based_on;
			ret[ i ].queue_mutex				= ret[ based_on ].queue_mutex;
			ret[ i ].queue						= ret[ based_on ].queue;
			ret[ i ].queue_family_index			= ret[ based_on ].queue_family_index;
			ret[ i ].supports_presentation		= ret[ based_on ].supports_presentation;
			ret[ i ].queue_family_properties	= ret[ based_on ].queue_family_properties;
			ret[ i ].based_on					= based_on;
		}
	}

	return ret;
}
