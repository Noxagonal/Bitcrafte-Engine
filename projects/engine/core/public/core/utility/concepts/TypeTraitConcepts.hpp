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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types are the same.
///
/// This concept checks if two types are exactly the same type.
///
/// @tparam Type
/// The first type to compare.
///
/// @tparam OtherType
/// The second type to compare.
template<typename Type, typename OtherType>
concept SameAs = std::same_as<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is derived from another type.
///
/// This concept checks if one type is derived from another type.
///
/// @tparam Derived
/// The derived type to test.
///
/// @tparam Base
/// The base type to test if first type derives from it.
template<typename Derived, typename Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is convertible to another type.
///
/// This concept checks if one type can be implicitly converted to another type.
///
/// @tparam From
/// The type to convert from.
///
/// @tparam To
/// The type to convert to.
template<typename From, typename To>
concept ConvertibleTo = std::convertible_to<From, To>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types share a common reference type.
///
/// This concept checks if two types have a common reference type that they can both be converted to. This common reference type
/// is a type that both original types can be converted to without losing constness or reference qualifiers.
///
/// @code
/// static_assert( CommonReferenceWith<int&, const int&>, "'int&' and 'const int&' have a common reference type of 'const int&'" );
/// static_assert( CommonReferenceWith<int&, int&&>, "'int&' and 'int&&' have a common reference type of 'int&'" );
/// static_assert( CommonReferenceWith<int&&, int&>, "'int&&' and 'int&' have a common reference type of 'int&'" );
/// @endcode
///
/// @tparam Type
/// The first type to compare.
///
/// @tparam OtherType
/// The second type to compare.
template<typename Type, typename OtherType>
concept CommonReferenceWith = std::common_reference_with<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types share a common type.
///
/// This concept checks if two types have a common type that they can both be converted to. This common type is a type that both
/// original types can be implicitly converted to.
///
/// @code
/// static_assert( CommonWith<int, double>, "'int' and 'double' have a common type of 'double'" );
/// static_assert( CommonWith<int, float>, "'int' and 'float' have a common type of 'float'" );
/// static_assert( CommonWith<float, int>, "'float' and 'int' have a common type of 'float'" );
/// @endcode
///
/// @tparam Type
/// The first type to compare.
///
/// @tparam OtherType
/// The second type to compare.
template<typename Type, typename OtherType>
concept CommonWith = std::common_with<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a character type.
///
/// Character is an integral type that is commonly used as a character type.
///
/// Character types include `signed char`, `unsigned char`, `wchar_t`, `char8_t`, `char16_t`, and `char32_t`. This concept
/// checks if a type is one of these character types.
///
/// @tparam Type
/// Type to test if it is a character type.
template<typename Type>
concept Character =
	std::is_same_v<Type, signed char> ||
	std::is_same_v<Type, unsigned char> ||
	std::is_same_v<Type, wchar_t> ||
	std::is_same_v<Type, char8_t> ||
	std::is_same_v<Type, char16_t> ||
	std::is_same_v<Type, char32_t>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is an integral type.
