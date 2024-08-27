#pragma once

#include "TextFormatter.hpp"
#include <core/diagnostic/exception/Exception.hpp>
#include "specializations/PrimitiveTextFormatter.hpp"
#include <core/conversion/text/primitives/StringToPrimitiveConversion.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>



namespace bc {
namespace text {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainerView TextContainerType>
constexpr void TextFormat_ArgumentParser(
	i64											current_argument,
	i64											requested_argument,
	typename TextContainerType::ThisFullType&	out,
	TextContainerType							parse_text
)
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	TextContainerType,
	typename					FirstType,
	typename					...RestTypePack
>
constexpr void TextFormat_ArgumentParser(
	i64											current_argument,
	i64											requested_argument,
	typename TextContainerType::ThisFullType&	out,
	TextContainerType							parse_text,
	const FirstType&							first,
	RestTypePack&&								...rest
)
{
	if( current_argument == requested_argument ) {
		TextFormatter<TextContainerType, FirstType> formatter;
		formatter.Parse( parse_text );
		formatter.Format( out, first );
	} else {
		TextFormat_ArgumentParser(
			current_argument + 1,
			requested_argument,
			out,
			parse_text,
			std::forward<RestTypePack>( rest )...
		);
	}
};



template<utility::TextContainerView TextContainerType>
constexpr auto TextFormat_ConvertTextToUTF32( const TextContainerType &in ) -> typename TextContainerType::template ThisContainerFullType<c32>
{
	using TextFullType32 = typename TextContainerType::template ThisContainerFullType<c32>;

	auto out = TextFullType32 {};
	out.Resize( in.Size() );
	auto conversion_result = conversion::UTFConvert<c32>(
		out.Data(),
		out.Data() + out.Size(),
		in.Data(),
		in.Data() + in.Size()
	);
	out.Resize( conversion_result.code_unit_count );
	if( conversion_result.outcome != conversion::UTFConvertResult::Outcome::SUCCESS )
	{
		// We should practically never get here.
		out = U"[Internal error, text format failed to convert text to UTF-32]";
	}

	return out;
}



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Used to format text similarly to std::format.
///
/// @note
/// Throws an exception if something went wrong.
///
///	Examples:
///
///		// Ordering
///		TextFormat( U"Example {}", "a" );					// -> U"Example a"
///		TextFormat( U"Example {} {}", "a", "b" );			// -> U"Example a b"
///		TextFormat( U"Example {0} {0}", "a" );				// -> U"Example a a"
///		TextFormat( U"Example {0} {1}", "a", "b" );			// -> U"Example a b"
///		TextFormat( U"Example {1} {0}", "a", "b" );			// -> U"Example b a"
///		TextFormat( U"Example {} {2}", "a", "b", "c" );		// -> U"Example a c"
///		TextFormat( U"Example {1} {}", "a", "b", "c" );		// -> U"Example b c"
///
///		// Per type formatting - integral:
///		// Binary
///		TextFormat( U"{:b}", 222 );							// -> U"11011110"
///
///		// Octal
///		TextFormat( U"{:o}", 222 );							// -> U"336"
///
///		// Hex
///		TextFormat( U"{:x}", 222 );							// -> U"DE"
///
///		// Zero fill
///		TextFormat( U"{:z8}", 222 );						// -> U"00000222"
///
///		// Binary with zero fill
///		TextFormat( U"{:bz16}", 222 );						// -> U"0000000011011110"
///		
///		// Per type formatting - float:
///		// decimal points
///		TextFormat( U"{.3}", 0.1f );						// -> U"0.100"
///
///		// Per type formatting - bool:
///		TextFormat( U"{}", true );							// -> U"true"
///		TextFormat( U"{:b}", true );						// -> U"1"
///
///		// Different types support different formatting options, see TextFormatter class and it's
///		// specialization for each different type to learn more.
/// 
/// @tparam CharacterType
///	Text character type we wish to use when formatting text, default = char32_t.
/// 
/// @tparam ...ArgumentsTypePack
///	Argument types of values passed to be used as parsed values inside the formatted text.
/// 
/// @param format_text
///	Format text printed out and used for format control.
/// 
/// @param ...args
///	Arguments passed to be used as parsed values inside the formatted text.
///				
/// @return
/// New formatted text object.
template<
	utility::TextContainerView	TextContainerType,
	typename					...ArgumentsTypePack
>
constexpr auto TextFormat(
	const TextContainerType&	format_text,
	ArgumentsTypePack&&			...args
) -> typename TextContainerType::ThisFullType
{
	using TextFullType		= typename TextContainerType::ThisFullType;
	using TextFullType32	= typename TextContainerType::template ThisContainerFullType<c32>;
	using TextViewType		= typename TextContainerType::template ThisViewType<true>;
	using TextCharacterType	= typename TextContainerType::ContainedCharacterType;

	constexpr i64 args_count = sizeof...( ArgumentsTypePack );
	TextFullType out_buffer;
	out_buffer.Reserve( 256 );

	i64 current_argument = 0;
	TextViewType argument_parse_text;

	auto it = format_text.begin();
	while( it != format_text.end() )
	{
		if( *it == TextCharacterType( '{' ) )
		{
			// Enter formatted area

			++it;
			while( *it != TextCharacterType( '}' ) )
			{
				if( *it >= TextCharacterType( '0' ) && *it <= TextCharacterType( '9' ) )
				{
					auto arg_number_text = format_text.SubText( it );
					auto conversion_result = conversion::StringToInteger(
						current_argument,
						arg_number_text.Data(),
						arg_number_text.Data() + arg_number_text.Size()
					);
					it += conversion_result.length;
					if( conversion_result.outcome != conversion::StringToPrimitiveResult::Outcome::SUCCESS )
					{
						auto message_print_record = diagnostic::MakePrintRecord(
							U"Error while formatting text, missing \"}\""
						);
						message_print_record += diagnostic::MakePrintRecord_Argument(
							U"format text",
							internal_::TextFormat_ConvertTextToUTF32( format_text )
						);
						diagnostic::Throw( diagnostic::Exception( message_print_record ) );
					}
					continue;
				}
				else if( *it == ':' )
				{
					// Formatted
					++it;
					auto begin = it;

					bool found_end = false;
					while( it != format_text.end() )
					{
						if( *it == TextCharacterType( '}' ) )
						{
							found_end = true;
							break;
						}
						++it;
					}

					if( !found_end )
					{
						auto message_print_record = diagnostic::MakePrintRecord(
							U"Error while formatting text, missing \"}\""
						);
						message_print_record += diagnostic::MakePrintRecord_Argument(
							U"format text",
							internal_::TextFormat_ConvertTextToUTF32( format_text )
						);
						diagnostic::Throw( diagnostic::Exception( message_print_record ) );
					}

					auto end = it;
					argument_parse_text = TextViewType { begin, end };
					continue;
				}
				else if( *it == ' ' )
				{
					// Allow spaces in formatting
					++it;
					continue;
				}
				else
				{
					auto message_print_record = diagnostic::MakePrintRecord(
						U"Error while formatting text, \"{\" must be followed by a number, \":\" or \"}\""
					);
					message_print_record += diagnostic::MakePrintRecord_Argument(
						U"format text",
						internal_::TextFormat_ConvertTextToUTF32( format_text )
					);
					diagnostic::Throw( diagnostic::Exception( message_print_record ) );
				}
			}
			if( current_argument >= args_count )
			{
				auto message_print_record = diagnostic::MakePrintRecord( U"Error while formatting text, text argument number out of range" );
				message_print_record += diagnostic::MakePrintRecord_ArgumentList(
					U"Tried accessing argument", current_argument,
					U"Number of arguments", args_count
				);
				diagnostic::Throw( diagnostic::Exception( message_print_record ) );
			}

			// Do formatting.
			internal_::TextFormat_ArgumentParser(
				0,
				current_argument,
				out_buffer,
				argument_parse_text,
				std::forward<ArgumentsTypePack>( args )...
			);
			argument_parse_text = {};

			++current_argument;
		}
		else
		{
			// Output format string directly.
			out_buffer.PushBack( *it );
		}

		++it;
	}
	return out_buffer;
}



} // text
} // bc
