#pragma once

#include <core/conversion/text/utf/UTFConversion.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/containers/simple/SimpleText.hpp>



namespace bc {
namespace diagnostic {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console from UTF8 formatted text.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @param text
///	Message you wish to print to the console window.
/// 
/// @param foreground_color
///	Color of the message.
/// 
/// @param background_color
///	Color of the background of the letters you wish to print. ( not entire console window )
void SystemConsolePrintRawUTF8(
	const char8_t*		raw_text,
	i64					raw_text_length,
	PrintRecordColor	foreground_color		= PrintRecordColor::DEFAULT,
	PrintRecordColor	background_color		= PrintRecordColor::DEFAULT
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Setup console settings at startup.
///
/// @param characters_per_line_num
/// Maximum number of characters per line before wrapping around to the next line, this can be used to make the console window
/// more readable, however, the console window does not expand to a new size, rather a scroll bar appears on the bottom of the
/// console window.
void SetupSystemConsole( i32 characters_per_line_num );



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @param print_record
/// Print record which to print to the console window.
void SystemConsolePrint( const PrintRecord& print_record );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @tparam ContainerType
///	Type of a text container.
/// 
/// @param text
///	Message you wish to print to the console window.
/// 
/// @param foreground_color
///	Color of the message.
/// 
/// @param background_color
///	Color of the background of the letters you wish to print. ( not entire console window )
template<utility::TextContainerView ContainerType>
void SystemConsolePrint(
	const ContainerType&	text,
	PrintRecordColor		foreground_color	= PrintRecordColor::DEFAULT,
	PrintRecordColor		background_color	= PrintRecordColor::DEFAULT
)
{
	if constexpr( std::is_same_v<char8_t, typename ContainerType::ContainedCharacterType> || std::is_same_v<char, typename ContainerType::ContainedCharacterType> ) {
		// char8_t and char can be printed directly without conversion.
		internal_::SystemConsolePrintRawUTF8(
			text.Data(),
			text.Size(),
			foreground_color,
			background_color
		);

	} else {
		// char16_t and char32_t must be converted into UTF8 first.
		auto buffer = conversion::ToUTF8( text );
		internal_::SystemConsolePrintRawUTF8(
			buffer.Data(),
			buffer.Size(),
			foreground_color,
			background_color
		);
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @tparam ContainerType
///	Type of a text container.
/// 
/// @param text
///	Message you wish to print to the console window.
/// 
/// @param theme
///	Text and background color of the message according to a theme.
template<utility::TextContainerView ContainerType>
void SystemConsolePrint(
	const ContainerType&	text,
	PrintRecordTheme		theme
)
{
	auto [foreground_color, background_color] = GetPrintRecordThemeColors( theme );
	SystemConsolePrint( text, foreground_color, background_color );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @tparam CharacterType
///	Type of a single character.
///
/// @tparam CharacterArraySize
/// Size of character array. This is automatically deduced.
/// 
/// @param text
///	Message you wish to print to the console window.
/// 
/// @param foreground_color
///	Color of the message.
/// 
/// @param background_color
///	Color of the background of the letters you wish to print. ( not entire console window )
template<utility::TextContainerCharacterType CharacterType, u64 CharacterArraySize>
void SystemConsolePrint(
	const CharacterType		( &text )[ CharacterArraySize ],
	PrintRecordColor		foreground_color					= PrintRecordColor::DEFAULT,
	PrintRecordColor		background_color					= PrintRecordColor::DEFAULT
)
{
	static_assert( CharacterArraySize > 0 );
	SystemConsolePrint( bc::internal_::SimpleTextViewBase<CharacterType, true>( text, CharacterArraySize ), foreground_color, background_color );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print a message to the console.
///
///	Usually used for debugging only as in the final application the console is not usually visible.
/// 
/// @note
/// Multithreading: Any thread.
///
/// @tparam CharacterType
///	Type of a single character.
///
/// @tparam CharacterArraySize
/// Size of character array. This is automatically deduced.
/// 
/// @param text
///	Message you wish to print to the console window.
/// 
/// @param theme
///	Text and background color of the message according to a theme.
template<utility::TextContainerCharacterType CharacterType, u64 CharacterArraySize>
void SystemConsolePrint(
	const CharacterType		( &text )[ CharacterArraySize ],
	PrintRecordTheme		theme
)
{
	static_assert( CharacterArraySize > 0 );
	SystemConsolePrint( bc::internal_::SimpleTextViewBase<CharacterType, true> { text, CharacterArraySize }, theme );
}



} // diagnostic
} // bc
