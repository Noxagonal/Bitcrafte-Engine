#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>
#include <utility>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a callable object. (Has operator()).
template<
	typename CallableType
>
concept CallableObject = requires(
	CallableType				callable
)
{
	{ &CallableType::operator() };
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to ensure callable matches the arguments.
template<
	typename CallableType,
	typename ...CallableParameterTypePack
>
concept CallableWithParameters = requires(
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
concept CallableWithReturnAndParameters = requires(
	CallableType				callable,
	CallableParameterTypePack	...parameter_pack
)
{
	{ callable( std::forward<CallableParameterTypePack>( parameter_pack )... ) } -> std::convertible_to<CallableReturnType>;
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

static_assert( CallableWithParameters<void( void )> == true );
static_assert( CallableWithParameters<void( int ), int> == true );
static_assert( CallableWithParameters<int( void )> == true );
static_assert( CallableWithParameters<int( int ), int> == true );

static_assert( CallableWithReturnAndParameters<void( void ), void> == true );
static_assert( CallableWithReturnAndParameters<void( int ), void, int> == true );
static_assert( CallableWithReturnAndParameters<int( void ), int> == true );
static_assert( CallableWithReturnAndParameters<int( int ), int, int> == true );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // utility
} // bc