///
/// Integral types are types that represent integer values, including both signed and unsigned integers.
///
/// @tparam Type
/// Type to test if it is an integral type.
template<typename Type>
concept Integral = std::integral<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a signed integral type.
///
/// Signed integral types are types that represent signed integer values.
///
/// @tparam Type
/// Type to test if it is a signed integral type.
template<typename Type>
concept SignedIntegral = std::signed_integral<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is an unsigned integral type.
///
/// Unsigned integral types are types that represent unsigned integer values.
///
/// @tparam Type
/// Type to test if it is an unsigned integral type.
template<typename Type>
concept UnsignedIntegral = std::unsigned_integral<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a floating-point type.
///
/// Floating-point types are types that represent real numbers with fractional parts.
///
/// @tparam Type
/// Type to test if it is a floating-point type.
template<typename Type>
concept FloatingPoint = std::floating_point<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is assignable from another type.
///
/// AssignableFrom types provide an implementation of the assignment operator, allowing an object of one type to be assigned a
/// value of another type.
///
/// @tparam Type
/// Type to test if it is assignable from the other type.
///
/// @tparam From
/// Type to be assigned.
template<typename Type, typename From>
concept AssignableFrom =
	std::is_lvalue_reference_v<Type> &&
	// Skip CommonReferenceWith because it would prevent assigning from non-related types. We actually want this.
	// CommonReferenceWith<const std::remove_reference_t<Type>&, const std::remove_reference_t<From>&> &&
	requires( Type type, From&& from )
	{
		{ type = static_cast<From&&>( from ) } -> SameAs<Type>;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is destructible.
///
/// Destructible types provide an implementation of a destructor, allowing objects of the type to be destroyed correctly.
///
/// @tparam Type
/// Type to test if it is destructible.
template<typename Type>
concept Destructible = std::destructible<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is constructible from a given set of argument types.
///
/// Constructible types provide an implementation of a constructor that takes the specified argument types.
///
/// @tparam Type
/// Type to test if it is constructible from the given argument types.
///
/// @tparam ArgumentTypePack
/// Types of the arguments to test for constructibility.
template<typename Type, typename ...ArgumentTypePack>
concept ConstructibleFrom = std::constructible_from<Type, ArgumentTypePack...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is default initializable.
///
/// Default initializable types provide an implementation of a default constructor, allowing objects of the type to be
/// default-initialized.
///
/// @tparam Type
/// Type to test if it is default initializable.
template<typename Type>
concept DefaultInitializable = std::default_initializable<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is move constructible.
///
/// Move constructible types provide an implementation of a move constructor, allowing objects of the type to be constructed
/// from rvalue references.
///
/// @tparam Type
/// Type to test if it is move constructible.
template<typename Type>
concept MoveConstructible = std::move_constructible<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is copy constructible.
///
/// Copy constructible types provide an implementation of a copy constructor, allowing objects of the type to be constructed
/// from lvalue references.
///
/// @tparam Type
/// Type to test if it is copy constructible.
template<typename Type>
concept CopyConstructible = std::copy_constructible<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is swappable.
///
/// Swappable types provide an implementation of the swap operation, allowing two objects of the same type to have their values
/// exchanged.
///
/// @tparam Type
/// Type to test if it is swappable with itself.
template<typename Type>
concept Swappable = std::swappable<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types are swappable with each other.
///
/// Types that are swappable with each other provide an implementation of the swap operation, allowing objects of the two types
/// to have their values exchanged.
///
/// @tparam Type
/// The first type to test if it is swappable with the other.
///
/// @tparam OtherType
/// The second type to test if it is swappable with the first.
template<typename Type, typename OtherType>
concept SwappableWith = std::swappable_with<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is movable.
///
/// Movable types provide an implementation of move construction and move assignment.
///
/// @tparam Type
/// Type to test if it is movable.
template<typename Type>
concept Movable = std::movable<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is copyable.
///
/// Copyable types provide an implementation of copy construction and copy assignment.
///
/// @tparam Type
/// Type to test if it is copyable.
template<typename Type>
concept Copyable = std::copyable<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is equality comparable.
///
/// Equality comparable types provide an implementation of the equality operators (==, !=).
///
/// @tparam Type
/// Type to test if it is equality comparable.
template<typename Type>
concept EqualityComparable = std::equality_comparable<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types are equality comparable with each other.
///
/// Types that are equality comparable with each other provide an implementation of the equality operators (==, !=) for testing
/// against each other.
///
/// @tparam Type
/// Type that implements equality operators.
///
/// @tparam OtherType
/// Type to test against.
template<typename Type, typename OtherType>
concept EqualityComparableWith = std::equality_comparable_with<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is totally ordered.
///
/// Totally ordered types provide an implementation of the equality and relational operators (==, !=, <, <=, >, >=).
///
/// @tparam Type
/// Type to test if it is totally ordered.
template<typename Type>
concept TotallyOrdered = std::totally_ordered<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if two types are totally ordered with each other.
///
/// Types that are totally ordered with each other provide an implementation of the equality and relational operators
/// (<, <=, >, >=, ==, !=) for testing against each other.
///
/// @tparam Type
/// Type that implements equality and relational operators.
///
/// @tparam OtherType
/// Type to test against.
template<typename Type, typename OtherType>
concept TotallyOrderedWith = std::totally_ordered_with<Type, OtherType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a semiregular type.
///
/// Semiregular types behave like built-in types with respect to construction, assignment, and destruction, but do not
/// need to be equality comparable.
///
/// Semiregular types satisfy these requirements:
/// - Default constructible
/// - Copy constructible
/// - Move constructible
/// - Copy assignable
/// - Move assignable
/// - Destructible
///
/// @tparam Type
/// Type to test if it is a semiregular type.
template<typename Type>
concept Semiregular = std::semiregular<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a regular type.
///
/// Regular types behave like built-in types with respect to construction, assignment, destruction, and equality comparison.
///
/// Regular types satisfy these requirements:
/// - Default constructible
/// - Copy constructible
/// - Move constructible
/// - Copy assignable
/// - Move assignable
/// - Destructible
/// - Equality comparable ('==' and '!=')
///
/// @tparam Type
/// Type to test if it is a regular type.
template<typename Type>
concept Regular = std::regular<Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a functor can be called with the specified argument types.
///
/// Test to see if a functor (a function, invocable object, or lambda) can be called with the specified argument types.
/// 
/// @tparam Functor
/// Functor to test if it can be called.
///
/// @tparam ArgumentTypePack
/// Types to test if the functor can be called with these argument types.
template<typename Functor, typename ...ArgumentTypePack>
concept Invocable = std::invocable<Functor, ArgumentTypePack...>;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a functor can be called with the specified argument types and returns a specified type.
///
/// Test to see if a functor (a function, invocable object, or lambda) can be called with the specified argument types and that
/// the functor return type matches the specified return type.
///
/// @tparam Functor
/// Functor to test if it can be called.
///
/// @tparam ReturnType
/// The expected return type of the functor.
///
/// @tparam ArgumentTypePack
/// Types to test if the functor can be called with these argument types.
template<typename Functor, typename ReturnType, typename ...ArgumentTypePack>
concept InvocableWithReturn = std::is_invocable_r_v<ReturnType, Functor, ArgumentTypePack...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a regular invocable functor.
///
/// A regular invocable functor is a type that can be invoked with a specified set of arguments and satisfies the requirements
/// of a regular type, which include being copyable, assignable, and default constructible.
///
/// @tparam Functor
/// Functor to test if it can be called and is a regular invocable.
///
/// @tparam ArgumentTypePack
/// Types to test if the functor can be called with these argument types.
template<typename Functor, typename ...ArgumentTypePack>
concept RegularInvocable = std::regular_invocable<Functor, ArgumentTypePack...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is an invocable object and not a function.
///
/// @tparam Functor
/// Type to test if it is an invocable object.
template<typename Functor>
concept InvocableObject =
	std::is_class_v<std::remove_reference_t<Functor>> &&
	requires
	{
		{ &Functor::operator() };
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a function and not an invocable object.
///
/// @tparam Functor
/// Type to test if it is a function.
template<typename Functor>
concept InvocableFunction =
	std::is_function_v<std::remove_pointer_t<Functor>> &&
	!InvocableObject<Functor>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a type is a predicate.
///
/// Checks if a type satisfies the requirements to be used as a predicate, i.e., a functor that returns a value convertible to
/// boolean.
///
/// @tparam Functor
/// Type to test if it is a predicate.
///
/// @tparam ArgumentTypePack
/// Types to test with the predicate.
template<typename Functor, typename ...ArgumentTypePack>
concept Predicate = std::predicate<Functor, ArgumentTypePack...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a functor can be used in a relation.
///
/// The Relation concept ensures that a functor can be used as a predicate in all combinations of the first and second argument
/// types. Relations are used in algorithms to define relationships between pairs of elements. A valid relation must work for
/// all combinations of the specified types.
///
/// @see
/// bc::utility::Predicate
///
/// @tparam Functor
/// Functor to test if it can be used in a relation.
///
/// @tparam FirstArgumentType
/// The type of the first argument in the relation.
///
/// @tparam SecondArgumentType
/// The type of the second argument in the relation.
template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept Relation = std::relation<Functor, FirstArgumentType, SecondArgumentType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a functor can be used in an equivalence relation.
///
/// An equivalence relation is a binary predicate that satisfies the following properties:
/// - Reflexive: relation(a, a) must be true.
/// - Symmetric: relation(a, b) must be true if and only if relation(b, a) is true.
/// - Transitive: If relation(a, b) and relation(b, c) are true, then relation(a, c) must be true.
///
/// @see
/// bc::utility::Relation
///
/// @tparam Functor
/// Functor to test if it can be used in an equivalence relation.
///
/// @tparam FirstArgumentType
/// The type of the first argument in the equivalence relation.
///
/// @tparam SecondArgumentType
/// The type of the second argument in the equivalence relation.
template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept EquivalenceRelation = std::equivalence_relation<Functor, FirstArgumentType, SecondArgumentType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to test if a functor can be used as a strict weak order.
///
/// A strict weak order is a binary predicate that satisfies the following properties:
/// - Irreflexive: relation(a, a) must be false for all a.
/// - Transitive: If relation(a, b) and relation(b, c) are true, then relation(a, c) must be true.
/// - Asymmetric: If relation(a, b) is true, then relation(b, a) must be false.
/// - Transitivity of Incomparability: If neither relation(a, b) nor relation(b, a) is true, and neither relation(b, c) nor
///     relation(c, b) is true, then neither relation(a, c) nor relation(c, a) is true.
///
/// @see
/// bc::utility::Relation
///
/// @tparam Functor
/// Functor to test if it can be used as a strict weak order.
///
/// @tparam FirstArgumentType
/// The type of the first argument in the strict weak order.
///
/// @tparam SecondArgumentType
/// The type of the second argument in the strict weak order.
template<typename Functor, typename FirstArgumentType, typename SecondArgumentType>
concept StrictWeakOrder = std::strict_weak_order<Functor, FirstArgumentType, SecondArgumentType>;



// TODO: Move tests to a test target.
#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {


void InvocableFunction_Test_void_void();
void InvocableFunction_Test_void_int( int value );
int InvocableFunction_Test_int_void();
int InvocableFunction_Test_int_int( int value );
struct InvocableObject_Test_void_void { void operator()() {} };
struct InvocableObject_Test_void_int { void operator()( int value ) {} };
struct InvocableObject_Test_int_void { int operator()() { return 1; } };
struct InvocableObject_Test_int_int { int operator()( int value ) { return value; } };
struct InvocableObject_Test_NonInvokable {};

static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_void_void ), void> );
static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_void_int ), void, int> );
static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_int_void ), void> );
static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_int_int ), int, int> );
static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_int_void ), void> ); // Can invoke without taking the return type.
static_assert( InvocableWithReturn<decltype( InvocableFunction_Test_int_int ), void, int> ); // Can invoke without taking the return type.

