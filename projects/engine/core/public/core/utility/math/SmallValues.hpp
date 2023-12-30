#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>

#include <cstdint>
#include <cfloat>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static constexpr float							SmallFloatValue					= 0.0001f;
static constexpr double							SmallDoubleValue				= 0.00001;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType FundamentalType>
consteval inline FundamentalType				GetSmallValue()
{
	if constexpr( std::is_same_v<FundamentalType, float> ) return SmallFloatValue;
	if constexpr( std::is_same_v<FundamentalType, double> ) return SmallDoubleValue;
	else return {};
}



} // utility
} // bc
