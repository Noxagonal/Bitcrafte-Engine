#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>

#include <core/data_types/FundamentalTypes.hpp>

#include <type_traits>
#include <memory>
#include <stdint.h>
#include <assert.h>



namespace bc {
namespace memory {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to store allocation information in front of the raw pointer given to user.
struct alignas( 64 ) SystemMemoryAllocationInfo
{
	void					*	system_allocated_location		= nullptr;
	void					*	payload_location				= nullptr;

	u64							system_allocation_size			= 0;
	u64							payload_size					= 0;
	u64							payload_alignment_requirement	= 0;

	u64							reserved[ 2 ]					= {};

	u64							checksum						= 0;
};
static_assert( sizeof( SystemMemoryAllocationInfo ) == 64 );
static_assert( alignof( SystemMemoryAllocationInfo ) == 64 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates the checksum specifically for the SystemMemoryAllocationInfo
///
/// @note
/// The last field of SystemMemoryAllocationInfo is a checksum, it is always ignored by this function.
///
/// @param allocation_info
/// Allocation info we want to calculate the checksum for. The last field of the SystemMemoryAllocationInfo is a checksum and is
/// ignored.
///
/// @return
/// Checksum for SystemMemoryAllocationInfo.
inline u64												CalculateSystemMemoryAllocationInfoInfoChecksum(
	const SystemMemoryAllocationInfo				&	allocation_info
)
{
	#if BITCRAFTE_DEVELOPMENT_BUILD
	// Checksum calculation is disabled for shipping builds, errors should have been caught in development builds.

	auto bytes = reinterpret_cast<const u8*>( &allocation_info );
	auto size = sizeof( SystemMemoryAllocationInfo ) - sizeof( u64 );
	u64 checksum = 0xDEADBEEFDEADBEEF;
	for( u64 i = 0; i < size; ++i )
	{
		checksum = ( checksum << 5 ) + ( checksum + static_cast<u64>( bytes[ i ] ) );
	}
	return checksum;

	#else

	return 0;

	#endif
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Align memory address to the next available address that lands on an integer divisible position of alignment requirement.
///
/// @param location
/// Pointer that needs to be aligned.
///
/// @param alignment_requirement
/// Required alignment.
///
/// @return
///	Aligned pointer.
inline void											*	AlignMemoryToRequirement(
	void											*	location,
	u64													alignment_requirement
)
{
	auto is_alignment_requirement_power_of_2 = ( alignment_requirement & ( alignment_requirement - 1 ) ) == 0;

	BHardAssert( alignment_requirement > 0, "Align memory to requirement, alignment must be larger than 0" );
	BHardAssert( alignment_requirement <= 0x8000, "Align memory to requirement, alignment must be smaller or equal to 32kb" );
	BHardAssert( is_alignment_requirement_power_of_2, "Align memory to requirement, alignment must be power of 2" );

	auto ptr_position = reinterpret_cast<uintptr_t>( location );
	auto aligned_ptr_position = ( ptr_position - 1 ) / alignment_requirement * alignment_requirement + alignment_requirement;

	// Test for overflow, however this shouldn't ever happen in practice, it may detect some errors.
	BHardAssert( aligned_ptr_position >= ptr_position, "Align memory to requirement, pointer overflow" );

	return reinterpret_cast<void*>( aligned_ptr_position );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculate total required space for allocating from memory pool or OS.
///
/// @warning
/// This is an internal function used when allocating a new memory block from system, please do not use.
///
/// @param payload_size
/// Size requirement for the payload.
/// 
/// @param payload_alignment_requirement
/// Alignment requirement for the payload.
///
/// @return
/// Required allocation size that has enough space for both SystemMemoryAllocationInfo and the payload at its correct alignment
/// requirement.
inline u64												CalculateMinimumRequiredSystemMemoryAllocationSize(
	u64													payload_size,
	u64													payload_alignment_requirement
)
{
	if( payload_alignment_requirement < alignof( SystemMemoryAllocationInfo ) ) payload_alignment_requirement = alignof( SystemMemoryAllocationInfo );
	auto minimum_allocation_size = payload_size + payload_alignment_requirement + sizeof( SystemMemoryAllocationInfo );

	assert( minimum_allocation_size > payload_size && "Allocation size was smaller than payload size, this should never happen" );

	return minimum_allocation_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This calculates the SystemMemoryAllocationInfo from an already allocated system allocation.
///
/// @warning
/// This is an internal function used when allocating a new memory block from system, please do not use.
///
/// @param system_allocated_location
/// Pointer to memory location that was originally received from underlaying allocation scheme, either OS or memory pool.
/// 
/// @param system_allocated_size
/// Size of the memory allocated from underlaying system in bytes.
///
/// @param payload_size
/// Size requirement for the payload.
/// 
/// @param payload_alignment_requirement
/// Alignment requirement for the payload.
///
/// @return
/// New allocation info that can be used to store memory allocation info in front of the pointer given to user.
inline SystemMemoryAllocationInfo						CalculateSystemMemoryAllocationInfoFromSystemAllocation(
	void											*	system_allocated_location,
	u64													system_allocated_size,
	u64													payload_size,
	u64													payload_alignment_requirement
)
{
	auto is_payload_alignment_requirement_power_of_2 = ( payload_alignment_requirement & ( payload_alignment_requirement - 1 ) ) == 0;

	BHardAssert( system_allocated_location != nullptr, "Calculate system memory allocation info from system allocation, no system memory pointer given" );

	BHardAssert( system_allocated_size > 0, "Calculate system memory allocation info from system allocation, system allocated size must be larger than 0" );
	BHardAssert( system_allocated_size < 0x0000FFFFFFFFFFFF, "Calculate system memory allocation info from system allocation, system allocated size too large" );

	BHardAssert( payload_size > 0, "Calculate system memory allocation info from system allocation, payload size must be larger than 0" );
	BHardAssert( payload_size < 0x0000FFFFFFFFFFFF, "Calculate system memory allocation info from system allocation, payload size too large" );

	BHardAssert( payload_alignment_requirement > 0, "Calculate system memory allocation info from system allocation, payload alignment must be larger than 0" );
	BHardAssert( payload_alignment_requirement <= 0x8000, "Calculate system memory allocation info from system allocation, payload alignment must be smaller or equal to 32kb" );
	BHardAssert( is_payload_alignment_requirement_power_of_2, "Calculate system memory allocation info from system allocation, alignment must be power of 2" );

	auto allocation_info							= SystemMemoryAllocationInfo {};
	allocation_info.system_allocated_location		= system_allocated_location;
	allocation_info.payload_location				= AlignMemoryToRequirement(
		reinterpret_cast<u8*>( system_allocated_location ) + sizeof( SystemMemoryAllocationInfo ),
		std::max( payload_alignment_requirement, alignof( SystemMemoryAllocationInfo ) )
	);
	allocation_info.system_allocation_size			= system_allocated_size;
	allocation_info.payload_size					= payload_size;
	allocation_info.payload_alignment_requirement	= payload_alignment_requirement;

	allocation_info.checksum						= CalculateSystemMemoryAllocationInfoInfoChecksum(
		allocation_info
	);

	auto system_allocated_ptr_memory_position = reinterpret_cast<uintptr_t>( allocation_info.system_allocated_location );
	auto system_allocated_ptr_memory_position_end = system_allocated_ptr_memory_position + system_allocated_size;
	auto payload_ptr_memory_position = reinterpret_cast<uintptr_t>( allocation_info.payload_location );

	assert( payload_ptr_memory_position >= system_allocated_ptr_memory_position && "Payload pointer was smaller than system allocated" );
	assert( !( payload_ptr_memory_position % payload_alignment_requirement ) && "Payload pointer not aligned properly" );
	assert( payload_ptr_memory_position + payload_size <= system_allocated_ptr_memory_position_end && "Payload does not fit into allocated memory" );

	return allocation_info;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Sets the system memory allocation info in front of user returned pointer.
///
/// @param allocation_info
/// Allocation info. This contains everything needed to set itself at the proper location.
inline void												SetSystemMemoryAllocationInfo(
	SystemMemoryAllocationInfo						&	allocation_info
)
{
	auto allocation_info_position = reinterpret_cast<uintptr_t>( allocation_info.payload_location ) - sizeof( SystemMemoryAllocationInfo );
	assert( !( allocation_info_position % alignof( SystemMemoryAllocationInfo ) ) && "Allocation info must be aligned to alignof( SystemMemoryAllocationInfo )");
	auto allocation_info_ptr = reinterpret_cast<SystemMemoryAllocationInfo*>( allocation_info_position );
	*allocation_info_ptr = allocation_info;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to get the SystemMemoryAllocationInfo residing in front of the raw memory given to user.
///
/// @param raw_location
/// Pointer give to user.
///
/// @return
/// Pointer to allocation info on success, nullptr if something went wrong.
inline const SystemMemoryAllocationInfo				*	GetSystemMemoryAllocationInfoFromRawPointer(
	const void										*	raw_location
)
{
	if( raw_location == nullptr ) return nullptr; // raw_location was nullptr, return.

	auto raw_ptr_position = reinterpret_cast<uintptr_t>( raw_location );
	if( raw_ptr_position % alignof( SystemMemoryAllocationInfo ) ) return nullptr; // raw_location isn't aligned to SystemMemoryAllocationInfo, which is the minimum, return.

	auto raw_location_bytes = reinterpret_cast<const u8*>( raw_location );

	auto allocation_info = reinterpret_cast<const SystemMemoryAllocationInfo*>( raw_location_bytes - sizeof( SystemMemoryAllocationInfo ) );

	#if BITCRAFTE_DEVELOPMENT_BUILD
	auto allocation_info_checksum = CalculateSystemMemoryAllocationInfoInfoChecksum( *allocation_info );
	if( allocation_info->checksum != allocation_info_checksum ) return nullptr; // Checksum mismatch, this is not a runtime allocated memory block.
	#endif

	return allocation_info;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to get the SystemMemoryAllocationInfo residing in front of the raw memory given to user.
///
/// @param raw_location
/// Pointer give to user.
///
/// @return
/// Pointer to allocation info on success, nullptr if something went wrong.
inline SystemMemoryAllocationInfo					*	GetSystemMemoryAllocationInfoFromRawPointer(
	void											*	raw_location
)
{
	if( raw_location == nullptr )
	{
		return nullptr;
	}

	auto raw_ptr_position = reinterpret_cast<uintptr_t>( raw_location );
	if( raw_ptr_position % alignof( SystemMemoryAllocationInfo ) )
	{
		// raw_location isn't aligned to SystemMemoryAllocationInfo, which is the minimum, return.
		return nullptr;
	}

	auto raw_location_bytes = reinterpret_cast<u8*>( raw_location );

	auto allocation_info = reinterpret_cast<SystemMemoryAllocationInfo*>( raw_location_bytes - sizeof( SystemMemoryAllocationInfo ) );

	#if BITCRAFTE_DEVELOPMENT_BUILD
	auto allocation_info_checksum = CalculateSystemMemoryAllocationInfoInfoChecksum( *allocation_info );
	if( allocation_info->checksum != allocation_info_checksum )
	{
		// Checksum mismatch, this is not a runtime allocated memory block.
		return nullptr;
	}
	#endif

	return allocation_info;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates the unused but allocated space for the payload in an allocation.
///
/// @param allocation_info
/// Allocation info to existing memory allocation.
///
/// @return
/// Unused space left after the payload.
inline u64												CalculateUnusedPayloadSpaceInAllocation(
	const SystemMemoryAllocationInfo				&	allocation_info
)
{
	BHardAssert(
		allocation_info.payload_location > allocation_info.system_allocated_location,
		"Unable to calculate unused space in allocation, payload location was before system allocation location"
	);

	auto payload_offset_to_system_ptr = reinterpret_cast<u64>( allocation_info.payload_location ) - reinterpret_cast<u64>( allocation_info.system_allocated_location );
	BHardAssert(
		allocation_info.system_allocation_size >= payload_offset_to_system_ptr + allocation_info.payload_size,
		"Unable to calculate unused space in allocation, apparent system allocation size was smaller than actual space required by the payload"
	);
	return allocation_info.system_allocation_size - payload_offset_to_system_ptr - allocation_info.payload_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if allocation is in-place re-allocate-able, meaning it can be reused.
///
/// @param allocation_info
/// Allocation info describing the allocated memory.
///
/// @param new_size
/// New size we want the allocation to be.
///
/// @return
/// True if allocation can accomodate the new size, false if new size does not fit into already allocated memory.
inline bool												IsInPlaceReallocateable_Runtime(
	const SystemMemoryAllocationInfo				&	allocation_info,
	u64													new_size
)
{
	auto allocation_remaining_payload_size = CalculateUnusedPayloadSpaceInAllocation( allocation_info );
	return allocation_info.payload_size + allocation_remaining_payload_size >= new_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Do in-place memory allocation.
///
/// @warning
/// Current system memory allocation must have enough space to accommodate the new payload size.
///
/// @param allocation_info
/// Allocation info struct describing the allocation.
///
/// @param new_size
/// New required size.
///
/// @return
/// Pointer to payload start, this is the same as original payload pointer.
inline void											*	InPlaceReallocateMemory_Runtime(
	SystemMemoryAllocationInfo						&	allocation_info,
	u64													new_size
)
{
	assert( IsInPlaceReallocateable_Runtime( allocation_info, new_size ) && "Not in place reallocateable, size too large" );

	// Reallocation fits inside the old memory pointer, we can just reuse it.
	auto new_allocation_info = CalculateSystemMemoryAllocationInfoFromSystemAllocation(
		allocation_info.system_allocated_location,
		allocation_info.system_allocation_size,
		new_size,
		allocation_info.payload_alignment_requirement
	);
	SetSystemMemoryAllocationInfo( new_allocation_info );
	assert(
		allocation_info.payload_location == new_allocation_info.payload_location &&
		"Expected old payload location to match new payload location."
	);
	return new_allocation_info.payload_location;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Frees compile time memory.
///
/// @note
/// Internal function, Please use memory::FreeRawMemory instead, it works in both runtime and compile time.
///
/// @param location
/// Pointer to start of the memory location we want to free the memory of.
///
/// @param count
/// Number of elements to free.
/// @warning
/// This must match the amount that was originally allocated in this memory location.
template<typename ValueType>
constexpr inline void			FreeMemory_Consteval(
	ValueType				*	location,
	u64							count
) noexcept
{
	std::allocator<ValueType>{}.deallocate( location, size_t( count ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Allocates memory at compile time.
///
/// @note
/// Internal function, Please use memory::AllocateRawMemory instead, it works in both runtime and compile time.
///
/// @tparam ValueType
/// Element type.
///
/// @param count
/// Number of elements to reserve space for.
///
/// @param alignment_requirement
/// Alignment in bytes.
/// @note
/// Must be above 0, must be below or equal to 32k, must power-of-2.
///
/// @return
/// Pointer to beginning of allocated memory.
template<typename ValueType>
constexpr ValueType			*	AllocateMemory_Consteval(
	u64							count,
	u64							alignment_requirement
) noexcept
{
	return std::allocator<ValueType>{}.allocate( size_t( count ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Reallocates memory at compile time.
///
/// Alignment is preserved from original allocation, to change alignment, you'll need to reallocate manually.
///
/// @warning
/// Contents are copied over byte by byte, no construction, destruction or deep copies are made, reallocated data must be
/// trivially constructible and trivially destructible or this function causes undefined behaviour in C++ objects.
///
/// @note
/// Internal function, Please use memory::ReallocateRawMemory instead, it works in both runtime and compile time.
///
/// @tparam ValueType
/// Element type.
///
/// @param old_location
/// Pointer to old memory location.
///
/// @param old_count
/// Number of elements in the old memory block.
/// @warning
/// This must match the number of elements that was allocated in this memory location.
///
/// @param new_count
/// Number of elements in the new memory block.
///
/// @return
/// Pointer to a new block of memory that has enough space to contain the new size.
template<typename ValueType>
constexpr ValueType			*	ReallocateMemory_Consteval(
	ValueType				*	old_location,
	u64							old_count,
	u64							new_count
) noexcept
{
	auto new_location = AllocateMemory_Consteval<ValueType>( new_count, alignof( ValueType ) );
	auto common_length = old_count < new_count ? old_count : new_count;
	for( u64 i = 0; i < common_length; i++ )
	{
		if constexpr( std::is_move_assignable_v<ValueType> ) new_location[ i ] = std::move( old_location[ i ] );
		if constexpr( std::is_copy_assignable_v<ValueType> ) new_location[ i ] = old_location[ i ];
	}
	FreeMemory_Consteval<ValueType>( old_location, old_count );
	return new_location;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Frees runtime memory.
///
/// @note
/// Internal function, Please use memory::FreeRawMemory instead, it works in both runtime and compile time.
///
/// @param location
/// Pointer to start of memory location we want to free.
BITCRAFTE_ENGINE_API
void							FreeRawMemory_Runtime(
	void					*	location
) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Allocates memory at runtime.
///
/// @note
/// Internal function, Please use memory::AllocateRawMemory instead, it works in both runtime and compile time.
///
/// @param size
/// Size in bytes of how much raw memory to allocate.
///
/// @param alignment_requirement
/// Alignment in bytes.
/// @note
/// Must be above 0, must be below or equal to 32k, must power-of-2.
///
/// @return
/// Pointer to beginning of allocated memory.
BITCRAFTE_ENGINE_API
void						*	AllocateRawMemory_Runtime(
	u64							size,
	u64							alignment_requirement
) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Reallocates memory at runtime.
///
/// Alignment is preserved from original allocation, to change alignment, you'll need to reallocate manually.
///
/// @warning
/// Contents are copied over byte by byte, no construction, destruction or deep copies are made, reallocated data must be
/// trivially constructible and trivially destructible or this function causes undefined behaviour in C++ objects.
///
/// @note
/// Internal function, Please use memory::ReallocateRawMemory instead, it works in both runtime and compile time.
///
/// @param old_location
/// Pointer to old raw memory.
///
/// @param new_size
/// Size of the new memory block, in bytes.
///
/// @return
/// Pointer to a new block of memory that has enough space to contain the new size.
BITCRAFTE_ENGINE_API
void						*	ReallocateRawMemory_Runtime(
	void					*	old_location,
	u64							new_size
) noexcept;



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr void					FreeMemory(
	ValueType				*	location,
	u64							count
) noexcept
{
	if( std::is_constant_evaluated() )
	{
		internal::FreeMemory_Consteval<ValueType>( location, count );
	}
	else
	{
		// Assert some common Visual studio fault addresses.
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Freeing memory outside of process" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Freeing already freed memory" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Freeing uninitialized global memory" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Freeing uninitialized stack memory" );

		BHardAssert( count < 0x0000FFFFFFFFFFFF, "Freeing memory, element count too high, something is not right" );

		internal::FreeRawMemory_Runtime( location );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr ValueType			*	AllocateMemory(
	u64							count,
	u64							alignment_requirement
) noexcept
{
	if( std::is_constant_evaluated() )
	{
		return internal::AllocateMemory_Consteval<ValueType>( count, alignment_requirement );
	}
	else
	{
		BHardAssert( count > 0, "Allocating memory, new element count must be larger than 0" );
		BHardAssert( count < 0x0000FFFFFFFFFFFF, "Allocating memory, new element count too high, something is not right" );

		return reinterpret_cast<ValueType*>( internal::AllocateRawMemory_Runtime( count * sizeof( ValueType ), alignment_requirement ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr ValueType			*	ReallocateMemory(
	ValueType				*	old_location,
	u64							old_count,
	u64							new_count
) noexcept
{
	static_assert( std::is_trivial_v<ValueType>, "Type must be trivial for it to be reallocated via this function" );

	if( std::is_constant_evaluated() )
	{
		return internal::ReallocateMemory_Consteval<ValueType>( old_location, old_count, new_count );
	}
	else
	{
		// Assert some common Visual studio fault addresses.
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Reallocating memory, with old pointer pointing to memory outside of process" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Reallocating memory, with old pointer pointing to freed memory" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Reallocating memory, with old pointer pointing to uninitialized global memory" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Reallocating memory, with old pointer pointing to uninitialized stack memory" );

		BHardAssert( old_location != nullptr, "Reallocating memory, old location is nullptr" );
		BHardAssert( old_count > 0, "Reallocating memory, old element count must be larger than 0" );
		BHardAssert( old_count < 0x0000FFFFFFFFFFFF, "Reallocating memory, old element count too high, something is not right" );

		BHardAssert( old_count != new_count, "Reallocating memory, new reserved element count is the same as the old element count, this check should be done earlier" );

		return reinterpret_cast<ValueType*>( internal::ReallocateRawMemory_Runtime( old_location, new_count * sizeof( ValueType ) ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr bool					IsInPlaceReallocateable(
	const ValueType			*	location,
	u64							new_count
)
{
	if( std::is_constant_evaluated() )
	{
		return false;
	}
	else
	{
		auto allocation_info = internal::GetSystemMemoryAllocationInfoFromRawPointer( location );
		return internal::IsInPlaceReallocateable_Runtime( *allocation_info, new_count * sizeof( ValueType ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr ValueType				*	InPlaceReallocateMemory(
	ValueType					*	old_location,
	u64								old_count,
	u64								new_count
)
{
	if( std::is_constant_evaluated() )
	{
		return internal::ReallocateMemory_Consteval<ValueType>( old_location, old_count, new_count );
	}
	else
	{
		auto allocation_info = internal::GetSystemMemoryAllocationInfoFromRawPointer( old_location );

		BHardAssert( allocation_info, "In place reallocate memory, new reserved element count is the same as the old element count, this check should be done earlier" );
		BHardAssert(
			allocation_info->payload_size != new_count * sizeof( ValueType ),
			"In place reallocate memory, new reserved element count is the same as the old element count, this check should be done earlier"
		);

		return reinterpret_cast<ValueType*>( internal::InPlaceReallocateMemory_Runtime( *allocation_info, new_count * sizeof( ValueType ) ) );
	}
}



} // memory
} // bc
