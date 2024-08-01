#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tests if a type is a valid TypeList.
/// 
/// @tparam TypeListCandidate
/// Type to test if it is a valid TypeList.
template<typename TypeListCandidate>
concept TypeListType = requires
{
	{ TypeListCandidate::Size() } -> std::convertible_to<u64>;
	// { TypeListType::template IndexToType<0> };
	{ TypeListCandidate::template CountType<int>() } -> std::convertible_to<u64>;
	{ TypeListCandidate::HasDuplicates() } -> std::convertible_to<bool>;
	// { TypeListCandidate::template IsEachDerivedFromBase<std::exception>() } -> std::convertible_to<bool>;
};



} // utility
} // bc
