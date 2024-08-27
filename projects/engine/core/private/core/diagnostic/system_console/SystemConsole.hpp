#pragma once

#include <core/conversion/text/utf/UTFConversion.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/memory/pod_auto_buffer/PODAutoBuffer.hpp>



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
) noexcept;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Setup console settings at startup.
///
/// @param characters_per_line_num
/// Maximum number of characters per line before wrapping around to the next line, this can be used to make the console window
/// more readable, however, the console window does not expand to a new size, rather a scroll bar appears on the bottom of the
/// console window.
void SetupSystemConsole( i32 characters_per_line_num ) noexcept;



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
void SystemConsolePrint( const PrintRecord& print_record ) noexcept;



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
	auto DoPrint = [ foreground_color, background_color ]( const c8* text, i64 text_length )
		{
			internal_::SystemConsolePrintRawUTF8(
				text,
				text_length,
				foreground_color,
				background_color
			);
		};
	if constexpr( std::is_same_v<char8_t, typename ContainerType::ContainedCharacterType> || std::is_same_v<char, typename ContainerType::ContainedCharacterType> )
	{
		// char8_t and char can be printed directly without conversion.
		DoPrint( static_cast<const c8*>( text.Data() ), text.Size() );
	}
	else
	{
		// char16_t and char32_t must be converted into UTF8 first.
		auto buffer = memory::PODAutoBuffer<c8>( text.Size() * 6 );
		auto conversion_result = conversion::UTFConvert( buffer.Data(), buffer.Size(), text.Data(), text.Size() );

		if( conversion_result.status != conversion::UTFConvertResult::Status::SUCCESS )
		{
			assert( 0 && "UTFConvert failed while trying to print text to system console." );
			constexpr c8 error_msg[] = u8"[UTFConvert failed while trying to print text to system console]";
			constexpr i64 error_msg_length = sizeof( error_msg ) - 1;
			DoPrint( error_msg, error_msg_length );
			return;
		}

		DoPrint( buffer.Data(), conversion_result.code_unit_count );
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
/// @param c_string
///	Message you wish to print to the console window.
/// 
/// @param foreground_color
///	Color of the message.
/// 
/// @param background_color
///	Color of the background of the letters you wish to print. ( not entire console window )
template<
	utility::TextCharacter	CharacterType,
	i64						CharacterArraySize>
void SystemConsolePrint(
	const CharacterType( &c_string )[ CharacterArraySize ],
	PrintRecordColor		foreground_color					= PrintRecordColor::DEFAULT,
	PrintRecordColor		background_color					= PrintRecordColor::DEFAULT
)
{
	static_assert( CharacterArraySize > 0 );
	SystemConsolePrint(
		c_string,
		CharacterArraySize ? ( c_string[ CharacterArraySize - 1 ] == '\0' ? CharacterArraySize - 1 : CharacterArraySize ) : 0,
		foreground_color,
		background_color
	);
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
/// @param c_string
///	Message you wish to print to the console window.
/// 
/// @param theme
///	Text and background color of the message according to a theme.
template<
	utility::TextCharacter	CharacterType,
	i64						CharacterArraySize
>
void SystemConsolePrint(
	const CharacterType( &c_string )[ CharacterArraySize ],
	PrintRecordTheme theme
)
{
	static_assert( CharacterArraySize > 0 );
	auto [foreground_color, background_color] = GetPrintRecordThemeColors( theme );
	SystemConsolePrint( c_string, foreground_color, background_color );
}



} // diagnostic
} // bc
