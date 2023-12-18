
#include <core/PreCompiledHeader.hpp>
#include <core/memory/raw/RawMemory.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::memory::internal::FreeRawMemory_Runtime(
	void			*	location
) noexcept
{
	auto allocation_info = GetSystemMemoryAllocationInfoFromRawPointer( location );

	assert( allocation_info );
	if( allocation_info == nullptr ) diagnostic::Panic( "Freeing memory that is not allocated using bc runtime allocation functions" );

	// TODO: Free from memory pool once it's implemented.
	delete[] reinterpret_cast<uint8_t*>( allocation_info->system_allocated_location );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * bc::memory::internal::AllocateRawMemory_Runtime(
	size_t				size,
	size_t				alignment_requirement
) noexcept
{
	auto minimum_required_allocation_size = CalculateMinimumRequiredSystemMemoryAllocationSize(
		size,
		alignment_requirement
	);

	// TODO: Allocate from memory pool once it's implemented.
	auto system_ptr = new uint8_t[ minimum_required_allocation_size ];

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
void * bc::memory::internal::ReallocateRawMemory_Runtime(
	void			*	old_location,
	size_t				new_size
) noexcept
{
	auto old_allocation_info = GetSystemMemoryAllocationInfoFromRawPointer( old_location );
	if( old_allocation_info == nullptr ) std::abort();

	auto old_size				= old_allocation_info->payload_size;
	auto alignment_requirement	= old_allocation_info->payload_alignment_requirement;

	// TODO: Test if new size fits inside the old allocation size, if it does, just return old_location instead.
	// TODO: Test if new size fits inside the old allocation size in the memory pool, if it does, update memory pool allocation
	// tracker, update SystemMemoryAllocationInfo, and return old_location.

	// We need to make sure that we have enough space for correct alignment requirement, so we allocate extra.
	auto new_ptr = AllocateRawMemory_Runtime(
		new_size,
		alignment_requirement
	);

	auto common_size = ( old_size < new_size ) ? old_size : new_size;
	for( size_t i = 0; i < common_size; ++i )
	{
		reinterpret_cast<uint8_t*>( new_ptr )[ i ] = reinterpret_cast<uint8_t*>( old_location )[ i ];
	}

	FreeRawMemory_Runtime( old_location );
	return new_ptr;
}
