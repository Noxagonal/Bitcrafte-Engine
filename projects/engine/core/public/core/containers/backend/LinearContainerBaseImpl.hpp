
#include <core/containers/backend/ContainerBase.hpp>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/diagnostic/assertion/Assert.hpp>

#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/diagnostic/assertion/HardAssert.hpp>

#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
namespace container_bases {



template<BC_CONTAINER_VALUE_TYPENAME ValueType, bool IsConst>
class BC_CONTAINER_NAME( LinearContainerViewBase );

template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( LinearContainerBase );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ContainerType, bool IsConst>
class BC_CONTAINER_NAME( LinearContainerIteratorBase )
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using ThisType				= BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>;
	using ContainedValueType	= typename ContainerType::ContainedValueType;
	using IteratorContainerType	= ContainerType;
	using DifferenceType		= std::ptrdiff_t;
	using Pointer				= std::conditional_t<IsConst, const ContainedValueType*, ContainedValueType*>;
	using Reference				= std::conditional_t<IsConst, const ContainedValueType&, ContainedValueType&>;

	// For stl compatibility.
	using iterator_category		= std::random_access_iterator_tag;
	using difference_type		= DifferenceType;
	using value_type			= ContainedValueType;
	using pointer				= Pointer;
	using reference				= Reference;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherContainerType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerIteratorBase );

	friend class BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, true>;
	friend class BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, false>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const ContainerType																				*	container		= nullptr;
	Pointer																								data			= nullptr;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct an iterator from LinearContainerIteratorBase type.
	///
	/// @param other
	/// Other iterator.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) noexcept requires( utility::IsConstConvertible<IsConst, IsOtherConst> ) :
		container( other.container ),
		data( other.data )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )(
		const ContainerType																			*	container,
		Pointer																							data
	) noexcept :
		container( container ),
		data( data )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Copy iterator from LinearContainerIteratorBase type.
	///
	/// @return
	/// Reference to this.
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )										&	operator=(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) noexcept requires( utility::IsConstConvertible<IsConst, IsOtherConst> )
	{
		this->container = other.container;
		this->data = other.data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Reference																					operator*() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const Reference																			operator*() const BC_CONTAINER_NOEXCEPT
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Pointer																					operator->() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const Pointer																				operator->() const BC_CONTAINER_NOEXCEPT
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return this->data == other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return this->data != other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator++() BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( 1, true );
		++this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator++(int) BC_CONTAINER_NOEXCEPT
	{
		BC_CONTAINER_NAME( LinearContainerIteratorBase ) tmp = *this;
		++( *this );
		return tmp;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator--() BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( -1, true );
		--this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator--(int) BC_CONTAINER_NOEXCEPT
	{
		BC_CONTAINER_NAME( LinearContainerIteratorBase ) tmp = *this;
		--( *this );
		return tmp;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>					operator+(
		DifferenceType																					value
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst> ret = *this;
		ret += value;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>					operator-(
		DifferenceType																					value
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst> ret = *this;
		ret -= value;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr DifferenceType																			operator-(
		BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>					iterator
	) const noexcept
	{
		return this->data - iterator.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator+=(
		DifferenceType																					value
	) BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( value, true );
		this->data += value;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsConst>				&	operator-=(
		DifferenceType																					value
	) BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( -value, true );
		this->data -= value;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Reference																					operator[](
		DifferenceType																					value
	) const BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( value );
		return *(this->data + value);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator<(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return data < other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator>(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return data > other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator<=(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return data <= other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator>=(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ContainerType, IsOtherConst>			&	other
	) const noexcept
	{
		return data >= other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const Pointer																				Get() const BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( 0, false );
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Pointer																					GetAddress() noexcept
	{
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ContainerType																	*	GetContainer() const noexcept
	{
		return this->container;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ContainerType																			*	GetContainer() noexcept requires( IsConst == false )
	{
		return this->container;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr DifferenceType																			GetIndex() const BC_CONTAINER_NOEXCEPT
	{
		CheckContainer();
		CheckBounds( 0, true );
		return GetIndex_NoCheck();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						IsValid() const noexcept
	{
		return
			( this->container != nullptr ) &&
			( this->data >= this->container->Data() ) &&
			( this->data <= this->container->Data() + this->container->Size() );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit operator bool() noexcept
	{
		if( this->container == nullptr ) return false;
		if( this->data == nullptr ) return false;
		if( this->data < this->container->Data() ) return false;
		if( this->data >= this->container->Data() + this->container->Size() ) return false;
		return true;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr DifferenceType																			GetIndex_NoCheck() const noexcept
	{
		return ( DifferenceType( this->data ) - DifferenceType( this->container->Data() ) ) / sizeof( ContainedValueType );
	}

	constexpr void																						CheckContainer() const
	{
		BC_ContainerAssert( this->container, U"Invalid use of iterator, container is nullptr" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						CheckBounds(
		DifferenceType																					offset,
		bool	 																						allow_one_past_last
	) const
	{
		BC_ContainerAssert( !this->container->IsEmpty(),
			U"Iterator is out of range, container is empty",
			U"Container size", this->container->Size(),
			U"Accessed container index", this->GetIndex_NoCheck() + offset
		);
		BC_ContainerAssert( this->data + offset >= this->container->Data(),
			U"Iterator out of range, lower than first value",
			U"Container size", this->container->Size(),
			U"Accessed container index", this->GetIndex_NoCheck() + offset
		);
		BC_ContainerAssert( this->data + offset < this->container->Data() + this->container->Size() + ( !!allow_one_past_last ),
			U"Iterator out of range, greater than last value",
			U"Container size", this->container->Size(),
			U"Accessed container index", this->GetIndex_NoCheck() + offset
		);
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This is a base for linear containers like List. Not meant to be used directly.
///
///	This specific container base can only access members, modifying the contained data is allowed but modifying the container is
/// not.
///
/// @tparam ValueType
///	Type of linear container value.
///
/// @tparam IsConst
///	Tells if the data should be kept read only. true if data should be read only or false if data is allowed to be modified.
template<BC_CONTAINER_VALUE_TYPENAME ValueType, bool IsConst>
class BC_CONTAINER_NAME( LinearContainerViewBase )
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= IsConst;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerType					= BC_CONTAINER_NAME( LinearContainerViewBase )<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<ValueType, IsConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( LinearContainerViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( LinearContainerBase )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	//template<bool IsOtherConst>
	//using IteratorBase						= BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsOtherConst>;
	//using ConstIterator						= IteratorBase<true>;
	//using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// template<bool IsOtherConst>
	// friend class IteratorBase;

	//friend ConstIterator;
	//friend Iterator;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( LinearContainerBase );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>					&	other
	) noexcept requires( IsDataConst == true )
		:
		data_ptr( other.data_ptr ),
		data_size( other.data_size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>					&	other
	) noexcept requires( utility::IsConstConvertible<IsDataConst, IsOtherConst> )
		:
		data_ptr( const_cast<ValueType*>( other.Data() ) ),
		data_size( other.Size() )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		const ValueType																				*	ptr,
		u64																								size
	) noexcept requires( IsDataConst == true )
		:
		data_ptr( const_cast<ValueType*>( ptr ) ),
		data_size( size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		ValueType																					*	ptr,
		u64																								size
	) noexcept requires( IsDataConst == false )
		:
		data_ptr( ptr ),
		data_size( size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType																		&	operator[](
		u64																								index
	) const BC_CONTAINER_NOEXCEPT
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType																				&	operator[](
		u64																								index
	) BC_CONTAINER_NOEXCEPT requires( IsDataConst == false )
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType																		&	At(
		u64																								index
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( index < this->data_size,
			U"Index out of range",
			U"Container size", this->data_size,
			U"Index", index
		);
		return data_ptr[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType																				&	At(
		u64																								index
	) BC_CONTAINER_NOEXCEPT requires( IsDataConst == false )
	{
		BC_ContainerAssert( index < this->data_size,
			U"Index out of range",
			U"Container size", this->data_size,
			U"Index", index
		);
		return this->data_ptr[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check each member to see if any match the parameter.
	///
	/// @note
	/// This is exactly the same as <tt>container.Find( <what> ) != container.end()</tt>
	/// 
	/// @param member
	///	Member or value we're looking for.
	/// 
	/// @return
	/// True if this container has member which equals what we're searching for, false if not found.
	constexpr bool																						HasMember(
		const ValueType																				&	member
	) const BC_CONTAINER_NOEXCEPT
	{
		return this->DoFind( member ) < this->data_ptr + this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this container is empty.
	/// 
	/// @return
	/// True if this container contains nothing, false otherwise.
	[[nodiscard]]
	constexpr bool																						IsEmpty() const BC_CONTAINER_NOEXCEPT
	{
		return !this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the first value of the container.
	/// 
	/// @return
	/// Reference to the first value.
	[[nodiscard]]
	constexpr const ValueType																		&	Front() const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot get container front value, container is empty.");
		return this->data_ptr[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the first value of the container.
	/// 
	/// @return
	/// Reference to the first value.
	[[nodiscard]]
	constexpr ValueType																				&	Front() BC_CONTAINER_NOEXCEPT requires( IsDataConst == false )
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot get container front value, container is empty." );
		return this->data_ptr[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the last value of the container.
	/// 
	/// @return
	/// Reference to the last value.
	[[nodiscard]]
	constexpr const ValueType																		&	Back() const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot get container back value, container is empty." );
		return this->data_ptr[ this->data_size - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the last value of the container.
	/// 
	/// @return
	/// Reference to the last value.
	[[nodiscard]]
	constexpr ValueType																				&	Back() BC_CONTAINER_NOEXCEPT requires( IsDataConst == false )
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot get container back value, container is empty." );
		return this->data_ptr[ this->data_size - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the amount of reserved space in this container.
	/// 
	/// @return
	/// Number of values space has been reserved for.
	[[nodiscard]]
	constexpr u64																						Size() const noexcept
	{
		return this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the container data directly.
	///
	/// @warning
	/// Do not use this function to get a C-style character string, it will not be null terminated.
	/// 
	/// @return
	/// Pointer to raw data.
	/// @note
	/// May not be nullptr even if container is empty.
	[[nodiscard]]
	constexpr ValueType																				*	Data() noexcept requires( IsDataConst == false )
	{
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the container data directly.
	///
	/// @warning
	/// Don't use this function to get a C-style character string, it will not be null terminated.
	/// 
	/// @return
	/// Pointer to raw data.
	/// @note
	/// May not be nullptr even if container is empty.
	[[nodiscard]]
	constexpr const ValueType																		*	Data() const noexcept
	{
		return this->data_ptr;
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																						*	data_ptr				= {};
	u64																									data_size				= {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This is a base for linear containers like List. Not meant to be used directly.
/// 
/// @tparam ValueType
///	Type of linear container values.
template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( LinearContainerBase ) :
	public BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, false>,
	protected ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, false>;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( LinearContainerBase )<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( LinearContainerViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( LinearContainerBase )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	//template<bool IsOtherConst>
	//using IteratorBase						= BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsOtherConst>;
	//using ConstIterator						= IteratorBase<true>;
	//using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	//friend ConstIterator;
	//friend Iterator;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( LinearContainerBase );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BC_CONTAINER_NAME( LinearContainerBase )() = default;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( LinearContainerBase )() BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		this->FreeMemory( this->data_ptr, this->data_capacity );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Reserves memory for number of values.
	/// 
	/// @param new_capacity
	/// New minimum capacity.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void																						Reserve(
		u64																								new_capacity,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT
	{
		if( this->data_capacity < new_capacity )
		{
			new_capacity += headroom;
			if( this->data_ptr == nullptr )
			{
				this->data_ptr		= AllocateMemory<ValueType>( new_capacity );
				this->data_capacity	= new_capacity;
				return;
			}
			auto new_ptr = this->ResizeRange( this->data_ptr, this->data_size, this->data_capacity, new_capacity );
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
	constexpr void																						Resize(
		u64																								new_size,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT
	{
		auto old_size = this->data_size;
		this->ResizeNoConstruct( new_size, headroom );
		if( old_size < new_size )
		{
			this->ConstructRange( this->data_ptr + old_size, new_size - old_size );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	template<utility::ContainerView OtherContainerType>
	constexpr void																						Append(
		const OtherContainerType																	&	other,
		u64																								count				= 1,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		u64 old_size			= this->Size();
		u64 other_size			= other.Size();
		u64 total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( u64 c = 0; c < count; ++c )
		{
			auto other_it			= other.begin();
			auto write_location		= other_size * c + old_size;
			for( u64 i = 0; i < other_size; ++i )
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
	constexpr void																						Append(
		const std::initializer_list<ValueType>														&	init_list,
		u64																								count				= 1,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		u64 old_size			= this->Size();
		u64 other_size			= init_list.size();
		u64 total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( u64 c = 0; c < count; ++c )
		{
			auto other_it			= init_list.begin();
			auto write_location		= other_size * c + old_size;
			for( u64 i = 0; i < other_size; ++i )
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
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		this->DestructRange( this->data_ptr, this->data_size );
		this->data_size = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the front of this container.
	///
	///	Shifts everything else towards the back by one index. This increments the indices of every value already in this
	/// container.
	/// 
	/// @param value
	/// Value to add to the front.
	constexpr void																						PushFront(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ShiftRight( 0, 1, reserve_space );
		new( &this->data_ptr[ 0 ] ) ValueType( value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the front of this container.
	///
	///	Shifts everything else towards the back by one index. This increments the indices of every value already in this
	/// container.
	///
	/// @param value
	/// Value to add to the front via move. Falls back to copying if more is not possible.
	constexpr void																						PushFront(
		ValueType																					&&	value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ShiftRight( 0, 1, reserve_space );
		// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
		if constexpr( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
		{
			new( &this->data_ptr[ 0 ] ) ValueType( std::move( value ) );
		}
		else
		{
			new( &this->data_ptr[ 0 ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container.
	/// 
	/// @param value
	/// Value to add to the back.
	constexpr void																						PushBack(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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
	/// Value to add to the back via move. Falls back to copying if more is not possible.
	constexpr void																						PushBack(
		ValueType																					&&	value
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, reserve_space );
		// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
		if constexpr( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
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
	/// Fills the front of this container with some value, shifts existing values towards the back.
	///
	///	This increments the indices of every value already in this container.
	/// 
	/// @param value
	/// Value to add to the front.
	///
	/// @param count
	/// How many times the value is inserted at the front.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void																						FillFront(
		const ValueType																				&	value,
		u64																								count,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		this->ShiftRight( 0, count, headroom );
		for( u64 i = 0; i < count; i++ )
		{
			new( &this->data_ptr[ i ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Fills the back of this container with some value, container is extended to accommodate the new capacity.
	/// 
	/// @param value
	/// Value to add to the back.
	///
	/// @param count
	/// How many times the value is inserted at the back.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void																						FillBack(
		const ValueType																				&	value,
		u64																								count,
		u64																								headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		auto old_size = this->data_size;
		auto reserve_space = this->data_size + count;
		this->ResizeNoConstruct( reserve_space, headroom );
		for( u64 i = 0; i < count; i++ )
		{
			new( &this->data_ptr[ old_size + i ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the front of this container by constructing it in place.
	///
	///	Shifts everything else towards the back by one index. This increments the indices of every value already in this
	/// container.
	///
	/// @tparam Constructor
	/// Argument types sent to the constructor of this object.
	/// 
	/// @param constructor_args
	///	Constructor arguments sent to the constructor of the value.
	template<typename ...ConstructorArgumentsTypePack>
	constexpr void																						EmplaceFront(
		ConstructorArgumentsTypePack																&&	...constructor_args
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ShiftRight( 0, 1, reserve_space );
		new( &this->data_ptr[ 0 ] ) ValueType( std::forward<ConstructorArgumentsTypePack>( constructor_args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container by constructing it in place.
	/// 
	/// @tparam Constructor
	/// Argument types sent to the constructor of this object.
	/// 
	/// @param constructor_args
	/// Constructor arguments sent to the constructor of the value.
	template<typename ...ConstructorArgumentsTypePack>
	constexpr void																						EmplaceBack(
		ConstructorArgumentsTypePack																&&	...constructor_args
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, reserve_space );
		new( &this->data_ptr[ this->data_size - 1 ] ) ValueType( std::forward<ConstructorArgumentsTypePack>( constructor_args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Removes the first value and shifts everything towards the front to fill the gap.
	///
	///	Decreases the index of all other values by 1.
	constexpr void																						PopFront() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot pop front, container is empty");
		this->ShiftLeft();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Removes the last value.
	constexpr void																						PopBack() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot pop back, container is empty");
		this->ResizeNoConstruct( this->data_size - 1, 0 );
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	DoErase(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot erase from container, container is already empty" );
		auto iterator = container_bases::internal_::DoLinearSearch<ValueType, false>( this->data_ptr, this->data_size, value );
		if( iterator >= this->data_ptr + this->data_size ) return iterator;
		return this->DoErase( iterator );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	DoErase(
		const ValueType																				*	at
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert(
			at >= this->data_ptr && at < this->data_ptr + this->data_size,
			U"Iterator out of range or it points to the end"
		);

		return this->DoErase( at, at + 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	DoErase(
		const ValueType																				*	from,
		const ValueType																				*	to
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert(
			from >= this->data_ptr && from < this->data_ptr + this->data_size,
			U"'from' iterator out of range or it points to the end"
		);
		BC_ContainerAssert(
			to >= this->data_ptr && to < this->data_ptr + this->data_size + 1,
			U"'to' iterator out of range"
		);

		u64 from_to_range		= to - from;
		u64 tail_range			= this->data_ptr + this->data_size - to;
		auto from_it			= &this->data_ptr[ from - this->data_ptr ];
		auto to_it				= &this->data_ptr[ to - this->data_ptr ];
		auto it_end				= this->data_ptr + this->data_size;
		auto it_last			= this->data_ptr + this->data_size - 1;
		while( to_it != it_end )
		{
			if constexpr( BC_CONTAINER_IS_MOVE_ASSIGNABLE<ValueType> )
			{
				*from_it = std::move( *( to_it ) );
			}
			else
			{
				*from_it = *( to_it );
			}
			++from_it;
			++to_it;
		}
		auto new_size = this->data_size - from_to_range;
		this->Resize( new_size );
		return it_end;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	DoInsert(
		const ValueType																				*	at,
		const ValueType																				&	value,
		u64																								count			= 1,
		u64																								headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		BC_ContainerAssert(
			( this->data_ptr == nullptr && at == nullptr ) ||
			( at >= this->data_ptr && at <= this->data_ptr + this->data_size ),
			U"Iterator out of range"
		);

		u64 at_index = at - this->data_ptr;
		this->ShiftRight( at_index, count, headroom );

		for( u64 i = 0; i < count; ++i )
		{
			new( &this->data_ptr[ at_index + i ] ) ValueType( value );
		}

		return &this->data_ptr[ at_index + count ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<utility::ContainerView OtherContainerType>
	constexpr ValueType																				*	DoInsert(
		const ValueType																				*	at,
		const OtherContainerType																	&	other,
		u64																								count			= 1,
		u64																								headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		BC_ContainerAssert(
			( this->data_ptr == nullptr && at == nullptr ) ||
			( at >= this->data_ptr && at <= this->data_ptr + this->data_size ),
			U"Iterator out of range"
		);

		auto CopyFunc = [ this, count, &at, headroom ](
			auto & other
		) -> ValueType*
			{
				u64 start_index			= at - this->data_ptr;
				u64 other_size			= other.Size();
				u64 total_insert_size	= other_size * count;

				this->ShiftRight( start_index, total_insert_size, headroom );

				for( u64 c = 0; c < count; ++c )
				{
					// Dirty SFINAE test to see if other container has Data() function.
					if constexpr( std::is_same_v<decltype( other.Data() ), decltype( other.Data() )> )
					{
						auto it = other.Data();
						for( u64 i = 0; i < other_size; ++i )
						{
							auto count_start_pos = c * other_size + start_index;
							new( &this->data_ptr[ count_start_pos + i ] ) ValueType( *it );
							++it;
						}
					}
					else
					{
						auto it = other.begin();
						for( u64 i = 0; i < other_size; ++i )
						{
							auto count_start_pos = c * other_size + start_index;
							new( &this->data_ptr[ count_start_pos + i ] ) ValueType( *it );
							++it;
						}
					}
				}
				return &this->data_ptr[ start_index + total_insert_size ];
			};

		if constexpr( utility::LinearContainerView<OtherContainerType> )
		{
			if( other.Data() >= this->data_ptr &&
				other.Data() < this->data_ptr + this->data_size )
			{
				// Other container data is either full or partial range from within this container, need to make a temporary.
				auto other_copy = BC_CONTAINER_NAME( LinearContainerBase )<typename OtherContainerType::ContainedValueType> {};
				other_copy.Reserve( other.Size() );
				this->CopyConstructRange( other_copy.Data(), this->data_ptr, other.Size() );
				return CopyFunc( other_copy );
			}
		}

		return CopyFunc( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Resizes the container to its proper size, but does not construct anything.
	/// 
	/// @param new_size
	/// New size the container should be.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void																						ResizeNoConstruct(
		u64																								new_size,
		u64																								headroom
	)
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
	constexpr void																						Swap(
		BC_CONTAINER_NAME( LinearContainerBase )													&	other
	) BC_CONTAINER_NOEXCEPT
	{
		std::swap( this->data_ptr, other.data_ptr );
		std::swap( this->data_size, other.data_size );
		std::swap( this->data_capacity, other.data_capacity );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Shifts everything right (towards end index), expands the container.
	///
	///	This function can also split the container in the middle allowing inserting values in the middle. values are
	/// copy-constructed, moved, copied and destructed as needed, move is preferred wherever possible.
	///
	///	Values are handled from back to front, first values are either move or copy constructed to empty memory locations, then
	/// rest are either move or copy assigned to previously occupied locations. Finally the unoccupied values locations at the
	/// front are destructed.
	///
	/// @param start_position
	///	Position where to start shifting from.
	///
	/// @param amount
	///	Number of positions to shift, the end is expanded as needed.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	constexpr void																						ShiftRight(
		u64																								start_position,
		u64																								amount,
		u64																								headroom
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		// TODO: Should combine shift and resize to the same copy-loop
		// if new memory is being allocated at the same time.
		// ResizeNoConstruct function may reallocate memory, and has to do a copy of the
		// values if it does allocate memory. It would be better to allocate new memory,
		// then instead of shifting values around in this function possibly the second
		// time, just move or copy construct the values into their new location in the
		// new memory. This needs tests to determine if a new memory "would be" allocated
		// before it actually is. ResizeNoConstruct could take in a boolean just telling it
		// if it should allocate or not, crash if it gets wrong info and let the caller
		// determine if new memory is needed or not. Or just let ResizeNoConstruct determine
		// the need for allocation the second time.

		auto old_size					= this->data_size;
		auto new_size					= this->data_size + amount;
		auto distance_to_end			= old_size - start_position;

		auto overlap_range_begin		= start_position + amount;
		auto overlap_range_end			= old_size;
		auto copy_range_begin			= ( distance_to_end < amount ) ? new_size - distance_to_end : old_size;
		auto copy_range_end				= new_size;

		auto destruct_begin				= start_position;
		auto destruct_size				= copy_range_end - copy_range_begin;

		this->ResizeNoConstruct( new_size, headroom );

		if( old_size > 0 )
		{
			// Construct the values assigned to a newly allocated memory from previous ones.
			for( u64 i = copy_range_end - 1; i >= copy_range_begin; --i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
				{
					new( &this->data_ptr[ i ] ) ValueType( std::move( this->data_ptr[ i - amount ] ) );
				}
				else
				{
					new( &this->data_ptr[ i ] ) ValueType( this->data_ptr[ i - amount ] );
				}
			}

			// For overlapping portion we can assign or move the values.
			for( u64 i = overlap_range_end - 1; i >= overlap_range_begin; --i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
				{
					this->data_ptr[ i ] = std::move( this->data_ptr[ i - amount ] );
				}
				else
				{
					this->data_ptr[ i ] = this->data_ptr[ i - amount ];
				}
			}

			// Destruct the previously occupied range.
			this->DestructRange(
				this->data_ptr + destruct_begin,
				destruct_size
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Shifts everything left (towards 0 index), destructs the leftmost value, shrinks the entire container.
	constexpr void																						ShiftLeft(
		// u64																								start_position,
		// u64																								amount
	) requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> || BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		// TODO: Implement start_position and amount parameters.

		if( this->data_size > 0 )
		{
			// Destruct first value.
			this->DestructRange( this->data_ptr, 1 );

			// Construct the first value from the next value.
			// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
			if constexpr( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
			{
				new( &this->data_ptr[ 0 ] ) ValueType( std::move( this->data_ptr[ 1 ] ) );
			}
			else
			{
				new( &this->data_ptr[ 0 ] ) ValueType( this->data_ptr[ 1 ] );
			}
			// For the rest we can assign.
			for( u64 i = 1; i < this->data_size; ++i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( BC_CONTAINER_IS_MOVE_ASSIGNABLE<ValueType> )
				{
					this->data_ptr[ i ] = std::move( this->data_ptr[ i + 1 ] );
				}
				else
				{
					this->data_ptr[ i ] = this->data_ptr[ i + 1 ];
				}
			}
			this->ResizeNoConstruct( this->data_size - 1, 0 );
		}
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	u64																									data_capacity				= {};
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if linear container base fulfills size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, true>, true> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, false>, true> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, true>, false> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, false>, false> ) == 16 );

static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerViewBase )<u32, true> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerViewBase )<u32, false> ) == 16 );

static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerBase )<u32> ) == 24 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if linear container iterator base passes container iterator concept.
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, true>, true>> );
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, false>, true>> );
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, true>, false>> );
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerViewBase )<u32, false>, false>> );
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerBase )<u32>, true>> );
static_assert( utility::ContainerIterator<BC_CONTAINER_NAME( LinearContainerIteratorBase )<BC_CONTAINER_NAME( LinearContainerBase )<u32>, false>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // container_bases
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
