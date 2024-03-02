#pragma once

#include "TextFormatCore.hpp"
#include <core/containers/simple/SimpleText.hpp>



namespace bc {
namespace text {



template<
	utility::TextContainerCharacterType									CharacterType,
	u64																	CharacterArraySize,
	typename															...ArgumentsTypePack
>
constexpr bc::internal_::SimpleTextBase<CharacterType>					TextFormat(
	const CharacterType ( &format_text )[ CharacterArraySize ],
	ArgumentsTypePack												&&	...arguments
)
{
	return TextFormat( bc::internal_::SimpleTextViewBase<CharacterType, true>( format_text, CharacterArraySize ), std::forward<ArgumentsTypePack>( arguments )... );
}



} // text
} // bc
