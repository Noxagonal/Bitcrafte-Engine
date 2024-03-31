
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
/// Container which may contains a single heap allocated object. Similar to std::unique_ptr.
///
/// @note
/// This container can handle objects which constructor may throw. If there's a chance that the object constructor may throw,
/// it should be wrapped into a UniquePtr or Optional.
///
/// @tparam ValueType
/// Type of the contained object/element.
template<typename ValueType>
class BC_CONTAINER_NAME( UniquePtr ) : private container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( UniquePtr )<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( UniquePtr )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	friend class BC_CONTAINER_NAME( UniquePtr );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )( ) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )( nullptr_t ) noexcept :
		data_ptr( nullptr )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )(
		const BC_CONTAINER_NAME( UniquePtr )														&	other
	) noexcept = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )(
		BC_CONTAINER_NAME( UniquePtr )																&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	constexpr BC_CONTAINER_NAME( UniquePtr )(
		BC_CONTAINER_NAME( UniquePtr )<OtherValueType>												&&	other
	) noexcept requires( std::is_base_of_v<ValueType, OtherValueType> && !std::is_same_v<OtherValueType, ValueType> )
	{
		this->data_ptr = other.data_ptr;
		other.data_ptr = nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit BC_CONTAINER_NAME( UniquePtr )(
		ValueType																					*	claim_pointer
	) noexcept
	{
		this->data_ptr = claim_pointer;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( UniquePtr )() BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		const BC_CONTAINER_NAME( UniquePtr )														&	other
	) noexcept = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		nullptr_t
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		ValueType																					*	claim_pointer
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		this->data_ptr = claim_pointer;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		BC_CONTAINER_NAME( UniquePtr )																&&	other
	) noexcept
	{
		if( this == std::addressof( other ) ) return *this;
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType>
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		BC_CONTAINER_NAME( UniquePtr )<OtherValueType>												&&	other
	) noexcept requires( std::is_base_of_v<ValueType, OtherValueType> && !std::is_same_v<OtherValueType, ValueType> )
	{
		this->Clear();
		this->data_ptr = other.data_ptr;
		other.data_ptr = nullptr;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	operator->() noexcept
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	operator->() const noexcept
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	operator*() noexcept
	{
		BC_ContainerAssert( this->Get(), U"Container is empty" );
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	operator*() const noexcept
	{
		BC_ContainerAssert( this->Get(), U"Container is empty" );
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
	constexpr void																						Emplace(
		ConstructorArgumentTypePack																	&&	...constructor_arguments
	)
	{
		if( this->data_ptr ) this->TryDestructHeapElement( this->data_ptr );
		if( this->data_ptr == nullptr ) this->AllocateContainedMemory();
		this->TryConstructHeapElement( this->data_ptr, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	Get() noexcept
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	Get() const noexcept
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
	/// Up-cast, down-cast and same type cast are supported. It not necessary to use this function to up-cast as it is implicit.
	///
	/// @note
	/// The contained pointer must be derived from ValueType.
	///
	/// @note
	/// If cast fails, exception will be thrown in debug builds. In release builds, an empty UniquePtr will be returned and current
	/// UniquePtr will retain ownership.
	/// 
	/// @tparam DerivedValueType
	/// Type to downcast to.
	/// 
	/// @return
	/// Downcasted unique pointer.
	template<typename CastToValueType>
	constexpr BC_CONTAINER_NAME( UniquePtr )<CastToValueType>											CastTo()
	{
		static_assert(
			std::is_base_of_v<ValueType, CastToValueType> || std::is_base_of_v<CastToValueType, ValueType>,
			"Cannot cast, Types have no correlation"
		);

		if constexpr( std::is_base_of_v<CastToValueType, ValueType> )
		{
			// No cast or upcast
			return BC_CONTAINER_NAME( UniquePtr )( std::move( *this ) );
		}
		else if constexpr( std::is_base_of_v<ValueType, CastToValueType> )
		{
			// Downcast
			auto casted_ptr = dynamic_cast<CastToValueType*>( this->data_ptr );
			BC_ContainerAssert( casted_ptr, U"Failed to downcast" );
			if( casted_ptr == nullptr ) return {};
			auto downcasted_unique_ptr = BC_CONTAINER_NAME( UniquePtr )<CastToValueType>();
			downcasted_unique_ptr.data_ptr = casted_ptr;
			this->data_ptr = nullptr;
			return downcasted_unique_ptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		if( this->data_ptr == nullptr ) return;
		this->TryDestructHeapElement( this->data_ptr );
		this->FreeContainedMemory();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						IsEmpty() const noexcept
	{
		return !Get();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						AllocateContainedMemory() noexcept
	{
		assert( this->data_ptr == nullptr && "This function should not be called on an occupied container" );
		this->data_ptr = memory::AllocateMemory<ValueType>( 1, alignof( ValueType ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						FreeContainedMemory() noexcept
	{
		assert( this->data_ptr && "This function should not be called on an empty container" );
		memory::FreeMemory( this->data_ptr, 1 );
		this->data_ptr = nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								SwapOther(
		BC_CONTAINER_NAME( UniquePtr )																&&	other
	) noexcept
	{
		std::swap( this->data_ptr, other.data_ptr );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																						*	data_ptr			= nullptr;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( UniquePtr )<u32> ) == 8 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( !utility::ContainerView<BC_CONTAINER_NAME( UniquePtr )<u32>> );
static_assert( !utility::ContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<u32>> );
static_assert( !utility::Container<BC_CONTAINER_NAME( UniquePtr )<u32>> );

static_assert( !utility::LinearContainerView<BC_CONTAINER_NAME( UniquePtr )<u32>> );
static_assert( !utility::LinearContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<u32>> );
static_assert( !utility::LinearContainer<BC_CONTAINER_NAME( UniquePtr )<u32>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
