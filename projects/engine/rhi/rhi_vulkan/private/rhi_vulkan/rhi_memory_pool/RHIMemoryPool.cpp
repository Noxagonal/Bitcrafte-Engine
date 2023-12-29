
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkDeviceSize CalculateAlignmentForBuffer(
	VkDeviceSize							unaligned_size,
	const VkPhysicalDeviceLimits		&	physical_device_limits
)
{
	VkDeviceSize buffer_alignment = std::max(
		physical_device_limits.minUniformBufferOffsetAlignment,
		physical_device_limits.minStorageBufferOffsetAlignment
	);
	buffer_alignment = std::max(
		buffer_alignment,
		VkDeviceSize( physical_device_limits.minMemoryMapAlignment )
	);

	return VkDeviceSize( ( ( ( int64_t( unaligned_size ) - 1 ) / buffer_alignment ) + 1 ) * buffer_alignment );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory::RHIPoolMemory(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	RHIMemoryPool		&	rhi_memory_pool
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	rhi_memory_pool( rhi_memory_pool )
{
	BAssertVkResult( vkMapMemory(
		rhi_vulkan_impl.GetVulkanDevice(),
		memory,
		offset,
		size,
		0,
		&mapped_memory
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory::RHIPoolMemory(
	RHIVulkanImpl		&	rhi_vulkan_impl,
	RHIMemoryPool		&	rhi_memory_pool,
	VkDeviceMemory			memory,
	VkDeviceSize			offset,
	VkDeviceSize			size,
	VkDeviceSize			alignment,
	uint64_t				chunk_id,
	uint64_t				block_id,
	uint32_t				memory_type_index,
	bool					is_non_linear
) :
	rhi_vulkan_impl( rhi_vulkan_impl ),
	rhi_memory_pool( rhi_memory_pool ),
	memory( memory ),
	offset( offset ),
	size( size ),
	alignment( alignment ),
	chunk_id( chunk_id ),
	block_id( block_id ),
	memory_type_index( memory_type_index ),
	is_non_linear( is_non_linear )
{
	is_allocated = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory::~RHIPoolMemory()
{
	vkUnmapMemory(
		rhi_vulkan_impl.GetVulkanDevice(),
		memory
	);

	rhi_memory_pool.FreePoolMemory( *this );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIMemoryPool::RHIMemoryPool(
	RHIVulkanImpl						&	rhi_vulkan_impl,
	const RHIMemoryPoolCreateInfo		&	create_info
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
bc::rhi::RHIPoolMemory bc::rhi::RHIMemoryPool::AllocateBufferMemory(
	VkBuffer						buffer,
	const VkBufferCreateInfo	&	buffer_create_info,
	VkMemoryPropertyFlags			property_flags
)
{
	auto memory_requirements		= GetBufferMemoryRequirements( buffer );
	auto memory_type_index			= FindMemoryTypeIndex( memory_requirements, property_flags );

	if( memory_type_index == UINT32_MAX ) return MakeEmptyPoolMemory();
	return AllocateMemory(
		true,
		memory_requirements,
		memory_type_index
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory bc::rhi::RHIMemoryPool::AllocateImageMemory(
	VkImage							image,
	const VkImageCreateInfo		&	image_create_info,
	VkMemoryPropertyFlags			property_flags
)
{
	auto memory_requirements		= GetImageMemoryRequirements( image );
	auto memory_type_index			= FindMemoryTypeIndex( memory_requirements, property_flags );

	if( memory_type_index == UINT32_MAX ) return MakeEmptyPoolMemory();
	return AllocateMemory(
		image_create_info.tiling != VK_IMAGE_TILING_OPTIMAL,
		memory_requirements,
		memory_type_index
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory bc::rhi::RHIMemoryPool::AllocateMemory(
	bool										is_non_linear,
	internal::RHIPoolMemoryRequirements		&	memory_requirements,
	uint32_t									memory_type_index
)
{
	// TODO: add tests and error reports

	assert( memory_type_index != UINT32_MAX );

	List<internal::RHIMemoryPoolChunk> * chunk_group = nullptr;
	if( is_non_linear )
	{
		chunk_group = &non_linear_chunks[ memory_type_index ];
	}
	else
	{
		chunk_group = &linear_chunks[ memory_type_index ];
	}

	internal::RHIMemoryPoolChunk			*	selected_chunk	= nullptr;
	internal::RHIMemoryPoolChunk::Block		*	selected_block	= nullptr;
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
		if( is_non_linear )
		{
			chunk_size		= ( ( std::max( memory_requirements.memory_requirements.size, non_linear_chunk_size ) + buffer_image_granularity - 1 ) / buffer_image_granularity ) * buffer_image_granularity;
		}
		else
		{
			chunk_size		= ( ( std::max( memory_requirements.memory_requirements.size, linear_chunk_size ) + buffer_image_granularity - 1 ) / buffer_image_granularity ) * buffer_image_granularity;
		}
		auto allocatedChunkInfo	= AllocateChunk( chunk_group, chunk_size, memory_type_index );
		selected_chunk		= allocatedChunkInfo.second;

		// ran out of device memory
		if( !selected_chunk ) return MakeEmptyPoolMemory();

		selected_block = AllocateBlock( *selected_chunk, memory_requirements );

		// should never happen, error
		assert( selected_block );
		if( !selected_block ) return MakeEmptyPoolMemory();
	}

	// construct RHIPoolMemory
	return RHIPoolMemory(
		rhi_vulkan_impl,
		*this,
		selected_chunk->memory,
		selected_block->offset,
		selected_block->size,
		selected_block->alignment,
		selected_chunk->id,
		selected_block->id,
		memory_type_index,
		is_non_linear
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreePoolMemory(
	RHIPoolMemory & memory
)
{
	if( memory.is_allocated )
	{
		FreeBlock( memory.memory_type_index, memory.is_non_linear, memory.chunk_id, memory.block_id );
	}
	memory.is_allocated		= false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::internal::RHIMemoryPoolChunk::Block * bc::rhi::RHIMemoryPool::AllocateBlock(
	internal::RHIMemoryPoolChunk			&	chunk,
	internal::RHIPoolMemoryRequirements		&	memory_requirements
)
{
	auto alignment				= memory_requirements.memory_requirements.alignment;
	auto size					= memory_requirements.memory_requirements.size;

	for( auto b = chunk.blocks.begin(); b != chunk.blocks.end(); ++b )
	{
		if( b->id != UINT64_MAX )
		{
			auto prev = b;
			--prev;

			auto range_begin	= prev->offset + prev->size;
			auto range_end		= b->offset;
			range_begin			= ( ( range_begin + alignment - 1 ) / alignment ) * alignment;
			auto range_size		= range_end - range_begin;

			if( range_size >= size )
			{
				auto block		= chunk.blocks.Insert( b, { chunk.block_id_counter, range_begin, size, alignment } );
				++chunk.block_id_counter;
				return &( *block );
			}
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::Pair<VkResult, bc::rhi::internal::RHIMemoryPoolChunk*> bc::rhi::RHIMemoryPool::AllocateChunk(
	bc::List<bc::rhi::internal::RHIMemoryPoolChunk>		*	chunk_group,
	VkDeviceSize											size,
	uint32_t												memory_type_index
)
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

	chunk_group->PushBack( internal::RHIMemoryPoolChunk() );
	auto new_chunk		= &chunk_group->Back();
	new_chunk->id		= chunk_id_counter++;
	new_chunk->memory	= memory;
	new_chunk->size		= size;
	new_chunk->result	= result;
	// To make make the code more compact we're going to push some dummy limits in front and back.
	// This will make sure we don't have to check if we're at the front or back of the list when searching for a free spot
	// ^^^^
	// This was me years ago, I have no idea what I was thinking, this is not clear code.
	// TODO: Clean this up.
	new_chunk->blocks.PushFront( { UINT64_MAX, 0, 0, 1 } );
	new_chunk->blocks.PushBack( { UINT64_MAX - 1, size, 0, 1 } );

	return { result, new_chunk };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeBlock(
	uint32_t	memory_type_index,
	bool		is_non_linear,
	uint64_t	chunk_id,
	uint64_t	block_id
)
{
	assert( memory_type_index != UINT32_MAX );
	assert( chunk_id != UINT64_MAX );
	assert( block_id != UINT64_MAX );

	List<internal::RHIMemoryPoolChunk> * chunk_group = nullptr;
	if( is_non_linear )
	{
		chunk_group = &non_linear_chunks[ memory_type_index ];
	}
	else
	{
		chunk_group = &linear_chunks[ memory_type_index ];
	}

	internal::RHIMemoryPoolChunk * selected_chunk = nullptr;
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
	if( selected_chunk->blocks.Size() <= 2 )
	{
		FreeChunk( *chunk_group, *selected_chunk );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeChunk(
	List<internal::RHIMemoryPoolChunk>	&	chunk_group,
	internal::RHIMemoryPoolChunk		&	chunk
)
{
	FreeChunkMemory( chunk );
	if( auto it = chunk_group.FindIf( [ &chunk ]( auto & c ) { return c.id == chunk.id; } ) )
	{
		chunk_group.Erase( it );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIMemoryPool::FreeChunkMemory(
	internal::RHIMemoryPoolChunk	&	chunk
)
{
	vkFreeMemory( rhi_vulkan_impl.GetVulkanDevice(), chunk.memory, rhi_vulkan_impl.GetMainThreadAllocationCallbacks() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::internal::RHIPoolMemoryRequirements bc::rhi::RHIMemoryPool::GetBufferMemoryRequirements(
	VkBuffer buffer
)
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

	auto result = internal::RHIPoolMemoryRequirements {};
	result.memory_requirements				= buffer_memory_requirements.memoryRequirements;
	result.requires_dedicated_allocation	= dedicated_requirements.requiresDedicatedAllocation;
	result.prefers_dedicated_allocation		= dedicated_requirements.prefersDedicatedAllocation;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::internal::RHIPoolMemoryRequirements bc::rhi::RHIMemoryPool::GetImageMemoryRequirements(
	VkImage image
)
{
	auto dedicated_image_memory_requirements = VkImageMemoryRequirementsInfo2 {};
	dedicated_image_memory_requirements.sType	= VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
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

	auto result = internal::RHIPoolMemoryRequirements {};
	result.memory_requirements				= buffer_memory_requirements.memoryRequirements;
	result.requires_dedicated_allocation	= dedicated_requirements.requiresDedicatedAllocation;
	result.prefers_dedicated_allocation		= dedicated_requirements.prefersDedicatedAllocation;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t bc::rhi::RHIMemoryPool::FindMemoryTypeIndex(
	internal::RHIPoolMemoryRequirements		&	memory_requirements,
	VkMemoryPropertyFlags						propertyFlags
)
{
	for( uint32_t i=0; i < physical_device_memory_properties.memoryTypeCount; ++i )
	{
		if( memory_requirements.memory_requirements.memoryTypeBits & ( 1 << i ) )
		{
			if( ( physical_device_memory_properties.memoryTypes[ i ].propertyFlags & propertyFlags ) == propertyFlags )
			{
				return i;
			}
		}
	}
	return UINT32_MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemory bc::rhi::RHIMemoryPool::MakeEmptyPoolMemory()
{
	return RHIPoolMemory( rhi_vulkan_impl, *this );
}