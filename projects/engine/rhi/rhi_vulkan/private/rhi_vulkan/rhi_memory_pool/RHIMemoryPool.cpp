
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIPoolMemoryHandle.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto CalculateAlignmentForBuffer(
	VkDeviceSize					unaligned_size,
	const VkPhysicalDeviceLimits&	physical_device_limits
) -> VkDeviceSize
{
	VkDeviceSize buffer_alignment = std::max(
		physical_device_limits.minUniformBufferOffsetAlignment,
		physical_device_limits.minStorageBufferOffsetAlignment
	);
	buffer_alignment = std::max(
		buffer_alignment,
		VkDeviceSize( physical_device_limits.minMemoryMapAlignment )
	);

	return VkDeviceSize( ( ( ( bc::i64( unaligned_size ) - 1 ) / buffer_alignment ) + 1 ) * buffer_alignment );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIMemoryPool::RHIMemoryPool(
	RHIVulkanImpl&					rhi_vulkan_impl,
	const RHIMemoryPoolCreateInfo&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	physical_device_memory_properties	= rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice().GetMemoryProperties().memoryProperties;
	physical_device_properties			= rhi_vulkan_impl.GetVulkanDevice().GetVulkanPhysicalDevice().GetProperties().properties;

	linear_chunk_size			= create_info.linear_allocation_chunk_size;
	non_linear_chunk_size		= create_info.non_linear_allocation_chunk_size;

	linear_chunks.Resize( physical_device_memory_properties.memoryTypeCount );
	non_linear_chunks.Resize( physical_device_memory_properties.memoryTypeCount );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIMemoryPool::~RHIMemoryPool()
{
	for( auto & c : linear_chunks )
	{
		for( auto & t : c )
		{
			FreeChunkMemory( t );
		}
	}
	for( auto & c : non_linear_chunks )
	{
		for( auto & t : c )
		{
			FreeChunkMemory( t );
		}
	}
	linear_chunks.Clear();
	non_linear_chunks.Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::AllocateBufferMemory(
	VkBuffer					buffer,
	VkMemoryPropertyFlagBits	property_flags
) -> RHIPoolMemoryHandle
{
	auto memory_requirements = GetBufferMemoryRequirements( buffer );

	return AllocateMemory(
		true,
		memory_requirements,
		property_flags
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::AllocateImageMemory(
	VkImage						image,
	VkImageTiling				image_tiling_info,
	VkMemoryPropertyFlagBits	property_flags
) -> RHIPoolMemoryHandle
{
	auto memory_requirements = GetImageMemoryRequirements( image );

	return AllocateMemory(
		image_tiling_info == VK_IMAGE_TILING_LINEAR,
		memory_requirements,
		property_flags
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::AllocateMemory(
	bool									is_linear,
	internal_::RHIPoolMemoryRequirements&	memory_requirements,
	VkMemoryPropertyFlagBits				property_flags
) -> RHIPoolMemoryHandle
{
	// TODO: add tests and error reports
	auto memory_type_index = FindMemoryTypeIndex( memory_requirements, property_flags );
	assert( memory_type_index != std::numeric_limits<decltype( memory_type_index )>::max() );

	List<internal_::RHIMemoryPoolChunk> * chunk_group = nullptr;
	if( is_linear )
	{
		chunk_group = &linear_chunks[ memory_type_index ];
	}
	else
	{
		chunk_group = &non_linear_chunks[ memory_type_index ];
	}

	internal_::RHIMemoryPoolChunk			*	selected_chunk	= nullptr;
	internal_::RHIMemoryPoolChunk::Block	*	selected_block	= nullptr;
	for( auto & c : *chunk_group )
	{
		selected_chunk				= &c;
		selected_block				= AllocateBlock( *selected_chunk, memory_requirements );
		if( selected_block ) break;
	}

	// no chunks with free space, allocate a new chunk from the device
	if( !selected_block )
	{
		auto				buffer_image_granularity	= physical_device_properties.limits.bufferImageGranularity;
		VkDeviceSize		chunk_size					= 0;
		if( is_linear )
		{
			chunk_size		= ( ( std::max( memory_requirements.memory_requirements.size, linear_chunk_size ) + buffer_image_granularity - 1 ) / buffer_image_granularity ) * buffer_image_granularity;
		}
		else
		{
			chunk_size		= ( ( std::max( memory_requirements.memory_requirements.size, non_linear_chunk_size ) + buffer_image_granularity - 1 ) / buffer_image_granularity ) * buffer_image_granularity;
		}
		auto allocatedChunkInfo	= AllocateChunk( chunk_group, chunk_size, memory_type_index );
		selected_chunk		= allocatedChunkInfo.second;

		// ran out of device memory
		if( selected_chunk->result != VK_SUCCESS ) return {};

		selected_block = AllocateBlock( *selected_chunk, memory_requirements );

		// should never happen, error
		assert( selected_block->size > 0 );
		if( selected_block->size == 0 ) return {};
	}

	// construct RHIPoolMemoryHandle
	return RHIPoolMemoryHandle(
		&rhi_vulkan_impl,
		this,
		selected_chunk->memory,
		selected_block->offset,
		selected_block->size,
		selected_block->alignment,
		selected_chunk->id,
		selected_block->id,
		memory_type_index,
		property_flags,
		is_linear
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreePoolMemory( RHIPoolMemoryHandle & memory )
{
	if( memory.is_allocated )
	{
		FreeBlock( memory.memory_type_index, memory.is_linear, memory.chunk_id, memory.block_id );
	}
	memory.is_allocated		= false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::AllocateBlock(
	internal_::RHIMemoryPoolChunk&			chunk,
	internal_::RHIPoolMemoryRequirements&	memory_requirements
) -> internal_::RHIMemoryPoolChunk::Block*
{
	auto alignment	= memory_requirements.memory_requirements.alignment;
	auto size		= memory_requirements.memory_requirements.size;

	auto InsertChunkBlock = [ &chunk ](
		List<internal_::RHIMemoryPoolChunk::Block>::Iterator	where,
		VkDeviceSize											offset,
		VkDeviceSize											size,
		VkDeviceSize											alignment
		) -> internal_::RHIMemoryPoolChunk::Block*
		{
			auto block = internal_::RHIMemoryPoolChunk::Block {};
			block.id		= chunk.block_id_counter;
			block.offset	= offset;
			block.size		= size;
			block.alignment	= alignment;
			return ( chunk.blocks.Insert( where, block ) - 1 ).Get();
		};

	if( chunk.blocks.IsEmpty() ) return InsertChunkBlock( chunk.blocks.begin(), 0, size, alignment );

	for( auto b = chunk.blocks.begin(); b != chunk.blocks.end(); ++b )
	{
		auto prev = b - 1;

		auto range_begin	= prev->offset + prev->size;
		auto range_end		= b->offset;
		range_begin			= ( ( range_begin + alignment - 1 ) / alignment ) * alignment;
		auto range_size		= range_end - range_begin;

		if( range_size >= size )
		{
			return InsertChunkBlock( b, range_begin, size, alignment );
		}
	}

	//for( auto b = chunk.blocks.begin(); b != chunk.blocks.end(); ++b )
	//{
	//	if( b->id != std::numeric_limits<decltype( b->id )>::max() )
	//	{
	//		auto prev = b - 1;
	//		
	//		auto range_begin	= prev->offset + prev->size;
	//		auto range_end		= b->offset;
	//		range_begin			= ( ( range_begin + alignment - 1 ) / alignment ) * alignment;
	//		auto range_size		= range_end - range_begin;
	//		
	//		if( range_size >= size )
	//		{
	//			auto block		= chunk.blocks.Insert( b, { chunk.block_id_counter, range_begin, size, alignment } );
	//			++chunk.block_id_counter;
	//			--block;
	//			return &( *block );
	//		}
	//	}
	//}
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::AllocateChunk(
	List<internal_::RHIMemoryPoolChunk>*	chunk_group,
	VkDeviceSize							size,
	u32										memory_type_index
) -> Pair<VkResult, internal_::RHIMemoryPoolChunk*>
{
	assert( chunk_group );

	auto memory_allocate_info = VkMemoryAllocateInfo {};
	memory_allocate_info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.pNext				= nullptr;
	memory_allocate_info.allocationSize		= size;
	memory_allocate_info.memoryTypeIndex	= memory_type_index;
	auto memory = VkDeviceMemory {};
	auto result = vkAllocateMemory(
		rhi_vulkan_impl.GetVulkanDevice(),
		&memory_allocate_info,
		nullptr,
		&memory
	);
	if( result != VK_SUCCESS )
	{
		return { result, nullptr };
	}

	chunk_group->PushBack( internal_::RHIMemoryPoolChunk() );
	auto new_chunk		= &chunk_group->Back();
	new_chunk->id		= chunk_id_counter++;
	new_chunk->memory	= memory;
	new_chunk->size		= size;
	new_chunk->result	= result;

	return { result, new_chunk };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeBlock(
	u32			memory_type_index,
	bool		is_linear,
	u64			chunk_id,
	u64			block_id
)
{
	assert( memory_type_index != std::numeric_limits<decltype( memory_type_index )>::max() );
	assert( chunk_id != std::numeric_limits<decltype( chunk_id )>::max() );
	assert( block_id != std::numeric_limits<decltype( block_id )>::max() );

	List<internal_::RHIMemoryPoolChunk> * chunk_group = nullptr;
	if( is_linear )
	{
		chunk_group = &linear_chunks[ memory_type_index ];
	}
	else
	{
		chunk_group = &non_linear_chunks[ memory_type_index ];
	}

	internal_::RHIMemoryPoolChunk * selected_chunk = nullptr;
	for( auto & c : *chunk_group )
	{
		if( c.id == chunk_id )
		{
			selected_chunk = &c;
			break;
		}
	}
	if( !selected_chunk )
	{
		return;
	}

	for( auto b = selected_chunk->blocks.begin(); b != selected_chunk->blocks.end(); ++b )
	{
		if( b->id == block_id )
		{
			selected_chunk->blocks.Erase( b );
			break;
		}
	}
	if( selected_chunk->blocks.IsEmpty() )
	{
		FreeChunk( *chunk_group, *selected_chunk );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeChunk(
	List<internal_::RHIMemoryPoolChunk>&	chunk_group,
	internal_::RHIMemoryPoolChunk&			chunk
)
{
	FreeChunkMemory( chunk );
	if( auto it = chunk_group.FindIf( [ &chunk ]( auto & c ) { return c.id == chunk.id; } ) )
	{
		chunk_group.Erase( it );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeChunkMemory( internal_::RHIMemoryPoolChunk& chunk )
{
	vkFreeMemory( rhi_vulkan_impl.GetVulkanDevice(), chunk.memory, rhi_vulkan_impl.GetMainThreadAllocationCallbacks() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::GetBufferMemoryRequirements( VkBuffer buffer ) -> internal_::RHIPoolMemoryRequirements
{
	auto dedicated_buffer_memory_requirements = VkBufferMemoryRequirementsInfo2 {};
	dedicated_buffer_memory_requirements.sType	= VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
	dedicated_buffer_memory_requirements.pNext	= nullptr;
	dedicated_buffer_memory_requirements.buffer	= buffer;

	auto dedicated_requirements = VkMemoryDedicatedRequirements {};
	dedicated_requirements.sType		= VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
	dedicated_requirements.pNext		= nullptr;

	auto buffer_memory_requirements = VkMemoryRequirements2 {};
	buffer_memory_requirements.sType	= VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	buffer_memory_requirements.pNext	= &dedicated_requirements;

	vkGetBufferMemoryRequirements2(
		rhi_vulkan_impl.GetVulkanDevice(),
		&dedicated_buffer_memory_requirements,
		&buffer_memory_requirements
	);

	auto result = internal_::RHIPoolMemoryRequirements {};
	result.memory_requirements				= buffer_memory_requirements.memoryRequirements;
	result.requires_dedicated_allocation	= dedicated_requirements.requiresDedicatedAllocation;
	result.prefers_dedicated_allocation		= dedicated_requirements.prefersDedicatedAllocation;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::GetImageMemoryRequirements( VkImage image ) -> internal_::RHIPoolMemoryRequirements
{
	auto dedicated_image_memory_requirements = VkImageMemoryRequirementsInfo2 {};
	dedicated_image_memory_requirements.sType	= VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
	dedicated_image_memory_requirements.pNext	= nullptr;
	dedicated_image_memory_requirements.image	= image;

	auto dedicated_requirements = VkMemoryDedicatedRequirements {};
	dedicated_requirements.sType		= VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
	dedicated_requirements.pNext		= nullptr;

	auto buffer_memory_requirements = VkMemoryRequirements2 {};
	buffer_memory_requirements.sType	= VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	buffer_memory_requirements.pNext	= &dedicated_requirements;

	vkGetImageMemoryRequirements2(
		rhi_vulkan_impl.GetVulkanDevice(),
		&dedicated_image_memory_requirements,
		&buffer_memory_requirements
	);

	auto result = internal_::RHIPoolMemoryRequirements {};
	result.memory_requirements				= buffer_memory_requirements.memoryRequirements;
	result.requires_dedicated_allocation	= dedicated_requirements.requiresDedicatedAllocation;
	result.prefers_dedicated_allocation		= dedicated_requirements.prefersDedicatedAllocation;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::rhi::RHIMemoryPool::FindMemoryTypeIndex(
	internal_::RHIPoolMemoryRequirements&	memory_requirements,
	VkMemoryPropertyFlags					propertyFlags
) -> u32
{
	for( u32 i=0; i < physical_device_memory_properties.memoryTypeCount; ++i )
	{
		if( memory_requirements.memory_requirements.memoryTypeBits & ( 1 << i ) )
		{
			if( ( physical_device_memory_properties.memoryTypes[ i ].propertyFlags & propertyFlags ) == propertyFlags )
			{
				return i;
			}
		}
	}
	return std::numeric_limits<u32>::max();
}