static_assert( InvocableWithReturn<InvocableObject_Test_void_void, void> );
static_assert( InvocableWithReturn<InvocableObject_Test_void_int, void, int> );
static_assert( InvocableWithReturn<InvocableObject_Test_int_void, int> );
static_assert( InvocableWithReturn<InvocableObject_Test_int_int, int, int> );
static_assert( InvocableWithReturn<InvocableObject_Test_int_void, void> ); // Can invoke without taking the return type.
static_assert( InvocableWithReturn<InvocableObject_Test_int_int, void, int> ); // Can invoke without taking the return type.

static_assert( InvocableWithReturn<decltype( []() {} ), void> );
static_assert( InvocableWithReturn<decltype( []( int ) {} ), void, int> );
static_assert( InvocableWithReturn<decltype( []() { return 0; } ), int> );
static_assert( InvocableWithReturn<decltype( []( int ) { return 0; } ), int, int> );
static_assert( InvocableWithReturn<decltype( []() { return 0; } ), void> ); // Can invoke without taking the return type.
static_assert( InvocableWithReturn<decltype( []( int ) { return 0; } ), void, int> ); // Can invoke without taking the return type.


static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_void_void ), int> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_void_void ), void, void> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_void_int ), int> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_void_int ), int, void> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_void_int ), int, int> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_int_void ), int, void> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_int_void ), int, int> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_int_int ), int, void> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_int_int ), void> );
static_assert( !InvocableWithReturn<decltype( InvocableFunction_Test_int_int ), int> );

