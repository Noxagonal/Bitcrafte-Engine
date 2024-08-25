#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/SmallValues.hpp>
#include <core/math/FundamentalComparison.hpp>
#include <core/math/Vector.hpp>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if 2 vectors are within range of each other component vise.
///
/// @param v1
/// First vector.
///
/// @param v2
/// Second vector.
///
/// @param range
/// Maximum distance between values for them to still be considered within range.
template<
	u64							VectorLength,
	utility::FundamentalValue	VectorValueType,
	utility::FundamentalValue	RangeValueType
>
constexpr auto AreValuesWithinRange(
	const math::VectorBase<VectorLength, VectorValueType>&	v1,
	const math::VectorBase<VectorLength, VectorValueType>&	v2,
	RangeValueType											range
) -> bool
	requires( VectorLength >= 2 && VectorLength <= 4 )
{
	static_assert(
		std::same_as<VectorValueType, RangeValueType>,
		"Range value type must be the same as the vector value type, you must explicitly ensure that the types are the same"
	);

	if constexpr( VectorLength == 2 )
	{
		return
			AreValuesWithinRange( v1.x, v2.x, range ) &&
			AreValuesWithinRange( v1.y, v2.y, range );
	}
	if constexpr( VectorLength == 3 )
	{
		return
			AreValuesWithinRange( v1.x, v2.x, range ) &&
			AreValuesWithinRange( v1.y, v2.y, range ) &&
			AreValuesWithinRange( v1.z, v2.z, range );
	}
	if constexpr( VectorLength == 4 )
	{
		return
			AreValuesWithinRange( v1.x, v2.x, range ) &&
			AreValuesWithinRange( v1.y, v2.y, range ) &&
			AreValuesWithinRange( v1.z, v2.z, range ) &&
			AreValuesWithinRange( v1.w, v2.w, range );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if a floating point vectors are close enough to be considered equal.
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
template<
	u64							VectorLength,
	utility::FundamentalValue	VectorValueType
>
constexpr auto IsFloatEqual(
	const math::VectorBase<VectorLength, VectorValueType>&	v1,
	const math::VectorBase<VectorLength, VectorValueType>&	v2
) noexcept -> bool
{
	return AreValuesWithinRange( v1, v2, GetSmallValue<VectorValueType>() );
}



} // math
} // bc
