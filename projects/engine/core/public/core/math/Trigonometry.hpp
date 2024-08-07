#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/utility/concepts/ValueConcepts.hpp>

#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
constexpr ValueType				Sin(
	ValueType					value
)
{
	return std::sin( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
constexpr ValueType				Cos(
	ValueType					value
)
{
	return std::cos( value );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
constexpr ValueType				Tan(
	ValueType					value
)
{
	return std::tan( value );
}



} // math
} // bc
