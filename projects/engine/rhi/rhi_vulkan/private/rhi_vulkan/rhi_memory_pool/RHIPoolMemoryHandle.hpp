#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstring>



namespace bc {
namespace rhi {

class RHIVulkanImpl;
class RHIMemoryPool;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIPoolMemoryHandle
{
	friend class RHIMemoryPool;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemoryHandle(
		RHIVulkanImpl*				rhi_vulkan_impl,
		RHIMemoryPool*				rhi_memory_pool,
		VkDeviceMemory				memory,
		VkDeviceSize				offset,
		VkDeviceSize				size,
		VkDeviceSize				alignment,
		u64							chunk_id,
		u64							block_id,
		u32							memory_type_index,
		VkMemoryPropertyFlagBits	property_flags,
		bool						is_linear
	);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemoryHandle() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemoryHandle(const RHIPoolMemoryHandle& other ) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIPoolMemoryHandle( RHIPoolMemoryHandle&& other );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~RHIPoolMemoryHandle();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( const RHIPoolMemoryHandle& other ) -> RHIPoolMemoryHandle& = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( RHIPoolMemoryHandle&& other ) -> RHIPoolMemoryHandle&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( std::nullptr_t ) -> RHIPoolMemoryHandle&;

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
	void DataCopy( const List<T>& data )
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
	void DataCopy(
		const T*		data,
		VkDeviceSize	count
	)
	{
		VkDeviceSize byte_size = sizeof( T ) * count;
		BAssert( byte_size < size, U"Cannot copy to vulkan buffer or image memory, size too small" );
		BAssert( property_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, U"Cannot copy to buffer or image memory, memory is not host visible" );

		std::memcpy( mapped_memory, data, byte_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline auto GetVulkanMemory() const -> VkDeviceMemory { return memory; }
	inline auto GetOffset() const -> VkDeviceSize { return offset; }
	inline auto GetSize() const -> VkDeviceSize { return size; }
	inline auto GetAlignment() const -> VkDeviceSize { return alignment; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Move( RHIPoolMemoryHandle& other );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void*					mapped_memory			= {};

	RHIVulkanImpl*			rhi_vulkan_impl			= nullptr;
	RHIMemoryPool*			rhi_memory_pool			= nullptr;

	VkDeviceMemory			memory					= VK_NULL_HANDLE;
	VkDeviceSize			offset					= 0;
	VkDeviceSize			size					= 0;
	VkDeviceSize			alignment				= 0;

	u64						chunk_id				= std::numeric_limits<u64>::max();
	u64						block_id				= std::numeric_limits<u64>::max();
	u32						memory_type_index		= std::numeric_limits<u32>::max();
	VkMemoryPropertyFlags	property_flags			= {};

	bool					is_allocated		:1	= false;
	bool					is_linear			:1	= true;
};



} // rhi
} // bc
