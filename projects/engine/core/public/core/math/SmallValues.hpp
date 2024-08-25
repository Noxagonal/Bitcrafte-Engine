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
static constexpr f32	SMALL_F32_VALUE		= 0.0001f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Small value for f64. Used in rounding error corrections.
static constexpr f64	SMALL_F64_VALUE		= 0.00001;



namespace internal_ {

template<utility::FundamentalValue FundamentalValue>
struct SmallValueGetter
{
    static constexpr FundamentalValue value = {};
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
/// Get small positive value for a given data type.
/// 
/// @tparam FundamentalValue
/// Type to get small value for.
template<utility::FundamentalValue FundamentalValue>
constexpr FundamentalValue SmallValue = internal_::SmallValueGetter<FundamentalValue>::value;



} // math
} // bc
