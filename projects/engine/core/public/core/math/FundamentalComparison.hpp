#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/SmallValues.hpp>

#include <concepts>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if 2 values are within range of each other.
///
/// @tparam ValueType_1
/// Value type. Must be same as ValueType_2 and ValueType_3.
///
/// @tparam ValueType_2
/// Value type. Must be same as ValueType_1 and ValueType_3.
///
/// @tparam ValueType_3
/// Value type. Must be same as ValueType_1 and ValueType_2.
///
/// @param v1
/// Ground value to test with.
///
/// @param v2
/// Value to test against to see if it is in range of v1.
///
/// @param range
/// Maximum distance between values for them to still be considered within range.
template<
	utility::FundamentalValue		ValueType_1,
	utility::FundamentalValue		ValueType_2,
	utility::FundamentalValue		ValueType_3
>
constexpr bool							AreValuesWithinRange(
	ValueType_1							v1,
	ValueType_2							v2,
	ValueType_3							range
) noexcept
{
	static_assert( std::same_as<ValueType_1, ValueType_2> && std::same_as<ValueType_1, ValueType_3>, "Value types must be same" );
	return v2 <= v1 + range && v2 >= v1 - range;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if a floating point values are close enough to be considered equal.
///
/// Used to get around floating point precision errors.
///
/// @tparam ValueType_1
/// Floating point type. Must be same as ValueType_2.
///
/// @tparam ValueType_2
/// Floating point type. Must be same as ValueType_1.
///
/// @param v1
/// First value.
///
/// @param v2
/// Value to test against v1 if it is close enough to it.
template<
	utility::FloatingPointValue	ValueType_1,
	utility::FloatingPointValue ValueType_2
>
constexpr bool							IsFloatEqual(
	ValueType_1							v1,
	ValueType_2							v2
) noexcept
{
	static_assert( std::same_as<ValueType_1, ValueType_2>, "Value types must be same" );
	return AreValuesWithinRange( v1, v2, SmallValue<ValueType_1> );
}



} // math
} // bc
