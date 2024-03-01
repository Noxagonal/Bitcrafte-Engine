#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>

#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value up to the next integral value.
///
/// @tparam ValueType
/// Floating point type.
///
/// @param value
/// Value to round.
template<utility::FloatingPointType ValueType>
constexpr ValueType					RoundToCeiling(
	ValueType						value
)
{
	return std::ceil( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value down to base integral value.
///
/// @tparam ValueType
/// Floating point type.
///
/// @param value
/// Value to round.
template<utility::FloatingPointType ValueType>
constexpr ValueType					RoundToFloor(
	ValueType						value
)
{
	return std::floor( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value to the closest integral value.
///
/// @tparam ValueType
/// Floating point type.
///
/// @param value
/// Value to round.
template<utility::FloatingPointType ValueType>
constexpr ValueType					RoundToClosest(
	ValueType						value
)
{
	return std::round( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value towards zero whole integral value.
///
/// @tparam ValueType
/// Floating point type.
///
/// @param value
/// Value to round.
template<utility::FloatingPointType ValueType>
constexpr ValueType					Truncate(
	ValueType						value
)
{
	return std::trunc( value );
}



} // utility
} // bc
