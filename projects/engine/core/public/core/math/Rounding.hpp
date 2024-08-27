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
/// @param value
/// Value to round.
constexpr auto RoundToCeiling( utility::FloatingPoint auto value )
{
	return std::ceil( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value down to base integral value.
///
/// @param value
/// Value to round.
constexpr auto RoundToFloor( utility::FloatingPoint auto value )
{
	return std::floor( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value to the closest integral value.
///
/// @param value
/// Value to round.
constexpr auto RoundToClosest( utility::FloatingPoint auto value )
{
	return std::round( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Rounds a floating point value towards zero whole integral value.
///
/// @param value
/// Value to round.
constexpr auto Truncate( utility::FloatingPoint auto value )
{
	return std::trunc( value );
}



} // utility
} // bc
