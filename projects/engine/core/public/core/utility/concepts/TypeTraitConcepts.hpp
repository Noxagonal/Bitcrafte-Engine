#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is copy constructible from another type.
///
/// @tparam Type
/// Type to test.
///
/// @tparam From
/// Type to copy construct from.
template<typename Type, typename From>
concept IsCopyConstructableFrom = requires( Type type, From from )
{
	Type( from );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is move constructible from another type.
///
/// @tparam Type
/// Type to test.
///
/// @tparam From
/// Type to move construct from.
template<typename Type, typename From>
concept IsMoveConstructableFrom = requires( Type type, From from )
{
	Type( std::move( from ) );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is copy assignable from another type.
///
/// @tparam Type
/// Type to test.
///
/// @tparam From
/// Type to copy assign from.
template<typename Type, typename From>
concept IsCopyAssignableFrom = requires( Type type, From from )
{
	type = from;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is move assignable from another type.
///
/// @tparam Type
/// Type to test.
///
/// @tparam From
/// Type to move assign from.
template<typename Type, typename From>
concept IsMoveAssignableFrom = requires( Type type, From from )
{
	type = std::move( from );
};



} // namespace utility
} // namespace bc
