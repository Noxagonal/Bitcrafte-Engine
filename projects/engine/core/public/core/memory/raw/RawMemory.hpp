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
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to store allocation information in front of the pointer given to user.
struct alignas( 64 ) MemoryAllocationHeader
{
	void*	system_allocated_location		= nullptr;
	void*	payload_location				= nullptr;

	u64		system_allocation_size			= 0;
	u64		payload_size					= 0;
	u64		payload_alignment_requirement	= 0;

	u64		reserved[ 2 ]					= {};

	u64		checksum						= 0;
};
static_assert( sizeof( MemoryAllocationHeader ) == 64 );
static_assert( alignof( MemoryAllocationHeader ) == 64 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates the checksum specifically for the MemoryAllocationHeader
///
/// @note
/// The last field of MemoryAllocationHeader is a checksum, it is always ignored by this function.
///
/// @param allocation_header
/// Allocation info we want to calculate the checksum for. The last field of the MemoryAllocationHeader is a checksum and is
/// ignored.
///
/// @return
/// Checksum for MemoryAllocationHeader.
inline auto CalculateMemoryAllocationHeaderChecksum( const MemoryAllocationHeader & allocation_header ) -> u64
{
	#if BITCRAFTE_GAME_DEVELOPMENT_BUILD
	// Checksum calculation is disabled for shipping builds, errors should have been caught in development builds.

	auto bytes = reinterpret_cast<const u8*>( &allocation_header );
	auto size = sizeof( MemoryAllocationHeader ) - sizeof( u64 );
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
inline auto AlignMemoryToRequirement(
	void*	location,
	u64		alignment_requirement
) -> void*
{
	auto is_alignment_requirement_power_of_2 = ( alignment_requirement & ( alignment_requirement - 1 ) ) == 0;

	BHardAssert( alignment_requirement > 0, U"Cannot align memory to requirement, alignment must be larger than 0" );
	BHardAssert( alignment_requirement <= 0x8000, U"Cannot align memory to requirement, alignment must be smaller or equal to 32kb" );
	BHardAssert( is_alignment_requirement_power_of_2, U"Cannot align memory to requirement, alignment must be power of 2" );

	auto ptr_position = reinterpret_cast<uintptr_t>( location );
	auto aligned_ptr_position = ( ptr_position - 1 ) / alignment_requirement * alignment_requirement + alignment_requirement;

	// Test for overflow, however this shouldn't ever happen in practice, it may detect some errors.
	BHardAssert( aligned_ptr_position >= ptr_position, U"Cannot align memory to requirement, pointer overflow" );

	return reinterpret_cast<void*>( aligned_ptr_position );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculate total required space for allocating from memory pool or OS.
///
/// @warning
/// This is an internal_ function used when allocating a new memory block from system, please do not use.
///
/// @param payload_size
/// Size requirement for the payload.
/// 
/// @param payload_alignment_requirement
/// Alignment requirement for the payload.
///
/// @return
/// Required allocation size that has enough space for both MemoryAllocationHeader and the payload at its correct alignment
/// requirement.
inline auto CalculateMinimumRequiredSystemMemoryAllocationSize(
	u64		payload_size,
	u64		payload_alignment_requirement
) -> u64
{
	if( payload_alignment_requirement < alignof( MemoryAllocationHeader ) ) payload_alignment_requirement = alignof( MemoryAllocationHeader );
	auto minimum_allocation_size = payload_size + payload_alignment_requirement + sizeof( MemoryAllocationHeader );

	assert( minimum_allocation_size > payload_size && "Allocation size was smaller than payload size, this should never happen" );

	return minimum_allocation_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This creates the MemoryAllocationHeader from an already allocated system allocation.
///
/// @warning
/// This is an internal_ function used when allocating a new memory block from system, please do not use.
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
inline auto CreateMemoryAllocationHeader(
	void*	system_allocated_location,
	u64		system_allocated_size,
	u64		payload_size,
	u64		payload_alignment_requirement
) -> MemoryAllocationHeader
{
	auto is_payload_alignment_requirement_power_of_2 = ( payload_alignment_requirement & ( payload_alignment_requirement - 1 ) ) == 0;

	BHardAssert( system_allocated_location != nullptr, U"Cannot calculate system memory allocation info from system allocation, no system memory pointer given" );

	BHardAssert( system_allocated_size > 0, U"Cannot calculate system memory allocation info from system allocation, system allocated size must be larger than 0" );
	BHardAssert( system_allocated_size < 0x0000FFFFFFFFFFFF, U"Cannot calculate system memory allocation info from system allocation, system allocated size too large" );

	BHardAssert( payload_size > 0, U"Cannot calculate system memory allocation info from system allocation, payload size must be larger than 0" );
	BHardAssert( payload_size < 0x0000FFFFFFFFFFFF, U"Cannot calculate system memory allocation info from system allocation, payload size too large" );

	BHardAssert( payload_alignment_requirement > 0, U"Cannot calculate system memory allocation info from system allocation, payload alignment must be larger than 0" );
	BHardAssert( payload_alignment_requirement <= 0x8000, U"Calculate system memory allocation info from system allocation, payload alignment must be smaller or equal to 32kb" );
	BHardAssert( is_payload_alignment_requirement_power_of_2, U"Cannot calculate system memory allocation info from system allocation, alignment must be power of 2" );

	auto allocation_header							= MemoryAllocationHeader {};
	allocation_header.system_allocated_location		= system_allocated_location;
	allocation_header.payload_location				= AlignMemoryToRequirement(
		reinterpret_cast<u8*>( system_allocated_location ) + sizeof( MemoryAllocationHeader ),
		std::max( payload_alignment_requirement, alignof( MemoryAllocationHeader ) )
	);
	allocation_header.system_allocation_size		= system_allocated_size;
	allocation_header.payload_size					= payload_size;
	allocation_header.payload_alignment_requirement	= payload_alignment_requirement;

	allocation_header.checksum						= CalculateMemoryAllocationHeaderChecksum(
		allocation_header
	);

	auto system_allocated_ptr_memory_position = reinterpret_cast<uintptr_t>( allocation_header.system_allocated_location );
	auto system_allocated_ptr_memory_position_end = system_allocated_ptr_memory_position + system_allocated_size;
	auto payload_ptr_memory_position = reinterpret_cast<uintptr_t>( allocation_header.payload_location );

	assert( payload_ptr_memory_position >= system_allocated_ptr_memory_position && "Payload pointer was smaller than system allocated" );
	assert( !( payload_ptr_memory_position % payload_alignment_requirement ) && "Payload pointer not aligned properly" );
	assert( payload_ptr_memory_position + payload_size <= system_allocated_ptr_memory_position_end && "Payload does not fit into allocated memory" );

	return allocation_header;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Sets the system memory allocation info in front of user returned pointer.
///
/// @param allocation_header
/// Allocation info. This contains everything needed to set itself at the proper location.
inline void SetMemoryAllocationHeader( MemoryAllocationHeader& allocation_header )
{
	auto allocation_header_position = reinterpret_cast<uintptr_t>( allocation_header.payload_location ) - sizeof( MemoryAllocationHeader );
	assert( !( allocation_header_position % alignof( MemoryAllocationHeader ) ) && "Allocation info must be aligned to alignof( MemoryAllocationHeader )");
	auto allocation_header_ptr = reinterpret_cast<MemoryAllocationHeader*>( allocation_header_position );
	*allocation_header_ptr = allocation_header;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to get the MemoryAllocationHeader residing in front of the memory pointer given to user.
///
/// @param user_location
/// Pointer given to user.
///
/// @return
/// Pointer to allocation info on success, nullptr if something went wrong.
inline auto GetMemoryAllocationHeaderFromUserPointer( const void* user_location ) -> const MemoryAllocationHeader*
{
	if( user_location == nullptr ) return nullptr; // user_location was nullptr, return.

	auto user_ptr_position = reinterpret_cast<uintptr_t>( user_location );
	if( user_ptr_position % alignof( MemoryAllocationHeader ) ) return nullptr; // user_location isn't aligned to MemoryAllocationHeader, which is the minimum, return.

	auto user_location_bytes = reinterpret_cast<const u8*>( user_location );

	auto allocation_header = reinterpret_cast<const MemoryAllocationHeader*>( user_location_bytes - sizeof( MemoryAllocationHeader ) );

	#if BITCRAFTE_GAME_DEVELOPMENT_BUILD
	auto allocation_header_checksum = CalculateMemoryAllocationHeaderChecksum( *allocation_header );
	if( allocation_header->checksum != allocation_header_checksum ) return nullptr; // Checksum mismatch, this is not a runtime allocated memory block.
	#endif

	return allocation_header;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to get the MemoryAllocationHeader residing in front of the memory pointer given to user.
///
/// @param user_location
/// Pointer given to user.
///
/// @return
/// Pointer to allocation info on success, nullptr if something went wrong.
inline auto GetMemoryAllocationHeaderFromUserPointer( void* user_location ) -> MemoryAllocationHeader*
{
	if( user_location == nullptr ) return nullptr; // user_location was nullptr, return.

	auto user_ptr_position = reinterpret_cast<uintptr_t>( user_location );
	if( user_ptr_position % alignof( MemoryAllocationHeader ) ) return nullptr; // user_location isn't aligned to MemoryAllocationHeader, which is the minimum, return.

	auto user_location_bytes = reinterpret_cast<u8*>( user_location );

	auto allocation_header = reinterpret_cast<MemoryAllocationHeader*>( user_location_bytes - sizeof( MemoryAllocationHeader ) );

	#if BITCRAFTE_GAME_DEVELOPMENT_BUILD
	auto allocation_header_checksum = CalculateMemoryAllocationHeaderChecksum( *allocation_header );
	if( allocation_header->checksum != allocation_header_checksum ) return nullptr; // Checksum mismatch, this is not a runtime allocated memory block.
	#endif

	return allocation_header;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Calculates the unused but allocated space for the payload in an allocation.
///
/// @param allocation_header
/// Allocation info to existing memory allocation.
///
/// @return
/// Unused space left after the payload.
inline auto CalculateUnusedPayloadSpaceInAllocation( const MemoryAllocationHeader& allocation_header ) -> u64
{
	BHardAssert(
		allocation_header.payload_location > allocation_header.system_allocated_location,
		U"Cannot calculate unused space in allocation, payload location was before system allocation location"
	);

	auto payload_offset_to_system_ptr = reinterpret_cast<u64>( allocation_header.payload_location ) - reinterpret_cast<u64>( allocation_header.system_allocated_location );
	BHardAssert(
		allocation_header.system_allocation_size >= payload_offset_to_system_ptr + allocation_header.payload_size,
		U"Cannot calculate unused space in allocation, apparent system allocation size was smaller than actual space required by the payload"
	);
	return allocation_header.system_allocation_size - payload_offset_to_system_ptr - allocation_header.payload_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if allocation is in-place re-allocate-able, meaning it can be reused.
///
/// @param allocation_header
/// Allocation info describing the allocated memory.
///
/// @param new_size
/// New size we want the allocation to be.
///
/// @return
/// True if allocation can accomodate the new size, false if new size does not fit into already allocated memory.
inline auto IsInPlaceReallocateable_Runtime(
	const MemoryAllocationHeader&	allocation_header,
	u64								new_size
) -> bool
{
	auto allocation_remaining_payload_size = CalculateUnusedPayloadSpaceInAllocation( allocation_header );
	return allocation_header.payload_size + allocation_remaining_payload_size >= new_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Do in-place memory allocation.
///
/// @warning
/// Current system memory allocation must have enough space to accommodate the new payload size.
///
/// @param allocation_header
/// Allocation info struct describing the allocation.
///
/// @param new_size
/// New required size.
///
/// @return
/// Pointer to payload start, this is the same as original payload pointer.
inline auto InPlaceReallocateMemory_Runtime(
	MemoryAllocationHeader&		allocation_header,
	u64							new_size
) -> void*
{
	BHardAssert( IsInPlaceReallocateable_Runtime( allocation_header, new_size ), U"Not in place reallocateable, size too large" );

	// Reallocation fits inside the old memory pointer, we can just reuse it.
	auto new_allocation_header = CreateMemoryAllocationHeader(
		allocation_header.system_allocated_location,
		allocation_header.system_allocation_size,
		new_size,
		allocation_header.payload_alignment_requirement
	);
	SetMemoryAllocationHeader( new_allocation_header );
	assert(
		allocation_header.payload_location == new_allocation_header.payload_location &&
		"Expected old payload location to match new payload location."
	);
	return new_allocation_header.payload_location;
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
constexpr inline void FreeMemory_Consteval(
	ValueType*	location,
	u64			count
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
constexpr auto AllocateMemory_Consteval(
	u64	count,
	u64	alignment_requirement
) noexcept -> ValueType*
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
constexpr auto ReallocateMemory_Consteval(
	ValueType*	old_location,
	u64			old_count,
	u64			new_count
) noexcept -> ValueType*
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
void FreeRawMemory_Runtime( void* location ) noexcept;



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
auto AllocateRawMemory_Runtime(
	u64	size,
	u64	alignment_requirement
) noexcept -> void*;



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
auto ReallocateRawMemory_Runtime(
	void*	old_location,
	u64		new_size
) noexcept -> void*;



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr void FreeMemory(
	ValueType*	location,
	u64			count
) noexcept
{
	#if __cpp_if_consteval
	if consteval
	#else
	if( std::is_constant_evaluated() )
	#endif
	{
		internal_::FreeMemory_Consteval<ValueType>( location, count );
	}
	else
	{
		// Assert some common Visual studio fault addresses.
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Cannot free memory, memory outside of process" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Cannot free memory, memory is already freed" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Cannot free memory, memory is uninitialized global memory" );
		BHardAssert( location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Cannot free memory, memory is uninitialized stack memory" );

		BHardAssert( count < 0x0000FFFFFFFFFFFF, U"Freeing memory, element count too high, something is not right" );

		internal_::FreeRawMemory_Runtime( location );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr auto AllocateMemory(
	u64	count,
	u64	alignment_requirement
) noexcept -> ValueType*
{
	#if __cpp_if_consteval
	if consteval
	#else
	if( std::is_constant_evaluated() )
	#endif
	{
		return internal_::AllocateMemory_Consteval<ValueType>( count, alignment_requirement );
	}
	else
	{
		BHardAssert( count > 0, U"Cannot allocate memory, new element count must be larger than 0" );
		BHardAssert( count < 0x0000FFFFFFFFFFFF, U"Cannot allocate memory, new element count too high, something is not right" );

		return reinterpret_cast<ValueType*>( internal_::AllocateRawMemory_Runtime( count * sizeof( ValueType ), alignment_requirement ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr auto ReallocateMemory(
	ValueType*	old_location,
	u64			old_count,
	u64			new_count
) noexcept -> ValueType*
{
	static_assert( std::is_trivial_v<ValueType>, "Type must be trivial for it to be reallocated via this function" );

	#if __cpp_if_consteval
	if consteval
	#else
	if( std::is_constant_evaluated() )
	#endif
	{
		return internal_::ReallocateMemory_Consteval<ValueType>( old_location, old_count, new_count );
	}
	else
	{
		// Assert some common Visual studio fault addresses.
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Cannot reallocate memory, old pointer pointing to memory outside of process" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Cannot reallocate memory, old pointer pointing to freed memory" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Cannot reallocate memory, old pointer pointing to uninitialized global memory" );
		BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Cannot reallocate memory, old pointer pointing to uninitialized stack memory" );

		BHardAssert( old_location != nullptr, U"Cannot reallocate memory, old location is nullptr" );
		BHardAssert( old_count > 0, U"Cannot reallocate memory, old element count must be larger than 0" );
		BHardAssert( old_count < 0x0000FFFFFFFFFFFF, U"Cannot reallocate memory, old element count too high, something is not right" );

		BHardAssert( old_count != new_count, U"Cannot reallocate memory, new reserved element count is the same as the old element count, this check should be done earlier" );

		return reinterpret_cast<ValueType*>( internal_::ReallocateRawMemory_Runtime( old_location, new_count * sizeof( ValueType ) ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Test if pointer has enough headroom to extend the allocation.
///
/// @see InPlaceReallocateMemory().
///
/// @tparam ValueType
/// Type of memory to be tested.
///
/// @param location
/// Pointer to beginning of the original allocation that should be in-place expanded.
///
/// @param new_count
/// Number of elements that should fit into the new allocation.
template<typename ValueType>
constexpr auto IsInPlaceReallocateable(
	const ValueType*	location,
	u64					new_count
) -> bool
{
	#if __cpp_if_consteval
	if consteval
	#else
	if( std::is_constant_evaluated() )
	#endif
	{
		return false;
	}
	else
	{
		auto allocation_header = internal_::GetMemoryAllocationHeaderFromUserPointer( location );
		return internal_::IsInPlaceReallocateable_Runtime( *allocation_header, new_count * sizeof( ValueType ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Reallocates memory in place.
///
/// The purpose of this function is to reconfigure the memory allocation header so that the allocation becomes larger.
/// This assumes that there is enough space at the end of the current allocation.
/// The memory pool is involved for in-place allocation if the current allocation does not have enough headroom.
/// 
/// @tparam ValueType
/// Type of memory to be allocated.
///
/// @param old_location
/// Pointer to beginning of the original allocation that should be in-place expanded.
///
/// @param old_count
/// Number of elements allocated at the old pointer.
///
/// @param new_count
/// Number of elements that should fit into the new allocation.
template<typename ValueType>
constexpr auto InPlaceReallocateMemory(
	ValueType*	old_location,
	u64			old_count,
	u64			new_count
) -> ValueType*
{
	#if __cpp_if_consteval
	if consteval
	#else
	if( std::is_constant_evaluated() )
	#endif
	{
		return internal_::ReallocateMemory_Consteval<ValueType>( old_location, old_count, new_count );
	}
	else
	{
		auto allocation_header = internal_::GetMemoryAllocationHeaderFromUserPointer( old_location );

		BHardAssert( allocation_header, U"Could not in-place reallocate memory, allocation header not found. This was not allocated using engine utilities or pointer did not point to the beginning of the allocated block." );
		BHardAssert(
			allocation_header->payload_size != new_count * sizeof( ValueType ),
			U"Could not in-place reallocate memory, new reserved element count is the same as the old element count, this check should be done earlier"
		);

		return reinterpret_cast<ValueType*>( internal_::InPlaceReallocateMemory_Runtime( *allocation_header, new_count * sizeof( ValueType ) ) );
	}
}



} // memory
} // bc
