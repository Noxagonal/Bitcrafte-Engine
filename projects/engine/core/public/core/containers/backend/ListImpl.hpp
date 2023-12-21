
#include <core/containers/backend/ContainerBase.hpp>

#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/containers/backend/LinearContainerBaseNormal.hpp>
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/containers/backend/LinearContainerBaseSimple.hpp>
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>


namespace bc {



template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( List );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// View into a bc::List container.
///
///	This view allows modifying values but not the list itself unless <tt>ListView<ContainedValueType, true></tt> is used.
/// 
/// @tparam ValueType
///	Type of individual value.
template<BC_CONTAINER_VALUE_TYPENAME ValueType, bool IsConst>
class BC_CONTAINER_NAME( ListViewBase ) :
	public container_bases::BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsConst>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::BC_CONTAINER_NAME( LinearContainerViewBase )<ValueType, IsConst>;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= IsConst;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerType					= BC_CONTAINER_NAME( ListViewBase )<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<ValueType, IsDataConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( ListViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( List )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( ListIteratorBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( ListViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( List );

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( ListViewBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( ListViewBase )(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) noexcept requires( container_bases::IsConstConvertible<IsDataConst, IsOtherConst> )
		:
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( ListViewBase )(
		const ValueType																				*	from_data,
		size_t																							size
	) noexcept requires( IsDataConst == true )
		:
		Base(
			from_data,
			size
		)
	{}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( ListViewBase )(
		ValueType																					*	from_data,
		size_t																							size
	) noexcept requires( IsDataConst == false )
		:
		Base(
			from_data,
			size
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( ListViewBase )														&	operator=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) noexcept requires( container_bases::IsConstConvertible<IsDataConst, IsOtherConst> )
	{
		this->data_ptr = other.Data();
		this->data_size = other.Size();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents match exactly.
	///
	/// @param other
	///	Other list to compare with.
	/// 
	/// @return
	/// true if every value of this list matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents differ.
	/// 
	/// @param other
	/// Other list to compare with.
	/// 
	/// @return
	/// true if any value differs from the other list, false if contents match.
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container.
	///
	/// @param value
	///	Value to find.
	/// 
	/// @return
	/// Pointer to value position where value was found.
	[[nodiscard]]
	constexpr ConstIterator																				Find(
		const ValueType																				&	value
	) const BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, this->DoFind( value ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container.
	///
	/// @param value
	///	Value to find.
	/// 
	/// @return
	/// Pointer to value position where value was found.
	[[nodiscard]]
	constexpr IteratorBase<IsConst>																		Find(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, this->DoFind( value ) );
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
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ this->data_size ] };
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
		return ConstIterator { this, &this->data_ptr[ this->data_size ] };
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

private:
};



template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( List ) :
	public container_bases::BC_CONTAINER_NAME( LinearContainerBase )<ValueType>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::BC_CONTAINER_NAME( LinearContainerBase )<ValueType>;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( List )<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( ListViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( List )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	template<bool IsConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( ListViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( List );

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )(
		const BC_CONTAINER_NAME( List )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )(
		BC_CONTAINER_NAME( List )																	&&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )(
		std::initializer_list<ValueType>																init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( init_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( List )(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit BC_CONTAINER_NAME( List )(
		size_t																							initial_size
	) BC_CONTAINER_NOEXCEPT
	{
		this->Resize( initial_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		const BC_CONTAINER_NAME( List )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		BC_CONTAINER_NAME( List )																	&&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		if( other.Data() >= this->Data() && other.Data() < this->Data() + this->Size() )
		{
			// Other data is a part of this container, we'll need to do a copy first.
			auto other_copy = BC_CONTAINER_NAME( List )<ValueType> { other };
			*this = std::move( other_copy );
			return *this;
		}

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		std::initializer_list<ValueType>																init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Clear();
		this->Append( init_list );
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
	constexpr BC_CONTAINER_NAME( List )																&	operator+=(
		const BC_CONTAINER_NAME( List )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr BC_CONTAINER_NAME( List )																&	operator+=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr BC_CONTAINER_NAME( List )																&	operator+=(
		const std::initializer_list<ValueType>														&	init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		auto reserve_space = this->data_size + init_list.size();
		this->Append( init_list, 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents match exactly.
	///
	/// @param other
	///	Other list to compare with.
	/// 
	/// @return
	/// true if every value of this list matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents differ.
	/// 
	/// @param other
	///	Other list to compare with.
	/// 
	/// @return
	/// true if any value differs from the other list, false if contents match.
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents match exactly.
	///
	/// @param other
	///	Other list to compare with.
	/// 
	/// @return
	/// true if every value of this list matches the other, false otherwise.
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( List )																&	other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( std::addressof( other ) == this ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if List contents differ.
	/// 
	/// @param other
	///	Other list to compare with.
	/// 
	/// @return
	/// true if any value differs from the other list, false if contents match.
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( List )																&	other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( std::addressof( other ) == this ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
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
		return Iterator {
			this,
			this->DoErase( value )
		};
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
		BC_ContainerAssert( at.GetContainer() == this,
			U"Cannot erase using iterator that doesn't point to the container we're erasing from"
		);
		return Iterator {
			this,
			this->DoErase( at.GetData() )
		};
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
		BC_ContainerAssert( from.GetContainer() == this,
			U"Cannot erase using 'from' iterator that doesn't point to the container we're erasing from"
		);
		BC_ContainerAssert( to.GetContainer() == this,
			U"Cannot erase using 'to' iterator that doesn't point to the container we're erasing from"
		);
		return Iterator {
			this,
			this->DoErase( from.GetData(), to.GetData() )
		};
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
	/// Iterator to the next value after inserted value.
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const ValueType																				&	value,
		size_t																							count			= 1,
		size_t																							headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ValueType> )
	{
		BC_ContainerAssert( at.GetContainer() && at.GetData(), U"Empty iterator" );
		BC_ContainerAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetData(),
				value,
				count,
				headroom
			)
		};
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
	///	How many times should the other values be inserted. Eg. When inserting text, "sample" with count 2, you may consider it
	/// as if "samplesample" was inserted.
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
		BC_ContainerAssert( at.GetContainer() && at.GetData(), "Empty iterator" );
		BC_ContainerAssert( at.GetContainer() == this, "Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetData(),
				other,
				count,
				headroom
			)
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container.
	///
	/// @param value
	///	Value to find.
	/// 
	/// @return
	/// Pointer to value position where value was found.
	[[nodiscard]]
	constexpr ConstIterator																				Find(
		const ValueType																				&	value
	) const BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<true>( *this ).Find( value );
		return ConstIterator { this, result.GetData() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container.
	///
	/// @param value
	///	Value to find.
	/// 
	/// @return
	/// Pointer to value position where value was found.
	[[nodiscard]]
	constexpr Iterator																					Find(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<true>( *this ).Find( value );
		return Iterator { this, result.GetData() };
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
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ this->data_size ] };
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
		return ConstIterator { this, &this->data_ptr[ this->data_size ] };
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator BC_CONTAINER_NAME( ListViewBase )<ValueType, true>() const BC_CONTAINER_NOEXCEPT
	{
		return BC_CONTAINER_NAME( ListViewBase )<ValueType, true> { this->data_ptr, this->data_size };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator BC_CONTAINER_NAME( ListViewBase )<ValueType, false>() BC_CONTAINER_NOEXCEPT
	{
		return BC_CONTAINER_NAME( ListViewBase )<ValueType, false> { this->data_ptr, this->data_size };
	}
};



template<BC_CONTAINER_VALUE_TYPENAME ValueType>
using BC_CONTAINER_NAME( ListView ) = BC_CONTAINER_NAME( ListViewBase )<ValueType, true>;

template<BC_CONTAINER_VALUE_TYPENAME ValueType>
using BC_CONTAINER_NAME( EditableListView ) = BC_CONTAINER_NAME( ListViewBase )<ValueType, false>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( ListView )<uint32_t> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( EditableListView )<uint32_t> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( List )<uint32_t> ) == 24 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( List )<uint32_t>> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( List )<uint32_t>> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( List )<uint32_t>> );

static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );
static_assert( !container_bases::Container<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );

static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( ListView )<uint32_t>> );
static_assert( !container_bases::ContainerEditableView<BC_CONTAINER_NAME( ListView )<uint32_t>> );
static_assert( !container_bases::Container<BC_CONTAINER_NAME( ListView )<uint32_t>> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( List )<uint32_t>> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( List )<uint32_t>> );
static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( List )<uint32_t>> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( EditableListView )<uint32_t>> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( ListView )<uint32_t>> );
static_assert( !container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( ListView )<uint32_t>> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( ListView )<uint32_t>> );

static_assert( !container_bases::TextContainerView<BC_CONTAINER_NAME( List )<char32_t>> );
static_assert( !container_bases::TextContainerEditableView<BC_CONTAINER_NAME( List )<char32_t>> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( List )<char32_t>> );

static_assert( !container_bases::TextContainerView<BC_CONTAINER_NAME( EditableListView )<char32_t>> );
static_assert( !container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableListView )<char32_t>> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( EditableListView )<char32_t>> );

static_assert( !container_bases::TextContainerView<BC_CONTAINER_NAME( ListView )<char32_t>> );
static_assert( !container_bases::TextContainerEditableView<BC_CONTAINER_NAME( ListView )<char32_t>> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( ListView )<char32_t>> );



} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
