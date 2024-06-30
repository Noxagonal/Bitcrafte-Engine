#pragma once

#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/exception/Exception.hpp>
#include "../TextFormatter.hpp"
#include <core/conversion/text/primitives/PrimitiveToTextConversion.hpp>
#include <core/conversion/text/primitives/TextToPrimitiveConversion.hpp>
#include <core/conversion/text/utf/UTFConversion.hpp>
#include <core/diagnostic/assertion/Assert.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////
// Bitcrafte Text types
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	utility::TextContainerView				ValueTextContainerType
>
class TextFormatter<OutTextContainerType, ValueTextContainerType>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;
	using ValueTextCharacterType = typename ValueTextContainerType::ContainedCharacterType;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		if( !parse_text.IsEmpty() )
		{
			diagnostic::Throw(
				OutTextContainerFullType<c32>( U"Unsupported text format options while formatting text, unrecognized parse text: \"" ) +
				conversion::ToUTF32( parse_text ) + U"\""
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const ValueTextContainerType & in )
	{
		if constexpr( std::is_same_v<OutTextCharacterType, ValueTextCharacterType> )
		{
			out.Append( in );

		}
		else
		{
			if constexpr( std::is_same_v<OutTextCharacterType, char8_t> )
			{
				out.Append( conversion::ToUTF8( in ) );

			}
			else if constexpr( std::is_same_v<OutTextCharacterType, char16_t> )
			{
				out.Append( conversion::ToUTF16( in ) );

			}
			else if constexpr( std::is_same_v<OutTextCharacterType, char32_t> )
			{
				out.Append( conversion::ToUTF32( in ) );

			}
			else
			{
				diagnostic::Throw(
					U"Cannot format text, tried converting from an UTF text to char, Bitcrafte considers char to be ASCII and thus cannot handle UTF text"
				);
			}
		}
	}
};



////////////////////////////////////////////////////////////////
// C-string types
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterCStringType =
std::is_same_v<T, char> ||
std::is_same_v<T, wchar_t> ||
std::is_same_v<T, char8_t> ||
std::is_same_v<T, char16_t> ||
std::is_same_v<T, char32_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	FormatterCStringType					ValueTextType
>
class TextFormatter<OutTextContainerType, const ValueTextType*>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		if( !parse_text.IsEmpty() )
		{
			diagnostic::Throw(
				OutTextContainerFullType<c32>( U"Unsupported text format options while formatting c-style string, unrecognized parse text: \"" ) + conversion::ToUTF32( parse_text ) + "\""
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const ValueTextType * in )
	{
		while( *in != '\0' )
		{
			out.PushBack( OutTextCharacterType( *in ) );
			++in;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	FormatterCStringType					ValueTextType,
	u64										StringArraySize
>
class TextFormatter<OutTextContainerType, ValueTextType[ StringArraySize ]>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using InTextAsOutViewType = typename OutTextContainerType::template ThisContainerViewType<ValueTextType, true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;
	using ValueTextCharacterType = ValueTextType;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		if( !parse_text.IsEmpty() )
		{
			diagnostic::Throw(
				OutTextContainerFullType<c32>( U"Unsupported text format options while formatting c-style string, unrecognized parse text: \"" ) +
				conversion::ToUTF32( parse_text ) + U"\""
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const ValueTextType( &in )[ StringArraySize ] )
	{
		if constexpr( std::is_same_v<OutTextCharacterType, ValueTextCharacterType> )
		{
			out.Append( InTextAsOutViewType { in, StringArraySize } );

		}
		else
		{
			if constexpr( std::is_same_v<OutTextCharacterType, char8_t> )
			{
				out.Append( conversion::ToUTF8( InTextAsOutViewType { in, StringArraySize } ) );
			}
			else if constexpr( std::is_same_v<OutTextCharacterType, char16_t> )
			{
				out.Append( conversion::ToUTF16( InTextAsOutViewType { in, StringArraySize } ) );
			}
			else if constexpr( std::is_same_v<OutTextCharacterType, char32_t> )
			{
				out.Append( conversion::ToUTF32( InTextAsOutViewType { in, StringArraySize } ) );
			}
			else
			{
				diagnostic::Throw(
					OutTextContainerFullType<c32>( U"Cannot format text, tried converting from an UTF text to char, Bitcrafte considers char to be ASCII and thus cannot handle UTF text, input text: \"" ) +
					conversion::ToUTF32( InTextAsOutViewType { in, StringArraySize } ) + "\""
				);
			}
		}
	}
};



////////////////////////////////////////////////////////////////
// Number types
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterIntegerType =
std::is_same_v<T, i8> ||
std::is_same_v<T, u8> ||
std::is_same_v<T, i16> ||
std::is_same_v<T, u16> ||
std::is_same_v<T, i32> ||
std::is_same_v<T, u32> ||
std::is_same_v<T, i64> ||
std::is_same_v<T, u64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	FormatterIntegerType					IntegerType
>
class TextFormatter<OutTextContainerType, IntegerType>
{
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;

	conversion::IntegerToTextConversionFormat base = conversion::IntegerToTextConversionFormat::DECIMAL;
	i64 zero_fill = 0;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		auto it = parse_text.begin();
		auto end = parse_text.end();
		while( it != end )
		{
			if( *it == 'b' )
			{
				base = conversion::IntegerToTextConversionFormat::BINARY;
				++it;
			}
			else if( *it == 'o' )
			{
				base = conversion::IntegerToTextConversionFormat::OCTAL;
				++it;
			}
			else if( *it == 'x' )
			{
				base = conversion::IntegerToTextConversionFormat::HEX;
				++it;
			}
			else if( *it == 'z' )
			{
				++it;
				BAssert(
					it.IsValid(),
					OutTextContainerFullType<c32>( U"Unsupported text format options while formatting integer primitive type, "
						"invalid number format while parsing integer zero fill parse text, unrecognized parse text: \"" ) +
					conversion::ToUTF32( parse_text ) + U"\n"
				);
				BAssert( zero_fill <= 1024,
					OutTextContainerFullType<c32>( U"Unsupported text format options while formatting integer primitive type, "
						"too many zeroes requested from zero fill, current limit is 1024, unrecognized parse text: \"" ) +
					conversion::ToUTF32( parse_text ) + U"\n"
				);
				it += conversion::TextToPrimitive(
					zero_fill,
					parse_text.SubText( it, parse_text.end() ),
					conversion::TextToIntegerConversionFormat::DECIMAL
				);
			}
			else
			{
				diagnostic::Throw(
					OutTextContainerFullType<c32>( U"Unsupported text format options while formatting integer primitive type, unrecognized parse text: " ) +
					conversion::ToUTF32( parse_text ) + U"\n"
				);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const IntegerType & in )
	{
		if( zero_fill )
		{
			OutTextFullType buffer;
			conversion::PrimitiveToText( buffer, in, base );
			if( buffer.Size() < zero_fill )
			{
				u64 add_count = zero_fill - buffer.Size();
				for( u64 i = 0; i < add_count; ++i )
				{
					out.PushBack( '0' );
				}
			}
			out.Append( buffer );
		}
		else
		{
			conversion::PrimitiveToText( out, in, base );
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterFloatingType =
std::is_same_v<T, f32> ||
std::is_same_v<T, f64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	FormatterFloatingType					FloatType
>
class TextFormatter<OutTextContainerType, FloatType>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;

	conversion::FloatToTextConversionFormat float_to_text_conversion_format = conversion::FloatToTextConversionFormat::GENERAL;
	i64 requested_decimal_count = INT64_MAX;
	bool decimals_set = false;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		bool first_format_setting = true;
		auto it = parse_text.begin();
		auto end = parse_text.end();
		while( it != end )
		{
			if( *it == 'f' )
			{
				if( first_format_setting )
				{
					first_format_setting = false;
					float_to_text_conversion_format = {};
				}
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToTextConversionFormat::FIXED;

			}
			else if( *it == 's' )
			{
				if( first_format_setting )
				{
					first_format_setting = false;
					float_to_text_conversion_format = {};
				}
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToTextConversionFormat::SCIENTIFIC;

			}
			else if( *it == 'x' )
			{
				if( first_format_setting )
				{
					first_format_setting = false;
					float_to_text_conversion_format = {};
				}
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToTextConversionFormat::HEX;

			}
			else if( *it == '.' )
			{
				++it;
				it += conversion::TextToPrimitive(
					requested_decimal_count,
					parse_text.SubText( it, parse_text.end() ),
					conversion::TextToIntegerConversionFormat::DECIMAL
				);
				decimals_set = true;
				BAssert(
					it.IsValid(),
					OutTextContainerFullType<c32>( U"Unsupported text format options while formatting floating primitive type, "
						"invalid number format while parsing floating point decimal count, unrecognized parse text: \"" ) +
					conversion::ToUTF32( parse_text ) + U"\n"
				);
				BAssert( requested_decimal_count <= 1024,
					OutTextContainerFullType<c32>( U"Unsupported text format options while formatting integer primitive type, "
						"too many decimal numbers requested from floating point text parser, current limit is 1024, unrecognized parse text: \"" ) +
					conversion::ToUTF32( parse_text ) + U"\n"
				);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const FloatType & in )
	{
		OutTextFullType buffer;
		conversion::PrimitiveToText( buffer, in, float_to_text_conversion_format );

		auto remaining_decimals = requested_decimal_count;

		enum class ParseState
		{
			WHOLE,
			DECIMAL,
			DECIMAL_NOT_INCLUDED,
			REMAINDER,
		};
		ParseState parse_state = ParseState::WHOLE;
		for( u64 i = 0; i < buffer.Size(); ++i )
		{
			auto c = buffer[ i ];

			switch( parse_state )
			{
			case ParseState::WHOLE:
			{
				out.PushBack( c );
				if( c == '.' )
				{
					parse_state = ParseState::DECIMAL;
				}
				break;
			}

			case ParseState::DECIMAL:
			{
				if( c < '0' || c > '9' )
				{
					parse_state = ParseState::REMAINDER;
					if( decimals_set )
					{
						for( u64 d = 0; d < remaining_decimals; ++d )
						{
							out.PushBack( '0' );
						}
					}
					remaining_decimals = 0;
					out.PushBack( c );
					break;
				}

				if( remaining_decimals )
				{
					--remaining_decimals;
				}
				else
				{
					parse_state = ParseState::DECIMAL_NOT_INCLUDED;
				}
				out.PushBack( c );
				break;
			}

			case ParseState::DECIMAL_NOT_INCLUDED:
			{
				if( c <= '0' || c >= '9' )
				{
					out.PushBack( c );
					parse_state = ParseState::REMAINDER;
					break;
				}
				break;
			}

			case ParseState::REMAINDER:
			{
				out.PushBack( c );
				break;
			}

			default:
				assert( 0 && "Invalid parse state" );
			}
		}
		remaining_decimals = 0;
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainerView OutTextContainerType>
class TextFormatter<OutTextContainerType, bool>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;

	bool binary = false;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextViewType parse_text )
	{
		if( !parse_text.IsEmpty() )
		{
			if( parse_text == "b" ) binary = true;
		}
		else
		{
			diagnostic::Throw(
				OutTextContainerFullType<c32>( U"Unsupported text format options while formatting boolean primitive type, "
					"unrecognized parse text: \"" ) +
				conversion::ToUTF32( parse_text ) + U"\n"
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const bool & in )
	{
		if( binary )
		{
			if( in )
			{
				out.PushBack( '1' );
				return;
			}
			out.PushBack( '0' );
			return;
		}

		if( in )
		{
			out.Append( "true" );
			return;
		}
		out.Append( "false" );
	}
};



} // text
} // bc
