#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>

#include <cstdint>
#include <cfloat>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr float							SMALL_FLOAT_VALUE				= 0.0001f;
static constexpr double							SMALL_DOUBLE_VALUE				= 0.00001;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType FundamentalType>
consteval inline FundamentalType				GetSmallValue()
{
	if constexpr( std::is_same_v<FundamentalType, float> ) return SMALL_FLOAT_VALUE;
	if constexpr( std::is_same_v<FundamentalType, double> ) return SMALL_DOUBLE_VALUE;
	else return {};
}



} // math
} // bc
