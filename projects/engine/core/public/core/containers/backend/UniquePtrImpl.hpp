
#include "ContainerBase.hpp"

#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include <memory>
#include <new>

#include "ContainerImplAddDefinitions.hpp"



namespace bc {



template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( UniquePtr );

template<BC_CONTAINER_VALUE_TYPENAME ValueType, typename ...ConstructorArgumentTypePack>
UniquePtr<ValueType>																					MakeUnique(
	ConstructorArgumentTypePack																		&&	...constructor_arguments
);


template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( UniquePtr ) :
	public container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( UniquePtr )<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( UniquePtr )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( UniquePtr );

	template<BC_CONTAINER_VALUE_TYPENAME ValueType, typename ...ConstructorArgumentTypePack>
	friend UniquePtr<ValueType>																			MakeUnique(
		ConstructorArgumentTypePack																	&&	...constructor_arguments
	);

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
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		ValueType																					*	claim_pointer
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		this->data_ptr = claim_pointer;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( UniquePtr )														&	operator=(
		BC_CONTAINER_NAME( UniquePtr )																&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
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
		if( this->data_ptr ) this->DestructContained();
		if( this->data_ptr == nullptr ) this->AllocateContainedMemory();
		this->ConstructContained( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
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
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		if( this->data_ptr == nullptr ) return;
		this->DestructContained();
		this->FreeContainedMemory();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						AllocateContainedMemory() noexcept
	{
		assert( this->data_ptr == nullptr && "This function should not be called on an occupied container" );
		this->data_ptr = this->AllocateMemory<ValueType>( 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						FreeContainedMemory() noexcept
	{
		assert( this->data_ptr && "This function should not be called on an empty container" );
		this->FreeMemory<ValueType>( this->data_ptr, 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ...ConstructorArgumentTypePack>
	constexpr void																						ConstructContained(
		ConstructorArgumentTypePack																	&&	...constructor_arguments
	) BC_CONTAINER_NOEXCEPT
	{
		assert( this->data_ptr && "This function should not be called on an empty container" );
		this->ConstructRange( this->data_ptr, 1, std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						DestructContained() BC_CONTAINER_NOEXCEPT
	{
		assert( this->data_ptr && "This function should not be called on an empty container" );
		this->DestructRange( this->data_ptr, 1 );
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



template<BC_CONTAINER_VALUE_TYPENAME ValueType, typename ...ConstructorArgumentTypePack>
UniquePtr<ValueType>																					MakeUnique(
	ConstructorArgumentTypePack																		&&	...constructor_arguments
)
{
	auto result = UniquePtr<ValueType> {};
	result.Emplace( std::forward<ConstructorArgumentTypePack>( constructor_arguments )... );
	return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( UniquePtr )<uint32_t> ) == 8 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( !container_bases::ContainerView<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );
static_assert( !container_bases::ContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );
static_assert( !container_bases::Container<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );

static_assert( !container_bases::LinearContainerView<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );
static_assert( !container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( UniquePtr )<uint32_t>> );

static_assert( !container_bases::TextContainerView<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );
static_assert( !container_bases::TextContainerEditableView<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( UniquePtr )<char32_t>> );



} // bc



#include "ContainerImplRemoveDefinitions.hpp"
