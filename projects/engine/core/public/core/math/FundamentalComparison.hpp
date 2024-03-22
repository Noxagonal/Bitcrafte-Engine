#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/SmallValues.hpp>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if 2 values are within range of each other.
///
/// @tparam ValueType
/// Value type
///
/// @param v1
/// Ground value to test with.
///
/// @param v2
/// Value to test against to see if it is in range of v1.
///
/// @param range
/// Maximum distance between values for them to still be considered within range.
template<utility::FundamentalType		ValueType>
constexpr bool							AreValuesWithinRange(
	ValueType							v1,
	ValueType							v2,
	ValueType							range
) noexcept
{
	return v2 <= v1 + range && v2 >= v1 - range;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if a floating point values are close enough to be considered equal.
///
/// Used to get around floating point precision errors.
///
/// @note
/// Uses bc::math::GetSmallValue() function to get the small value to test for range.
///
/// @tparam ValueType
/// Floating point type.
///
/// @param v1
/// First value.
///
/// @param v2
/// Value to test against v1 if it is close enough to it.
template<utility::FloatingPointType		ValueType>
constexpr bool							IsFloatEqual(
	ValueType							v1,
	ValueType							v2
) noexcept
{
	return AreValuesWithinRange( v1, v2, GetSmallValue<ValueType>() );
}



} // math
} // bc
