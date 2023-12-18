
#include "ContainerBase.hpp"

#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include "ContainerImplAddDefinitions.hpp"



namespace bc {



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
	constexpr BC_CONTAINER_NAME( Pair )(
		const BC_CONTAINER_NAME( Pair )																&	other
	) BC_CONTAINER_NOEXCEPT = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		BC_CONTAINER_NAME( Pair )																	&&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		const FirstType																				&	first,
		const SecondType																			&	second
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<FirstType> && std::is_copy_constructible_v<SecondType> ) :
		first( first ),
		second( second )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		const FirstType																				&	first,
		SecondType																					&&	second
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<FirstType> && std::is_move_constructible_v<SecondType> ) :
		first( first ),
		second( std::move( second ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		FirstType																					&&	first,
		const SecondType																			&	second
	) BC_CONTAINER_NOEXCEPT requires( std::is_move_constructible_v<FirstType> && std::is_copy_constructible_v<SecondType> ) :
		first( std::move( first ) ),
		second( second )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )(
		FirstType																					&&	first,
		SecondType																					&&	second
	) noexcept requires( std::is_move_constructible_v<FirstType> && std::is_move_constructible_v<SecondType> ) :
		first( std::move( first ) ),
		second( std::move( second ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )																&	operator=(
		const BC_CONTAINER_NAME( Pair )																&	other
	) BC_CONTAINER_NOEXCEPT = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Pair )																&	operator=(
		BC_CONTAINER_NAME( Pair )																	&&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( Pair )																&	other
	) const noexcept
	{
		return this->first == other.first && this->second == other.second;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( Pair )																&	other
	) const noexcept
	{
		return this->first != other.first || this->second != other.second;
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if pair container fulfills size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Pair )<uint32_t, uint32_t> ) == ( sizeof( uint32_t ) + sizeof( uint32_t ) ) );



} // bc

#include "ContainerImplRemoveDefinitions.hpp"