static_assert( !InvocableWithReturn<InvocableObject_Test_void_void, int> );
static_assert( !InvocableWithReturn<InvocableObject_Test_void_void, void, void> );
static_assert( !InvocableWithReturn<InvocableObject_Test_void_int, int> );
static_assert( !InvocableWithReturn<InvocableObject_Test_void_int, int, void> );
static_assert( !InvocableWithReturn<InvocableObject_Test_void_int, int, int> );
static_assert( !InvocableWithReturn<InvocableObject_Test_int_void, int, void> );
static_assert( !InvocableWithReturn<InvocableObject_Test_int_void, int, int> );
static_assert( !InvocableWithReturn<InvocableObject_Test_int_int, int, void> );
static_assert( !InvocableWithReturn<InvocableObject_Test_int_int, void> );
static_assert( !InvocableWithReturn<InvocableObject_Test_int_int, int> );

static_assert( !InvocableWithReturn<decltype( []() {} ), int> );
static_assert( !InvocableWithReturn<decltype( []() {} ), void, void> );
static_assert( !InvocableWithReturn<decltype( []( int ) {} ), int> );
static_assert( !InvocableWithReturn<decltype( []( int ) {} ), int, void> );
static_assert( !InvocableWithReturn<decltype( []( int ) {} ), int, int> );
static_assert( !InvocableWithReturn<decltype( []() { return 0; } ), int, void> );
static_assert( !InvocableWithReturn<decltype( []() { return 0; } ), int, int> );
static_assert( !InvocableWithReturn<decltype( []( int ) { return 0; } ), int, void> );
static_assert( !InvocableWithReturn<decltype( []( int ) { return 0; } ), void> );
static_assert( !InvocableWithReturn<decltype( []( int ) { return 0; } ), int> );


