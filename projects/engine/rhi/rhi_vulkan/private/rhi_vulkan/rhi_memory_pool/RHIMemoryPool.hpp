#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPoolCreateInfo.hpp>



namespace bc {
namespace rhi {

class RHIVulkanImpl;
class RHIPoolMemoryHandle;
class RHIMemoryPool;

namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIMemoryPoolChunk
{
	// Block is a single virtual allocation from the Chunk. Aka, assignment from a single pool.
	struct Block
	{
		u64				id			= std::numeric_limits<u64>::max();
		VkDeviceSize	offset		= 0;
		VkDeviceSize	size		= 0;
		VkDeviceSize	alignment	= 1;
	};

	u64								id					= std::numeric_limits<u64>::max();
	VkDeviceMemory					memory				= VK_NULL_HANDLE;
	VkDeviceSize					size				= 0;
	List<RHIMemoryPoolChunk::Block>	blocks;
	VkResult						result				= VK_RESULT_MAX_ENUM;

	u64								block_id_counter	= 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct RHIPoolMemoryRequirements
{
	VkMemoryRequirements	memory_requirements				= {};
	bool					prefers_dedicated_allocation	= {};
	bool					requires_dedicated_allocation	= {};
};



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIMemoryPool
{
	friend class RHIPoolMemoryHandle;
	friend struct RHIMemoryPoolDataImpl;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIMemoryPool(
		RHIVulkanImpl&					rhi_vulkan_impl,
		const RHIMemoryPoolCreateInfo&	create_info
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
	auto AllocateBufferMemory(
		VkBuffer					buffer,
		VkMemoryPropertyFlagBits	property_flags
	) -> RHIPoolMemoryHandle;

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
	auto AllocateImageMemory(
		VkImage						image,
		VkImageTiling				image_tiling_info,
		VkMemoryPropertyFlagBits	property_flags
	) -> RHIPoolMemoryHandle;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto AllocateMemory(
		bool									is_linear,
		internal_::RHIPoolMemoryRequirements&	memory_requirements,
		VkMemoryPropertyFlagBits				property_flags
	) -> RHIPoolMemoryHandle;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FreePoolMemory( RHIPoolMemoryHandle& memory );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto AllocateBlock(
		internal_::RHIMemoryPoolChunk&			chunk,
		internal_::RHIPoolMemoryRequirements&	memory_requirements
	) -> internal_::RHIMemoryPoolChunk::Block*;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto AllocateChunk(
		List<internal_::RHIMemoryPoolChunk>*	chunk_group,
		VkDeviceSize							size,
		u32										memory_type_index
	) -> Pair<VkResult, internal_::RHIMemoryPoolChunk*>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FreeBlock(
		u32		memory_type_index,
		bool	is_linear,
		u64		chunk_id,
		u64		block_id
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FreeChunk(
		List<internal_::RHIMemoryPoolChunk>&	chunk_group,
		internal_::RHIMemoryPoolChunk&			chunk
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FreeChunkMemory( internal_::RHIMemoryPoolChunk& chunk );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetBufferMemoryRequirements( VkBuffer buffer ) -> internal_::RHIPoolMemoryRequirements;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetImageMemoryRequirements( VkImage image ) -> internal_::RHIPoolMemoryRequirements;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto FindMemoryTypeIndex(
		internal_::RHIPoolMemoryRequirements&	memory_requirements,
		VkMemoryPropertyFlags					property_flags
	) -> u32;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIVulkanImpl&								rhi_vulkan_impl;

	VkPhysicalDeviceMemoryProperties			physical_device_memory_properties	= {};
	VkPhysicalDeviceProperties					physical_device_properties			= {};

	u64											chunk_id_counter					= {};

	VkDeviceSize								linear_chunk_size					= {};
	VkDeviceSize								non_linear_chunk_size				= {};

	List<List<internal_::RHIMemoryPoolChunk>>	linear_chunks;						// buffers and linear images
	List<List<internal_::RHIMemoryPoolChunk>>	non_linear_chunks;					// optimal images

};



} // rhi
} // bc
