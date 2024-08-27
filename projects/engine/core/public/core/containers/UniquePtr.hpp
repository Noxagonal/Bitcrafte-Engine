#pragma once

#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Container which may contains a single heap allocated object. Similar to std::unique_ptr.
///
/// @note
/// This container can handle objects which constructor may throw. If there's a chance that the object constructor may throw,
/// it can still be wrapped into a UniquePtr or Optional.
///
/// @note
/// Aquiring raw pointers is not supported. Likewise releasing raw pointers is not supported. This is because internal memory
/// management is not as straightforward as std::unique_ptr.
///
/// @tparam ValueType
/// Type of the contained object/element.
template<typename ValueType>
class UniquePtr : private container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherValueType>
	using ThisContainerType					= UniquePtr<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherValueType>
	using ThisContainerFullType				= UniquePtr<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	friend class UniquePtr;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr UniquePtr() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr UniquePtr( std::nullptr_t ) noexcept
		:
		data_ptr( nullptr )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr UniquePtr( const ThisType& other ) noexcept = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr UniquePtr( ThisType&& other ) noexcept
	{
		this->Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructor to move data from another UniquePtr which contains an inherited type.
	///
	/// This allows the use of UniquePtr as a base class and directly move another UniquePtr to it with inherited type.
	///
	/// @tparam OtherValueType
	/// Type of the data in the other UniquePtr.
	///
	/// @param other
	/// The other container.
	///
	/// @return
	/// Reference to this.
	template<typename OtherValueType>
	constexpr UniquePtr( ThisContainerType<OtherValueType>&& other ) noexcept
		requires( std::is_base_of_v<ValueType, OtherValueType> && !std::is_same_v<OtherValueType, ValueType> )
	{
		this->data_ptr = other.data_ptr;
		other.data_ptr = nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~UniquePtr()
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ThisType& other ) noexcept -> ThisType & = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( std::nullptr_t ) -> ThisType&
	{
		this->Clear();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ThisType&& other ) noexcept -> ThisType&
	{
		if( this == std::addressof( other ) ) return *this;

		this->Swap( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Operator to move data from another UniquePtr which contains an inherited type.
	///
	/// This allows the use of UniquePtr as a base class and directly move another UniquePtr to it with inherited type.
	///
	/// @tparam OtherValueType
	/// Type of the data in the other UniquePtr.
	///
	/// @param other
	/// The other container.
	///
	/// @return
	/// Reference to this.
	template<typename OtherValueType>
	constexpr auto operator=( ThisContainerType<OtherValueType>&& other ) noexcept -> ThisType&
		requires( std::is_base_of_v<ValueType, OtherValueType> && !std::is_same_v<OtherValueType, ValueType> )
	{
		this->Clear();

		this->data_ptr = other.data_ptr;
		other.data_ptr = nullptr;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator->() const noexcept -> const ValueType*
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator->() noexcept -> ValueType*
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator*() noexcept -> ValueType&
	{
		BAssert( this->Get(), U"Container is empty" );
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator*() const noexcept -> const ValueType&
	{
		BAssert( this->Get(), U"Container is empty" );
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs the member inside.
	///
	/// @note
	/// If contained value has already been constructed and alive, it is destructed first, however, memory is recycled and the new
	/// object will share the old pointer.
	///
	/// @tparam ...ConstructorArgumentTypePack
	/// Types of arguments sent to the contained value constructor.
	///
	/// @param ...constructor_arguments
	/// Arguments passed to the contained value constructor.
	template<typename ...ConstructorArgumentTypePack>
	constexpr void Emplace( ConstructorArgumentTypePack&& ...constructor_arguments )
	{
		if( this->data_ptr ) this->TryDestructHeapElement( this->data_ptr );
		if( this->data_ptr == nullptr ) this->AllocateContainedMemory();
		this->TryConstructHeapElement( this->data_ptr, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Get() const noexcept -> const ValueType*
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Get() noexcept -> ValueType*
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Casts and releases the contained pointer to the new unique pointer.
	///
	/// Transfers ownership of the contained pointer to a new unique pointer. This is a helper function primarily for downcasting,
	/// this function is provided mostly because releasing ownership of the contained pointer is not supported. This is because raw
	/// pointer memory management is not as straightforward as std::unique_ptr.
	///
	/// @note
	/// Up-cast, down-cast and same type cast are supported. It's not necessary to use this function to up-cast as it is implicit.
	///
	/// @note
	/// The contained pointer must be derived from ValueType.
	///
	/// @note
	/// If cast fails, exception will be thrown in debug builds. In release builds, an empty UniquePtr will be returned and current
	/// UniquePtr will retain ownership.
	///
	/// @tparam DerivedValueType
	/// Type to cast to.
	/// 
	/// @return
	/// New unique pointer which is taking ownership of this UniquePtr.
	template<typename CastToValueType>
	constexpr auto CastTo() -> ThisContainerType<CastToValueType>
	{
		static_assert(
			std::is_base_of_v<ValueType, CastToValueType> || std::is_base_of_v<CastToValueType, ValueType>,
			"Cannot cast, Types have no correlation"
			);

		if constexpr( std::is_base_of_v<CastToValueType, ValueType> )
		{
			// No cast or upcast.
			return UniquePtr( std::move( *this ) );
		}
		else if constexpr( std::is_base_of_v<ValueType, CastToValueType> )
		{
			// Downcast.
			auto casted_ptr = dynamic_cast<CastToValueType*>( this->data_ptr );
			BAssert( casted_ptr, U"Failed to downcast" );
			if( casted_ptr == nullptr ) return {};
			auto downcasted_unique_ptr = UniquePtr<CastToValueType>();
			downcasted_unique_ptr.data_ptr = casted_ptr;
			this->data_ptr = nullptr;
			return downcasted_unique_ptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Clear()
	{
		if( this->data_ptr == nullptr ) return;
		this->TryDestructHeapElement( this->data_ptr );
		this->FreeContainedMemory();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto IsEmpty() const noexcept -> bool
	{
		return !Get();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void AllocateContainedMemory() noexcept
	{
		BEngineHardAssert( this->data_ptr == nullptr, U"This function should not be called on an occupied container" );
		this->data_ptr = memory::AllocateMemory<ValueType>( 1, alignof( ValueType ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void FreeContainedMemory() noexcept
	{
		BEngineHardAssert( this->data_ptr, U"This function should not be called on an empty container" );
		memory::FreeMemory( this->data_ptr, 1 );
		this->data_ptr = nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Swap( ThisType& other ) noexcept
	{
		std::swap( this->data_ptr, other.data_ptr );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType*	data_ptr	= nullptr;
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if container fulfill size requirements.
static_assert( sizeof( UniquePtr<u32> ) == 8 );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if container fulfill concept requirements.
static_assert( !utility::ContainerView<UniquePtr<u32>> );
static_assert( !utility::ContainerEditableView<UniquePtr<u32>> );
static_assert( !utility::Container<UniquePtr<u32>> );

static_assert( !utility::LinearContainerView<UniquePtr<u32>> );
static_assert( !utility::LinearContainerEditableView<UniquePtr<u32>> );
static_assert( !utility::LinearContainer<UniquePtr<u32>> );

static_assert( !utility::TextContainerView<UniquePtr<char32_t>> );
static_assert( !utility::TextContainerEditableView<UniquePtr<char32_t>> );
static_assert( !utility::TextContainer<UniquePtr<char32_t>> );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if container is constructible from other containers.
// Not constructible via copy.
static_assert(
	!utility::ConstructibleFrom<
	UniquePtr<u32>,
	std::add_lvalue_reference_t<UniquePtr<u32>>
	>
	);
// Constructible via move.
static_assert(
	utility::ConstructibleFrom<
	UniquePtr<u32>,
	std::add_rvalue_reference_t<UniquePtr<u32>>
	>
	);
// Not assignable via copy.
static_assert(
	!utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<u32>>,
	std::add_lvalue_reference_t<UniquePtr<u32>>
	>
	);
// Assignable via move.
static_assert(
	utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<u32>>,
	std::add_rvalue_reference_t<UniquePtr<u32>>
	>
	);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UniquePtrTestContainerBase {};
struct UniquePtrTestContainerDerived : UniquePtrTestContainerBase {};
struct UniquePtrTestContainerNotDerived {};
// Not copy constructible from derived.
static_assert(
	!utility::ConstructibleFrom<
	UniquePtr<UniquePtrTestContainerBase>,
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerDerived>>
	>
	);
// Not copy constructible from not derived.
static_assert(
	!utility::ConstructibleFrom<
	UniquePtr<UniquePtrTestContainerBase>,
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerNotDerived>>
	>
	);
// Move constructible from derived.
static_assert(
	utility::ConstructibleFrom<
	UniquePtr<UniquePtrTestContainerBase>,
	std::add_rvalue_reference_t<UniquePtr<UniquePtrTestContainerDerived>>
	>
	);
// Not move constructible from not derived.
static_assert(
	!utility::ConstructibleFrom<
	UniquePtr<UniquePtrTestContainerBase>,
	std::add_rvalue_reference_t<UniquePtr<UniquePtrTestContainerNotDerived>>
	>
	);
// Not copy assignable from derived.
static_assert(
	!utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerBase>>,
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerDerived>>
	>
	);
// Not copy assignable from not derived.
static_assert(
	!utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerBase>>,
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerNotDerived>>
	>
	);

// Move assignable from derived.
static_assert(
	utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerBase>>,
	std::add_rvalue_reference_t<UniquePtr<UniquePtrTestContainerDerived>>
	>
	);
// Not move assignable from not derived.
static_assert(
	!utility::AssignableFrom<
	std::add_lvalue_reference_t<UniquePtr<UniquePtrTestContainerBase>>,
	std::add_rvalue_reference_t<UniquePtr<UniquePtrTestContainerNotDerived>>
	>
	);



} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make unique pointer container and construct its contained value.
///
/// Functionally this is very similar to std::make_unique, except it uses engine provided memory pool and engine provided
/// assertions.
///
/// @tparam ValueType
/// Contained value type of the unique pointer.
///
/// @tparam ...ConstructorArgumentTypePack
/// Parameter types forwarded to value constructor.
///
/// @param ...constructor_arguments
/// Arguments forwarded to contained value constructor.
///
/// @return
/// A new unique pointer with its contained value constructed.
template<
	typename ValueType,
	typename ...ConstructorArgumentTypePack
>
auto MakeUniquePtr( ConstructorArgumentTypePack&& ...constructor_arguments ) -> UniquePtr<ValueType>
{
	auto result = UniquePtr<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



} // bc
