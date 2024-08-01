
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIPoolMemoryHandle.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>
#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemoryHandle::RHIPoolMemoryHandle(
	RHIVulkanImpl				*	rhi_vulkan_impl,
	RHIMemoryPool				*	rhi_memory_pool,
	VkDeviceMemory					memory,
	VkDeviceSize					offset,
	VkDeviceSize					size,
	VkDeviceSize					alignment,
	u64								chunk_id,
	u64								block_id,
	u32								memory_type_index,
	VkMemoryPropertyFlagBits		property_flags,
	bool							is_linear
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
	property_flags( property_flags ),
	is_linear( is_linear )
{
	is_allocated = true;

	if( property_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
	{
		BAssertVkResult( vkMapMemory(
			rhi_vulkan_impl->GetVulkanDevice(),
			memory,
			offset,
			size,
			0,
			&mapped_memory
		) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemoryHandle::RHIPoolMemoryHandle(
	RHIPoolMemoryHandle && other
)
{
	MoveOther( std::move( other ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemoryHandle::~RHIPoolMemoryHandle()
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemoryHandle & bc::rhi::RHIPoolMemoryHandle::operator=(
	RHIPoolMemoryHandle && other
)
{
	MoveOther( std::move( other ) );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::RHIPoolMemoryHandle & bc::rhi::RHIPoolMemoryHandle::operator=(
	std::nullptr_t
)
{
	Clear();
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIPoolMemoryHandle::Clear()
{
	if( rhi_vulkan_impl == nullptr ) return;
	if( !is_allocated ) return;

	if( property_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT && memory )
	{
		vkUnmapMemory(
			rhi_vulkan_impl->GetVulkanDevice(),
			memory
		);
		memory = nullptr;
	}

	rhi_memory_pool->FreePoolMemory( *this );

	// HACK: // This effectively renders this handle useless. A little hacky but does the job.
	rhi_vulkan_impl = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::RHIPoolMemoryHandle::MoveOther(
	RHIPoolMemoryHandle && other
)
{
	std::swap( mapped_memory, other.mapped_memory );
	std::swap( rhi_vulkan_impl, other.rhi_vulkan_impl );
	std::swap( rhi_memory_pool, other.rhi_memory_pool );
	std::swap( memory, other.memory );
	std::swap( offset, other.offset );
	std::swap( size, other.size );
	std::swap( alignment, other.alignment );
	std::swap( chunk_id, other.chunk_id );
	std::swap( block_id, other.block_id );
	std::swap( memory_type_index, other.memory_type_index );
	std::swap( property_flags, other.property_flags );
	//std::swap( is_allocated, other.is_allocated );
	//std::swap( is_linear, other.is_linear );

	// There doesn't seem to be a std::swap for booleans.
	bool is_allocated_buffer = other.is_allocated;
	other.is_allocated = is_allocated;
	is_allocated = is_allocated_buffer;

	bool is_linear_buffer = other.is_linear;
	other.is_linear = is_linear;
	is_linear = is_linear_buffer;
}
