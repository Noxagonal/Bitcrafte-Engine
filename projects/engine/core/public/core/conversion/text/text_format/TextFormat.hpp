#pragma once

#include "TextFormatCore.hpp"
#include <core/containers/simple/SimpleText.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerCharacterType	CharacterType,
	i64									CharacterArraySize,
	typename							...ArgumentsTypePack
>
constexpr auto TextFormat(
	const CharacterType		( &format_text )[ CharacterArraySize ],
	ArgumentsTypePack&&		...arguments
) -> bc::internal_::SimpleTextBase<CharacterType>
{
	return TextFormat( bc::internal_::SimpleTextViewBase<CharacterType, true>( format_text, CharacterArraySize ), std::forward<ArgumentsTypePack>( arguments )... );
}



} // text
} // bc
