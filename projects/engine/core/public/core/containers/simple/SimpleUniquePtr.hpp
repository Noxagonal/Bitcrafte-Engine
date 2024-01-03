#pragma once

#define BI_CONTAINER_IMPLEMENTATION_SIMPLE 1
#include <core/containers/backend/UniquePtrImpl.hpp>
#undef BI_CONTAINER_IMPLEMENTATION_SIMPLE


namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make unique pointer container and construct its contained value.
///
/// Functionally this is very similar to std::make_unique, except it uses engine provided memory pool and engine provided
/// assertions.
///
/// @tparam ValueType
/// Contained value type of the unique pointer.
///
/// @tparam ...ConstructorArgumentTypePack
/// Parameter types forwarded to value constructor.
///
/// @param ...constructor_arguments
/// Arguments forwarded to contained value constructor.
///
/// @return
/// A new unique pointer with its contained value constructed.
template<typename ValueType, typename ...ConstructorArgumentTypePack>
SimpleUniquePtr<ValueType>								MakeSimpleUniquePtr(
	ConstructorArgumentTypePack						&&	...constructor_arguments
)
{
	auto result = SimpleUniquePtr<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



} // bc
