#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPoolCreateInfo.hpp>



namespace bc {
namespace rhi {

class RHIVulkanImpl;
class RHIPoolMemoryHandle;
class RHIMemoryPool;

namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIMemoryPoolChunk
{
	// Block is a single virtual allocation from the Chunk. Aka, assignment from a single pool.
	struct Block
	{
		uint64_t										id									= UINT64_MAX;
		VkDeviceSize									offset								= 0;
		VkDeviceSize									size								= 0;
		VkDeviceSize									alignment							= 1;
	};

	uint64_t											id									= UINT64_MAX;
	VkDeviceMemory										memory								= VK_NULL_HANDLE;
	VkDeviceSize										size								= 0;
	List<RHIMemoryPoolChunk::Block>						blocks;
	VkResult											result								= VK_RESULT_MAX_ENUM;

	uint64_t											block_id_counter					= 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIPoolMemoryRequirements
{
	VkMemoryRequirements								memory_requirements						= {};
	bool												prefers_dedicated_allocation			= {};
	bool												requires_dedicated_allocation			= {};
};



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIMemoryPool
{
	friend class RHIPoolMemoryHandle;
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
	RHIPoolMemoryHandle									AllocateBufferMemory(
		VkBuffer										buffer,
		VkMemoryPropertyFlagBits						property_flags
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
	RHIPoolMemoryHandle									AllocateImageMemory(
		VkImage											image,
		VkImageTiling									image_tiling_info,
		VkMemoryPropertyFlagBits						property_flags
	);

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemoryHandle									AllocateMemory(
		bool											is_linear,
		internal::RHIPoolMemoryRequirements			&	memory_requirements,
		VkMemoryPropertyFlagBits						property_flags
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void												FreePoolMemory(
		RHIPoolMemoryHandle							&	memory
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
		bool											is_linear,
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
