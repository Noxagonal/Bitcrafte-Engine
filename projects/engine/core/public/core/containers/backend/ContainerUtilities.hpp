#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/utility/concepts/ContainerConcepts.hpp>

#include <cstdint>
#include <type_traits>
#include <assert.h>



#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
namespace container_bases {
namespace internal {



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
	::bc::utility::ContainerView	FirstContainerType,
	::bc::utility::ContainerView	SecondContainerType
>
[[nodiscard]]
constexpr bool											CheckContainerContentsMatch(
	const FirstContainerType						&	first_container,
	const SecondContainerType						&	second_container
) noexcept
{
	using FirstValueType	= typename FirstContainerType::ContainedValueType;
	using SecondValueType	= typename SecondContainerType::ContainedValueType;

	auto first_size = first_container.Size();
	auto second_size = second_container.Size();

	if( first_size != second_size ) return false;

	if constexpr( utility::LinearContainerView<FirstContainerType> &&
		utility::LinearContainerView<SecondContainerType> &&
		std::is_same_v<FirstValueType, SecondValueType> )
	{
		if( first_container.Data() == second_container.Data() ) return true;
	}

	auto first_it = first_container.begin();
	auto second_it = second_container.begin();
	for( u64 i = 0; i < first_size; ++i )
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
	::bc::utility::ContainerView	FirstContainerType,
	::bc::utility::ContainerView	SecondContainerType
>
[[nodiscard]]
constexpr bool											CheckContainerContentsDiffer(
	const FirstContainerType						&	first_container,
	const SecondContainerType						&	second_container
) noexcept
{
	using FirstValueType	= typename FirstContainerType::ContainedValueType;
	using SecondValueType	= typename SecondContainerType::ContainedValueType;

	auto first_size = first_container.Size();
	auto second_size = second_container.Size();

	if( first_size != second_size ) return true;

	auto first_it = first_container.begin();
	auto second_it = second_container.begin();
	for( u64 i = 0; i < first_size; ++i )
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Find the first occurance of a specific value in linear memory.
///
/// @tparam ValueType
/// Type of the input values and the value we want to find.
///
/// @tparam IsConst
/// Determines if the input and output values should be const.
///
/// @param data
/// Address to beginning of memory which we should search.
///
/// @param range
/// How many elements we should search from the beginning of the data.
///
/// @param value
///	Value to find.
/// 
/// @return
/// Pointer to value position where value was found.
template<typename ValueType, bool IsConst>
[[nodiscard]]
constexpr std::conditional_t<IsConst, const ValueType, ValueType>								*	DoLinearSearch(
	std::conditional_t<IsConst, const ValueType, ValueType>										*	data,
	u64																								range,
	const ValueType																				&	value
) BC_CONTAINER_NOEXCEPT
{
	auto it = data;
	auto it_end = data + range;
	while( it != it_end )
	{
		if( *it == value )
		{
			return it;
		}
		++it;
	}
	return it_end;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Find the first occurance of a specific value in linear memory for element where lambda returns true.
///
/// @tparam ValueType
/// Type of the input values and the value we want to find.
///
/// @tparam IsConst
/// Determines if the input and output values should be const.
///
/// @tparam LambdaType
/// Type of the lambda which is used to do the conditional search.
///
/// @param data
/// Address to beginning of memory which we should search.
///
/// @param range
/// How many elements we should search from the beginning of the data.
///
/// @param lambda
///	Lambda which determines if we found the correct element.
/// 
/// @return
/// Pointer to value position where value was found.
template<typename ValueType, bool IsConst, typename LambdaType>
[[nodiscard]]
constexpr std::conditional_t<IsConst, const ValueType, ValueType>								*	DoLinearSearchIf(
	std::conditional_t<IsConst, const ValueType, ValueType>										*	data,
	u64																								range,
	LambdaType																					&&	lambda
) BC_CONTAINER_NOEXCEPT
{
	auto it = data;
	auto it_end = data + range;
	while( it != it_end )
	{
		if( lambda( *it ) )
		{
			return it;
		}
		++it;
	}
	return it_end;
}



} // internal
} // container_bases
} // bc

#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
