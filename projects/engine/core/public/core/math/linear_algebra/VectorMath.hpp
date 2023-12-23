#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/math/linear_algebra/Vector.hpp>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t VectorLength, typename VectorValueType>
VectorValueType												Dot(
	const VectorBase<VectorLength, VectorValueType>		&	v1,
	const VectorBase<VectorLength, VectorValueType>		&	v2
) requires( VectorLength >= 2 && VectorLength <= 4 )
{
	if constexpr( VectorLength == 2 )
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	if constexpr( VectorLength == 3 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	if constexpr( VectorLength == 4 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t VectorLength, typename VectorValueType>
VectorValueType												Cross(
	const VectorBase<VectorLength, VectorValueType>		&	v1,
	const VectorBase<VectorLength, VectorValueType>		&	v2
) requires( VectorLength == 2 )
{
	return v1.x * v2.y - v1.y * v2.x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t VectorLength, typename VectorValueType>
VectorBase<VectorLength, VectorValueType>					Cross(
	const VectorBase<VectorLength, VectorValueType>		&	v1,
	const VectorBase<VectorLength, VectorValueType>		&	v2
) requires( VectorLength == 3 )
{
	if constexpr( VectorLength == 3 )
	{
		return {
		   v1.y * v2.z - v1.z * v2.y,
		   v1.z * v2.x - v1.x * v2.z,
		   v1.x * v2.y - v1.y * v2.x
		};
	}
}



} // math
} // bc
