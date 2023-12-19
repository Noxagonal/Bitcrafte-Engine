
#include <core/containers/backend/ContainerBase.hpp>

#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
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
template<BC_CONTAINER_VALUE_TYPENAME ValueType, bool IsConst>
class BC_CONTAINER_NAME( LinearContainerIteratorBase )
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using ContainedValueType	= ValueType;

	using value_type			= ValueType;	// For stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerIteratorBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( LinearContainerBase );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsConst>								*	container;
	ValueType																						*	data;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsOtherConst>				&	other
	) noexcept requires( IsConstConvertible<IsConst, IsOtherConst> )
		:
		container( reinterpret_cast<decltype( this->container )>( const_cast<BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>*>( other.container ) ) ),
		data( const_cast<ValueType*>( other.data ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst = IsConst>
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )(
		const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>					*	container,
		const ValueType																				*	data
	) noexcept requires( IsConstConvertible<IsConst, IsOtherConst> )
		:
		container( reinterpret_cast<decltype( this->container )>( const_cast<BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>*>( container ) ) ),
		data( const_cast<ValueType*>( data ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	operator*() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	operator*() const BC_CONTAINER_NOEXCEPT
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	operator->() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	operator->() const BC_CONTAINER_NOEXCEPT
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsOtherConst>				&	other
	) const noexcept
	{
		return this->data == other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsOtherConst>				&	other
	) const noexcept
	{
		return this->data != other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>					&	operator++() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data + 1 <= this->container->data_ptr + this->container->data_size,
			U"Tried to increment iterator past end",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() + 1
		);
		++this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>					&	operator--() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data - 1 >= this->container->data_ptr,
			U"Tried to decrement iterator past first value",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() - 1
		);
		--this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>						operator+(
		size_t																							value
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data + value <= this->container->data_ptr + this->container->data_size,
			U"Tried to increment iterator past end",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() + value
		);
		BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst> ret = *this;
		ret.data += value;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>						operator-(
		size_t																							value
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data - value >= this->container->data_ptr,
			U"Tried to decrement iterator past first value",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() - value
		);
		BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst> ret = *this;
		ret.data -= value;
		return ret;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>					&	operator+=(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data + value <= this->container->data_ptr + this->container->data_size,
			U"Tried to increment iterator past end",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() + value
		);
		this->data += value;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>					&	operator-=(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->data - value >= this->container->data_ptr,
			U"Tried to decrement iterator past first value",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck() - value
		);
		this->data -= value;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	Get() const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( this->data >= this->container->data_ptr && this->data < ( this->container->data_ptr + this->container->data_size ),
			U"Iterator is out of range",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck()
		);
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	Get() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( this->data >= this->container->data_ptr && this->data < ( this->container->data_ptr + this->container->data_size ),
			U"Iterator is out of range",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck()
		);
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	GetData() const noexcept
	{
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	GetData() noexcept requires( IsConst == false )
	{
		return this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsConst>				*	GetContainer() const noexcept
	{
		return this->container;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsConst>						*	GetContainer() noexcept requires( IsConst == false )
	{
		return this->container;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr int64_t																					GetIndex() const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( this->data >= this->container->data_ptr && this->data < ( this->container->data_ptr + this->container->data_size ),
			U"Iterator is out of range",
			U"Container size", this->container->data_size,
			U"Iterator index", this->GetIndex_NoCheck()
		);
		return GetIndex_NoCheck();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						IsValid() const noexcept
	{
		return
			( this->container != nullptr ) &&
			( this->data >= this->container->data_ptr ) &&
			( this->data <= this->container->data_ptr + this->container->data_size );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr int64_t																					GetIndex_NoCheck() const noexcept
	{
		return ( int64_t( this->data ) - int64_t( this->container->data_ptr ) ) / sizeof( ValueType );
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
	using ThisType							= ThisContainerType<ValueType, IsDataConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( LinearContainerViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( LinearContainerBase )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	template<bool IsOtherConst>
	using IteratorBase						= BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// template<bool IsOtherConst>
	// friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

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
		const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>	&	other
	) noexcept requires( IsDataConst == true )
		:
		data_ptr( other.data_ptr ),
		data_size( other.data_size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		const BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsOtherConst>	&	other
	) noexcept requires( IsConstConvertible<IsDataConst, IsOtherConst> )
		:
		data_ptr( const_cast<ValueType*>( other.Data() ) ),
		data_size( other.Size() )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		const ValueType																				*	ptr,
		size_t																							size
	) noexcept requires( IsDataConst == true )
		:
		data_ptr( const_cast<ValueType*>( ptr ) ),
		data_size( size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( LinearContainerViewBase )(
		ValueType																					*	ptr,
		size_t																							size
	) noexcept requires( IsDataConst == false )
		:
		data_ptr( ptr ),
		data_size( size )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType																		&	operator[](
		size_t																							index
	) const BC_CONTAINER_NOEXCEPT
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ValueType																				&	operator[](
		size_t																							index
	) BC_CONTAINER_NOEXCEPT requires( IsDataConst == false )
	{
		return this->At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr const ValueType																		&	At(
		size_t																							index
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
		size_t																							index
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
	/// Find the first occurance of a specific value in this container.
	///
	/// @param value
	///	Value to find.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	[[nodiscard]]
	constexpr IteratorBase<IsDataConst>																	Find(
		const ValueType																				&	value
	) const BC_CONTAINER_NOEXCEPT
	{
		auto it = this->data_ptr;
		auto it_end = this->data_ptr + this->data_size;
		if( it != it_end ) {
			if( *it == value ) {
				return IteratorBase<IsDataConst>( this, it );
			}
			++it;
		}
		return IteratorBase<IsDataConst>( this, it );
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
		return Find( member ) != end();
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
		BC_ContainerAssert( this->IsEmpty(), "Cannot get container front value, container is empty.");
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
		BC_ContainerAssert( this->IsEmpty(), "Cannot get container front value, container is empty." );
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
		BC_ContainerAssert( this->IsEmpty(), "Cannot get container back value, container is empty." );
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
		BC_ContainerAssert( this->IsEmpty(), "Cannot get container back value, container is empty." );
		return this->data_ptr[ this->data_size - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the amount of reserved space in this container.
	/// 
	/// @return
	/// Number of values space has been reserved for.
	[[nodiscard]]
	constexpr size_t																					Size() const noexcept
	{
		return this->data_size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the container data directly.
	///
	/// @warning
	/// Never use this function to get a C-style character string, it will not be null terminated.
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
	/// Never use this function to get a C-style character string, it will not be null terminated.
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first value.
	[[nodiscard]]
	constexpr IteratorBase<IsDataConst>																	begin() noexcept
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first value.
	[[nodiscard]]
	constexpr ConstIterator																				begin() const noexcept
	{
		return ConstIterator { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first value.
	[[nodiscard]]
	constexpr ConstIterator																				cbegin() const noexcept
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last value.
	[[nodiscard]]
	constexpr IteratorBase<IsDataConst>																	end() noexcept
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last value.
	[[nodiscard]]
	constexpr ConstIterator																				end() const noexcept
	{
		return ConstIterator { this, &this->data_ptr[ data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last value.
	[[nodiscard]]
	constexpr ConstIterator																				cend() const noexcept
	{
		return this->end();
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																						*	data_ptr				= {};
	size_t																								data_size				= {};
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

	template<bool IsOtherConst>
	using IteratorBase						= BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( LinearContainerViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( LinearContainerBase );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BC_CONTAINER_NAME( LinearContainerBase )() = default;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<container_bases::LinearContainer OtherContainerType>
	//BC_CONTAINER_NAME( LinearContainerBase )(
	//	const OtherContainerType									&	other
	//)
	//{
	//	// Do not use Append() as it may construct a temporary with same type using this constructor.
	//	auto other_size = other.Size();
	//
	//	this->ResizeNoConstruct( other_size, 0 );
	//
	//	auto other_it = other.begin();
	//	for( size_t i = 0; i < other_size; ++i )
	//	{
	//		new( &this->data_ptr[ i ] ) ValueType( *other_it );
	//		++other_it;
	//	}
	//}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( LinearContainerBase )() BC_CONTAINER_NOEXCEPT
	{
		this->DestructRange( this->data_ptr, this->data_size );
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
		size_t																							new_capacity,
		size_t																							headroom			= 0
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
			auto new_ptr = this->ResizeRange( this->data_ptr, this->data_size, new_capacity );
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
		size_t																							new_size,
		size_t																							headroom			= 0
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
	template<container_bases::ContainerView OtherContainerType>
	constexpr void																						Append(
		const OtherContainerType																	&	other,
		size_t																							count				= 1,
		size_t																							headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
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
	constexpr void																						Append(
		const std::initializer_list<ValueType>														&	init_list,
		size_t																							count				= 1,
		size_t																							headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	/// Inserts another value at position.
	///
	///	This insert function copies the value.
	///
	/// @param at
	///	Iterator location where to insert the new value.
	///
	/// @param value
	/// New value to insert.
	///
	/// @param count
	///	How many times should the new value be inserted. Eg. When inserting a character, 't' with count 4, you may consider it
	/// as if "tttt" was inserted.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	///
	/// @return
	/// Iterator one past the the inserted value. Or first original value after insertion.
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const ValueType																				&	value,
		size_t																							count			= 1,
		size_t																							headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		auto * this_as_const_view = reinterpret_cast<ThisViewType<true>*>( this );
		BC_ContainerAssert( at.container == this_as_const_view,
			"Cannot insert into container using iterator that doesn't point to the container we're inserting into"
		);

		size_t at_index = at.GetIndex();
		this->ShiftRight( at_index, count, headroom );

		for( size_t i = 0; i < count; ++i )
		{
			new( &this->data_ptr[ at_index + i ] ) ValueType( value );
		}

		return Iterator { this, &this->data_ptr[ at_index + count ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Inserts values from another container at position, optionally multiple times.
	///
	/// This insert function copies values.
	///
	/// @param at
	///	Iterator location where to start inserting other values.
	///
	/// @param other
	///	Linear container view type to insert values from.
	///
	/// @param count
	///	How many times should the other values be inserted. Eg. When inserting text, "text" with count 2, you may consider it
	/// as if "texttext" was inserted.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded.
	///
	/// @return
	/// Iterator one past the the last inserted value. Or first original value after insertion.
	template<container_bases::ContainerView OtherContainerType>
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const OtherContainerType																	&	other,
		size_t																							count			= 1,
		size_t																							headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		BC_ContainerAssert( reinterpret_cast<const void*>( at.container ) == reinterpret_cast<const void*>( this ),
			"Cannot insert into container using iterator that doesn't point to the container we're inserting into"
		);

		auto CopyFunc = [ this, count, &at, headroom ](
			auto		other
			) -> Iterator
			{
				size_t start_index			= at.GetIndex();
				size_t other_size			= other.Size();
				size_t total_insert_size	= other_size * count;

				this->ShiftRight( start_index, total_insert_size, headroom );

				for( size_t c = 0; c < count; ++c )
				{
					auto it = other.begin();
					for( size_t i = 0; i < other_size; ++i )
					{
						auto count_start_pos = c * other_size + start_index;
						new( &this->data_ptr[ count_start_pos + i ] ) ValueType( *it );
						++it;
					}
				}
				return Iterator { this, &this->data_ptr[ start_index + total_insert_size ] };
			};

		if constexpr( container_bases::LinearContainerView<OtherContainerType> )
		{
			if( reinterpret_cast<const void*>( other.Data() ) >= reinterpret_cast<const void*>( this->data_ptr ) &&
				reinterpret_cast<const void*>( other.Data() ) < reinterpret_cast<const void*>( this->data_ptr + this->data_size ) )
			{
				// Other container data is either full or partial range from within this container, need to make a temporary.
				auto other_copy = BC_CONTAINER_NAME( LinearContainerBase )<typename OtherContainerType::ContainedValueType> { other };
				return CopyFunc( other_copy );
			}
		}

		return CopyFunc( other );
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ShiftRight( 0, 1, reserve_space );
		// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
		if constexpr( std::is_move_constructible_v<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + 1;
		this->ResizeNoConstruct( reserve_space, 0 );
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
	/// Add value to the front of this container.
	///
	///	Shifts everything else towards the back by one index. This increments the indices of every value already in this
	/// container.
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
		size_t																							count,
		size_t																							headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		this->ShiftRight( 0, count, headroom );
		for( size_t i = 0; i < count; i++ )
		{
			new( &this->data_ptr[ i ] ) ValueType( value );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add value to the back of this container a specific number of times.
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
		size_t																							count,
		size_t																							headroom			= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
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
		BC_ContainerAssert( !this->IsEmpty(), "Cannot pop front, container is empty");
		this->ShiftLeft();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Removes the last value.
	constexpr void																						PopBack() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), "Cannot pop back, container is empty");
		this->ResizeNoConstruct( this->data_size - 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase the first found value from this container.
	/// 
	///	Remaining value at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param value
	///	Value to erase from the list, if not found, returns an iterator to the end.
	/// 
	/// @return
	/// Iterator to the next value which replaced the erased value or end if not found.
	constexpr Iterator																					Erase(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( !this->IsEmpty(), "Cannot erase from container, container is already empty" );
		auto iterator = this->Find( value );
		if( iterator == this->end() ) return iterator;
		return this->Erase( iterator );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase value at iterator location.
	///
	///	Remaining values at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param at
	///	Iterator position to value which to erase.
	/// 
	/// @return
	/// Iterator to the next value which replaced the erased value, if erased last value then returned iterator points to
	/// the end.
	constexpr Iterator																					Erase(
		ConstIterator																					at
	) BC_CONTAINER_NOEXCEPT
	{
		auto * this_as_const_view = reinterpret_cast<ThisViewType<true>*>( this );
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert( at.container == this_as_const_view,
			U"Cannot erase using iterator that doesn't point to the container we're erasing from"
		);
		BC_ContainerAssert( at.data < this->data_ptr + this->data_size,
			U"Cannot erase from container, Iterator out of range or it points to the end",
			U"Container size", this->data_size,
			U"Iterator index", at.GetIndex_NoCheck()
		);

		return this->Erase( at, at + 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase value range.
	///
	///	Remaining values at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param from
	///	Iterator to first value to erase.
	/// 
	/// @param to
	///	Iterator to last value where to stop erasing. This value is not erased from this container, rather it tells the
	/// position where to stop.
	/// 
	/// @return
	/// Iterator to the the first value which was not erased.
	constexpr Iterator																					Erase(
		ConstIterator																					from,
		ConstIterator																					to
	) BC_CONTAINER_NOEXCEPT
	{
		auto * this_as_const_view = reinterpret_cast<ThisViewType<true>*>( this );
		BC_ContainerAssert( !this->IsEmpty(), U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert( from.container == this_as_const_view,
			U"Cannot erase using a container iterator that doesn't point to the container we're erasing from"
		);
		BC_ContainerAssert( to.container == this_as_const_view,
			U"Cannot erase using container iterator that doesn't point to the container we're erasing from"
		);
		BC_ContainerAssert( from.data < this->data_ptr + this->data_size,
			U"Cannot erase from container, \"From\" Iterator out of range or it points to the end",
			U"Container size", this->data_size,
			U"\"From\" Iterator index", from.GetIndex_NoCheck(),
			U"\"To\" Iterator index", to.GetIndex_NoCheck()
		);
		BC_ContainerAssert( from.data < this->data_ptr + this->data_size + 1,
			U"Cannot erase from container, \"To\" Iterator out of range or it points to the end",
			U"Container size", this->data_size,
			U"\"From\" Iterator index", from.GetIndex_NoCheck(),
			U"\"To\" Iterator index", to.GetIndex_NoCheck()
		);

		size_t range = to.data - from.data;
		auto it = from.data;
		auto it_last = this->data_ptr + this->data_size - 1;
		while( it != it_last )
		{
			if constexpr( std::is_move_assignable_v<ValueType> )
			{
				*it = std::move( *( it + range ) );
			}
			else
			{
				*it = *( it + range );
			}
			++it;
		}
		auto last_occupied_slot		= ( it_last - this->data_ptr ) - range;
		this->Resize( last_occupied_slot + 1 );
		return Iterator( this, from.data );
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
	constexpr void																						ResizeNoConstruct(
		size_t																							new_size,
		size_t																							headroom
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
	constexpr void																						SwapOther(
		BC_CONTAINER_NAME( LinearContainerBase )													&&	other
	) BC_CONTAINER_NOEXCEPT
	{
		if( this != std::addressof( other ) )
		{
			std::swap( this->data_ptr, other.data_ptr );
			std::swap( this->data_size, other.data_size );
			std::swap( this->data_capacity, other.data_capacity );
		}
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
		size_t																							start_position,
		size_t																							amount,
		size_t																							headroom
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
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
			for( size_t i = copy_range_end - 1; i >= copy_range_begin; --i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( std::is_move_constructible_v<ValueType> )
				{
					new( &this->data_ptr[ i ] ) ValueType( std::move( this->data_ptr[ i - amount ] ) );
				}
				else
				{
					new( &this->data_ptr[ i ] ) ValueType( this->data_ptr[ i - amount ] );
				}
			}

			// For overlapping portion we can assign or move the values.
			for( size_t i = overlap_range_end - 1; i >= overlap_range_begin; --i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( std::is_move_constructible_v<ValueType> )
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
		// size_t																							start_position,
		// size_t																							amount
	) requires( std::is_copy_constructible_v<ValueType> || std::is_move_constructible_v<ValueType> )
	{
		// TODO: Implement start_position and amount parameters.

		if( this->data_size > 0 )
		{
			// Destruct first value.
			this->DestructRange( this->data_ptr, 1 );

			// Construct the first value from the next value.
			// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
			if constexpr( std::is_move_constructible_v<ValueType> )
			{
				new( &this->data_ptr[ 0 ] ) ValueType( std::move( this->data_ptr[ 1 ] ) );
			}
			else
			{
				new( &this->data_ptr[ 0 ] ) ValueType( this->data_ptr[ 1 ] );
			}
			// For the rest we can assign.
			for( size_t i = 1; i < this->data_size; ++i )
			{
				// This test is needed in cases where either the copy constructor or the move constructor has been explicitly deleted.
				if constexpr( std::is_move_assignable_v<ValueType> )
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
	size_t																								data_capacity				= {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if linear container base fullfills size requirements.

static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<uint32_t, true> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerIteratorBase )<uint32_t, false> ) == 16 );

static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerViewBase )<uint32_t, true> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerViewBase )<uint32_t, false> ) == 16 );

static_assert( sizeof( BC_CONTAINER_NAME( LinearContainerBase )<uint32_t> ) == 24 );



} // container_bases
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
