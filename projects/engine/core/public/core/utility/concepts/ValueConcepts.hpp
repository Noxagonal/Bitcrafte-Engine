#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Type>
concept FundamentalType = std::is_fundamental_v<Type>;




} // utility
} // bc
