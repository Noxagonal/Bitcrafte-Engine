
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

	template<bool IsConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>;
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
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ValueType, IsConst>;
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
