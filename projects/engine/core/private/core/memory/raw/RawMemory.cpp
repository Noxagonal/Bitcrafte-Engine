
#include <core/PreCompiledHeader.hpp>
#include <core/memory/raw/RawMemory.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::memory::internal_::FreeRawMemory_Runtime(
	void			*	location
) noexcept
{
	if( location == nullptr ) return;

	auto allocation_info = GetSystemMemoryAllocationInfoFromRawPointer( location );
	BHardAssert( allocation_info, "Couldn't free runtime memory, memory pointer was not allocated from bc::memory utilities" );

	// TODO: Free from memory pool once it's implemented.
	delete[] reinterpret_cast<u8*>( allocation_info->system_allocated_location );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * bc::memory::internal_::AllocateRawMemory_Runtime(
	u64				size,
	u64				alignment_requirement
) noexcept
{
	auto minimum_required_allocation_size = CalculateMinimumRequiredSystemMemoryAllocationSize(
		size,
		alignment_requirement
	);

	// TODO: Allocate from memory pool once it's implemented.
	auto system_ptr = new u8[ minimum_required_allocation_size ];
	if( system_ptr == nullptr ) std::abort();
	BHardAssert( ( reinterpret_cast<uintptr_t>( system_ptr ) & 0xFFFF000000000000ULL ) == 0ULL, "Allocated memory from system needs to have high 16 bits unused" );

	auto system_allocation_info = CalculateSystemMemoryAllocationInfoFromSystemAllocation(
		system_ptr,
		minimum_required_allocation_size,
		size, 
		alignment_requirement
	);
	SetSystemMemoryAllocationInfo( system_allocation_info );

	return system_allocation_info.payload_location;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * bc::memory::internal_::ReallocateRawMemory_Runtime(
	void			*	old_location,
	u64					new_size
) noexcept
{
	auto old_allocation_info = GetSystemMemoryAllocationInfoFromRawPointer( old_location );
	BHardAssert( old_allocation_info, "Couldn't reallocate runtime memory, old memory pointer was not allocated from bc::memory utilities" );

	auto old_size								= old_allocation_info->payload_size;

	// This checks if the allocation has enough payload space remaining at the end to accommodate new payload size, in this case
	// we can just recalculate the allocation info struct and return the old pointer.
	if( IsInPlaceReallocateable_Runtime( *old_allocation_info, new_size ) )
	{
		return InPlaceReallocateMemory_Runtime( *old_allocation_info, new_size );
	}

	// TODO: Test if new size fits inside the old allocation size in the memory pool, if it does, update memory pool allocation
	// tracker, update SystemMemoryAllocationInfo, and return old_location.

	// We need to make sure that we have enough space for correct alignment requirement, so we allocate extra.
	auto new_ptr = AllocateRawMemory_Runtime(
		new_size,
		old_allocation_info->payload_alignment_requirement
	);

	auto common_size = ( old_size < new_size ) ? old_size : new_size;
	for( u64 i = 0; i < common_size; ++i )
	{
		reinterpret_cast<u8*>( new_ptr )[ i ] = reinterpret_cast<u8*>( old_location )[ i ];
	}

	FreeRawMemory_Runtime( old_location );
	return new_ptr;
}
