#pragma once

#include <core/diagnostic/exception/Exception.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>
#include "../TextFormatter.hpp"
#include <core/conversion/text/primitives/PrimitiveToStringConversion.hpp>
#include <core/conversion/text/primitives/StringToPrimitiveConversion.hpp>
#include <core/conversion/text/utf/UTFConversion.hpp>
#include <core/diagnostic/assertion/Assert.hpp>
#include <core/containers/Text.hpp>

#include <cctype>



namespace bc {
namespace text {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	ParseTextType,
	i64							MessageBeginCharacterArraySize
>
auto MakeTextFormatStingErrorMessage(
	const c32( &message_begin )[ MessageBeginCharacterArraySize ],
	const ParseTextType & format_text
) -> diagnostic::PrintRecord
{
	auto message_print_record = diagnostic::MakePrintRecord( message_begin );
	message_print_record += diagnostic::MakePrintRecord_Argument(
		U"format_text",
		format_text
	);
	return message_print_record;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	ParseTextType,
	i64							MessageBeginCharacterArraySize
>
void ThrowFormatStringError(
	const c32( &message_begin )[ MessageBeginCharacterArraySize ],
	const ParseTextType&				parse_text,
	const diagnostic::SourceLocation&	source_location		= diagnostic::SourceLocation::Current(),
	const diagnostic::StackTrace&		stack_trace			= diagnostic::StackTrace::Current()
)
{
	auto message = MakeTextFormatStingErrorMessage(
		message_begin,
		parse_text
	);
	diagnostic::Throw( diagnostic::Exception( message, source_location, stack_trace ) );
}

template<
	typename				OutTextCharacterType,
	utility::TextContainer	InputTextContainerType
>
auto MakeUTFConvertedText(
	const InputTextContainerType& in
) -> TextBase<OutTextCharacterType>
{
	using InTextCharacterType = typename InputTextContainerType::ContainedCharacterType;

	auto out = TextBase<OutTextCharacterType>();
	out.Resize( in.Size() * std::min( 1, sizeof( InTextCharacterType ) / sizeof( OutTextCharacterType ) ) );

	auto conversion_result = conversion::UTFConvert<OutTextCharacterType, InTextCharacterType>(
		out.Data(),
		out.Data() + out.Size(),
		in.Data(),
		in.Data() + in.Size()
	);
	BAssert( conversion_result.outcome == conversion::UTFConvertResult::Outcome::SUCCESS, "error converting UTF text in text formatter" );

	out.Resize( conversion_result.code_unit_count );
	return out;
}

} // namespace internal_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	OutTextContainerType,
	utility::TextContainerView	ValueTextContainerType
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
			internal_::ThrowFormatStringError(
				U"Unsupported text format options while formatting text: \"",
				parse_text
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType& out, const ValueTextContainerType& in )
	{
		static_assert(
			!utility::SameAs<OutTextCharacterType, char> || utility::SameAs<ValueTextCharacterType, char>,
			"Cannot convert from unicode to char, output text type can only be char if input text type is also char"
		);

		if constexpr( utility::SameAs<OutTextCharacterType, ValueTextCharacterType> )
		{
			out.Append( in );
		}
		else
		{
			out.Append( internal_::MakeUTFConvertedText<OutTextCharacterType>( in ) );
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterCString =
	utility::SameAs<T, char> ||
	utility::SameAs<T, c8> ||
	utility::SameAs<T, c16> ||
	utility::SameAs<T, c32>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	OutTextContainerType,
	FormatterCString			ValueTextType
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
			internal_::ThrowFormatStringError(
				U"Unsupported text format options while formatting c-style string: \"",
				parse_text
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType& out, const ValueTextType* in )
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
	utility::TextContainerView	OutTextContainerType,
	FormatterCString			ValueTextType,
	u64							CharacterArraySize
>
class TextFormatter<OutTextContainerType, ValueTextType[ CharacterArraySize ]>
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
			internal_::ThrowFormatStringError(
				U"Unsupported text format options while formatting c-style string: \"",
				parse_text
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const ValueTextType( &in )[ CharacterArraySize ] )
	{
		static_assert(
			!utility::SameAs<OutTextCharacterType, char> || utility::SameAs<ValueTextCharacterType, char>,
			"Cannot convert from unicode to char, output text type can only be char if input text type is also char"
		);

		if constexpr( utility::SameAs<OutTextCharacterType, ValueTextCharacterType> )
		{
			out.Append( in );
		}
		else
		{
			out.Append( internal_::MakeUTFConvertedText<OutTextCharacterType>( TextViewBase<ValueTextCharacterType>( in ) ) );
		}
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
			internal_::ThrowFormatStringError(
				U"Unsupported text format options while formatting boolean primitive type: \"",
				parse_text
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterIntegerType =
	utility::SameAs<T, i8> ||
	utility::SameAs<T, u8> ||
	utility::SameAs<T, i16> ||
	utility::SameAs<T, u16> ||
	utility::SameAs<T, i32> ||
	utility::SameAs<T, u32> ||
	utility::SameAs<T, i64> ||
	utility::SameAs<T, u64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	OutTextContainerType,
	FormatterIntegerType		IntegerType
>
class TextFormatter<OutTextContainerType, IntegerType>
{
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;

	i64 zero_fill = 0;
	conversion::IntegerToStringConversionFormat base = conversion::IntegerToStringConversionFormat::DECIMAL;
	bool add_prefix = false;
	bool uppercase = false;

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
				base = conversion::IntegerToStringConversionFormat::BINARY;
				++it;
			}
			else if( *it == 'o' )
			{
				base = conversion::IntegerToStringConversionFormat::OCTAL;
				++it;
			}
			else if( *it == 'x' )
			{
				base = conversion::IntegerToStringConversionFormat::HEX;
				++it;
			}
			else if( *it == 'p' )
			{
				add_prefix = true;
				++it;
			}
			else if( *it == 'u' )
			{
				uppercase = true;
				++it;
			}
			else if( *it == 'z' )
			{
				++it;
				BAssert(
					it.IsInBounds(),
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting integer primitive type, "
						"while parsing integer zero fill parse text, expected a digit after \"z\"",
						parse_text
					)
				);
				BAssert(
					std::isdigit( *it ),
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting integer primitive type "
						"while parsing integer zero fill parse text, expected a digit after \"z\"",
						parse_text
					)
				);
				auto conversion_result = conversion::StringToInteger<OutTextCharacterType>(
					zero_fill,
					it.GetAddress(),
					end.GetAddress()
				);
				it += conversion_result.length;
				BAssert(
					conversion_result.outcome == conversion::StringToPrimitiveResult::Outcome::SUCCESS,
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting integer primitive type, "
						"invalid number while parsing zero-fill from parse text",
						parse_text
					)
				);
				BAssert(
					zero_fill <= 1024,
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting integer primitive type, "
						"too many zeroes requested from zero fill, current limit is 1024",
						parse_text
					)
				);
			}
			else
			{
				++it;
				internal_::ThrowFormatStringError(
					U"Unsupported text format options while formatting integer primitive type",
					parse_text
				);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const IntegerType & in )
	{
		auto ResizeInitialOrToCapacity = [ this, &out ]()
			{
				if( out.Size() <= 0 )
				{
					out.Resize( zero_fill + 128 );
				}
				else
				{
					out.Resize( out.Capacity() );
				}
			};

		auto TryConvert = [ this, in ](
			OutTextCharacterType* text_buffer_begin,
			OutTextCharacterType* text_buffer_end
			) -> conversion::PrimitiveToStringResult
			{
				if constexpr( std::is_signed_v<IntegerType> )
				{
					return conversion::IntegerToString(
						text_buffer_begin,
						text_buffer_end,
						in,
						this->base,
						this->add_prefix,
						this->uppercase
					);
				}
				else
				{
					return conversion::UnsignedIntegerToString(
						text_buffer_begin,
						text_buffer_end,
						in,
						this->base,
						this->add_prefix,
						this->uppercase
					);
				}
			};

		auto DoZeroFill = [ this ](
			OutTextCharacterType* text_buffer_begin,
			OutTextCharacterType* text_buffer_end,
			i64 number_of_written_characters
			)
			{
				// zero_fill is is the total required length.
				// Range [text_buffer_begin, text_buffer_end] is the total length of the text buffer we have to work within and should always be large enough.
				if( number_of_written_characters >= zero_fill ) return;

				i64 text_range = text_buffer_end - text_buffer_begin;
				BEngineAssert( text_range <= zero_fill, U"Range was larger than zero_fill, this should never happen" );

				// number_of_written_characters is the number of characters written to the text buffer starting from the beginning.
				// zero_add_count is the number of zeros that must be added to the text buffer to make it the same length as zero_fill,
				//   if this is negative, do nothing. Should never be below zero because we checked for that above.

				i64 zero_add_count = zero_fill - number_of_written_characters;
				for( i64 i = number_of_written_characters - 1; i >= 0; --i )
				{
					text_buffer_begin[ i + zero_add_count ] = text_buffer_begin[ i ];
				}
				for( i64 i = 0; i < zero_add_count; ++i )
				{
					text_buffer_begin[ i ] = '\0';
				}
			};

		// Resize output buffer to hold converted text.
		auto original_out_size = out.Size();
		ResizeInitialOrToCapacity();

		// Try to convert to text without allocating more space first.
		auto conversion_result = TryConvert(
			out.Data() + original_out_size,
			out.Data() + out.Size()
		);
		if( conversion_result.outcome == conversion::PrimitiveToStringResult::Outcome::INCOMPLETE )
		{
			// If incomplete, reallocate output text container with enough space to contain any integer value.
			out.Resize( original_out_size + zero_fill + 128 );

			conversion_result = TryConvert(
				out.Data() + original_out_size,
				out.Data() + out.Size()
			);
		}
		BAssert(
			conversion_result.outcome == conversion::PrimitiveToStringResult::Outcome::SUCCESS,
			U"Unable to convert integer primitive type to text"
		);

		DoZeroFill(
			out.Data() + original_out_size,
			out.Data() + out.Size(),
			conversion_result.length
		);

		i64 size_increase = std::max( conversion_result.length, zero_fill );
		out.Resize( original_out_size + size_increase );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
concept FormatterFloatingType =
	utility::SameAs<T, f32> ||
	utility::SameAs<T, f64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	OutTextContainerType,
	FormatterFloatingType		FloatType
>
class TextFormatter<OutTextContainerType, FloatType>
{
	using OutTextFullType = typename OutTextContainerType::ThisFullType;
	template<typename CharacterType>
	using OutTextContainerFullType = typename OutTextContainerType::template ThisContainerFullType<CharacterType>;
	using OutTextViewType = typename OutTextContainerType::template ThisViewType<true>;
	using OutTextCharacterType = typename OutTextContainerType::ContainedCharacterType;

	conversion::FloatToStringConversionFormat float_to_text_conversion_format = conversion::FloatToStringConversionFormat::GENERAL;
	i64 precision = -1;
	bool uppercase = false;

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
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToStringConversionFormat::FIXED;
				++it;
			}
			else if( *it == 's' )
			{
				if( first_format_setting )
				{
					first_format_setting = false;
					float_to_text_conversion_format = {};
				}
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToStringConversionFormat::SCIENTIFIC;
				++it;
			}
			else if( *it == 'x' )
			{
				if( first_format_setting )
				{
					first_format_setting = false;
					float_to_text_conversion_format = {};
				}
				float_to_text_conversion_format = float_to_text_conversion_format | conversion::FloatToStringConversionFormat::HEX;
				++it;
			}
			else if( *it == 'u' )
			{
				uppercase = true;
				++it;
			}
			else if( *it == '.' )
			{
				++it;
				BAssert(
					it.IsInBounds(),
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting float primitive type, "
						"while parsing float precision parse text, expected a digit after \".\"",
						parse_text
					)
				);
				BAssert(
					std::isdigit( *it ),
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting float primitive type, "
						"while parsing float precision parse text, expected a digit after \".\"",
						parse_text
					)
				);
				auto conversion_result = conversion::StringToInteger<OutTextCharacterType>(
					precision,
					it.GetAddress(),
					end.GetAddress()
				);
				it += conversion_result.length;
				BAssert(
					conversion_result.outcome == conversion::StringToPrimitiveResult::Outcome::SUCCESS,
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting float primitive type, "
						"while parsing float precision, invalid number after \".\"",
						parse_text
					)
				);
				BAssert(
					precision <= 1024,
					internal_::MakeTextFormatStingErrorMessage(
						U"Unsupported text format options while formatting float primitive type, "
						"too large float precision requested, current limit is 1024",
						parse_text
					)
				);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextFullType & out, const FloatType & in )
	{
		auto ResizeInitialOrToCapacity = [ this, &out ]()
			{
				if( out.Size() <= 0 )
				{
					auto zero_fill = i64( 0 );
					if( ( float_to_text_conversion_format & conversion::FloatToStringConversionFormat::FIXED ) == conversion::FloatToStringConversionFormat::FIXED )
					{
						zero_fill = ( precision < 0 ) ? 0 : precision;
					}
					out.Resize( zero_fill + 128 );
				}
				else
				{
					out.Resize( out.Capacity() );
				}
			};

		auto TryConvert = [ this, in ](
			OutTextCharacterType* text_buffer_begin,
			OutTextCharacterType* text_buffer_end
			) -> conversion::PrimitiveToStringResult
			{
				return conversion::FloatToString(
					text_buffer_begin,
					text_buffer_end,
					in,
					float_to_text_conversion_format,
					precision
				);
			};

		auto original_out_size = out.Size();
		ResizeInitialOrToCapacity();

		auto conversion_result = TryConvert(
			out.Data(),
			out.Data() + out.Size()
		);
		if( conversion_result.outcome == conversion::PrimitiveToStringResult::Outcome::INCOMPLETE )
		{
			out.Resize( out.Size() + ( ( precision < 0 ) ? 0 : precision ) + 2048 ); // Retry with a much larger buffer
			conversion_result = TryConvert(
				out.Data(),
				out.Data() + out.Size()
			);
		}
		BAssert(
			conversion_result.outcome == conversion::PrimitiveToStringResult::Outcome::SUCCESS,
			U"Unable to convert float primitive type to text"
		);
		out.Resize( original_out_size + conversion_result.length );
	}
};



} // text
} // bc
