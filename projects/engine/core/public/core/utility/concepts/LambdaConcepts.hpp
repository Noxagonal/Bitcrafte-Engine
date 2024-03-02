#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if a lambda returns void.
template<
	typename LambdaType,
	typename ...LambdaParamsT
>
concept IsLambdaReturningVoid = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	{ lambda( std::forward<LambdaParamsT>( params )... ) } -> std::convertible_to<void>;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if a lambda returns a specific type.
template<
	typename LambdaType,
	typename LambdaReturnT,
	typename ...LambdaParamsT
>
concept IsLambdaReturningType = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	{ lambda( std::forward<LambdaParamsT>( params )... ) } -> std::convertible_to<LambdaReturnT>;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if a lambda accepts specific parameters.
template<
	typename LambdaType,
	typename ...LambdaParamsT
>
concept IsLambdaAcceptingParameters = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	lambda( std::forward<LambdaParamsT>( params )... );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if a lambda matches given signature.
template<
	typename LambdaType,
	typename LambdaReturnT,
	typename ...LambdaParamsT
>
concept IsLambdaSignature = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	lambda( std::forward<LambdaParamsT>( params )... );
	{ lambda( std::forward<LambdaParamsT>( params )... ) } -> std::convertible_to<LambdaReturnT>;
};



} // utility
} // bc
