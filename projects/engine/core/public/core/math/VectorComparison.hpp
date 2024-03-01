#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/Vector.hpp>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Checks if 2 vectors are within range of each other component vise.
///
/// @tparam ValueType
/// Value type.
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
	size_t															VectorLength,
	utility::FundamentalType										VectorValueType
>
constexpr bool														AreValuesWithinRange(
	const math::VectorBase<VectorLength, VectorValueType>		&	v1,
	const math::VectorBase<VectorLength, VectorValueType>		&	v2,
	VectorValueType													range
) requires( VectorLength >= 2 && VectorLength <= 4 )
{
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



} // utility
} // bc
