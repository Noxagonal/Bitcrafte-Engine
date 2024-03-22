#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>
#include <type_traits>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get index to a specific type in a parameter pack.
///
/// @note
/// If the parameter pack contains multiple of the same type, only the index to the first occurrence of the type will be
/// returned.
///
/// @note
/// If the type is not in the parameter pack, this will cause a compile time error with message "Type not found".
///
/// Usage example:
/// @code
/// constexpr size_t index = GetTemplateParameterPackIndexFromType<float, int, float, double>::value; // This will result in 1
/// @endcode
///
/// @tparam MatchType
/// Type to find index for.
///
/// @tparam ...InTypePack
/// Template parameter pack of types we are searching a specific type in.
template <typename MatchType, typename ...InTypePack>
struct GetTemplateParameterPackIndexFromType;

// Specialization for when we reached the end of the parameter pack without finding the type.
template <typename MatchType>
struct GetTemplateParameterPackIndexFromType<MatchType>
	: std::integral_constant<
		size_t,
		std::numeric_limits<size_t>::max()
	>
{
	static_assert( 0, "Type not found" );
};

// Specialization for when the first type matches the one we're looking for.
template <typename MatchType, typename ...TypePack>
struct GetTemplateParameterPackIndexFromType<MatchType, MatchType, TypePack...>
	: std::integral_constant<
		size_t,
		0
	>
{};

// Specialization for when the first type does not match the one we're looking for.
template <typename MatchType, typename NonMatcingType, typename ...TypePack>
struct GetTemplateParameterPackIndexFromType<MatchType, NonMatcingType, TypePack...>
	: std::integral_constant<
		size_t,
		1 + GetTemplateParameterPackIndexFromType<MatchType, TypePack...>::value
	>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
// This should cause a compile time error with message "Type not found"
//static_assert( GetTemplateParameterPackIndexFromType<int>::value == std::numeric_limits<size_t>::max() );
static_assert( GetTemplateParameterPackIndexFromType<int, int>::value == 0 );
static_assert( GetTemplateParameterPackIndexFromType<int, int, float>::value == 0 );
static_assert( GetTemplateParameterPackIndexFromType<int, int, float, double>::value == 0 );
static_assert( GetTemplateParameterPackIndexFromType<float, int, float>::value == 1 );
static_assert( GetTemplateParameterPackIndexFromType<float, int, float, double>::value == 1 );
static_assert( GetTemplateParameterPackIndexFromType<double, int, float, double>::value == 2 );
static_assert( GetTemplateParameterPackIndexFromType<int, int, int, int>::value == 0 );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get template type at index in a template parameter pack.
///
/// Get the type at a specific index in a template parameter pack. Index is zero based.
///
/// @note
/// If the index is out of bounds, this will cause a compile time error with message "Out of bounds".
///
/// Usage example:
/// @code
/// using Type = GetTemplateParameterPackTypeAtIndex<1, int, float, double>; // This will result in float
/// @endcode
/// 
/// @tparam Index
/// Index to a type in InTypePack.
/// 
/// @tparam ...InTypePack
/// Template parameter pack of types.
template <size_t Index, typename ...InTypePack>
struct GetTemplateParameterPackTypeAtIndex;

// Specialization for when we reached the end of the parameter pack.
template <size_t Index>
struct GetTemplateParameterPackTypeAtIndex<Index>
{
	static_assert( 0, "Out of bounds" );
	using Type = void; // This is here just to prevent polluting the error log with missing type.
};

// Specialization for when the first type is the one we're looking for.
template <typename CurrentType, typename ...RemainingTypePack>
struct GetTemplateParameterPackTypeAtIndex<0, CurrentType, RemainingTypePack...>
{
	using Type = CurrentType;
};

// Recursive specialization to handle the case where the index is not zero.
template <size_t Index, typename CurrentType, typename ...RemainingTypePack>
struct GetTemplateParameterPackTypeAtIndex<Index, CurrentType, RemainingTypePack...>
	: GetTemplateParameterPackTypeAtIndex<Index - 1, RemainingTypePack...>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
