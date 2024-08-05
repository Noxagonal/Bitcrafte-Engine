#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/template/TemplateParameterPackOps.hpp>
#include <core/utility/concepts/TypeListTypeConcept.hpp>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Helper class to represent a list of types.
///
/// This class is useful when you need to store a list of types and query information about them.
///
/// @tparam ...TypePack
/// Template parameter pack of types that the TypeList represents.
template <typename ...TypePack>
class TypeList
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Apply the TypeList types to an existing class template.
	///
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	///
	/// template<typename... ArgumentsPack>
	/// class ClassTemplateSample {};
	///
	/// using PopulatedClassTemplateSample = TypeListSample::ApplyTo<ClassTemplateSample>;
	/// // PopulatedClassTemplateSample will be equivalent to ClassTemplateSample<int, float, double>
	/// @endcode
	///
	/// @tparam ClassTemplate
	/// Class template to apply the TypeList parameter pack to.
	template<template<typename...> class ClassTemplate>
	using ApplyTo = ClassTemplate<TypePack...>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get template type at index in TypeList.
	///
	/// @note
	/// If the index is out of bounds, this will cause a compile time error with message "Out of bounds".
	///
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	/// using Type_0 = TypeListSample::template IndexToType<0>;
	/// using Type_1 = TypeListSample::template IndexToType<1>;
	/// // Type_0 will be "int", Type_1 will be "float"
	/// @endcode
	/// 
	/// @tparam Index
	/// Index of Type to get.
	template<u64 Index>
	using IndexToType = typename GetTemplateParameterPackTypeAtIndex<Index, TypePack...>::Type;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get index to a specific type in TypeList.
	///
	/// @note
	/// If this TypeList contains multiple of the same type, only the index to the first occurrence of the type will be returned.
	///
	/// @note
	/// If the type was not found, a compile time error will be generated with message "Type not found".
	///
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	/// constexpr u64 index_int = TypeListSample::template TypeToIndex<int>();
	/// constexpr u64 index_float = TypeListSample::template TypeToIndex<float>();
	/// // index_int will be 0, index_float will be 1
	/// @endcode
	///
	/// @tparam TypeToFind
	/// Type to find index for.
	/// 
	/// @return
	/// Index to first occurrence of TypeToFind in TypeList.
	template<typename TypeToFind>
	consteval static u64 TypeToIndex()
	{
		return GetTemplateParameterPackIndexFromType<TypeToFind, TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the number of template parameters in the TypeList.
	/// 
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	/// constexpr u64 count = TypeListSample::Size();
	/// // count will be 3
	/// @endcode
	///
	/// @return
	/// Number of template parameters in the TypeList.
	consteval static u64 Size()
	{
		return sizeof...( TypePack );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the size of the largest type in the TypeList.
	/// 
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<i32, u8, f64, u16>;
	/// constexpr u64 max_size = TypeListSample::template TypeMaxSize();
	/// // max_size will be 8 because f64 is 8 bytes.
	/// @endcode
	///
	/// @return
	/// Maximum size of the TypeList.
	consteval static u64 TypeMaxSize()
	{
		return FindMaxSizeTypeInParameterPack<TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts the number of instances of CountType in this TypeList.
	/// 
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double, float>;
	/// constexpr u64 count = TypeListSample::template CountType<float>();
	/// // count will be 2
	/// @endcode
	/// 
	/// @tparam TypeToCount
	/// Type that we want to find the number of occurrences of in this TypeList.
	/// 
	/// @return
	/// Number of instances of CountType in this TypeList.
	template<typename TypeToCount>
	consteval static u64 CountType()
	{
		return CountTypeInParameterPack<TypeToCount, TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if a specific type is found in this TypeList.
	/// 
	/// @note
	/// If TypeList is empty, this will always return false.
	/// 
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	/// constexpr bool has_float_type = TypeListSample::template HasType<float>();
	/// constexpr bool has_char_type = TypeListSample::template HasType<char>();
	/// // has_float_type will be "true", has_char_type will be "false"
	/// @endcode
	/// 
	/// @tparam TypeToCheck
	/// Type to check if it exists in this TypeList.
	template<typename TypeToCheck>
	consteval static bool HasType()
	{
		return IsTypeInParameterPack<TypeToCheck, TypePack...>::value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if Type is unique in TypeList.
	///
	/// @note
	/// If the type is not found in the TypeList, a compile time error will be generated with message "Type not found".
	///
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double, false>;
	/// constexpr bool is_float_unique = TypeListSample::template IsTypeUnique<float>();
	/// constexpr bool is_int_unique = TypeListSample::template IsTypeUnique<int>();
	/// // constexpr bool is_char_unique = TypeListSample::template IsTypeUnique<char>();
	/// // is_float_unique will be "false", is_int_unique will be "true", is_char_unique would cause a compile time error.
	/// @endcode
	/// 
	/// @tparam TypeToCheck
	/// Type to check if it is unique in TypeList.
	/// 
	/// @return
	/// True if TypeToCheck is found once in TypeList. False if Check type was not found in TypeList or if it was found multiple times.
	template<typename TypeToCheck>
	consteval static u64 IsTypeUnique()
	{
		return IsTypeUniqueInParameterPack<TypeToCheck, TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if TypeList has any duplicate types.
	///
	/// Usage example:
	/// @code
	/// using TypeListSample1 = TypeList<int, float, double>;
	/// using TypeListSample2 = TypeList<int, float, double, int>;
	/// constexpr bool has_duplicates_1 = TypeListSample1::HasDuplicates();
	/// constexpr bool has_duplicates_2 = TypeListSample2::HasDuplicates();
	/// // has_duplicates_1 will be "false", has_duplicates_2 will be "true"
	/// @endcode
	///
	/// @return
	/// True if TypeList has any duplicate types.
	consteval static bool HasDuplicates()
	{
		return HasTemplateParameterPackDuplicates<TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if each type in TypeList is derived from BaseType.
	/// 
	/// @note
	/// If TypeList is empty, this will return false.
	/// 
	/// Usage example:
	/// @code
	/// struct Base_A {};
	/// struct Base_B {};
	/// struct Derived_A1 : Base_A {};
	/// struct Derived_A2 : Base_A {};
	/// struct Derived_B3 : Base_B {};
	/// using TypeListSample_1 = TypeList<Derived_A1, Derived_A2>;
	/// using TypeListSample_2 = TypeList<Derived_A1, Derived_A2, Derived_B3>;
	/// constexpr bool is_each_derived_from_base_1 = TypeListSample_1::template IsEachDerivedFromBase<Base_A>();
	/// constexpr bool is_each_derived_from_base_2 = TypeListSample_2::template IsEachDerivedFromBase<Base_A>();
	/// // is_each_derived_from_base_1 will be "true", is_each_derived_from_base_2 will be "false"
	/// @endcode
	/// 
	/// @tparam BaseType
	/// Base type to check if each type in TypeList is derived from.
	/// 
	/// @return
	/// True if each type in TypeList is derived from BaseType. False if any type in TypeList is not derived from BaseType, or if
	/// TypeList is empty.
	template<typename BaseType>
	consteval static bool IsEachDerivedFromBase()
	{
		return IsEachParameterPackTypeDerivedFromType<BaseType, TypePack...>::value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief 
	/// Test if types contained in this TypeList matches the types contained in other TypeList.
	/// 
	/// @note
	/// If this TypeList and the other TypeList are empty, this will always return true.
	/// 
	/// Usage example:
	/// @code
	/// using TypeListSample = TypeList<int, float, double>;
	/// using OtherTypeListSample_1 = TypeList<int, float, double>;
	/// using OtherTypeListSample_2 = TypeList<int, float, char>;
	/// constexpr bool matches_1 = TypeListSample::template Matches<OtherTypeListSample_1>();
	/// constexpr bool matches_2 = TypeListSample::template Matches<OtherTypeListSample_2>();
	/// // matches_1 will be "true", matches_2 will be "false"
	/// @endcode
	/// 
	/// @tparam OtherTypeList
	/// Other TypeList to compare with.
	/// 
	/// @return
	/// True if this TypeList types matches the other TypeList types.
	template<TypeListType OtherTypeList>
	consteval static bool Matches()
	{
		if constexpr( OtherTypeList::Size() != Size() ) return false;
		else return EqualityTest( TypeList {}, OtherTypeList {} );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if types contained in this TypeList matches the types contained in other TypeList.
	/// 
	/// @note
	/// If this TypeList and the other TypeList are empty, this will always return true.
	/// 
	/// Usage example:
	/// @code
	/// auto type_list_sample = TypeList<int, float, double>;
	/// auto other_type_list_sample_1 = TypeList<int, float, double>;
	/// auto other_type_list_sample_2 = TypeList<int, float, char>;
	/// constexpr bool matches_1 = type_list_sample == other_type_list_sample_1;
	/// constexpr bool matches_2 = type_list_sample == other_type_list_sample_2;
	/// // matches_1 will be "true", matches_2 will be "false"
	/// @endcode
	/// 
	/// @tparam OtherTypeList
	/// Other TypeList type to compare with.
	/// 
	/// @param
	/// Other TypeList object to compare with.
	/// 
	/// @return
	/// True if this TypeList types matches the other TypeList types.
	template<TypeListType OtherTypeList>
	consteval bool operator==(
		OtherTypeList&
	)
	{
		return Matches<OtherTypeList>();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief 
	/// Equality test helper.
	/// 
	/// @param Type
	/// Input type, ignored.
	/// 
	/// @param ...ParamaterPack
	/// Remaining types, ignored.
	/// 
	/// @return
	/// True.
	template <typename Type, typename... ParameterPack>
	struct MatchCheckHelper
	{
		consteval static bool EqualityTest()
		{
			return true;
		}
	};

	// Specialization for when TypeLists are not empty.
	template <typename T, typename... Ts, typename U, typename... Us>
	struct MatchCheckHelper<TypeList<T, Ts...>, TypeList<U, Us...>>
	{
		consteval static bool EqualityTest()
		{
			// Return as soon as we find a mismatch, skipping the remaining types.
			if constexpr( !std::is_same_v<T, U> ) return false;

			// Recursively call EqualityTest with the remaining elements.
			return MatchCheckHelper<TypeList<Ts...>, TypeList<Us...>>::EqualityTest();
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief 
	/// Start equality test.
	/// 
	/// @param ...TypeListTypeParamaterPack
	/// TypeList to compare with.
	/// 
	/// @param ...OtherTypeListTypeParamaterPack
	/// TypeList to compare with.
	/// 
	/// @return
	/// True if the two TypeLists contain the same types and their order is equal.
	template <typename... TypeListTypeParamaterPack, typename... OtherTypeListTypeParamaterPack>
	consteval static bool EqualityTest(
		TypeList<TypeListTypeParamaterPack...>,
		TypeList<OtherTypeListTypeParamaterPack...>
	)
	{
		return MatchCheckHelper<TypeList<TypeListTypeParamaterPack...>, TypeList<OtherTypeListTypeParamaterPack...>>::EqualityTest();
	}
};



#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {
struct TypeListTestBase_1 {};
struct TypeListTestBase_2 : TypeListTestBase_1 {};
struct TypeListTestDerived_1 : TypeListTestBase_1 {};
struct TypeListTestDerived_2 : TypeListTestBase_2 {};
struct TypeListTestDerived_3 : TypeListTestBase_2 {};

template<typename ...TypePack>
struct TypeListTestTemplateClass {};

// Tests for TypeList.
static_assert( sizeof( TypeList<> ) == 1 );
static_assert( sizeof( TypeList<int, float, double> ) == 1 );

static_assert( std::is_same_v<TypeList<int, float, double>::ApplyTo<TypeListTestTemplateClass>, TypeListTestTemplateClass<int, float, double>> );

static_assert( std::is_same_v<TypeList<int, float, double>::template IndexToType<0>, int> );
static_assert( std::is_same_v<TypeList<int, float, double>::template IndexToType<1>, float> );
static_assert( std::is_same_v<TypeList<int, float, double>::template IndexToType<2>, double> );

static_assert( TypeList<int, float, double>::template TypeToIndex<int>() == 0 );
static_assert( TypeList<int, float, double>::template TypeToIndex<float>() == 1 );
static_assert( TypeList<int, float, double>::template TypeToIndex<double>() == 2 );

static_assert( TypeList<>::Size() == 0 );
static_assert( TypeList<int>::Size() == 1 );
static_assert( TypeList<int, float, double>::Size() == 3 );
static_assert( TypeList<int, int, int>::Size() == 3 );
static_assert( TypeList<int, float, float>::Size() == 3 );

static_assert( TypeList<>::template CountType<int>() == 0 );
static_assert( TypeList<int>::template CountType<int>() == 1 );
static_assert( TypeList<int, int>::template CountType<int>() == 2 );
static_assert( TypeList<int, int, int>::template CountType<int>() == 3 );
static_assert( TypeList<int, float, int>::template CountType<int>() == 2 );
static_assert( TypeList<int, float>::template CountType<int>() == 1 );
static_assert( TypeList<int, float, double>::template CountType<int>() == 1 );
static_assert( TypeList<int, float, double>::template CountType<float>() == 1 );
static_assert( TypeList<int, float, double>::template CountType<double>() == 1 );
static_assert( TypeList<int, float, double>::template CountType<unsigned int>() == 0 );

static_assert( TypeList<>::HasDuplicates() == false );
static_assert( TypeList<int>::HasDuplicates() == false );
static_assert( TypeList<int, float, double>::HasDuplicates() == false );
static_assert( TypeList<int, double, double>::HasDuplicates() == true );
static_assert( TypeList<int, int, double>::HasDuplicates() == true );
static_assert( TypeList<int, float, int, double>::HasDuplicates() == true );

static_assert( TypeList<TypeListTestBase_1>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestBase_1, TypeListTestBase_1>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestDerived_1>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestDerived_2>::template IsEachDerivedFromBase<TypeListTestBase_2>() == true );
static_assert( TypeList<TypeListTestBase_2>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestBase_1>::template IsEachDerivedFromBase<TypeListTestBase_2>() == false );
static_assert( TypeList<TypeListTestDerived_1, TypeListTestBase_2>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestDerived_1, TypeListTestBase_2>::template IsEachDerivedFromBase<TypeListTestBase_2>() == false );
static_assert( TypeList<TypeListTestDerived_1, TypeListTestBase_2>::template IsEachDerivedFromBase<TypeListTestDerived_1>() == false );
static_assert( TypeList<TypeListTestDerived_2, TypeListTestBase_2>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );
static_assert( TypeList<TypeListTestDerived_2, TypeListTestBase_2, TypeListTestDerived_3>::template IsEachDerivedFromBase<TypeListTestBase_1>() == true );

static_assert( TypeList<>::template Matches<TypeList<>>() == true );
static_assert( TypeList<int>::template Matches<TypeList<int>>() == true );
static_assert( TypeList<int, float, double>::template Matches<TypeList<int>>() == false );
static_assert( TypeList<int, float, double>::template Matches<TypeList<int, float>>() == false );
static_assert( TypeList<int, float, double>::template Matches<TypeList<int, float, double>>() == true );

static auto type_list_sample = TypeList<int, float, double>();
static auto other_type_list_sample = TypeList<int, float, double>();
static_assert( type_list_sample == other_type_list_sample );

// Tests for TypeListType concept.
static_assert( TypeListType<TypeList<>> == true );
static_assert( TypeListType<TypeList<int>> == true );
static_assert( TypeListType<TypeList<int, float, double>> == true );
static_assert( TypeListType<int> == false );
static_assert( TypeListType<TypeListTestDerived_3> == false );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // utility
} // bc
