#pragma once

#include <core/containers/backend/ContainerBase.hpp>
#include <core/containers/backend/ContainerUtilities.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>

#include <core/containers/backend/LinearContainerBase.hpp>


namespace bc {



template<typename ValueType>
class List;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// View into a bc::List container.
///
///	This view allows modifying values but not the list itself unless <tt>ListView<ContainedValueType, true></tt> is used.
/// 
/// @tparam ValueType
///	Type of individual value.
template<typename ValueType, bool IsConst>
class ListViewBase : public container_bases::LinearContainerViewBase<ValueType, IsConst>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::LinearContainerViewBase<ValueType, IsConst>;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= IsConst;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerType					= ListViewBase<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<ValueType, IsDataConst>;

	template<bool IsOtherConst>
	using ThisTypeOtherConst				= ThisContainerType<ValueType, IsOtherConst>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= ListViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<typename OtherValueType>
	using ThisContainerFullType				= List<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::LinearContainerIteratorBase<ThisType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherValueType, bool IsOtherConst>
	friend class ListIteratorBase;

	template<typename OtherValueType, bool IsOtherConst>
	friend class ListViewBase;

	template<typename OtherValueType>
	friend class List;

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ListViewBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr ListViewBase( ThisTypeOtherConst<IsOtherConst> other ) noexcept
		requires( utility::IsConstConvertible<IsDataConst, IsOtherConst> )
	:
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ListViewBase(
		const ValueType*	from_data,
		i64					size
	) noexcept requires( IsDataConst == true )
		:
		Base(
			from_data,
			size
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ListViewBase(
		ValueType*	from_data,
		i64			size
	) noexcept requires( IsDataConst == false )
		:
		Base(
			from_data,
			size
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr auto operator=( ThisTypeOtherConst<IsOtherConst> other ) noexcept -> ThisType&
		requires( utility::IsConstConvertible<IsDataConst, IsOtherConst> )
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
	constexpr auto operator==( ThisTypeOtherConst<IsOtherConst> other ) const noexcept -> bool
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
	constexpr auto operator!=( ThisTypeOtherConst<IsOtherConst> other ) const noexcept -> bool
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
	constexpr auto Find( const ValueType& value ) const -> ConstIterator
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
	constexpr auto Find( const ValueType& value ) -> IteratorBase<IsConst>
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
	constexpr auto FindIf( LambdaType&& lambda ) const -> ConstIterator
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
	constexpr auto FindIf( LambdaType&& lambda ) -> IteratorBase<IsConst>
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
	constexpr auto begin() noexcept -> IteratorBase<IsDataConst>
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
	constexpr auto begin() const noexcept -> ConstIterator
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
	constexpr auto cbegin() const noexcept -> ConstIterator
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
	constexpr auto end() noexcept -> IteratorBase<IsDataConst>
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
	constexpr auto end() const noexcept -> ConstIterator
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
	constexpr auto cend() const noexcept -> ConstIterator
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
template<typename ValueType>
class List : public container_bases::LinearContainerBase<ValueType>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::LinearContainerBase<ValueType>;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherValueType>
	using ThisContainerType					= List<OtherValueType>;
	using ThisType							= ThisContainerType<ValueType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= ListViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<ValueType, IsOtherConst>;

	template<typename OtherValueType>
	using ThisContainerFullType				= List<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<ValueType>;

	template<bool IsConst>
	using IteratorBase						= container_bases::LinearContainerIteratorBase<ThisType, IsConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	template<typename OtherValueType, bool IsOtherConst>
	friend class ListViewBase;

	template<typename OtherValueType>
	friend class List;

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr List() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr List( const ThisType& other )
		requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr List( ThisType&& other )
	{
		this->Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr List( std::initializer_list<ValueType> init_list )
		requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( init_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr List( ThisViewType<IsOtherConst> other )
		requires( std::is_copy_constructible_v<ValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit List( i64 initial_size )
	{
		this->Resize( initial_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ThisType& other )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		ThisType { other }.Swap( *this );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ThisType&& other ) -> ThisType&
	{
		if( std::addressof( other ) == this ) return *this;

		this->Swap( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr auto operator=( ThisViewType<IsOtherConst> other )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
	{
		if( other.Data() >= this->Data() && other.Data() < this->Data() + this->Size() )
		{
			// Other data is a part of this container, we'll need to do a copy first.
			auto other_copy = List<ValueType> { other };
			*this = std::move( other_copy );
			return *this;
		}

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( std::initializer_list<ValueType> init_list )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr auto operator+=( const ThisType& other )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr auto operator+=( ThisViewType<IsOtherConst> other )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr auto operator+=( std::initializer_list<ValueType> init_list )->ThisType&
		requires( std::is_copy_constructible_v<ValueType> )
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
	constexpr auto operator==( ThisViewType<IsOtherConst> other ) const -> bool
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
	constexpr auto operator!=( ThisViewType<IsOtherConst> other ) const -> bool
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
	constexpr auto operator==( const ThisType& other ) const -> bool
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
	constexpr auto operator!=( const ThisType& other ) const -> bool
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
	constexpr auto Erase( const ValueType& value ) -> Iterator
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
	constexpr auto Erase( ConstIterator at ) -> Iterator
	{
		BAssert( at.GetContainer() == this,
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
	constexpr auto Erase(
		ConstIterator	from,
		ConstIterator	to
	) -> Iterator
	{
		BAssert( from.GetContainer() == this,
			U"Cannot erase using 'from' iterator that doesn't point to the container we're erasing from"
		);
		BAssert( to.GetContainer() == this,
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
	///	Run for every element of the list and if invocable returns true, that element is removed.
	///
	/// @note
	/// This function goes through every element, it will not stop at the first occurrence.
	/// 
	/// @param invocable
	///	Invocable object, typically a lambda, which is called per element. If the invocable returns true, that element is erased.
	template<utility::InvocableWithReturn<bool, const ValueType&> InvocableType>
	constexpr void EraseIf( InvocableType&& invocable )
	{
		auto it = begin();
		while( it != end() )
		{
			if( invocable( *it ) )
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
	///	Run for every element of the list and if invocable returns true, that element is removed.
	///
	/// @note
	/// This function only erases the first found element and returns as soon as it is erased.
	/// 
	/// @param invocable
	///	Invocable object, typically a lambda, which is called per element. If the invocable returns true, that element is erased.
	template<utility::InvocableWithReturn<bool, const ValueType&> InvocableType>
	constexpr void EraseFirstIf( InvocableType&& invocable )
	{
		auto it = begin();
		while( it != end() )
		{
			if( invocable( *it ) )
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
	constexpr auto Insert(
		ConstIterator		at,
		const ValueType&	value,
		i64					count			= 1,
		i64					headroom		= 0
	) -> Iterator
		requires( std::is_copy_constructible_v<ValueType> )
	{
		BAssert( at.GetContainer(), U"Iterator points to nothing" );
		BAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
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
	constexpr auto Insert(
		ConstIterator				at,
		const OtherContainerType&	other,
		i64							count			= 1,
		i64							headroom		= 0
	) -> Iterator
		requires( std::is_copy_constructible_v<ValueType> && std::is_same_v<ValueType, typename OtherContainerType::ContainedValueType> )
	{
		BAssert( at.GetContainer(), U"Iterator points to nothing" );
		BAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
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
	constexpr auto Find( const ValueType& value ) const -> ConstIterator
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
	constexpr auto Find( const ValueType& value ) -> Iterator
	{
		auto result = ThisViewType<false>( *this ).Find( value );
		return Iterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a specific value in this container if lambda returns true.
	///
	/// @tparam InvokableType
	/// Type of the invokable which is used to search.
	///
	/// @param invokable
	///	Invokable object or function that is called to test if we found what we're looking for. If the invokable returns true, then
	/// we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename InvokableType>
	[[nodiscard]]
	constexpr auto FindIf( InvokableType&& invokable ) const -> ConstIterator
	{
		auto result = ThisViewType<true>( *this ).FindIf( invokable );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a specific value in this container if lambda returns true.
	///
	/// @tparam InvokableType
	/// Type of the invokable which is used to search.
	///
	/// @param invokable
	///	Invokable object or function that is called to test if we found what we're looking for. If the invokable returns true, then
	/// we found what we're looking for.
	/// 
	/// @return
	/// Iterator to value position where value was found.
	template<typename InvokableType>
	[[nodiscard]]
	constexpr auto FindIf( InvokableType&& invokable ) -> Iterator
	{
		auto result = ThisViewType<false>( *this ).FindIf( invokable );
		return Iterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first value.
	[[nodiscard]]
	constexpr auto begin() noexcept -> IteratorBase<IsDataConst>
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
	constexpr auto begin() const noexcept -> ConstIterator
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
	constexpr auto cbegin() const noexcept -> ConstIterator
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
	constexpr auto end() noexcept -> IteratorBase<IsDataConst>
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
	constexpr auto end() const noexcept -> ConstIterator
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
	constexpr auto cend() const noexcept -> ConstIterator
	{
		return this->end();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator ThisViewType<true>() const
	{
		return ThisViewType<true> { this->data_ptr, this->data_size };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator ThisViewType<false>()
	{
		return ThisViewType<false> { this->data_ptr, this->data_size };
	}
};



template<typename ValueType>
using ListView = ListViewBase<ValueType, true>;

template<typename ValueType>
using EditableListView = ListViewBase<ValueType, false>;



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( ListView<u32> ) == 16 );
static_assert( sizeof( EditableListView<u32> ) == 16 );
static_assert( sizeof( List<u32> ) == 24 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( utility::ContainerView<List<u32>> );
static_assert( utility::ContainerEditableView<List<u32>> );
static_assert( utility::Container<List<u32>> );

static_assert( utility::ContainerView<EditableListView<u32>> );
static_assert( utility::ContainerEditableView<EditableListView<u32>> );
static_assert( !utility::Container<EditableListView<u32>> );

static_assert( utility::ContainerView<ListView<u32>> );
static_assert( !utility::ContainerEditableView<ListView<u32>> );
static_assert( !utility::Container<ListView<u32>> );

static_assert( utility::LinearContainerView<List<u32>> );
static_assert( utility::LinearContainerEditableView<List<u32>> );
static_assert( utility::LinearContainer<List<u32>> );

static_assert( utility::LinearContainerView<EditableListView<u32>> );
static_assert( utility::LinearContainerEditableView<EditableListView<u32>> );
static_assert( !utility::LinearContainer<EditableListView<u32>> );

static_assert( utility::LinearContainerView<ListView<u32>> );
static_assert( !utility::LinearContainerEditableView<ListView<u32>> );
static_assert( !utility::LinearContainer<ListView<u32>> );

static_assert( !utility::TextContainerView<List<char32_t>> );
static_assert( !utility::TextContainerEditableView<List<char32_t>> );
static_assert( !utility::TextContainer<List<char32_t>> );

static_assert( !utility::TextContainerView<EditableListView<char32_t>> );
static_assert( !utility::TextContainerEditableView<EditableListView<char32_t>> );
static_assert( !utility::TextContainer<EditableListView<char32_t>> );

static_assert( !utility::TextContainerView<ListView<char32_t>> );
static_assert( !utility::TextContainerEditableView<ListView<char32_t>> );
static_assert( !utility::TextContainer<ListView<char32_t>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc
