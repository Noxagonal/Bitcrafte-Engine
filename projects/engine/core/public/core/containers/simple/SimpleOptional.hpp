#pragma once

#define BC_CONTAINER_IMPLEMENTATION_SIMPLE 1
#include <core/containers/backend/OptionalImpl.hpp>
#undef BC_CONTAINER_IMPLEMENTATION_SIMPLE


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
SimpleOptional<ValueType>								MakeSimpleOptional(
	ConstructorArgumentTypePack						&&	...constructor_arguments
)
{
	auto result = SimpleOptional<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



} // bc
