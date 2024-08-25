
#include <core/containers/backend/ContainerBase.hpp>
#include <core/containers/backend/ContainerUtilities.hpp>

#include <initializer_list>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/containers/backend/LinearContainerBaseNormal.hpp>
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/containers/backend/LinearContainerBaseSimple.hpp>
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
BC_CONTAINER_NAMESPACE_START;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Linear container similar to std::array.
///
/// @warning
/// Constructors of the contained type should not throw. If you need to construct a class or struct which constructor can throw,
/// look into heap allocating single-value containers like UniquePtr which can handle constructor throw.
///
/// @tparam ValueType
/// Type of the contained element.
template<BC_CONTAINER_VALUE_TYPENAME ValueType, i64 ValueCount>
class BC_CONTAINER_NAME( Array ) :
	public container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
	using ThisContainerType					= BC_CONTAINER_NAME( Array )<OtherValueType, OtherValueCount>;
	using ThisType							= ThisContainerType<ValueType, ValueCount>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
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

	//template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount, bool IsOtherConst>
	//friend class BC_CONTAINER_NAME( ArrayViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
	friend class BC_CONTAINER_NAME( Array );

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )( const BC_CONTAINER_NAME( Array )&	other ) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )( BC_CONTAINER_NAME( Array )&& other ) = default;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<utility::ConstructibleFrom<ValueType> ...ValueArgumentTypePack>
	constexpr BC_CONTAINER_NAME( Array )( ValueArgumentTypePack&& ...values )
	{
		static_assert( sizeof...( ValueArgumentTypePack ) <= ValueCount, "Too many values given to constructor" );
		FillFromTemplateParameterPack<0>( std::forward<ValueArgumentTypePack>( values )... );
		// Fill in the remaining values with default constructed values.
		for( u64 i = sizeof...( ValueArgumentTypePack ); i < ValueCount; i++ )
		{
			data[ i ] = ValueType {};
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<utility::InvocableFunction ...ValueArgumentTypePack>
	constexpr BC_CONTAINER_NAME( Array )( ValueArgumentTypePack&& ...values )
	{
		static_assert( sizeof...( ValueArgumentTypePack ) <= ValueCount, "Too many values given to constructor" );
		FillFromTemplateParameterPack<0>( std::forward<ValueArgumentTypePack>( values )... );
		// Fill in the remaining values with default constructed values.
		for( i64 i = sizeof...( ValueArgumentTypePack ); i < ValueCount; i++ )
		{
			data[ i ] = ValueType {};
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( Array )() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const BC_CONTAINER_NAME( Array )& other ) -> BC_CONTAINER_NAME( Array )& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( BC_CONTAINER_NAME( Array )&& other ) -> BC_CONTAINER_NAME( Array )& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Front() const -> const ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Front() -> ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Back() const -> const ValueType&
	{
		return At( ValueCount - 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Back() -> ValueType&
	{
		return At( ValueCount - 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) const -> const ValueType&
	{
		return At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) -> ValueType&
	{
		return At( index );
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
	constexpr auto operator==( const BC_CONTAINER_NAME( Array )& other ) const BC_CONTAINER_NOEXCEPT -> bool
	{
		if( std::addressof( other ) == this ) return true;

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
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
	constexpr auto operator!=( const BC_CONTAINER_NAME( Array )& other ) const BC_CONTAINER_NOEXCEPT -> bool
	{
		if( std::addressof( other ) == this ) return false;

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
	constexpr auto Find( const ValueType& value ) const BC_CONTAINER_NOEXCEPT -> ConstIterator
	{
		return ConstIterator { this, container_bases::internal_::DoLinearSearch<ValueType, true>( this->data, ValueCount, value ) };
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
	constexpr auto Find( const ValueType& value ) BC_CONTAINER_NOEXCEPT -> Iterator
	{
		return Iterator { this, container_bases::internal_::DoLinearSearch<ValueType, false>( this->data, ValueCount, value ) };
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
	constexpr auto FindIf( LambdaType&&	lambda ) const BC_CONTAINER_NOEXCEPT -> ConstIterator
	{
		return ConstIterator { this, container_bases::internal_::DoLinearSearchIf<ValueType, true>( this->data, ValueCount, lambda ) };
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
	constexpr auto FindIf( LambdaType&& lambda ) BC_CONTAINER_NOEXCEPT -> Iterator
	{
		return Iterator { this, container_bases::internal_::DoLinearSearchIf<ValueType, false>( this->data, ValueCount, lambda ) };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto At( i64 index ) const -> const ValueType&
	{
		BC_ContainerAssert( index < ValueCount, U"Index out of range" );
		return data[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto At( i64 index ) -> ValueType&
	{
		BC_ContainerAssert( index < ValueCount, U"Index out of range" );
		return data[ index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Data() const noexcept -> const ValueType*
	{
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Data() noexcept -> ValueType*
	{
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Size() const noexcept -> i64
	{
		return ValueCount;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto IsEmpty() const noexcept -> bool
	{
		return !Size();
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
		return IteratorBase<IsDataConst> { this, &this->data[ 0 ] };
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
		return ConstIterator { this, &this->data[ 0 ] };
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
	constexpr auto end() noexcept 
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
	constexpr auto end() const noexcept -> ConstIterator
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
	constexpr auto cend() const noexcept -> ConstIterator
	{
		return this->end();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Fills this container with template parameter pack.
	///
	/// @tparam Index
	/// Index of the current element in the parameter pack.
	///
	/// @tparam FirstType
	/// Type of the first element in the parameter pack.
	///
	/// @tparam RestTypePack
	/// Parameter pack for types sent to the constructor of the element.
	///
	/// @param first
	/// First element in the parameter pack.
	///
	/// @param ...rest
	/// Remaining elements in the parameter pack.
	template<
		i64									Index,
		std::constructible_from<ValueType>	FirstType,
		std::constructible_from<ValueType>	...RestTypePack
	>
	constexpr void FillFromTemplateParameterPack(
		FirstType&&		first,
		RestTypePack&&	...rest
	)
	{
		static_assert( Index < ValueCount, "Index out of range" );
		data[ Index ] = std::forward<FirstType>( first );
		if constexpr( sizeof...( RestTypePack ) ) FillFromTemplateParameterPack<Index + 1>( std::forward<RestTypePack>( rest )... );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType data[ ValueCount ];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Linear container similar to std::array.
///
/// @warning
/// Constructors of the contained type should not throw. If you need to construct a class or struct which constructor can throw,
/// look into heap allocating single-value containers like UniquePtr which can handle constructor throw.
///
/// @tparam ValueType
/// Type of the contained element.
template<BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( Array )<ValueType, 0> : public container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= ValueType;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
	using ThisContainerType					= BC_CONTAINER_NAME( Array )<OtherValueType, OtherValueCount>;
	using ThisType							= ThisContainerType<ValueType, 0>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Array )<OtherValueType, OtherValueCount>;
	using ThisFullType						= ThisContainerFullType<ValueType, 0>;

	template<bool IsConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<ThisType, IsConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ValueType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	//template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount, bool IsOtherConst>
	//friend class BC_CONTAINER_NAME( ArrayViewBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherValueType, i64 OtherValueCount>
	friend class BC_CONTAINER_NAME( Array );

	friend ConstIterator;
	friend Iterator;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )( const BC_CONTAINER_NAME( Array )& other ) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Array )( BC_CONTAINER_NAME( Array )&& other ) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( Array )() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const BC_CONTAINER_NAME( Array )& other ) -> BC_CONTAINER_NAME( Array )& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( BC_CONTAINER_NAME( Array )&& other ) -> BC_CONTAINER_NAME( Array )& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Front() const -> const ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Front() -> ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Back() const -> const ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Back() -> ValueType&
	{
		return At( 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) const -> const ValueType&
	{
		return At( index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) -> ValueType&
	{
		return At( index );
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
	constexpr auto operator==( const BC_CONTAINER_NAME( Array )& other ) const BC_CONTAINER_NOEXCEPT -> bool
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
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
	constexpr auto operator!=( const BC_CONTAINER_NAME( Array )& other ) const BC_CONTAINER_NOEXCEPT -> bool
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
	constexpr auto Find( const ValueType& value ) const BC_CONTAINER_NOEXCEPT -> ConstIterator
	{
		return ConstIterator { this, nullptr };
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
	constexpr auto Find( const ValueType& value ) BC_CONTAINER_NOEXCEPT -> Iterator
	{
		return Iterator { this, nullptr };
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
	constexpr auto FindIf( LambdaType&& lambda ) const BC_CONTAINER_NOEXCEPT -> ConstIterator
	{
		return ConstIterator { this, nullptr };
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
	constexpr auto FindIf( LambdaType&& lambda ) BC_CONTAINER_NOEXCEPT -> Iterator
	{
		return Iterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[noreturn]]
	constexpr auto At( i64 ) const -> const ValueType&
	{
		BC_ContainerThrowOrCrash( U"Index out of range, Array is empty" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[noreturn]]
	constexpr auto At( i64 ) -> ValueType&
	{
		BC_ContainerThrowOrCrash( U"Index out of range, Array is empty" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Data() const noexcept -> const ValueType*
	{
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Data() noexcept -> ValueType*
	{
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto Size() const noexcept -> i64
	{
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto IsEmpty() const noexcept -> bool
	{
		return !Size();
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
		return IteratorBase<IsDataConst> { this, nullptr };
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
		return ConstIterator { this, nullptr };
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
		return IteratorBase<IsDataConst> { this, nullptr };
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
		return ConstIterator { this, nullptr };
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
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Array )<u32, 5> ) == sizeof( u32 ) * 5 );
static_assert( sizeof( BC_CONTAINER_NAME( Array )<u32, 1> ) == sizeof( u32 ) * 1 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( utility::ContainerView<BC_CONTAINER_NAME( Array )<u32, 4>> );
static_assert( utility::ContainerEditableView<BC_CONTAINER_NAME( Array )<u32, 4>> );
static_assert( utility::Container<BC_CONTAINER_NAME( Array )<u32, 4>> );

static_assert( utility::LinearContainerView<BC_CONTAINER_NAME( Array )<u32, 4>> );
static_assert( utility::LinearContainerEditableView<BC_CONTAINER_NAME( Array )<u32, 4>> );
static_assert( utility::LinearContainer<BC_CONTAINER_NAME( Array )<u32, 4>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( Array )<char32_t, 4>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( Array )<char32_t, 4>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( Array )<char32_t, 4>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
