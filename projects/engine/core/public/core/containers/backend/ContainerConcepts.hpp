#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <type_traits>



namespace bc {
namespace container_bases {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Type>
concept ContainerIterator = requires(
	Type											type,
	const typename Type::IteratorContainerType	*	container,
	const typename Type::ContainedValueType		*	data
)
{
	typename Type::ContainedValueType;
	typename Type::IteratorContainerType;

	container = type.GetContainer();
	data = type.GetData();
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for accepted text container character types.
///
///	Used with generic programming to only allow certain character types.
///
/// @tparam CharacterType
///	Type to test if it is allowed to be used as a text container character type.
template<typename CharacterType>
concept TextContainerCharacterType =
std::is_same_v<CharacterType, char> ||
std::is_same_v<CharacterType, char8_t> ||
std::is_same_v<CharacterType, char16_t> ||
std::is_same_v<CharacterType, char32_t>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for types that are allowed in simple containers.
///
/// Used with generic programming to determine which types are allowed in containers which name starts with "Simple".
///
/// @tparam ValueType
/// Type to test if it is allowed to be used in simple containers.
template<typename ValueType>
concept SimpleContainerAllowedValueType =
( !std::is_default_constructible_v<ValueType>		|| std::is_nothrow_default_constructible_v<ValueType> ) &&
( !std::is_destructible_v<ValueType>				|| std::is_nothrow_destructible_v<ValueType> ) &&
( !std::is_copy_constructible_v<ValueType>			|| std::is_nothrow_copy_constructible_v<ValueType> ) &&
( !std::is_move_constructible_v<ValueType>			|| std::is_nothrow_move_constructible_v<ValueType> ) &&
( !std::is_swappable_v<ValueType>					|| std::is_nothrow_swappable_v<ValueType> );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for stl containers.
///
///	Used with generic programming to determine if type is an stl container.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept STLContainer = requires(
	ContainerType										container,
	typename ContainerType::const_iterator				const_iterator,
	bool												boolean
	)
{
	const_iterator	= container.begin();
	const_iterator	= container.end();
	boolean			= container.empty();
	container.swap( container );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for any Bitcrafte container view type.
///
///	Used with generic programming to determine if type is any of the Bitcrafte container view types.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept ContainerView = requires(
	ContainerType											container,
	size_t													size,
	typename ContainerType::ConstIterator					const_iterator
	)
{
	typename ContainerType::Base;

	//typename ContainerType::IteratorBase;
	typename ContainerType::Iterator;
	typename ContainerType::ConstIterator;

	typename ContainerType::ContainedValueType;

	typename ContainerType::value_type;	// For stl compatibility.

	//typename ContainerType::ThisContainerType;
	typename ContainerType::ThisType;
	//typename ContainerType::ThisContainerViewType;
	typename ContainerType::template ThisViewType<true>;
	//typename ContainerType::ThisContainerFullType;
	typename ContainerType::ThisFullType;

	//container = ContainerType {};
	//container = ContainerType( container );
	//container = ContainerType( std::move( container ) );

	//container = container_2;
	//container = std::move( container );

	size = container.Size();

	const_iterator = container.begin();
	const_iterator = container.end();
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for iteratable container
///
///	Used with generic programming to determine if type is any of the Bitcrafte container editable view types.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept ContainerEditableView = requires(
	ContainerType											container
	)
{
	requires( ContainerView<ContainerType> );
	requires( ContainerType::IsDataConst == false );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for any Bitcrafte container type.
///
///	Used with generic programming to determine if type is any of the Bitcrafte container types.
///
/// @tparam ContainerType
/// Generic container type
template<typename ContainerType>
concept Container = requires(
	ContainerType											container
	)
{
	requires( ContainerEditableView<ContainerType> );
	requires( std::is_same_v<ContainerType, typename ContainerType::ThisFullType> );

	//container = ContainerType( view );
	//container = ContainerType( editable_view );
	//container = view;
	//container = editable_view;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for container to determine if it is linear.
///
///	Used with generic programming to determine if container view is linear.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept LinearContainerView = requires(
	ContainerType											container,
	const typename ContainerType::ContainedValueType	*	const_data
	)
{
	requires( ContainerView<ContainerType> );

	const_data = container.Data();
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for container to determine if it is linear.
///
///	Used with generic programming to determine if container editable view is linear.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept LinearContainerEditableView = requires(
	ContainerType											container,
	typename ContainerType::ContainedValueType			*	data
	)
{
	requires( LinearContainerView<ContainerType> );
	requires( ContainerEditableView<ContainerType> );

	data = container.Data();
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for container to determine if it is linear.
///
///	Used with generic programming to determine if container is linear.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept LinearContainer = requires(
	ContainerType											container,
	size_t													size
)
{
	requires( LinearContainerEditableView<ContainerType> );
	requires( Container<ContainerType> );

	container.Reserve( size );
	container.Resize( size );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for text view container.
///
///	Used with generic programming to determine if type is a text view container.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept TextContainerView = requires(
	ContainerType											container
)
{
	requires( LinearContainerView<ContainerType> );

	typename ContainerType::ContainedCharacterType;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for text editable view container.
///
///	Used with generic programming to determine if type is a text editable view container.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept TextContainerEditableView = requires(
	ContainerType											container
	)
{
	requires( LinearContainerEditableView<ContainerType> );
	requires( TextContainerView<ContainerType> );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept for text container.
///
///	Used with generic programming to determine if type is a text container.
///
/// @tparam ContainerType
///	Generic container type
template<typename ContainerType>
concept TextContainer = requires(
	ContainerType											container,
	size_t													size
	)
{
	requires( LinearContainer<ContainerType> );
	requires( TextContainerEditableView<ContainerType> );

	container = ContainerType( "text" );
	container = "text";
	container = container += "text";
	container = container += "text";
	container.Append( "text" );
	container.Append( "text", size );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
///	Used to disallow conversion from const to non-const. Allows all other conversions.
///
/// @tparam IsDestinationConst
///	Destination constness to what we're trying to convert to.
/// 
/// @tparam IsSourceConst
///	Source constness we're trying to convert from.
template<
	bool IsDestinationConst,
	bool IsSourceConst
>
concept IsConstConvertible =
IsDestinationConst == true ||
IsDestinationConst == IsSourceConst;

static_assert( IsConstConvertible<true, true> );
static_assert( !IsConstConvertible<false, true> );
static_assert( IsConstConvertible<true, false> );
static_assert( IsConstConvertible<false, false> );



} // container_bases
} // bc
