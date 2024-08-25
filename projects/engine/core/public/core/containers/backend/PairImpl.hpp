
#include <core/containers/backend/ContainerBase.hpp>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
BC_CONTAINER_NAMESPACE_START;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BC_CONTAINER_VALUE_TYPENAME FirstType, BC_CONTAINER_VALUE_TYPENAME SecondType>
class BC_CONTAINER_NAME( Pair )
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using FirstContainedType				= FirstType;
	using SecondContainedType				= SecondType;
	using ContainedKeyType					= FirstType;
	using ContainedValueType				= SecondType;

	template<BC_CONTAINER_VALUE_TYPENAME OtherFirstType, BC_CONTAINER_VALUE_TYPENAME OtherSecondType>
	using ThisContainerType					= BC_CONTAINER_NAME( Pair )<OtherFirstType, OtherSecondType>;
	using ThisType							= ThisContainerType<FirstType, SecondType>;

	using ThisContainerViewType				= void;
	using ThisViewType						= void;

	template<BC_CONTAINER_VALUE_TYPENAME OtherFirstType, BC_CONTAINER_VALUE_TYPENAME OtherSecondType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Pair )<OtherFirstType, OtherSecondType>;
	using ThisFullType						= ThisContainerFullType<FirstType, SecondType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FirstType																							first;
	SecondType																							second;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )() BC_CONTAINER_NOEXCEPT = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )( const ThisType& other ) BC_CONTAINER_NOEXCEPT = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )( ThisType&&	other ) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		const FirstType&	first,
		const SecondType&	second
	) BC_CONTAINER_NOEXCEPT
		requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<FirstType> && BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<SecondType> )
	:
		first( first ),
		second( second )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		const FirstType&	first,
		SecondType&&		second
	) BC_CONTAINER_NOEXCEPT
		requires( BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<FirstType> && BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<SecondType> )
	:
		first( first ),
		second( std::move( second ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		FirstType&&			first,
		const SecondType&	second
	) BC_CONTAINER_NOEXCEPT
		requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<FirstType> && BC_CONTAINER_IS_COPY_CONSTRUCTIBLE<SecondType> )
	:
		first( std::move( first ) ),
		second( second )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		FirstType&&			first,
		SecondType&&		second
	) noexcept
		requires( BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<FirstType> && BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE<SecondType> )
	:
		first( std::move( first ) ),
		second( std::move( second ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ThisType& other ) BC_CONTAINER_NOEXCEPT -> ThisType& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ThisType&& other ) noexcept -> ThisType& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator==( const ThisType& other ) const noexcept -> bool
	{
		return this->first == other.first && this->second == other.second;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator!=( const ThisType& other ) const noexcept -> bool
	{
		return this->first != other.first || this->second != other.second;
	}
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if pair container fulfills size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Pair )<u32, u32> ) == ( sizeof( u32 ) + sizeof( u32 ) ) );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
