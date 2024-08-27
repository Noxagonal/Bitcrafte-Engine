#pragma once

#include "TextFormatCore.hpp"
#include <core/containers/Text.hpp>



// TODO: TextFormat() Doesn't use bc::Text because low level throwing functions use TextFormat, which means that bc::Text uses TextFormat.
//       bc::internal_::SimpleText is a temporary solution for now, it should be changed to bc::Text. This means that diagnostic utilities like
//       throwing, asserts, exceptions, console, logger, print record, etc. should not use TextFormat for anything, instead they should use
//       even lower level functions provided by the conversion utilities.



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter	CharacterType,
	i64						CharacterArraySize,
	typename				...ArgumentsTypePack
>
constexpr auto TextFormat(
	const CharacterType		( &format_text )[ CharacterArraySize ],
	ArgumentsTypePack&&		...arguments
) -> bc::TextBase<CharacterType>
{
	return TextFormat( bc::TextViewBase<CharacterType, true>( format_text, CharacterArraySize ), std::forward<ArgumentsTypePack>( arguments )... );
}



} // text
} // bc
