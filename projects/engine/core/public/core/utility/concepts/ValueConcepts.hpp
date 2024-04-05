#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Type>
concept FundamentalValue = std::is_fundamental_v<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Type>
concept IntegralValue = std::is_integral_v<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Type>
concept FloatingPointValue = std::is_floating_point_v<Type>;



} // utility
} // bc