static_assert( std::is_same_v<int, GetTemplateParameterPackTypeAtIndex<0, int>::Type> );
static_assert( std::is_same_v<int, GetTemplateParameterPackTypeAtIndex<0, int, float>::Type> );
static_assert( std::is_same_v<int, GetTemplateParameterPackTypeAtIndex<0, int, float, double>::Type> );
static_assert( std::is_same_v<float, GetTemplateParameterPackTypeAtIndex<1, int, float>::Type> );
static_assert( std::is_same_v<float, GetTemplateParameterPackTypeAtIndex<1, int, float, double>::Type> );
static_assert( std::is_same_v<double, GetTemplateParameterPackTypeAtIndex<2, int, float, double>::Type> );
// This should cause a compile time error with message "Out of bounds"
//static_assert( std::is_same_v<double, GetTemplateParameterPackTypeAtIndex<3, int, float, double>::Type> );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Finds and counts the number of types in given template parameter pack.
/// 
/// Usage example:
/// @code
/// constexpr size_t count = CountTypeInParameterPack<float, int, float, double, float>(); // This will result in 2
/// @endcode
/// 
/// @tparam TypeToCount
/// Type to count in InTypePack.
/// 
/// @tparam ...InTypePack
/// Template parameter pack of types to search.
/// 
/// @return
/// Number of TypeToCount types found in InTypePack.
template <typename TypeToCount, typename ...InTypePack>
struct CountTypeInParameterPack;

// Specialization for when we reach the end of the parameter pack, count is zero.
template <typename TypeToCount>
struct CountTypeInParameterPack<TypeToCount>
	: std::integral_constant<
		size_t,
		0
	>
{};

// Specialization for when we have not reached the end of the parameter pack.
template <typename TypeToCount, typename CurrentType, typename ...RemainingTypePack>
struct CountTypeInParameterPack<TypeToCount, CurrentType, RemainingTypePack...>
	: std::integral_constant<
		size_t,
		std::is_same_v<TypeToCount, CurrentType> +CountTypeInParameterPack<TypeToCount, RemainingTypePack...>::value
	>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
static_assert( CountTypeInParameterPack<int>::value == 0 );
static_assert( CountTypeInParameterPack<int, float>::value == 0 );
static_assert( CountTypeInParameterPack<int, int>::value == 1 );
static_assert( CountTypeInParameterPack<int, int, int>::value == 2 );
static_assert( CountTypeInParameterPack<int, float, int>::value == 1 );
static_assert( CountTypeInParameterPack<int, int, float, int>::value == 2 );
static_assert( CountTypeInParameterPack<float, int, float, int>::value == 1 );
static_assert( CountTypeInParameterPack<float, char, float, float, int, double, float>::value == 3 );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Test if a specific type is unique in template parameter pack.
///
/// @note
/// If the type is not found in the template parameter pack, a compile time error will be generated with message
/// "Type not found".
///
/// Usage example:
/// @code
/// constexpr bool is_unique = IsTypeUniqueInParameterPack<float, int, float, double>::value;
/// @endcode
///
/// @return
/// True if CheckType is found once in TypeList. False if Check type was not found in TypeList or if it was found multiple times.
template <typename TypeToCheck, typename ...TypePack>
struct IsTypeUniqueInParameterPack
	: std::integral_constant<
		bool,
		CountTypeInParameterPack<TypeToCheck, TypePack...>::value == 1
	>
{
	static_assert( CountTypeInParameterPack<TypeToCheck, TypePack...>::value > 0, "Type not found" );
};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
static_assert( IsTypeUniqueInParameterPack<int, double, int, float>::value == true );
static_assert( IsTypeUniqueInParameterPack<int, int, float, char>::value == true );
static_assert( IsTypeUniqueInParameterPack<int, int, int, float, char>::value == false );
static_assert( IsTypeUniqueInParameterPack<int, int, float, char, int>::value == false );
static_assert( IsTypeUniqueInParameterPack<int, int, float, int, char, int>::value == false );
static_assert( IsTypeUniqueInParameterPack<double, double, int, float>::value == true );
static_assert( IsTypeUniqueInParameterPack<double, int, float, char, double>::value == true );
static_assert( IsTypeUniqueInParameterPack<float, int, int, float, char>::value == true );
static_assert( IsTypeUniqueInParameterPack<float, int, float, char, int>::value == true );
static_assert( IsTypeUniqueInParameterPack<float, int, float, int, char, float, int>::value == false );
// This should cause a compile time error with message "Type not found"
//static_assert( IsTypeUniqueInParameterPack<float>::value == false );
//static_assert( IsTypeUniqueInParameterPack<float, int>::value == false );
//static_assert( IsTypeUniqueInParameterPack<float, int, double>::value == false );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Test if a specific type is found in template parameter pack.
/// 
/// @note
/// If template parameter pack is empty, this will always return false.
/// 
/// Usage example:
/// @code
/// constexpr bool has_type = IsTypeInParameterPack<float, int, float, double>::value;
/// @endcode
/// 
/// @tparam TypeToCheck
/// Type to check if it exists in InTypePack.
/// 
/// @tparam ...TypePack
/// Template parameter pack of types to search.
template<typename TypeToCheck, typename ...TypePack>
struct IsTypeInParameterPack;

