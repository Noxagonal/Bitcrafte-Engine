#pragma once

#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>

#define BC_CONTAINER_IMPLEMENTATION_NORMAL 1
#include <core/containers/backend/OptionalImpl.hpp>
#undef BC_CONTAINER_IMPLEMENTATION_NORMAL


namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make optional container and construct its contained value.
///
/// Functionally this is very similar to std::optional, except it uses engine provided assertions.
///
/// @tparam ValueType
/// Contained value type of the optional.
///
/// @tparam ...ConstructorArgumentTypePack
/// Parameter types forwarded to contained value constructor.
///
/// @param ...constructor_arguments
/// Arguments forwarded to contained value constructor.
///
/// @return
/// A new optional container with its contained value constructed.
template<typename ValueType, typename ...ConstructorArgumentTypePack>
Optional<ValueType>										MakeOptional(
	ConstructorArgumentTypePack						&&	...constructor_arguments
)
{
	auto result = Optional<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



} // bc
