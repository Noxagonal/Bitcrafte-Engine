#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>

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

	size_t						system_allocation_size			= 0;
	size_t						payload_size					= 0;
	size_t						payload_alignment_requirement	= 0;

	size_t						reserved[ 2 ]					= {};

	size_t						checksum						= 0;
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
inline size_t											CalculateSystemMemoryAllocationInfoInfoChecksum(
	const SystemMemoryAllocationInfo				&	allocation_info
)
{
	#if BITCRAFTE_DEVELOPMENT_BUILD
	// Checksum calculation is disabled for shipping builds, errors should have been caught in development builds.

	auto bytes = reinterpret_cast<const uint8_t*>( &allocation_info );
	auto size = sizeof( SystemMemoryAllocationInfo ) - sizeof( uintptr_t );
	size_t checksum = 0;
	for( size_t i = 0; i < size; ++i )
	{
		checksum ^= bytes[ i ];
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
	size_t												alignment_requirement
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
inline size_t											CalculateMinimumRequiredSystemMemoryAllocationSize(
	size_t												payload_size,
	size_t												payload_alignment_requirement
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
	size_t												system_allocated_size,
	size_t												payload_size,
	size_t												payload_alignment_requirement
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
		reinterpret_cast<uint8_t*>( system_allocated_location ) + sizeof( SystemMemoryAllocationInfo ),
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
	void											*	raw_location
)
{
	if( raw_location == nullptr ) return nullptr; // raw_location was nullptr, return.

	auto raw_ptr_position = reinterpret_cast<uintptr_t>( raw_location );
	if( raw_ptr_position % alignof( SystemMemoryAllocationInfo ) ) return nullptr; // raw_location isn't aligned to SystemMemoryAllocationInfo, which is the minimum, return.

	auto raw_location_bytes = reinterpret_cast<uint8_t*>( raw_location );

	auto allocation_info = reinterpret_cast<SystemMemoryAllocationInfo*>( raw_location_bytes - sizeof( SystemMemoryAllocationInfo ) );

	#if BITCRAFTE_DEVELOPMENT_BUILD
	auto allocation_info_checksum = CalculateSystemMemoryAllocationInfoInfoChecksum( *allocation_info );
	if( allocation_info->checksum != allocation_info_checksum ) return nullptr; // Checksum mismatch, this is not a runtime allocated memory block.
	#endif

	return allocation_info;
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
	size_t 						count
) noexcept
{
	std::allocator<ValueType>{}.deallocate( location, count );
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
	size_t						count,
	size_t						alignment_requirement
) noexcept
{
	return std::allocator<ValueType>{}.allocate( count );
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
	size_t 						old_count,
	size_t						new_count
) noexcept
{
	auto new_location = AllocateMemory_Consteval<ValueType>( new_count, alignof( ValueType ) );
	auto common_length = old_count < new_count ? old_count : new_count;
	for (size_t i = 0; i < common_length; i++)
	{
		new_location[ i ] = old_location[ i ];
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
	size_t						size,
	size_t						alignment_requirement
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
	size_t						new_size
) noexcept;



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr void					FreeMemory(
	ValueType				*	location,
	size_t 						count
) noexcept
{
	if( std::is_constant_evaluated() )
	{
		internal::FreeMemory_Consteval<ValueType>( location, count );
	}
	else
	{
		internal::FreeRawMemory_Runtime( location );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr ValueType			*	AllocateMemory(
	size_t						count,
	size_t						alignment_requirement
) noexcept
{
	if( std::is_constant_evaluated() )
	{
		return internal::AllocateMemory_Consteval<ValueType>( count, alignment_requirement );
	}
	else
	{
		return reinterpret_cast<ValueType*>( internal::AllocateRawMemory_Runtime( count * sizeof( ValueType ), alignment_requirement ) );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
constexpr ValueType			*	ReallocateMemory(
	ValueType				*	old_location,
	size_t						old_count,
	size_t						new_count
) noexcept
{
	static_assert( std::is_trivial_v<ValueType>, "Type must be trivial for it to be reallocated via this function" );

	if( std::is_constant_evaluated() )
	{
		return internal::ReallocateMemory_Consteval<ValueType>( old_location, old_count, new_count );
	}
	else
	{
		return reinterpret_cast<ValueType*>( internal::ReallocateRawMemory_Runtime( old_location, new_count * sizeof( ValueType ) ) );
	}
}



} // memory
} // bc
