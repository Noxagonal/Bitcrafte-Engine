#pragma once

#include <core/containers/backend/ContainerBase.hpp>

#include <core/diagnostic/assertion/Assert.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Container which may contains a single stack object. Similar to std::optional.
///
/// @note
/// This container can handle objects which constructor may throw. If there's a chance that the object constructor may throw,
/// it should be wrapped into an Optional or UniquePtr.
///
/// @tparam ValueType
/// Type of the contained object/element.
template<typename ValueType>
class Optional : private container_bases::ContainerResource
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class NonTrivialType
	{
	public:
		inline NonTrivialType() {}
		u8 dummy_value = {};
	};
	static_assert( !std::is_trivial_v<NonTrivialType> );
	static_assert( sizeof( NonTrivialType ) == 1 );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherValueType>
	using ThisContainerType					= Optional<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherValueType>
	using ThisContainerFullType				= Optional<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	friend class Optional;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Optional() noexcept :
		dummy_value( {} ),
		has_data( false )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit Optional( std::nullptr_t ) noexcept :
		dummy_value( {} ),
		has_data( false )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Optional( const ThisType& other )
		requires( std::is_copy_constructible_v<ValueType> )
	:
		dummy_value( {} ),
		has_data( false )
	{
		if( !other.has_data ) return;
		this->ConstructStackElement( this->data, other.data );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Optional( ThisType&& other ) noexcept
		requires( std::is_move_constructible_v<ValueType> )
	:
		dummy_value( {} ),
		has_data( false )
	{
		if( !other.has_data ) return;
		this->ConstructStackElement( this->data, std::move( other.data ) );
		other.has_data = false;
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Optional( const ValueType& other_value )
		requires( std::is_copy_constructible_v<ValueType> )
	:
		dummy_value( {} ),
		has_data( false )
	{
		this->ConstructStackElement( this->data, other_value );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Optional( ValueType&& other_value ) noexcept
		requires( std::is_move_constructible_v<ValueType> )
	:
		dummy_value( {} ),
		has_data( false )
	{

		this->ConstructStackElement( this->data, std::move( other_value ) );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~Optional()
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( std::nullptr_t ) -> ThisType&
	{
		this->Clear();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ThisType& other )->ThisType&
		requires( std::is_copy_assignable_v<ValueType> )
	{
		this->CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ThisType&& other ) noexcept -> ThisType&
		requires( std::is_move_assignable_v<ValueType> )
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ValueType& other_value )->ThisType&
		requires( std::is_copy_assignable_v<ValueType> )
	{
		this->Emplace( other_value );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ValueType&& other_value )->ThisType&
		requires( std::is_move_constructible_v<ValueType> && std::is_move_assignable_v<ValueType> )
	{
		if( this->has_data )
		{
			this->data = std::move( other_value );
		}
		else
		{
			this->ConstructStackElement( this->data, other_value );
		}
		this->has_data = true;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator->() const noexcept -> const ValueType*
	{
		return &this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator->() noexcept -> ValueType*
	{
		return &this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator*() const noexcept -> const ValueType&
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator*() noexcept -> ValueType&
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs the member inside.
	///
	/// @note
	/// If contained value has already been constructed and alive, it is destructed first, however, memory is recycled and the new
	/// object will share the old pointer.
	///
	/// @warning
	/// If constructor of the new object throws, the old contained value is destructed and is unrecoverable. The Optional is in a
	/// cleared state.
	///
	/// @tparam ...ConstructorArgumentTypePack
	/// Types of arguments sent to the contained value constructor.
	///
	/// @param ...constructor_arguments
	/// Arguments passed to the contained value constructor.
	template<typename ...ConstructorArgumentTypePack>
	constexpr void Emplace( ConstructorArgumentTypePack&& ...constructor_arguments )
	{
		this->Clear();
		this->ConstructStackElement( this->data, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Get() const -> const ValueType&
	{
		BAssert( this->has_data, U"Container is empty" );
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Get() -> ValueType&
	{
		BAssert( this->has_data, U"Container is empty" );
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Clear()
	{
		if( !this->has_data ) return;
		this->DestructStackElement( this->data );
		this->has_data = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto IsEmpty() const noexcept -> bool
	{
		return !this->has_data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit operator bool() const noexcept
	{
		return !IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CopyOther( const ThisType& other ) noexcept
		requires( std::is_copy_assignable_v<ValueType> )
	{
		if( std::addressof( other ) == this ) return;
		this->Clear();
		this->data = other.data;
		this->has_data = other.has_data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SwapOther( ThisType&& other ) noexcept
		requires( std::is_move_assignable_v<ValueType> )
	{
		if( std::addressof( other ) == this ) return;
		std::swap( this->has_data, other.has_data );
		std::swap( this->data, other.data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	union
	{
		NonTrivialType	dummy_value			= {};
		ValueType		data;
	};
	bool				has_data			= false;
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( Optional<u8> ) == 2 );
static_assert( sizeof( Optional<u16> ) == 4 );
static_assert( sizeof( Optional<u32> ) == 8 );
static_assert( sizeof( Optional<u64> ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( !utility::ContainerView<Optional<u32>> );
static_assert( !utility::ContainerEditableView<Optional<u32>> );
static_assert( !utility::Container<Optional<u32>> );

static_assert( !utility::LinearContainerView<Optional<u32>> );
static_assert( !utility::LinearContainerEditableView<Optional<u32>> );
static_assert( !utility::LinearContainer<Optional<u32>> );

static_assert( !utility::TextContainerView<Optional<c32>> );
static_assert( !utility::TextContainerEditableView<Optional<c32>> );
static_assert( !utility::TextContainer<Optional<c32>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make optional container and construct its contained value.
///
/// Functionally this is very similar to std::optional, except it uses engine provided assertions.
///
/// @tparam ValueType
/// Contained value type of the optional.
///
/// @tparam ...ConstructorArgumentTypePack
/// Parameter types forwarded to contained value constructor.
///
/// @param ...constructor_arguments
/// Arguments forwarded to contained value constructor.
///
/// @return
/// A new optional container with its contained value constructed.
template<
	typename ValueType,
	typename ...ConstructorArgumentTypePack
>
auto MakeOptional( ConstructorArgumentTypePack&& ...constructor_arguments ) -> Optional<ValueType>
{
	auto result = Optional<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



} // bc
