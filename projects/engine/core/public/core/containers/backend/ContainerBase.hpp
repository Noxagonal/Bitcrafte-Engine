#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>
#include <core/memory/raw/RawMemory.hpp>
#include <core/utility/concepts/ContainerConcepts.hpp>

#include <cstdint>

#include <type_traits>
#include <memory>
#include <new>
#include <assert.h>



namespace bc {
namespace container_bases {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ContainerResource
{
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr uintptr_t UndeadObjectPointerBitMask = 0x0F00000000000000ULL;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	static auto MakeObjectPointerUndead( ValueType* location ) -> ValueType*
	{
		return reinterpret_cast<ValueType*>( reinterpret_cast<uintptr_t>( location ) | UndeadObjectPointerBitMask );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	static auto MakeObjectPointerAlive( ValueType* location ) -> ValueType*
	{
		return reinterpret_cast<ValueType*>( reinterpret_cast<uintptr_t>( location ) & ~UndeadObjectPointerBitMask );
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	static auto IsObjectPointerUndead( const ValueType* location ) -> bool
	{
		return ( reinterpret_cast<uintptr_t>( location ) & UndeadObjectPointerBitMask ) == UndeadObjectPointerBitMask;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Do construct element.
	///
	/// This is a special function that can be used when constructing a single object that may throw.
	///
	/// @tparam ValueType
	/// Type of the element to construct.
	///
	/// @tparam ...ConstructorArgumentTypePack
	/// Parameter pack for types sent to the constructor of the element.
	///
	/// @param location
	/// Memory location where the object should be constructed.
	///
	/// @param ...constructor_arguments
	/// Constructor arguments as parameter pack sent to the constructor of the element.
	///
	/// @return
	/// Pointer to memory location where the object was constructed.
	template<typename ValueType, typename ...ConstructorArgumentTypePack>
	static constexpr auto DoTryConstructHeapElement(
		ValueType*						location,
		ConstructorArgumentTypePack&&	...constructor_arguments
	) -> ValueType*
	{
		if( std::is_constant_evaluated() )
		{
			new( location ) ValueType{ std::forward<ConstructorArgumentTypePack>( constructor_arguments )... };
			return location;
		}
		else
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Constructing object, destination is in memory outside of process" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Constructing object, destination is in freed memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Constructing object, destination is in uninitialized global memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Constructing object, destination is in uninitialized stack memory" );

			// Construct in place with the temporary pointer, which contains the original address, if contained value
			// throws in its constructor, then original location will remain modified.
			new( location ) ValueType{ std::forward<ConstructorArgumentTypePack>( constructor_arguments )... };
			return location;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Destruct element.
	///
	/// This is a conditional operation, input location accepts undead object pointers as well as alive ones. If an undead object
	/// pointer is encoutered, its alive version is returned and destruction of the object is not attempted.
	///
	/// @tparam ValueType
	/// Type of the value we try to destruct.
	///
	/// @param location
	/// Memory location of the object.
	///
	/// @return
	/// Pointer to the object location in memory when it was alive.
	template<typename ValueType>
	static constexpr auto DoTryDestructHeapElement( ValueType* location ) -> ValueType*
	{
		if( std::is_constant_evaluated() )
		{
			location->~ValueType();
			return location;
		}
		else
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Destructing range, location is in memory outside of process" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Destructing range, location is in freed memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Destructing range, location is in uninitialized global memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Destructing range, location is in uninitialized stack memory" );

			if( IsObjectPointerUndead( location ) )
			{
				return MakeObjectPointerAlive( location );
			}

			location->~ValueType();
			return location;
		}
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Try constructing a single element.
	///
	/// This is a special function that can be used when constructing a single object that may throw.
	///
	/// @tparam ValueType
	/// Type of the element to construct.
	///
	/// @tparam ...ConstructorArgumentTypePack
	/// Parameter pack for types sent to the constructor of the element.
	///
	/// @param [in|out] location
	/// Memory location where the object should be constructed, note that this value will also be modified, specifically the address
	/// will be made "undead" by modifying the high bits of it. This value can then be used to detect if the object construction
	/// failed. This is useful when a container goes out of scope, it can determine if the destructor for the value should be called
	/// or not.
	///
	/// @param ...constructor_arguments
	/// Constructor arguments as parameter pack sent to the constructor of the element.
	template<typename ValueType, typename ...ConstructorArgumentTypePack>
	static constexpr auto TryConstructHeapElement(
		ValueType*&						location,
		ConstructorArgumentTypePack&&	...constructor_arguments
	) -> void
	{
		if( std::is_constant_evaluated() )
		{
			new( location ) ValueType{ std::forward<ConstructorArgumentTypePack>( constructor_arguments )... };
		}
		else
		{
			// This piece of code solves a very specific problem.
			// Constructor of the contained type may throw during construction, if it does, the destructor for the
			// contained type should not be called.
			// By the time we reach this function, this object is likely already alive but the contained object is not,
			// however we already have memory allocated for it, which means when this container goes out of scope, eg.
			// in stack unwinding, this container will try to destruct the contained object too.
			// Since x64 only uses 48 least significant bits of the address, we can use the 8 most significant bits of
			// the address to indicate we tried but failed constructing the object. The original address is kept intact
			// but cannot be used. At destruction, we detect this change in the destruction function and skip
			// destruction if we found the modified address.

			// Make pointer undead first, so if constructor throws, it will be handled accordingly.
			volatile auto temp = location;
			location = MakeObjectPointerUndead( location );
			location = DoTryConstructHeapElement( temp, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Try destructing a single element.
	///
	/// This is a conditional operation, input location accepts undead object pointers as well as alive ones. If an undead object
	/// pointer is encoutered, its alive version is returned and destruction of the object is not attempted.
	///
	/// @tparam ValueType
	/// Type of the value we try to destruct.
	///
	/// @param [in|out] location
	/// Memory location of the object.
	///
	/// @return
	/// Pointer to the object location in memory when it was alive.
	template<typename ValueType>
	static constexpr void TryDestructHeapElement( ValueType*& location )
	{
		location = DoTryDestructHeapElement( location );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct a single element.
	///
	/// This is a simple call to the object constructor, object constructor may throw but nothing is done about it it does.
	///
	/// @tparam ValueType
	/// Type of the element to construct.
	///
	/// @tparam ...ConstructorArgumentTypePack
	/// Parameter pack for types sent to the constructor of the element.
	///
	/// @param object
	/// Reference to would be object that we want to construct.
	///
	/// @param ...constructor_arguments
	/// Constructor arguments as parameter pack sent to the constructor of the element.
	template<typename ValueType, typename ...ConstructorArgumentTypePack>
	static constexpr void ConstructStackElement(
		ValueType&						object,
		ConstructorArgumentTypePack&&	...constructor_arguments
	)
	{
		new( &object ) ValueType{ std::forward<ConstructorArgumentTypePack>( constructor_arguments )... };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Destruct a single element.
	///
	/// This is a simple call to the object destructor, no undead operations are performed.
	///
	/// @tparam ValueType
	/// Type of the object we try to destruct.
	///
	/// @param object
	/// Reference to object to destruct.
	template<typename ValueType>
	static constexpr void DestructStackElement( ValueType& object )
	{
		object.~ValueType();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType, typename ...ConstructorArgumentTypePack>
	constexpr void ConstructRange(
		ValueType*						destination,
		i64								element_count,
		ConstructorArgumentTypePack&&	...constructor_arguments
	) const
	{
		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Constructing range, destination is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, U"Constructing range, destination is nullptr" );

		BHardAssert( element_count > 0, U"Constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, U"Constructing range, element count too high, something is not right" );

		auto end = destination + element_count;
		for( i64 i = 0; i < element_count; ++i )
		{
			new( &destination[ i ] ) ValueType{ std::forward<ConstructorArgumentTypePack>( constructor_arguments )... };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void CopyConstructRange(
		ValueType*			destination,
		const ValueType*	source,
		i64					element_count
	) const requires( std::is_copy_constructible_v<ValueType> )
	{
		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Copy constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Copy constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Copy constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Copy constructing range, destination is in uninitialized stack memory" );

			BHardAssert( source != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Copy constructing range, source is in memory outside of process" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Copy constructing range, source is in freed memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Copy constructing range, source is in uninitialized global memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Copy constructing range, source is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, U"Copy constructing range, destination is nullptr" );
		BHardAssert( source != nullptr, U"Copy constructing range, source is nullptr" );
		BHardAssert( destination != source, U"Copy constructing range, destination is source, this should be checked earlier than this" );

		BHardAssert( element_count > 0, U"Copy constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, U"Copy constructing range, element count too high, something is not right" );

		auto end = destination + element_count;
		for( i64 i = 0; i < element_count; ++i )
		{
			new( &destination[ i ] ) ValueType{ source[ i ] };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void MoveConstructRange(
		ValueType*	destination,
		ValueType*	source,
		i64			element_count
	) const requires( std::is_move_constructible_v<ValueType> )
	{
		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Move constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Move constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Move constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Move constructing range, destination is in uninitialized stack memory" );

			BHardAssert( source != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Move constructing range, source is in memory outside of process" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Move constructing range, source is in freed memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Move constructing range, source is in uninitialized global memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Move constructing range, source is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, U"Move constructing range, destination is nullptr" );
		BHardAssert( source != nullptr, U"Move constructing range, source is nullptr" );
		BHardAssert( destination != source, U"Move constructing range, destination is source, this should be checked earlier than this" );

		BHardAssert( element_count > 0, U"Move constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, U"Move constructing range, element count too high, something is not right" );

		for( i64 i = 0; i < element_count; ++i )
		{
			new( &destination[ i ] ) ValueType{ std::move( source[ i ] ) };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void DestructRange(
		ValueType*	location,
		i64			element_count
	) const noexcept
	{
		if( location == nullptr ) return;
		if( element_count <= 0 ) return;

		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, U"Destructing range, element count too high, something is not right" );

		auto end = location + element_count;
		if constexpr( !std::is_trivially_destructible_v<ValueType> )
		{
			for( ValueType * it = end - 1; it >= location; --it )
			{
				it->~ValueType();
			}
		}

		#if BITCRAFTE_GAME_DEVELOPMENT_BUILD

		// Fill destructed object memory range with byte 0xCB. This is used to catch errors in development builds.
		if( !std::is_constant_evaluated() )
		{
			auto uninitialize_memory_ptr = reinterpret_cast<u8*>( location );
			for( i64 i = 0; i < element_count * sizeof( ValueType ); ++i )
			{
				uninitialize_memory_ptr[ i ] = 0xCB;
			}
		}

		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	[[nodiscard]]
	constexpr auto ResizeRange(
		ValueType*	old_location,
		i64			old_element_count,
		i64			old_reserved_element_count,
		i64			new_reserved_element_count
	) const noexcept -> ValueType*
		requires ( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
	{
		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), U"Resizing range, old location is in memory outside of process" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), U"Resizing range, old location is in freed memory" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), U"Resizing range, old location is in uninitialized global memory" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), U"Resizing range, old location is in uninitialized stack memory" );
		}

		BHardAssert( old_location != nullptr, U"Resizing range, old location is nullptr" );

		BHardAssert( old_element_count >= 0, U"Resizing range, old element count must a positive number or 0" );
		BHardAssert( old_element_count < 0x0000FFFFFFFFFFFF, U"Resizing range, old element count too high, something is not right" );

		BHardAssert( new_reserved_element_count > 0, U"Resizing range, new reserved element count must be larger than 0" );
		BHardAssert( new_reserved_element_count < 0x0000FFFFFFFFFFFF, U"Resizing range, new reserved element count too high, something is not right" );

		BHardAssert( old_reserved_element_count > 0, U"Resizing range, old reserved element count must be larger than 0" );
		BHardAssert( old_reserved_element_count < 0x0000FFFFFFFFFFFF, U"Resizing range, old reserved element count too high, something is not right" );
		BHardAssert( old_reserved_element_count != new_reserved_element_count, U"Resizing range, new reserved element count is the same as the old reserved element count, this check should be done earlier" );

		if constexpr( std::is_trivial_v<ValueType> )
		{
			// For trivial stuff we can try and reallocate the memory, this can be a cheaper operation than allocating.
			return static_cast<ValueType*>( this->ReallocateMemory( old_location, old_reserved_element_count, new_reserved_element_count ) );
		}
		else
		{
			// ValueType is not trivial, we need to copy elements one by one.
			if( this->IsInPlaceReallocateable( old_location, new_reserved_element_count ) )
			{
				auto new_location = this->InPlaceReallocateMemory( old_location, old_reserved_element_count, new_reserved_element_count );
				assert( old_location == new_location );
				return new_location;
			}

			// If the type is not in-place re-allocate-able then we need to allocate a new block and copy to it either by
			// invoking the copy or more constructors.
			auto new_location = static_cast<ValueType*>( this->AllocateMemory<ValueType>( new_reserved_element_count ) );
			if constexpr( std::is_move_constructible_v<ValueType> )
			{
				this->MoveConstructRange( new_location, old_location, old_element_count );
			}
			else
			{
				this->CopyConstructRange( new_location, old_location, old_element_count );
			}
			this->DestructRange( old_location, old_element_count );
			this->FreeMemory( old_location, old_reserved_element_count );
			return new_location;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	[[nodiscard]]
	constexpr auto AllocateMemory( i64 new_element_count ) const noexcept -> ValueType*
	{
		return memory::AllocateMemory<ValueType>( new_element_count, alignof( ValueType ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	[[nodiscard]]
	constexpr auto ReallocateMemory(
		ValueType*	old_location,
		i64			old_element_count,
		i64			new_element_count
	) const noexcept -> ValueType*
	{
		static_assert( std::is_trivial_v<ValueType>, "ValueType must be trivial for it to be reallocated via this function" );

		return memory::ReallocateMemory<ValueType>( old_location, old_element_count, new_element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void FreeMemory(
		ValueType*	location,
		i64			element_count
	) const noexcept
	{
		if( location == nullptr ) return;

		memory::FreeMemory<ValueType>( location, element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr auto IsInPlaceReallocateable(
		const ValueType*	location,
		i64					new_reserved_element_count
	) const -> bool
	{
		return memory::IsInPlaceReallocateable<ValueType>( location, new_reserved_element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr auto InPlaceReallocateMemory(
		ValueType*	old_location,
		i64			old_reserved_element_count,
		i64			new_reserved_element_count
	) const -> ValueType*
	{
		return memory::InPlaceReallocateMemory<ValueType>( old_location, old_reserved_element_count, new_reserved_element_count );
	}
};



} // container_bases
} // bc
