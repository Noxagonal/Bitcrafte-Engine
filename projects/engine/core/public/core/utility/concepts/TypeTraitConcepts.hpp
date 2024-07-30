#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The purpose of this header is to provide type trait concepts that are used in the engine. The C++ standard library does
// provide nearly all of these concepts, but some of their functionality has been altered to better fit the needs of the engine.
// For consistency, this header provides a consistent naming scheme for concepts used throughout the engine.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>
#include <concepts>



namespace bc {
namespace utility {



template<typename Type, typename OtherType>
concept SameAs = std::same_as<Type, OtherType>;

template<typename Derived, typename Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

template<typename From, typename To>
concept ConvertibleTo = std::convertible_to<From, To>;

template<typename Type, typename OtherType>
concept CommonReferenceWith = std::common_reference_with<Type, OtherType>;

template<typename Type, typename OtherType>
concept CommonWith = std::common_with<Type, OtherType>;

template<typename Type>
concept Integral = std::integral<Type>;

template<typename Type>
concept SignedIntegral = std::signed_integral<Type>;

template<typename Type>
concept UnsignedIntegral = std::unsigned_integral<Type>;

template<typename Type>
concept FloatingPoint = std::floating_point<Type>;

template<typename Type, typename From>
concept AssignableFrom =
	std::is_lvalue_reference_v<Type> &&
	// Skip CommonReferenceWith because it would prevent assigning from non-related types.
	// CommonReferenceWith<const std::remove_reference_t<Type>&, const std::remove_reference_t<From>&> &&
	requires( Type type, From&& from )
	{
		{ type = static_cast<From&&>( from ) } -> SameAs<Type>;
	};

template<typename Type>
concept Destructible = std::destructible<Type>;

template<typename Type, typename ...ArgumentTypePack>
concept ConstructibleFrom = std::constructible_from<Type, ArgumentTypePack...>;

template<typename Type>
concept DefaultInitializable = std::default_initializable<Type>;

template<typename Type>
concept MoveConstructible = std::move_constructible<Type>;

template<typename Type>
concept CopyConstructible = std::copy_constructible<Type>;

template<typename Type>
concept Swappable = std::swappable<Type>;

template<typename Type, typename OtherType>
concept SwappableWith = std::swappable_with<Type, OtherType>;

template<typename Type>
concept Movable =
	std::is_object_v<Type> &&
	MoveConstructible<Type> &&
	AssignableFrom<Type&, Type>;

template<typename Type>
concept Copyable =
	Movable<Type> &&
	CopyConstructible<Type> &&
	AssignableFrom<Type&, Type&> &&
	AssignableFrom<Type&, const Type&> &&
	AssignableFrom<Type&, const Type>;

template<typename Type>
concept Semiregular =
	Copyable<Type> &&
	DefaultInitializable<Type>;

template<typename Type>
concept EqualityComparable = std::equality_comparable<Type>;

template<typename Type, typename OtherType>
concept EqualityComparableWith = std::equality_comparable_with<Type, OtherType>;

template<typename Type>
concept TotallyOrdered = std::totally_ordered<Type>;

template<typename Type, typename OtherType>
concept TotallyOrderedWith = std::totally_ordered_with<Type, OtherType>;

template<typename Type>
concept Regular =
	Semiregular<Type> &&
	EqualityComparable<Type>;

template<typename Functor, typename ...ArgumentTypePack>
concept Invocable = std::invocable<Functor, ArgumentTypePack...>;

template<typename Functor, typename ...ArgumentTypePack>
concept RegularInvocable = std::regular_invocable<Functor, ArgumentTypePack...>;

template<typename Functor, typename ...ArgumentTypePack>
concept Predicate = std::predicate<Functor, ArgumentTypePack...>;

template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept Relation = std::relation<Functor, FirstArgumentType, SecondArgumentType>;

template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept EquivalenceRelation = std::equivalence_relation<Functor, FirstArgumentType, SecondArgumentType>;

template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept StrictWeakOrder = std::strict_weak_order<Functor, FirstArgumentType, SecondArgumentType>;



} // namespace utility
} // namespace bc