static_assert( !InvocableObject<decltype( InvocableFunction_Test_void_void )> );
static_assert( !InvocableObject<decltype( InvocableFunction_Test_void_int )> );
static_assert( !InvocableObject<decltype( InvocableFunction_Test_int_void )> );
static_assert( !InvocableObject<decltype( InvocableFunction_Test_int_int )> );

static_assert( InvocableObject<InvocableObject_Test_void_void> );
static_assert( InvocableObject<InvocableObject_Test_void_int> );
static_assert( InvocableObject<InvocableObject_Test_int_void> );
static_assert( InvocableObject<InvocableObject_Test_int_int> );

static_assert( InvocableObject<decltype( []() {} )> );
static_assert( InvocableObject<decltype( []( int ) {} )> );
static_assert( InvocableObject<decltype( []() { return 0; } )> );
static_assert( InvocableObject<decltype( []( int ) { return 0; } )> );

static_assert( !InvocableObject<int> );
static_assert( !InvocableObject<float> );
static_assert( !InvocableObject<void> );
static_assert( !InvocableObject<InvocableObject_Test_NonInvokable> );


static_assert( InvocableFunction<decltype( InvocableFunction_Test_void_void )> );
static_assert( InvocableFunction<decltype( InvocableFunction_Test_void_int )> );
static_assert( InvocableFunction<decltype( InvocableFunction_Test_int_void )> );
static_assert( InvocableFunction<decltype( InvocableFunction_Test_int_int )> );

static_assert( !InvocableFunction<InvocableObject_Test_void_void> );
static_assert( !InvocableFunction<InvocableObject_Test_void_int> );
static_assert( !InvocableFunction<InvocableObject_Test_int_void> );
static_assert( !InvocableFunction<InvocableObject_Test_int_int> );

static_assert( !InvocableFunction<decltype( []() {} )> );
static_assert( !InvocableFunction<decltype( []( int ) {} )> );
static_assert( !InvocableFunction<decltype( []() { return 0; } )> );
static_assert( !InvocableFunction<decltype( []( int ) { return 0; } )> );

static_assert( !InvocableFunction<int> );
static_assert( !InvocableFunction<float> );
static_assert( !InvocableFunction<void> );
static_assert( !InvocableFunction<InvocableObject_Test_NonInvokable> );



} // namespace tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

} // namespace utility
} // namespace bc
