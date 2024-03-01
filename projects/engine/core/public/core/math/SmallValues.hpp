#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>

#include <cstdint>
#include <cfloat>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr f32							SMALL_F32_VALUE				= 0.0001f;
static constexpr f64							SMALL_F64_VALUE				= 0.00001;



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
