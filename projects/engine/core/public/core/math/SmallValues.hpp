#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/data_types/FundamentalTypes.hpp>

#include <cstdint>
#include <cfloat>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Small value for f32. Used in rounding error corrections.
static constexpr f32							SMALL_F32_VALUE				= 0.0001f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Small value for f64. Used in rounding error corrections.
static constexpr f64							SMALL_F64_VALUE				= 0.00001;



namespace internal_ {

template<utility::FundamentalType FundamentalType>
struct SmallValueGetter
{
    static constexpr FundamentalType value = {};
};

template<>
struct SmallValueGetter<f32>
{
	static constexpr f32 value = SMALL_F32_VALUE;
};

template<>
struct SmallValueGetter<f64>
{
	static constexpr f64 value = SMALL_F64_VALUE;
};

} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get small value for a given data type.
/// 
/// @tparam FundamentalType
/// Type to get small value for.
template<utility::FundamentalType FundamentalType>
constexpr FundamentalType SmallValue = internal_::SmallValueGetter<FundamentalType>::value;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType FundamentalType>
consteval inline FundamentalType				GetSmallValue()
{
	if constexpr( std::is_same_v<FundamentalType, f32> ) return SMALL_F32_VALUE;
	if constexpr( std::is_same_v<FundamentalType, f64> ) return SMALL_F64_VALUE;
	else return {};
}



} // math
} // bc
