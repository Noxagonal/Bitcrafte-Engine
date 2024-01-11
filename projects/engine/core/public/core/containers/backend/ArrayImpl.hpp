
#include <core/containers/backend/ContainerBase.hpp>
#include <core/containers/backend/ContainerUtilities.hpp>

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
/// Linear container similar to std::vector.
///
/// @warning
/// Constructors of the contained type should not throw. If you need to construct a class or struct which constructor can throw,
/// look into heap allocating single-value containers like UniquePtr which can handle constructor throw.
///
/// @tparam ValueType
/// Type of the contained element.
template<BC_CONTAINER_VALUE_TYPENAME ValueType, size_t ValueCount>
class BC_CONTAINER_NAME( Array ) :
	public container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, size_t OtherValueCount>
	using ThisContainerType					= BC_CONTAINER_NAME( Array )<OtherValueType, OtherValueCount>;
	using ThisType							= ThisContainerType<ValueType, ValueCount>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void; // BC_CONTAINER_NAME( ArrayViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= void; // ThisContainerViewType<ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, size_t OtherValueCount>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Array )<OtherValueType, OtherValueCount>;
	using ThisFullType						= ThisContainerFullType<ValueType, ValueCount>;

	template<bool IsConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	//template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, size_t OtherValueCount, bool IsOtherConst>
	//friend class BC_CONTAINER_NAME( ArrayViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, size_t OtherValueCount>
	friend class BC_CONTAINER_NAME( Array );

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )() BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_DEFAULT_CONSTRUCTIBLE<ValueType> ) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )(
		const BC_CONTAINER_NAME( Array )															&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_DEFAULT_CONSTRUCTIBLE<ValueType> && BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		this->CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )(
		BC_CONTAINER_NAME( Array )																	&&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_DEFAULT_CONSTRUCTIBLE<ValueType> && BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	operator[](
		size_t																							index
	) const
	{
		return At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	operator[](
		size_t																							index
	)
	{
		return At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )															&	operator=(
		const BC_CONTAINER_NAME( Array )															&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		this->CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )															&	operator=(
		BC_CONTAINER_NAME( Array )																	&&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		if( std::addressof( other ) == this ) return *this;

		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if Array contents match exactly.
	///
	/// @param other
	///	Other array to compare with.
	/// 
	/// @return
	/// true if every value of this array matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( Array )																		other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::internal::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if Array contents differ.
	/// 
	/// @param other
	///	Other array to compare with.
	/// 
	/// @return
	/// true if any value differs from the other array, false if contents match.
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( Array )																		other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::internal::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if Array contents match exactly.
	///
	/// @param other
	///	Other array to compare with.
	/// 
	/// @return
	/// true if every value of this array matches the other, false otherwise.
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( Array )															&	other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( std::addressof( other ) == this ) return true;

		return container_bases::internal::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if Array contents differ.
	/// 
	/// @param other
	///	Other array to compare with.
	/// 
	/// @return
	/// true if any value differs from the other array, false if contents match.
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( Array )															&	other
	) const BC_CONTAINER_NOEXCEPT
	{
		if( std::addressof( other ) == this ) return false;

		return container_bases::internal::CheckContainerContentsDiffer( *this, other );
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
		return ConstIterator { this, container_bases::internal::DoLinearSearch<ValueType, true>( this->data, ValueCount, value ) };
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
		return Iterator { this, container_bases::internal::DoLinearSearch<ValueType, false>( this->data, ValueCount, value ) };
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
		return ConstIterator { this, container_bases::internal::DoLinearSearchIf<ValueType, true>( this->data, ValueCount, lambda ) };
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
		return Iterator { this, container_bases::internal::DoLinearSearchIf<ValueType, false>( this->data, ValueCount, lambda ) };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		&	At(
		size_t																							index
	) const
	{
		BC_ContainerAssert( index < ValueCount, U"Index out of range" );
		return data[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				&	At(
		size_t																							index
	)
	{
		BC_ContainerAssert( index < ValueCount, U"Index out of range" );
		return data[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType																		*	Data() const noexcept
	{
		return data;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType																				*	Data() noexcept
	{
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	consteval size_t																					Size() noexcept
	{
		return ValueCount;
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
		return IteratorBase<IsDataConst> { this, &this->data[ 0 ] };
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
		return ConstIterator { this, &this->data[ 0 ] };
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
		return IteratorBase<IsDataConst> { this, &this->data[ ValueCount ] };
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
		return ConstIterator { this, &this->data[ ValueCount ] };
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								CopyOther(
		const BC_CONTAINER_NAME( Array )															&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<ValueType> )
	{
		for( size_t i = 0; i < ValueCount; i++ )
		{
			this->data[ i ] = other[ i ];
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								SwapOther(
		BC_CONTAINER_NAME( Array )																	&&	other
	) BC_CONTAINER_NOEXCEPT requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<ValueType> )
	{
		for( size_t i = 0; i < ValueCount; i++ )
		{
			this->data[ i ] = std::move( other[ i ] );
		}
	}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																							data[ ValueCount ];
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Array )<uint32_t, 5> ) == sizeof( uint32_t ) * 5 );
static_assert( sizeof( BC_CONTAINER_NAME( Array )<uint32_t, 1> ) == sizeof( uint32_t ) * 1 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( utility::ContainerView<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );
static_assert( utility::ContainerEditableView<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );
static_assert( utility::Container<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );

static_assert( utility::LinearContainerView<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );
static_assert( utility::LinearContainerEditableView<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );
static_assert( utility::LinearContainer<BC_CONTAINER_NAME( Array )<uint32_t, 4>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( Array )<char32_t, 4>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( Array )<char32_t, 4>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( Array )<char32_t, 4>> );



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