// Specialization for empty parameter pack.
template<typename TypeToCheck>
struct IsTypeInParameterPack<TypeToCheck> : std::false_type {};

// Specialization for when we have not reached the end of the parameter pack.
template <typename TypeToCheck, typename CurrentType, typename ...RemainingTypePack>
struct IsTypeInParameterPack<TypeToCheck, CurrentType, RemainingTypePack...>
	: std::conditional_t<
		std::is_same_v<TypeToCheck, CurrentType>,
		std::true_type,
		IsTypeInParameterPack<TypeToCheck, RemainingTypePack...>
	>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
static_assert( IsTypeInParameterPack<int>::value == false );
static_assert( IsTypeInParameterPack<int, int>::value == true );
static_assert( IsTypeInParameterPack<int, float>::value == false );
static_assert( IsTypeInParameterPack<int, int, int>::value == true );
static_assert( IsTypeInParameterPack<int, int, float, int>::value == true );
static_assert( IsTypeInParameterPack<float, int, float, int>::value == true );
static_assert( IsTypeInParameterPack<float, char, float, float, int, double, float>::value == true );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Test if template parameter pack has any duplicate types.
///
/// Usage example:
/// @code
/// using TypeListSample = TypeList<int, float, double>;
/// constexpr bool has_duplicates = TypeListSample::HasDuplicates();
/// @endcode
/// 
/// @tparam ...TypePack
/// Template parameter pack to check if it has any duplicate types.
///
/// @return
/// True if TypeList has any duplicate types.
template <typename ...TypePack>
struct HasTemplateParameterPackDuplicates;

// Specialization for empty template parameter pack.
template <>
struct HasTemplateParameterPackDuplicates<> : std::false_type {};

// Specialization for iterating over all types in the template parameter pack.
template <typename CurrentType, typename... RemainingTypePack>
struct HasTemplateParameterPackDuplicates<CurrentType, RemainingTypePack...>
	: std::conditional_t<
		// Testing with >= 1 because CurrentType is already accounted for, we're just testing if there is more of this type at the end.
		// If the type is not found at the end, we recursively follow to the next type.
		CountTypeInParameterPack<CurrentType, RemainingTypePack...>::value >= 1,
		std::true_type,
		HasTemplateParameterPackDuplicates<RemainingTypePack...>
	>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
