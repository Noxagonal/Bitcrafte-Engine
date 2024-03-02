#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to ensure callable matches the arguments.
template<
	typename CallableType,
	typename ...CallableParameterTypePack
>
concept Callable = requires(
	CallableType				callable,
	CallableParameterTypePack	...parameter_pack
)
{
	callable( std::forward<CallableParameterTypePack>( parameter_pack )... );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to ensure callable matches the arguments and the return type.
template<
	typename CallableType,
	typename CallableReturnType,
	typename ...CallableParameterTypePack
>
concept CallableWithReturn = requires(
	CallableType				callable,
	CallableParameterTypePack	...parameter_pack
)
{
	{ callable( std::forward<CallableParameterTypePack>( parameter_pack )... ) } -> std::convertible_to<CallableReturnType>;
};



} // utility
} // bc
