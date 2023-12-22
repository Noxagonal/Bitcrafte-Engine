#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>
#include <core/memory/raw/RawMemory.hpp>
#include <core/utility/ContainerConcepts.hpp>

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
protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct a range of elements.
	///
	/// @tparam ValueType
	/// Type of the element we want to construct.
	///
	/// @tparam ...ConstructorArgumentsTypePack
	/// Parameter types given as arguments to the Type constructor.
	///
	/// @param destination
	/// Pointer to the first element in a linear array to construct.
	///
	/// @param element_count
	/// Number of elements to construct.
	///
	/// @param ...constructor_arguments
	/// Arguments passed to the constructor of each of the elements.
	template<typename ValueType, typename ...ConstructorArgumentsTypePack>
	constexpr void										ConstructRange(
		ValueType									*	destination,
		size_t											element_count,
		ConstructorArgumentsTypePack				&&	...constructor_arguments
	) const
	{
		static_assert( sizeof( size_t ) == 8, "This function is build for 64 bit systems only" );

		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Constructing range, destination is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, "Constructing range, destination is nullptr" );

		BHardAssert( element_count > 0, "Constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, "Constructing range, element count too high, something is not right" );

		auto end = destination + element_count;
		for( ValueType * it = destination; it < end; ++it )
		{
			new( it ) ValueType( std::forward<ConstructorArgumentsTypePack>( constructor_arguments )... );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct range of elements by using another range of elements as a source.
	///
	/// @tparam ValueType
	/// Type of the element we want to construct.
	///
	/// @param destination 
	/// @param source 
	/// @param element_count 
	template<typename ValueType>
	constexpr void										CopyConstructRange(
		ValueType									*	destination,
		const ValueType								*	source,
		size_t											element_count
	) const requires( std::is_copy_constructible_v<ValueType> )
	{
		static_assert( sizeof( size_t ) == 8, "This function is build for 64 bit systems only" );

		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Copy constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Copy constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Copy constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Copy constructing range, destination is in uninitialized stack memory" );

			BHardAssert( source != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Copy constructing range, source is in memory outside of process" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Copy constructing range, source is in freed memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Copy constructing range, source is in uninitialized global memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Copy constructing range, source is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, "Copy constructing range, destination is nullptr" );
		BHardAssert( source != nullptr, "Copy constructing range, source is nullptr" );
		BHardAssert( destination != source, "Copy constructing range, destination is source, this should be checked earlier than this" );

		BHardAssert( element_count > 0, "Copy constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, "Copy constructing range, element count too high, something is not right" );

		auto end = destination + element_count;
		for( size_t i = 0; i < element_count; ++i )
		{
			new( &destination[ i ] ) ValueType( source[ i ] );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void										MoveConstructRange(
		ValueType									*	destination,
		ValueType									*	source,
		size_t											element_count
	) const requires( std::is_move_constructible_v<ValueType> )
	{
		static_assert( sizeof( size_t ) == 8, "This function is build for 64 bit systems only" );

		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Move constructing range, destination is in memory outside of process" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Move constructing range, destination is in freed memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Move constructing range, destination is in uninitialized global memory" );
			BHardAssert( destination != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Move constructing range, destination is in uninitialized stack memory" );

			BHardAssert( source != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Move constructing range, source is in memory outside of process" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Move constructing range, source is in freed memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Move constructing range, source is in uninitialized global memory" );
			BHardAssert( source != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Move constructing range, source is in uninitialized stack memory" );
		}

		BHardAssert( destination != nullptr, "Move constructing range, destination is nullptr" );
		BHardAssert( source != nullptr, "Move constructing range, source is nullptr" );
		BHardAssert( destination != source, "Move constructing range, destination is source, this should be checked earlier than this" );

		BHardAssert( element_count > 0, "Move constructing range, element count must be larger than 0" );
		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, "Move constructing range, element count too high, something is not right" );

		for( size_t i = 0; i < element_count; ++i )
		{
			new( &destination[ i ] ) ValueType( std::move( source[ i ] ) );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void										DestructRange(
		ValueType									*	location,
		size_t											element_count
	) const noexcept
	{
		static_assert( sizeof( size_t ) == 8, "This function is build for 64 bit systems only" );

		if( location == nullptr ) return;
		if( element_count == 0 ) return;

		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Destructing range, location is in memory outside of process" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Destructing range, location is in freed memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Destructing range, location is in uninitialized global memory" );
			BHardAssert( location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Destructing range, location is in uninitialized stack memory" );
		}

		BHardAssert( element_count < 0x0000FFFFFFFFFFFF, "Destructing range, element count too high, something is not right" );

		auto end = location + element_count;
		if constexpr( !std::is_trivially_destructible_v<ValueType> )
		{
			for( ValueType * it = end - 1; it >= location; --it )
			{
				it->~ValueType();
			}
		}

		#if BITCRAFTE_DEVELOPMENT_BUILD
		if( !std::is_constant_evaluated() )
		{
			auto uninitialize_memory_ptr = reinterpret_cast<uint8_t*>( location );
			for( size_t i = 0; i < element_count * sizeof( ValueType ); ++i )
			{
				uninitialize_memory_ptr[ i ] = 0xcd;
			}
		}
		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	[[nodiscard]]
	constexpr ValueType								*	ResizeRange(
		ValueType									*	old_location,
		size_t											old_element_count,
		size_t											old_reserved_element_count,
		size_t											new_reserved_element_count
	) const noexcept requires ( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
	{
		if( !std::is_constant_evaluated() )
		{
			// Assert some common Visual studio fault addresses.
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xFDFDFDFDFDFDFDFD ), "Resizing range, old location is in memory outside of process" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xDDDDDDDDDDDDDDDD ), "Resizing range, old location is in freed memory" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCDCDCDCDCDCDCDCD ), "Resizing range, old location is in uninitialized global memory" );
			BHardAssert( old_location != reinterpret_cast<ValueType*>( 0xCCCCCCCCCCCCCCCC ), "Resizing range, old location is in uninitialized stack memory" );
		}

		BHardAssert( old_location != nullptr, "Resizing range, old location is nullptr" );
		BHardAssert( old_element_count < 0x0000FFFFFFFFFFFF, "Resizing range, old element count too high, something is not right" );

		BHardAssert( new_reserved_element_count > 0, "Resizing range, new reserved element count must be larger than 0" );
		BHardAssert( new_reserved_element_count < 0x0000FFFFFFFFFFFF, "Resizing range, new reserved element count too high, something is not right" );

		BHardAssert( old_reserved_element_count > 0, "Resizing range, old reserved element count must be larger than 0" );
		BHardAssert( old_reserved_element_count < 0x0000FFFFFFFFFFFF, "Resizing range, old reserved element count too high, something is not right" );
		BHardAssert( old_reserved_element_count != new_reserved_element_count, "Resizing range, new reserved element count is the same as the old reserved element count, this check should be done earlier" );

		if constexpr( std::is_trivial_v<ValueType> )
		{
			// For trivial stuff we can try and reallocate the memory, this can be a cheaper operation than allocating.
			return static_cast<ValueType*>( this->ReallocateMemory( old_location, old_reserved_element_count, new_reserved_element_count ) );
		}
		else
		{
			// ValueType is not trivial, we need to do some extra work.
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
	constexpr ValueType								*	AllocateMemory(
		size_t											new_element_count
	) const noexcept
	{
		return memory::AllocateMemory<ValueType>( new_element_count, alignof( ValueType ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	[[nodiscard]]
	constexpr ValueType								*	ReallocateMemory(
		ValueType									*	old_location,
		size_t											old_element_count,
		size_t											new_element_count
	) const noexcept
	{
		static_assert( std::is_trivial_v<ValueType>, "ValueType must be trivial for it to be reallocated via this function" );

		return memory::ReallocateMemory<ValueType>( old_location, old_element_count, new_element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr void										FreeMemory(
		ValueType									*	location,
		size_t											element_count
	) const noexcept
	{
		if( location == nullptr ) return;

		memory::FreeMemory<ValueType>( location, element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr bool										IsInPlaceReallocateable(
		const ValueType								*	location,
		size_t											new_reserved_element_count
	) const
	{
		return memory::IsInPlaceReallocateable<ValueType>( location, new_reserved_element_count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ValueType>
	constexpr ValueType								*	InPlaceReallocateMemory(
		ValueType									*	old_location,
		size_t											old_reserved_element_count,
		size_t											new_reserved_element_count
	) const
	{
		return memory::InPlaceReallocateMemory<ValueType>( old_location, old_reserved_element_count, new_reserved_element_count );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Check container elements match with another.
///
///	Compares any two container contents to see if they match.
///
///	Container elements do not need to be exactly the same type but they must be comparable, eg. A bc::Text32 may be compared
/// with an bc::List<char> as char32_t and char may be compared directly with each other.
///
///	For class elements this uses "operator!=" so it must be defined.
///
/// @tparam FirstContainerType
///	First container type to compare contents with.
///
/// @tparam SecondContainerType
///	Second container type to compare contents with.
///
/// @param first_container
///	First container to compare contents with the second.
/// 
/// @param second_container
///	Second container to compare contents with the first.
/// 
/// @return
/// True if contents match, false if they do not.
template<
	utility::ContainerView	FirstContainerType,
	utility::ContainerView	SecondContainerType
>
[[nodiscard]]
constexpr bool											CheckContainerContentsMatch(
	const FirstContainerType						&	first_container,
	const SecondContainerType						&	second_container
) noexcept
{
	using FirstValueType	= typename FirstContainerType::ContainedValueType;
	using SecondValueType	= typename SecondContainerType::ContainedValueType;

	size_t first_size = first_container.Size();
	size_t second_size = second_container.Size();

	if( first_size != second_size ) return false;

	if constexpr( utility::LinearContainerView<FirstContainerType> &&
		utility::LinearContainerView<SecondContainerType> &&
		std::is_same_v<FirstValueType, SecondValueType> )
	{
		if( first_container.Data() == second_container.Data() ) return true;
	}

	auto first_it = first_container.begin();
	auto second_it = second_container.begin();
	for( size_t i = 0; i < first_size; ++i )
	{
		if( *first_it != *second_it ) return false;
		++first_it;
		++second_it;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Check container elements do not match with another.
///
///	Compares any two container contents to see if they do not match.
///
///	Container elements do not need to be exactly the same type but they must be comparable, eg. A bc::Text32 may be compared
/// with an bc::List<char> as char32_t and char may be compared directly with each other.
///
///	For class elements this uses "operator!=" so it must be defined.
///
/// @tparam FirstContainerType
///	First container type to compare contents with.
///		    
/// @tparam SecondContainerT
///	Second container type to compare contents with.
///		    
/// @paramf irst_container
///	First container to compare contents with the second.
/// 	    
/// @params econd_container
///	Second container to compare contents with the first.
/// 	    
/// @return True if contents do not match, false if they do.
template<
	utility::ContainerView	FirstContainerType,
	utility::ContainerView	SecondContainerType
>
[[nodiscard]]
constexpr bool											CheckContainerContentsDiffer(
	const FirstContainerType						&	first_container,
	const SecondContainerType						&	second_container
) noexcept
{
	using FirstValueType	= typename FirstContainerType::ContainedValueType;
	using SecondValueType	= typename SecondContainerType::ContainedValueType;

	size_t first_size = first_container.Size();
	size_t second_size = second_container.Size();

	if( first_size != second_size ) return true;

	auto first_it = first_container.begin();
	auto second_it = second_container.begin();
	for( size_t i = 0; i < first_size; ++i )
	{
		if constexpr( !std::is_class_v<FirstValueType> && !std::is_class_v<SecondValueType> )
		{
			if( *first_it != FirstValueType( *second_it ) ) return true;
		}
		else
		{
			if( *first_it != *second_it ) return true;
		}
		++first_it;
		++second_it;
	}
	return false;
}



} // container_bases
} // bc
