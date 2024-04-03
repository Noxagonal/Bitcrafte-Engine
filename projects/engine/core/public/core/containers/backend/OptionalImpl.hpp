
#include <core/containers/backend/ContainerBase.hpp>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
BC_CONTAINER_NAMESPACE_START;



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
template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( Optional ) : private container_bases::ContainerResource
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
	using ThisContainerType					= BC_CONTAINER_NAME( Optional )<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Optional )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	friend class BC_CONTAINER_NAME( Optional );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )( ) noexcept :
		dummy_value( {} ),
		has_data( false )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit BC_CONTAINER_NAME( Optional )( nullptr_t ) noexcept :
		dummy_value( {} ),
		has_data( false )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )(
		const BC_CONTAINER_NAME( Optional )															&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> ) :
		dummy_value( {} ),
		has_data( false )
	{
		if( !other.has_data ) return;
		this->ConstructStackElement( this->data, other.data );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )(
		BC_CONTAINER_NAME( Optional )																&&	other
	) noexcept requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> ) :
		dummy_value( {} ),
		has_data( false )
	{
		if( !other.has_data ) return;
		this->ConstructStackElement( this->data, std::move( other.data ) );
		other.has_data = false;
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )(
		const ValueType																				&	other_value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> ) :
		dummy_value( {} ),
		has_data( false )
	{
		this->ConstructStackElement( this->data, other_value );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )(
		ValueType																					&&	other_value
	) noexcept requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> ) :
		dummy_value( {} ),
		has_data( false )
	{

		this->ConstructStackElement( this->data, std::move( other_value ) );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( Optional )() BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )															&	operator=(
		nullptr_t
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )															&	operator=(
		const BC_CONTAINER_NAME( Optional )															&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_ASSIGNABLE<ValueType> )
	{
		this->CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )															&	operator=(
		BC_CONTAINER_NAME( Optional )																&&	other
	) noexcept requires( BC_CONTAINER_IS_MOVE_ASSIGNABLE<ValueType> )
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )															&	operator=(
		const ValueType																				&	other_value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_ASSIGNABLE<ValueType> )
	{
		this->Emplace( other_value );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Optional )															&	operator=(
		ValueType																					&&	other_value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> && BC_CONTAINER_IS_MOVE_ASSIGNABLE<ValueType> )
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
	constexpr ValueType																				*	operator->() noexcept
	{
		return &this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	operator->() const noexcept
	{
		return &this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	operator*() noexcept
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	operator*() const noexcept
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
	constexpr void																						Emplace(
		ConstructorArgumentTypePack																	&&	...constructor_arguments
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		this->ConstructStackElement( this->data, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
		this->has_data = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	Get() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->has_data, U"Container is empty" );
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	Get() const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->has_data, U"Container is empty" );
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		if( !this->has_data ) return;
		this->DestructStackElement( this->data );
		this->has_data = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						IsEmpty() const noexcept
	{
		return !this->has_data;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								CopyOther(
		const BC_CONTAINER_NAME( Optional )															&	other
	) noexcept requires( BC_CONTAINER_IS_COPY_ASSIGNABLE<ValueType> )
	{
		if( std::addressof( other ) == this ) return;
		this->Clear();
		this->data = other.data;
		this->has_data = other.has_data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								SwapOther(
		BC_CONTAINER_NAME( Optional )																&&	other
	) noexcept requires( BC_CONTAINER_IS_MOVE_ASSIGNABLE<ValueType> )
	{
		if( std::addressof( other ) == this ) return;
		std::swap( this->has_data, other.has_data );
		std::swap( this->data, other.data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	union
	{
		NonTrivialType																					dummy_value			= {};
		ValueType																						data;
	};
	bool																								has_data			= false;
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Optional )<u8> ) == 2 );
static_assert( sizeof( BC_CONTAINER_NAME( Optional )<u16> ) == 4 );
static_assert( sizeof( BC_CONTAINER_NAME( Optional )<u32> ) == 8 );
static_assert( sizeof( BC_CONTAINER_NAME( Optional )<u64> ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( !utility::ContainerView<BC_CONTAINER_NAME( Optional )<u32>> );
static_assert( !utility::ContainerEditableView<BC_CONTAINER_NAME( Optional )<u32>> );
static_assert( !utility::Container<BC_CONTAINER_NAME( Optional )<u32>> );

static_assert( !utility::LinearContainerView<BC_CONTAINER_NAME( Optional )<u32>> );
static_assert( !utility::LinearContainerEditableView<BC_CONTAINER_NAME( Optional )<u32>> );
static_assert( !utility::LinearContainer<BC_CONTAINER_NAME( Optional )<u32>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( Optional )<c32>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( Optional )<c32>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( Optional )<c32>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
