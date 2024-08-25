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
/// @param v1
/// Ground value to test with.
///
/// @param v2
/// Value to test against to see if it is in range of v1.
///
/// @param range
/// Maximum distance between values for them to still be considered within range.
constexpr auto AreValuesWithinRange(
	utility::FundamentalValue auto		v1,
	utility::FundamentalValue auto		v2,
	utility::FundamentalValue auto		range
) noexcept -> bool
{
	static_assert( std::same_as<decltype( v1 ), decltype( v2 )> && std::same_as<decltype( v1 ), decltype( range )>, "Value types must be same" );
	return v2 <= v1 + range && v2 >= v1 - range;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if a floating point values are close enough to be considered equal.
///
/// Used to get around floating point precision errors.
///
/// @param v1
/// First value.
///
/// @param v2
/// Value to test against v1 if it is close enough to it.
constexpr auto IsFloatEqual(
	utility::FloatingPointValue auto	v1,
	utility::FloatingPointValue auto	v2
) noexcept -> bool
{
	static_assert( std::same_as<decltype( v1 ), decltype( v2 )>, "Value types must be same" );
	return AreValuesWithinRange( v1, v2, SmallValue<decltype( v1 )> );
}



} // math
} // bc