static_assert( HasTemplateParameterPackDuplicates<>::value == false );
static_assert( HasTemplateParameterPackDuplicates<int>::value == false );
static_assert( HasTemplateParameterPackDuplicates<int, float>::value == false );
static_assert( HasTemplateParameterPackDuplicates<int, float, double>::value == false );
static_assert( HasTemplateParameterPackDuplicates<int, float, float>::value == true );
static_assert( HasTemplateParameterPackDuplicates<int, int, float>::value == true );
static_assert( HasTemplateParameterPackDuplicates<int, double, float, double>::value == true );
static_assert( HasTemplateParameterPackDuplicates<int, double, float, double, float>::value == true );
static_assert( HasTemplateParameterPackDuplicates<int, double, float, double, int, double>::value == true );
static_assert( HasTemplateParameterPackDuplicates<int, double, float, double, int, double, double>::value == true );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tests if each type in the template parameter pack is derived from the given base type.
/// 
/// @note
/// If template parameter pack is empty, then this will always return false.
/// 
/// Usage example:
/// @code
/// constexpr bool are_all_derived = IsEachParameterPackTypeDerivedFromType<Base, Derived1, Derived2>::value;
/// @endcode
/// 
/// @tparam RequiredBaseType
/// The base type to check if each type in the template parameter pack is derived from.
/// 
/// @tparam ...InTypePack
/// Template parameter pack of types to check if they are derived from the RequiredBaseType.
template <typename RequiredBaseType, typename ...InTypePack>
struct IsEachParameterPackTypeDerivedFromType;

// Specialization for empty template parameter pack. Results in false type.
template<typename RequiredBaseType>
struct IsEachParameterPackTypeDerivedFromType<RequiredBaseType> : std::false_type {};

// Specialization for the last type in the template parameter pack.
template<typename RequiredBaseType, typename CurrentType>
struct IsEachParameterPackTypeDerivedFromType<RequiredBaseType, CurrentType>
	: std::is_base_of<RequiredBaseType, CurrentType>
{};

// Specialization for iterating over all types in the template parameter pack.
template <typename RequiredBaseType, typename CurrentType, typename ...RemainingTypePack>
struct IsEachParameterPackTypeDerivedFromType<RequiredBaseType, CurrentType, RemainingTypePack...>
	: std::conditional_t<
		std::is_base_of<RequiredBaseType, CurrentType>::value,
		IsEachParameterPackTypeDerivedFromType<RequiredBaseType, RemainingTypePack...>,
		std::false_type
	>
{};

#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
struct IsEachParameterPackTypeDerivedFromTypeTestBase1 {};
struct IsEachParameterPackTypeDerivedFromTypeTestBase2 {};
struct IsEachParameterPackTypeDerivedFromTypeTestDerived1 : IsEachParameterPackTypeDerivedFromTypeTestBase1 {};
struct IsEachParameterPackTypeDerivedFromTypeTestDerived2 : IsEachParameterPackTypeDerivedFromTypeTestBase1 {};
struct IsEachParameterPackTypeDerivedFromTypeTestDerived3 : IsEachParameterPackTypeDerivedFromTypeTestBase2 {};
struct IsEachParameterPackTypeDerivedFromTypeTestDerived4 : IsEachParameterPackTypeDerivedFromTypeTestBase2 {};
struct IsEachParameterPackTypeDerivedFromTypeTestNonDerived {};
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase2>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestDerived1>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestDerived1>::value == true );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestDerived1, IsEachParameterPackTypeDerivedFromTypeTestDerived2>::value == true );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestDerived1, IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase2, IsEachParameterPackTypeDerivedFromTypeTestDerived1>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase2, IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == true );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase2, IsEachParameterPackTypeDerivedFromTypeTestDerived1, IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase2, IsEachParameterPackTypeDerivedFromTypeTestDerived4, IsEachParameterPackTypeDerivedFromTypeTestDerived3>::value == true );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestNonDerived>::value == false );
static_assert( IsEachParameterPackTypeDerivedFromType<IsEachParameterPackTypeDerivedFromTypeTestBase1, IsEachParameterPackTypeDerivedFromTypeTestNonDerived, IsEachParameterPackTypeDerivedFromTypeTestNonDerived>::value == false );
} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // utility
} // bc
