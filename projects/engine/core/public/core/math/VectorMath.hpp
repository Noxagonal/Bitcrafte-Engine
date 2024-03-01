#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/math/Vector.hpp>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t DimensionCount, typename VectorValueType>
VectorValueType												Distance(
	const VectorBase<DimensionCount, VectorValueType>	&	v1,
	const VectorBase<DimensionCount, VectorValueType>	&	v2
) requires( DimensionCount >= 2 && DimensionCount <= 4 )
{
	return ( v2 - v1 ).Magnitude();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t DimensionCount, typename VectorValueType>
VectorValueType												Dot(
	const VectorBase<DimensionCount, VectorValueType>	&	v1,
	const VectorBase<DimensionCount, VectorValueType>	&	v2
) requires( DimensionCount >= 2 && DimensionCount <= 4 )
{
	if constexpr( DimensionCount == 2 )
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	if constexpr( DimensionCount == 3 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	if constexpr( DimensionCount == 4 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t DimensionCount, typename VectorValueType>
VectorValueType												Cross(
	const VectorBase<DimensionCount, VectorValueType>	&	v1,
	const VectorBase<DimensionCount, VectorValueType>	&	v2
) requires( DimensionCount == 2 )
{
	return v1.x * v2.y - v1.y * v2.x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t DimensionCount, typename VectorValueType>
VectorBase<DimensionCount, VectorValueType>					Cross(
	const VectorBase<DimensionCount, VectorValueType>	&	v1,
	const VectorBase<DimensionCount, VectorValueType>	&	v2
) requires( DimensionCount == 3 )
{
	if constexpr( DimensionCount == 3 )
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
