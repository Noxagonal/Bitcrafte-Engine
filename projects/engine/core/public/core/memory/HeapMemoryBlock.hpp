#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/memory/raw/RawMemory.hpp>



namespace bc {
namespace memory {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// A proto-smart pointer, used to store a block of heap allocated memory.
///
/// This is meant to be used as a backing storage type for all heap allocated memory.
///
/// The primary purpose of this is to make sure memory gets freed when this object goes out of scope. This is useful when used
/// in objects which constructor might throw an exception. This ensures that the memory gets freed regardless of whether the
/// object constructor throws an exception or not.
///
/// @warning
/// This is meant as a low-level type, and should be used with care. Proper usage is expected.
///
/// @tparam ElementType
/// Type of elements stored in the memory block.
template<typename ElementType>
class HeapMemoryBlock
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HeapMemoryBlock() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	explicit HeapMemoryBlock( ElementType* data, u64 count ) noexcept : data( data ), count( count ) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HeapMemoryBlock( const HeapMemoryBlock& other ) noexcept = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HeapMemoryBlock( HeapMemoryBlock&& other ) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~HeapMemoryBlock() noexcept { Clear(); }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( const HeapMemoryBlock& other ) noexcept -> HeapMemoryBlock& = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( HeapMemoryBlock&& other ) noexcept -> HeapMemoryBlock& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto Allocate( u64 count, u64 alignment_requirement ) -> ElementType*
	{
		if( this->count == count ) return this->data;

		Clear();

		this->data = memory::AllocateMemory<ElementType>( count, alignment_requirement );
		this->count = count;

		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto Reallocate( u64 new_count, u64 alignment_requirement ) -> ElementType* requires( utility::Trivial<ElementType> )
	{
		if( this->data == nullptr ) return Allocate( new_count, alignment_requirement );

		if( this->count == new_count ) return this->data;

		this->data = memory::ReallocateMemory( data, count, new_count );
		this->count = new_count;

		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto Data() noexcept -> ElementType*
	{
		return data;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto Data() const noexcept -> ElementType*
	{
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto Size() const noexcept -> u64
	{
		return count;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear() noexcept
	{
		if( this->data )
		{
			memory::FreeMemory( this->data, this->count );
		}
		this->count	= 0;
		this->data	= nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ElementType*	data	= nullptr;
	u64				count	= 0;
};



} // namespace memory
} // namespace bc
