
#include <core/containers/backend/ContainerBase.hpp>
#include <core/containers/backend/ContainerUtilities.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/containers/backend/LinearContainerBaseNormal.hpp>
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
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
	) noexcept requires( utility::IsConstConvertible<IsDataConst, IsOtherConst> )
		:
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( ListViewBase )(
		const ValueType																				*	from_data,
		u64																								size
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
		u64																								size
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
	) noexcept requires( utility::IsConstConvertible<IsDataConst, IsOtherConst> )
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

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
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

		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
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
	constexpr ConstIterator																				Find(
		const ValueType																				&	value
	) const BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, container_bases::internal_::DoLinearSearch<ValueType, true>( this->data_ptr, this->data_size, value ) );
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
	constexpr IteratorBase<IsConst>																		Find(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, container_bases::internal_::DoLinearSearch<ValueType, IsConst>( this->data_ptr, this->data_size, value ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container if lambda returns true.
	///
	/// @tparam LambdaType
	/// Type of the lambda which is used to search.
	///
	/// @param lambda
	///	Lambda function that is invoked to test if we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename LambdaType>
	[[nodiscard]]
	constexpr ConstIterator																				FindIf(
		LambdaType																					&&	lambda
	) const BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, container_bases::internal_::DoLinearSearchIf<ValueType, true>( this->data_ptr, this->data_size, lambda ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container if lambda returns true.
	///
	/// @tparam LambdaType
	/// Type of the lambda which is used to search.
	///
	/// @param lambda
	///	Lambda function that is invoked to test if we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename LambdaType>
	[[nodiscard]]
	constexpr IteratorBase<IsConst>																		FindIf(
		LambdaType																					&&	lambda
	) BC_CONTAINER_NOEXCEPT
	{
		return IteratorBase<IsConst>( this, container_bases::internal_::DoLinearSearchIf<ValueType, IsConst>( this->data_ptr, this->data_size, lambda ) );
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Linear container similar to std::vector.
///
/// @warning
/// Constructors of the contained type should not throw. If you need to construct a class or struct which constructor can throw,
/// look into heap allocating single-value containers like UniquePtr which can handle constructor throw.
///
/// @tparam ValueType
/// Type of the contained element.
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
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )(
		BC_CONTAINER_NAME( List )																	&&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )(
		std::initializer_list<ValueType>																init_list
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		this->Append( init_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( List )(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit BC_CONTAINER_NAME( List )(
		u64																								initial_size
	) BC_CONTAINER_NOEXCEPT
	{
		this->Resize( initial_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		const BC_CONTAINER_NAME( List )																&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		BC_CONTAINER_NAME( List ) { other }.Swap( *this );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		BC_CONTAINER_NAME( List )																	&&	other
	) BC_CONTAINER_NOEXCEPT
	{
		if( std::addressof( other ) == this ) return *this;

		this->Swap( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( List )																&	operator=(
		BC_CONTAINER_NAME( ListViewBase )<ValueType, IsOtherConst>										other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
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

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
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

		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
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

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
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

		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
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
			this->DoErase( at.GetAddress() )
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
			this->DoErase( from.GetAddress(), to.GetAddress() )
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase value if callback return true.
	///
	///	Run for every element of the list and if callable returns true, that element is removed.
	///
	/// @note
	/// This function goes through every element, it will not stop at the first occurrence.
	/// 
	/// @param callable
	///	Callable object, typically a lambda, which is called per element. If the callable returns true, that element is erased.
	template<utility::InvocableWithReturn<bool, const ValueType&> CallableType>
	constexpr void																						EraseIf(
		CallableType																					callable
	) BC_CONTAINER_NOEXCEPT
	{
		auto it = begin();
		while( it != end() )
		{
			if( callable( *it ) )
			{
				it = Erase( it );
			}
			else
			{
				++it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase first value if callback return true.
	///
	///	Run for every element of the list and if callable returns true, that element is removed.
	///
	/// @note
	/// This function only erases the first found element and returns as soon as it is erased.
	/// 
	/// @param callable
	///	Callable object, typically a lambda, which is called per element. If the callable returns true, that element is erased.
	template<utility::InvocableWithReturn<bool, const ValueType&> CallableType>
	constexpr void																						EraseFirstIf(
		CallableType																					callable
	) BC_CONTAINER_NOEXCEPT
	{
		auto it = begin();
		while( it != end() )
		{
			if( callable( *it ) )
			{
				Erase( it );
				return;
			}
			++it;
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
	/// Iterator to the next value after inserted value, which is the original value that was occupying the spot that we inserted
	/// into.
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const ValueType																				&	value,
		u64																								count			= 1,
		u64																								headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		BC_ContainerAssert( at.GetContainer(), U"Iterator points to nothing" );
		BC_ContainerAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetAddress(),
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
	/// Iterator to the next value after inserted value, which is the original value that was occupying the spot that we inserted
	/// into.
	template<utility::ContainerView OtherContainerType>
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const OtherContainerType																	&	other,
		u64																								count			= 1,
		u64																								headroom		= 0
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		BC_ContainerAssert( at.GetContainer(), U"Iterator points to nothing" );
		BC_ContainerAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetAddress(),
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
	/// Iterator to value position where value was found.
	[[nodiscard]]
	constexpr ConstIterator																				Find(
		const ValueType																				&	value
	) const BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<true>( *this ).Find( value );
		return ConstIterator { this, result.GetAddress() };
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
	constexpr Iterator																					Find(
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<false>( *this ).Find( value );
		return Iterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container if lambda returns true.
	///
	/// @tparam LambdaType
	/// Type of the lambda which is used to search.
	///
	/// @param lambda
	///	Lambda function that is invoked to test if we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename LambdaType>
	[[nodiscard]]
	constexpr ConstIterator																				FindIf(
		LambdaType																					&&	lambda
	) const BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<true>( *this ).FindIf( lambda );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurance of a specific value in this container if lambda returns true.
	///
	/// @tparam LambdaType
	/// Type of the lambda which is used to search.
	///
	/// @param lambda
	///	Lambda function that is invoked to test if we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename LambdaType>
	[[nodiscard]]
	constexpr Iterator																					FindIf(
		LambdaType																					&&	lambda
	) BC_CONTAINER_NOEXCEPT
	{
		auto result = ThisViewType<false>( *this ).FindIf( lambda );
		return Iterator { this, result.GetAddress() };
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



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( ListView )<u32> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( EditableListView )<u32> ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( List )<u32> ) == 24 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( utility::ContainerView<BC_CONTAINER_NAME( List )<u32>> );
static_assert( utility::ContainerEditableView<BC_CONTAINER_NAME( List )<u32>> );
static_assert( utility::Container<BC_CONTAINER_NAME( List )<u32>> );

static_assert( utility::ContainerView<BC_CONTAINER_NAME( EditableListView )<u32>> );
static_assert( utility::ContainerEditableView<BC_CONTAINER_NAME( EditableListView )<u32>> );
static_assert( !utility::Container<BC_CONTAINER_NAME( EditableListView )<u32>> );

static_assert( utility::ContainerView<BC_CONTAINER_NAME( ListView )<u32>> );
static_assert( !utility::ContainerEditableView<BC_CONTAINER_NAME( ListView )<u32>> );
static_assert( !utility::Container<BC_CONTAINER_NAME( ListView )<u32>> );

static_assert( utility::LinearContainerView<BC_CONTAINER_NAME( List )<u32>> );
static_assert( utility::LinearContainerEditableView<BC_CONTAINER_NAME( List )<u32>> );
static_assert( utility::LinearContainer<BC_CONTAINER_NAME( List )<u32>> );

static_assert( utility::LinearContainerView<BC_CONTAINER_NAME( EditableListView )<u32>> );
static_assert( utility::LinearContainerEditableView<BC_CONTAINER_NAME( EditableListView )<u32>> );
static_assert( !utility::LinearContainer<BC_CONTAINER_NAME( EditableListView )<u32>> );

static_assert( utility::LinearContainerView<BC_CONTAINER_NAME( ListView )<u32>> );
static_assert( !utility::LinearContainerEditableView<BC_CONTAINER_NAME( ListView )<u32>> );
static_assert( !utility::LinearContainer<BC_CONTAINER_NAME( ListView )<u32>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( List )<char32_t>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( List )<char32_t>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( List )<char32_t>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( EditableListView )<char32_t>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( EditableListView )<char32_t>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( EditableListView )<char32_t>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( ListView )<char32_t>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( ListView )<char32_t>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( ListView )<char32_t>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
