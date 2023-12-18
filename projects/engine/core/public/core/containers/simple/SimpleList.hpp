#pragma once

#define BI_CONTAINER_IMPLEMENTATION_SIMPLE 1
#include "../backend/ListImpl.hpp"
#undef BI_CONTAINER_IMPLEMENTATION_SIMPLE

/*

#include <core/containers/backend/ContainerBase.hpp>

#include <type_traits>
#include <initializer_list>



// TODO: Implement proper iterators for simple containers.



namespace bc {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept SimpleListAllowedValueType=
( !std::is_default_constructible_v<ValueType>		|| std::is_nothrow_default_constructible_v<ValueType> ) &&
( !std::is_destructible_v<ValueType>				|| std::is_nothrow_destructible_v<ValueType> ) &&
( !std::is_copy_constructible_v<ValueType>			|| std::is_nothrow_copy_constructible_v<ValueType> ) &&
( !std::is_move_constructible_v<ValueType>			|| std::is_nothrow_move_constructible_v<ValueType> ) &&
( !std::is_swappable_v<ValueType>					|| std::is_nothrow_swappable_v<ValueType> );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SimpleListAllowedValueType ValueType>
class SimpleList;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	SimpleListAllowedValueType		ValueType,
	bool										IsConst
>
class SimpleListView
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base									= void;
	using ContainedValueType					= ValueType;
	static constexpr bool IsDataConst			= IsConst;

	template<SimpleListAllowedValueType OtherValueType, bool IsOtherConst>
	using ThisContainerType						= SimpleListView<OtherValueType, IsOtherConst>;
	using ThisType								= ThisContainerType<ValueType, IsConst>;

	template<SimpleListAllowedValueType OtherValueType, bool IsOtherConst>
	using ThisContainerViewType					= SimpleListView<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType							= ThisContainerViewType<ValueType, IsOtherConst>;

	template<SimpleListAllowedValueType OtherValueType>
	using ThisContainerFullType					= SimpleList<OtherValueType>;
	using ThisFullType							= ThisContainerFullType<ValueType>;

	template<bool IsOtherConst>
	using IteratorBase							= std::conditional_t<IsOtherConst, const ValueType*, ValueType*>;
	using ConstIterator							= IteratorBase<true>;
	using Iterator								= IteratorBase<false>;

	using value_type							= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cannot friend non-class types. TODO: enable this if we ever implement real iterators for this class.
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<SimpleListAllowedValueType OtherValueType, bool IsOtherConst>
	friend class SimpleListView;

	template<SimpleListAllowedValueType OtherValueType>
	friend class SimpleList;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SimpleListView() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleListView(
		SimpleListView<ValueType, IsOtherConst>			other
	) noexcept requires( bc::container_bases::IsConstConvertible<IsConst, IsOtherConst> )
	{
		this->data_ptr			= other.data_ptr;
		this->data_size			= other.data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleListView(
		const ValueType												*	from_data,
		size_t															from_size
	) noexcept requires( IsConst == true )
	{
		this->data_ptr			= const_cast<ValueType*>( from_data );
		this->data_size			= from_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleListView(
		ValueType													*	from_data,
		size_t															from_size
	) noexcept requires( IsConst == false )
	{
		this->data_ptr			= from_data;
		this->data_size			= from_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType										&	operator[](
		size_t															index
	) const noexcept
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType												&	operator[](
		size_t															index
	) noexcept requires( IsConst == false )
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator==(
		SimpleListView<ValueType, IsOtherConst>			other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator!=(
		SimpleListView<ValueType, IsOtherConst>			other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType										&	At(
		size_t															index
	) const noexcept
	{
		assert( index < this->data_size );
		return *std::addressof( this->data_ptr[ index ] );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType												&	At(
		size_t															index
	) noexcept requires( IsConst == false )
	{
		assert( index < this->data_size );
		return *std::addressof( this->data_ptr[ index ] );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType										&	Front() const noexcept
	{
		assert( !this->IsEmpty() );
		return this->data_ptr[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType												&	Front() noexcept requires( IsConst == false )
	{
		assert( !this->IsEmpty() );
		return this->data_ptr[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType										&	Back() const noexcept
	{
		assert( !this->IsEmpty() );
		return this->data_ptr[ this->data_size - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType												&	Back() noexcept requires( IsConst == false )
	{
		assert( !this->IsEmpty() );
		return this->data_ptr[ this->data_size - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType										*	Data() const noexcept
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType												*	Data() noexcept requires( IsConst == false )
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr size_t													Size() const noexcept
	{
		return this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr bool														IsEmpty() const noexcept
	{
		return !this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory to the first value.
	[[nodiscard]]
	constexpr IteratorBase<IsConst>										begin()
	{
		return IteratorBase<IsConst> { &data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory to the first value.
	[[nodiscard]]
	constexpr ConstIterator												begin() const
	{
		return ConstIterator { &data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory to the first value.
	[[nodiscard]]
	constexpr ConstIterator												cbegin() const
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory one over the last value.
	[[nodiscard]]
	constexpr IteratorBase<IsConst>										end()
	{
		return IteratorBase<IsConst> { &data_ptr[ data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory one over the last value.
	[[nodiscard]]
	constexpr ConstIterator												end() const
	{
		return ConstIterator { &data_ptr[ data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Pointer in memory one over the last value.
	[[nodiscard]]
	constexpr ConstIterator												cend() const
	{
		return this->end();
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType														*	data_ptr				= nullptr;
	size_t																data_size				= 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// A very simple linear container similar to bc::List.
///
///	This is meant for internal use only and only meant to be used when bc::List cannot be used.
///
/// @warning
/// This container will never throw and forbids it's values from throwing. Any errors will instead crash the application.
/// Error checking is only enabled on debug builds, not on development builds, correct use is expected.
///
/// @tparam ValueType
///	value type this container handles.
template<SimpleListAllowedValueType ValueType>
class SimpleList :
	public SimpleListView<ValueType, false>,
	public bc::container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base									= SimpleListView<ValueType, false>;
	using ContainedValueType					= ValueType;
	static constexpr bool IsDataConst			= false;

	template<SimpleListAllowedValueType OtherValueType>
	using ThisContainerType						= SimpleList<OtherValueType>;
	using ThisType								= ThisContainerType<ValueType>;

	template<SimpleListAllowedValueType OtherValueType, bool IsOtherConst>
	using ThisContainerViewType					= SimpleListView<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType							= ThisContainerViewType<ValueType, IsOtherConst>;

	template<SimpleListAllowedValueType OtherValueType>
	using ThisContainerFullType					= SimpleList<OtherValueType>;
	using ThisFullType							= ThisContainerFullType<ValueType>;

	template<bool IsOtherConst>
	using IteratorBase							= std::conditional_t<IsOtherConst, const ValueType*, ValueType*>;
	using ConstIterator							= IteratorBase<true>;
	using Iterator								= IteratorBase<false>;

	using value_type							= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cannot friend non-class types. TODO: enable this if we ever implement real iterators for this class.
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<
		SimpleListAllowedValueType	OtherValueType,
		bool						IsOtherConst
	>
	friend class SimpleListView;

	template<SimpleListAllowedValueType OtherValueType>
	friend class SimpleList;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList(
		const SimpleList											&	other
	) noexcept requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList(
		SimpleList													&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleList(
		SimpleListView<ValueType, IsOtherConst>							other
	) noexcept requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList(
		const std::initializer_list<ValueType>						&	from_initializer_list
	) noexcept requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		this->Append( from_initializer_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList(
		size_t															initial_size
	) noexcept
	{
		this->Resize( initial_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~SimpleList() noexcept
	{
		this->DestructRange( this->data_ptr, this->data_size );
		this->FreeMemory( this->data_ptr, this->data_capacity );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList											&	operator=(
		const SimpleList											&	other
	) noexcept requires( std::is_nothrow_copy_assignable_v<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleList											&	operator=(
		SimpleList													&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleList											&	operator=(
		SimpleListView<ValueType, IsOtherConst>							other
	) noexcept requires( std::is_nothrow_copy_assignable_v<ValueType> )
	{
		if( other.Data() >= this->Data() && other.Data() < this->Data() + this->Size() )
		{
			// Other data is a part of this container, we'll need to do a copy first.
			auto other_copy = SimpleList<ValueType> { other };
			*this = std::move( other_copy );
			return *this;
		}

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other simple list to self by pushing it's values to the back of this list.
	/// 
	/// @param other
	///	Other simple list to append values from.
	/// 
	/// @return
	/// Reference to this.
	constexpr SimpleList											&	operator+=(
		const SimpleList											&	other
	) requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + other.Size();
		this->Append( other, 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other list to self by pushing it's values to the back of this list.
	/// 
	/// @param other
	///	Other list to append values from.
	/// 
	/// @return
	/// Reference to this.
	template<bool IsOtherConst>
	constexpr SimpleList											&	operator+=(
		SimpleListView<ValueType, IsOtherConst>							other
		) requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + other.Size();
		this->Append( other, 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append values from initializer list to end of this container.
	/// 
	/// @param init_list
	///	Initializer list to append values from.
	/// 
	/// @return
	/// Reference to this.
	constexpr SimpleList											&	operator+=(
		const std::initializer_list<ValueType>						&	init_list
	) requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + init_list.size();
		this->Append( init_list, 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator==(
		SimpleListView<ValueType, IsOtherConst> other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator!=(
		SimpleListView<ValueType, IsOtherConst> other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool														operator==(
		const SimpleList											&	other
	) const noexcept
	{
		if( std::addressof( other ) == this ) return *this;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool														operator!=(
		const SimpleList											&	other
	) const noexcept
	{
		if( std::addressof( other ) == this ) return *this;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Reserves memory for number of values.
	///
	/// @param new_capacity
	///	New minimum capacity.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void														Reserve(
		size_t															new_capacity,
		size_t															headroom			= 0
	) noexcept
	{
		if( this->data_capacity < new_capacity )
		{
			new_capacity += headroom;
			auto new_ptr = this->ResizeRange( this->data_ptr, this->data_capacity, new_capacity );
			if( !new_ptr ) return;
			this->data_ptr		= new_ptr;
			this->data_capacity	= new_capacity;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Resizes the container to a new size.
	///
	/// @param new_size
	///	New size of the container.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void														Resize(
		size_t															new_size,
		size_t															headroom		= 0
	) noexcept
	{
		auto old_size = this->data_size;
		this->ResizeNoConstruct( new_size, headroom );
		if( old_size < new_size )
		{
			this->ConstructRange( this->data_ptr + old_size, new_size - old_size );
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Appends another container at the end of this container.
	///
	/// @tparam OtherContainerType
	/// Type of another container, container that is being appended does not need to be the same type as this.
	///
	/// @param other
	/// Const reference to another container from which to copy values from.
	///
	/// @param count
	/// How many times the other container is appended into this container. For example if the appended container contains numbers
	/// [0123] and we append it twice, [01230123] is the final result that gets appended.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	template<container_bases::ContainerView OtherContainerType>
	constexpr void														Append(
		const OtherContainerType									&	other,
		size_t															count				= 1,
		size_t															headroom			= 0
	) requires( std::is_nothrow_copy_constructible_v<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		size_t old_size				= this->Size();
		size_t other_size			= other.Size();
		size_t total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( size_t c = 0; c < count; ++c )
		{
			auto other_it			= other.begin();
			auto write_location		= other_size * c + old_size;
			for( size_t i = 0; i < other_size; ++i )
			{
				new( &this->data_ptr[ write_location + i ] ) ValueType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Appends initializer list at the end of this container.
	///
	/// @param init_list
	/// Initializer list from which to copy values from.
	///
	/// @param count
	/// How many times the other container is appended into this container. For example if the appended container contains numbers
	/// [0123] and we append it twice, [01230123] is the final result that gets appended.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	void																Append(
		const std::initializer_list<ValueType>						&	init_list,
		size_t															count				= 1,
		size_t															headroom			= 0
	) requires( std::is_copy_constructible_v<ValueType> )
	{
		size_t old_size				= this->Size();
		size_t other_size			= init_list.size();
		size_t total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( size_t c = 0; c < count; ++c )
		{
			auto other_it			= init_list.begin();
			auto write_location		= other_size * c + old_size;
			for( size_t i = 0; i < other_size; ++i )
			{
				new( &this->data_ptr[ write_location + i ] ) ValueType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Removes and destroys all values from the container.
	///
	/// @note
	/// Does not change capacity.
	constexpr void														Clear() noexcept
	{
		this->DestructRange( this->data_ptr, this->data_size );
		this->data_size		= 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container.
	///
	/// @param value
	/// value to add to the back.
	constexpr void														PushBack(
		const ValueType												&	value
	) noexcept requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, reserve_space );
		new( &this->data_ptr[ this->data_size - 1 ] ) ValueType( value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container.
	///
	/// @param value
	/// value to add to the back via move.
	constexpr void														PushBack(
		ValueType													&&	value
	) noexcept requires( std::is_nothrow_copy_constructible_v<ValueType> || std::is_nothrow_move_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, reserve_space );
		// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
		if constexpr( std::is_move_constructible_v<ValueType> )
		{
			new( &this->data_ptr[ this->data_size - 1 ] ) ValueType( std::move( value ) );
		}
		else
		{
			new( &this->data_ptr[ this->data_size - 1 ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container a specific number of times.
	/// 
	/// @param value
	/// value to add to the back.
	///
	/// @param count
	/// How many times the value is inserted at the back.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void														FillBack(
		const ValueType												&	value,
		size_t															count,
		size_t															headroom			= 0
	) requires( std::is_nothrow_copy_constructible_v<ValueType> )
	{
		auto old_size = this->data_size;
		auto reserve_space = this->data_size + count;
		this->ResizeNoConstruct( reserve_space, headroom );
		for( size_t i = 0; i < count; i++ )
		{
			new( &this->data_ptr[ old_size + i ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct value to the back of this container.
	///
	/// @tparam ConstructorArgumentsTypePack
	/// Parameter types we want to call the value constructor with.
	/// 
	/// @param ...constructor_args
	///	Parameters sent to the constructor of the value.
	template<typename ...ConstructorArgumentsTypePack>
	constexpr void														EmplaceBack(
		ConstructorArgumentsTypePack								&&	...constructor_args
	) noexcept requires( std::is_nothrow_constructible_v<ValueType, ConstructorArgumentsTypePack...> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, reserve_space );
		new( &this->data_ptr[ this->data_size - 1 ] ) ValueType( std::forward<ConstructorArgumentsTypePack>( constructor_args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Removes the last value.
	constexpr void														PopBack() noexcept
	{
		assert( this->data_size && "Cannot pop back, container is empty" );
		this->ResizeNoConstruct( this->data_size - 1, 0 );
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Resizes the container to its proper size, but does not construct anything.
	/// 
	/// @param new_size
	/// New size the container should be.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void														ResizeNoConstruct(
		size_t															new_size,
		size_t															headroom
	) noexcept
	{
		auto old_size = this->data_size;
		this->Reserve( new_size, headroom );
		if( old_size > new_size )
		{
			// Shrinking
			this->DestructRange( this->data_ptr + new_size, old_size - new_size );
		}
		this->data_size = new_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void														SwapOther(
		SimpleList									&&	other
	) noexcept
	{
		if( this != std::addressof( other ) )
		{
			std::swap( this->data_ptr, other.data_ptr );
			std::swap( this->data_size, other.data_size );
			std::swap( this->data_capacity, other.data_capacity );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t																data_capacity					= 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( SimpleList<uint32_t> ) == 24 );
static_assert( sizeof( SimpleListView<uint32_t, true> ) == 16 );
static_assert( sizeof( SimpleListView<uint32_t, false> ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check that all basic data types are allowed as contained types
static_assert( SimpleListAllowedValueType<void> );
static_assert( SimpleListAllowedValueType<bool> );
static_assert( SimpleListAllowedValueType<int8_t> );
static_assert( SimpleListAllowedValueType<int16_t> );
static_assert( SimpleListAllowedValueType<int32_t> );
static_assert( SimpleListAllowedValueType<int64_t> );
static_assert( SimpleListAllowedValueType<uint8_t> );
static_assert( SimpleListAllowedValueType<uint16_t> );
static_assert( SimpleListAllowedValueType<uint32_t> );
static_assert( SimpleListAllowedValueType<uint64_t> );
static_assert( SimpleListAllowedValueType<size_t> );
static_assert( SimpleListAllowedValueType<float> );
static_assert( SimpleListAllowedValueType<double> );
static_assert( SimpleListAllowedValueType<char> );
static_assert( SimpleListAllowedValueType<wchar_t> );
static_assert( SimpleListAllowedValueType<char8_t> );
static_assert( SimpleListAllowedValueType<char16_t> );
static_assert( SimpleListAllowedValueType<char32_t> );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check that simple linear container can be used as a template parameter in itself.
static_assert( SimpleListAllowedValueType<SimpleList<uint32_t>> );
static_assert( SimpleListAllowedValueType<SimpleListView<uint32_t, true>> );
static_assert( SimpleListAllowedValueType<SimpleListView<uint32_t, false>> );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( container_bases::ContainerView<SimpleList<uint32_t>> );
static_assert( container_bases::ContainerEditableView<SimpleList<uint32_t>> );
static_assert( container_bases::Container<SimpleList<uint32_t>> );

static_assert( container_bases::ContainerView<SimpleListView<uint32_t, false>> );
static_assert( container_bases::ContainerEditableView<SimpleListView<uint32_t, false>> );
static_assert( !container_bases::Container<SimpleListView<uint32_t, false>> );

static_assert( container_bases::ContainerView<SimpleListView<uint32_t, true>> );
static_assert( !container_bases::ContainerEditableView<SimpleListView<uint32_t, true>> );
static_assert( !container_bases::Container<SimpleListView<uint32_t, true>> );

static_assert( container_bases::LinearContainerView<SimpleList<uint32_t>> );
static_assert( container_bases::LinearContainerEditableView<SimpleList<uint32_t>> );
static_assert( container_bases::LinearContainer<SimpleList<uint32_t>> );

static_assert( container_bases::LinearContainerView<SimpleListView<uint32_t, false>> );
static_assert( container_bases::LinearContainerEditableView<SimpleListView<uint32_t, false>> );
static_assert( !container_bases::LinearContainer<SimpleListView<uint32_t, false>> );

static_assert( container_bases::LinearContainerView<SimpleListView<uint32_t, true>> );
static_assert( !container_bases::LinearContainerEditableView<SimpleListView<uint32_t, true>> );
static_assert( !container_bases::LinearContainer<SimpleListView<uint32_t, true>> );

static_assert( !container_bases::TextContainerView<SimpleList<char32_t>> );
static_assert( !container_bases::TextContainerEditableView<SimpleList<char32_t>> );
static_assert( !container_bases::TextContainer<SimpleList<char32_t>> );

static_assert( !container_bases::TextContainerView<SimpleListView<char32_t, false>> );
static_assert( !container_bases::TextContainerEditableView<SimpleListView<char32_t, false>> );
static_assert( !container_bases::TextContainer<SimpleListView<char32_t, false>> );

static_assert( !container_bases::TextContainerView<SimpleListView<char32_t, true>> );
static_assert( !container_bases::TextContainerEditableView<SimpleListView<char32_t, true>> );
static_assert( !container_bases::TextContainer<SimpleListView<char32_t, true>> );



} // internal
} // bc

*/
