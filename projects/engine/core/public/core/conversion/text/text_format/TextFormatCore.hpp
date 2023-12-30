#pragma once

#include "TextFormatter.hpp"
#include <core/diagnostic/exception/Exception.hpp>
#include "specializations/PrimitiveTextFormatter.hpp"
#include <core/conversion/text/primitives/TextToPrimitiveConversion.hpp>



namespace bc {
namespace text {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainerView TextContainerType>
constexpr void															TextFormat_Collector(
	size_t																current_argument,
	size_t																requested_argument,
	typename TextContainerType::ThisFullType						&	out,
	TextContainerType													parse_text
)
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView											TextContainerType,
	typename															FirstType,
	typename															...RestTypePack
>
constexpr void															TextFormat_Collector(
	size_t																current_argument,
	size_t																requested_argument,
	typename TextContainerType::ThisFullType						&	out,
	TextContainerType													parse_text,
	const FirstType													&	first,
	RestTypePack													&&	...rest
)
{
	if( current_argument == requested_argument ) {
		TextFormatter<TextContainerType, FirstType> formatter;
		formatter.Parse( parse_text );
		formatter.Format( out, first );
	} else {
		TextFormat_Collector(
			current_argument + 1,
			requested_argument,
			out,
			parse_text,
			std::forward<RestTypePack>( rest )...
		);
	}
};



} // internal



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
	utility::TextContainerView											TextContainerType,
	typename															...ArgumentsTypePack
>
constexpr typename TextContainerType::ThisFullType						TextFormat(
	const TextContainerType											&	format_text,
	ArgumentsTypePack												&&	...args
)
{
	using TextContainerFullType = typename TextContainerType::ThisFullType;
	using TextContainerConstViewType = typename TextContainerType::template ThisViewType<true>;
	using TextContainerCharacterType = typename TextContainerType::ContainedCharacterType;

	constexpr size_t args_count = sizeof...( ArgumentsTypePack );
	TextContainerFullType out_buffer;
	out_buffer.Reserve( 256 );

	size_t current_argument = 0;
	TextContainerConstViewType argument_parse_text;

	// TODO: Get rid of std::from_chars and std::isdigit inside TextFormat function
	// to lower dependencies to libraries, use our own primitive conversion instead.

	auto it = format_text.begin();
	while( it != format_text.end() )
	{

		if( *it == TextContainerCharacterType( '{' ) )
		{
			// Enter formatted area

			++it;
			while( *it != TextContainerCharacterType( '}' ) )
			{
				if( *it >= TextContainerCharacterType( '0' ) && *it <= TextContainerCharacterType( '9' ) )
				{
					auto arg_number_end_it = it;
					bool found_end = false;

					while( arg_number_end_it != format_text.end() )
					{
						if( *arg_number_end_it == TextContainerCharacterType( ':' ) )
						{
							found_end = true;
							break;

						}
						else if( *arg_number_end_it == TextContainerCharacterType( '}' ) )
						{
							found_end = true;
							break;
						}
						++arg_number_end_it;
					}
					BAssert( found_end,
						TextContainerFullType( "Error while formatting text, missing \"}\", format text: \"" ) +
						format_text + "\""
					);
					it += conversion::TextToPrimitive( current_argument, format_text.SubText( it, arg_number_end_it ) );
				}
				else if( *it == ':' )
				{
					// Formatted
					++it;
					auto begin = it;

					bool found_end = false;
					while( it != format_text.end() )
					{
						if( *it == TextContainerCharacterType( '}' ) )
						{
							found_end = true;
							break;
						}
						++it;
					}

					BAssert( found_end,
						TextContainerFullType( "Error while formatting text, missing \"}\", format text: \"" ) +
						format_text + "\""
					);

					auto end = it;
					argument_parse_text = TextContainerConstViewType { begin, end };
				}
				else if( *it == ' ' )
				{
					// Allow spaces in formatting
					++it;
				}
				else
				{
					diagnostic::Throw(
						TextContainerFullType( "Error while formatting text, \"{\" must be followed by a number, \":\" or \"}\", format text: \"" ) +
						format_text + "\""
					);
				}
			}
			// Do formatting.
			if( current_argument >= args_count )
			{
				auto current_argument_as_text = TextContainerFullType {};
				auto argument_count_as_text = TextContainerFullType {};
				conversion::PrimitiveToText<TextContainerFullType>( current_argument_as_text, current_argument );
				conversion::PrimitiveToText<TextContainerFullType>( argument_count_as_text, args_count );
				diagnostic::Throw( TextContainerFullType( "Error while formatting text, text argument number out of range, format text: \"" ) +
					"\".\nTried accessing argument :\"" + current_argument_as_text +
					"\".\nNumber of arguments: \"" + argument_count_as_text + "\""
				);
			}
			internal::TextFormat_Collector( 0, current_argument, out_buffer, argument_parse_text, std::forward<ArgumentsTypePack>( args )... );
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
