#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPoolCreateInfo.hpp>



namespace bc {
namespace rhi {

class RHIVulkanImpl;
class RHIPoolMemory;
class RHIMemoryPool;

namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIMemoryPoolChunk
{
	// Block is a single virtual allocation from the Chunk. Aka, assignment from a single pool.
	struct Block
	{
		uint64_t									id									= UINT64_MAX;
		VkDeviceSize								offset								= 0;
		VkDeviceSize								size								= 0;
		VkDeviceSize								alignment							= 1;
	};

	uint64_t										id									= UINT64_MAX;
	VkDeviceMemory									memory								= VK_NULL_HANDLE;
	VkDeviceSize									size								= 0;
	List<RHIMemoryPoolChunk::Block>					blocks;
	VkResult										result								= VK_RESULT_MAX_ENUM;

	uint64_t										block_id_counter					= 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIPoolMemoryRequirements
{
	VkMemoryRequirements							memory_requirements						= {};
	bool											prefers_dedicated_allocation			= {};
	bool											requires_dedicated_allocation			= {};
};



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIPoolMemory
{
	friend class RHIMemoryPool;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemory(
		RHIVulkanImpl								&	rhi_vulkan_impl,
		RHIMemoryPool								&	rhi_memory_pool
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemory(
		RHIVulkanImpl								&	rhi_vulkan_impl,
		RHIMemoryPool								&	rhi_memory_pool,
		VkDeviceMemory									memory,
		VkDeviceSize									offset,
		VkDeviceSize									size,
		VkDeviceSize									alignment,
		uint64_t										chunk_id,
		uint64_t										block_id,
		uint32_t										memory_type_index,
		bool											is_non_linear
	);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~RHIPoolMemory();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Copy memory into the backing buffer from external location.
	///
	/// @tparam ValueType
	/// Type of the value we're copying.
	/// 
	/// @param data
	/// Linear container where the data is copied from.
	template<typename T>
	void												DataCopy(
		const List<T>								&	data
	)
	{
		return DataCopy<T>( data.Data(), VkDeviceSize( data.Size() ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Copy memory into the backing buffer from external location.
	///
	/// @tparam ValueType
	/// Type of the value we're copying.
	/// 
	/// @param data
	/// Linear container where the data is copied from.
	template<typename T>
	void												DataCopy(
		const T										*	data,
		VkDeviceSize									count
	)
	{
		VkDeviceSize byte_size = sizeof( T ) * count;
		BAssert( byte_size < size, U"Cannot copy to buffer, buffer size too small" );

		std::memcpy( mapped_memory, data, byte_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline VkDeviceMemory								GetVulkanMemory() const { return memory; }
	inline VkDeviceSize									GetOffset() const { return offset; }
	inline VkDeviceSize									GetSize() const { return size; }
	inline VkDeviceSize									GetAlignment() const { return alignment; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void											*	mapped_memory						= {};

	RHIVulkanImpl									&	rhi_vulkan_impl;
	RHIMemoryPool									&	rhi_memory_pool;

	VkDeviceMemory										memory								= VK_NULL_HANDLE;
	VkDeviceSize										offset								= 0;
	VkDeviceSize										size								= 0;
	VkDeviceSize										alignment							= 0;

	uint64_t											chunk_id							= UINT64_MAX;
	uint64_t											block_id							= UINT64_MAX;
	uint32_t											memory_type_index					= UINT32_MAX;
	bool												is_non_linear					:1	= true;
	bool												is_allocated					:1	= false;
	bool												is_host_visible					:1	= false;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIMemoryPool
{
	friend class RHIPoolMemory;
	friend struct RHIMemoryPoolDataImpl;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIMemoryPool(
		RHIVulkanImpl								&	rhi_vulkan_impl,
		const RHIMemoryPoolCreateInfo				&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~RHIMemoryPool();

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Allocates memory for an image resource.
	///
	/// @param image
	/// Vulkan image class to allocate memory for.
	///
	/// @param image_create_info
	/// Create info that was used to create the image.
	/// 
	/// @param property_flags
	/// Vulkan property flags for this resource.
	/// 
	/// @return
	/// Handle to pool memory.
	RHIPoolMemory										AllocateBufferMemory(
		VkBuffer										buffer,
		const VkBufferCreateInfo					&	buffer_create_info,
		VkMemoryPropertyFlags							property_flags
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Allocates memory for an image resource.
	///
	/// @param image
	/// Vulkan image class to allocate memory for.
	///
	/// @param image_create_info
	/// Create info that was used to create the image.
	/// 
	/// @param property_flags
	/// Vulkan property flags for this resource.
	/// 
	/// @return
	/// Handle to pool memory.
	RHIPoolMemory										AllocateImageMemory(
		VkImage											image,
		const VkImageCreateInfo						&	image_create_info,
		VkMemoryPropertyFlags							property_flags
	);

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemory										AllocateMemory(
		bool											is_non_linear,
		internal::RHIPoolMemoryRequirements			&	memory_requirements,
		uint32_t										memory_type_index
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												FreePoolMemory(
		RHIPoolMemory								&	memory
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	internal::RHIMemoryPoolChunk::Block				*	AllocateBlock(
		internal::RHIMemoryPoolChunk				&	chunk,
		internal::RHIPoolMemoryRequirements			&	memory_requirements
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Pair<VkResult, internal::RHIMemoryPoolChunk*>		AllocateChunk(
		List<internal::RHIMemoryPoolChunk>			*	chunk_group,
		VkDeviceSize									size,
		uint32_t										memory_type_index
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												FreeBlock(
		uint32_t										memory_type_index,
		bool											is_non_linear,
		uint64_t										chunk_id,
		uint64_t										block_id
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												FreeChunk(
		List<internal::RHIMemoryPoolChunk>			&	chunk_group,
		internal::RHIMemoryPoolChunk				&	chunk
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												FreeChunkMemory(
		internal::RHIMemoryPoolChunk				&	chunk
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	internal::RHIPoolMemoryRequirements					GetBufferMemoryRequirements(
		VkBuffer										buffer
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	internal::RHIPoolMemoryRequirements					GetImageMemoryRequirements(
		VkImage											image
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t											FindMemoryTypeIndex(
		internal::RHIPoolMemoryRequirements			&	memory_requirements,
		VkMemoryPropertyFlags							property_flags
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemory										MakeEmptyPoolMemory();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl									&	rhi_vulkan_impl;

	VkPhysicalDeviceMemoryProperties					physical_device_memory_properties			= {};
	VkPhysicalDeviceProperties							physical_device_properties					= {};

	uint64_t											chunk_id_counter							= {};

	VkDeviceSize										linear_chunk_size							= {};
	VkDeviceSize										non_linear_chunk_size						= {};

	List<List<internal::RHIMemoryPoolChunk>>			linear_chunks;								// buffers and linear images
	List<List<internal::RHIMemoryPoolChunk>>			non_linear_chunks;							// optimal images

};



} // rhi
} // bc
