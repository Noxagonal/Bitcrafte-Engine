
#include <core/PreCompiledHeader.hpp>
#include <core/conversion/text/primitives/PrimitiveToStringConversion.hpp>

#include <cmath>
#include <charconv>



namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts boolean value into text.
///
///	Appends either "true" or "false" text depending on input.
///
/// @note
/// If resulting value does not fit in the output_string_begin, this function returns 0.
///
/// @tparam TextCharacterType
///	Character type of the text we want to append the converted value into.
/// 
/// @tparam ValueType
///	Value type we want to convert.
/// 
/// @param output_text
///	Location of the text where we want to append to.
///
/// @param output_string_end
///	Size of the output text. This is to make sure that we don't write over the end of the output text.
///
/// @param input_value
///	Value to convert into text.
///
/// @param uppercase
///	Should the output text be in upper-case or lower-case.
///
/// @return
/// Result describing if the operation was successful and how many characters were written.
template<
	typename	TextCharacterType,
	typename	ValueType
>
static constexpr auto DoBoolToString(
	TextCharacterType*	output_string_begin,
	TextCharacterType*	output_string_end,
	ValueType			input_value,
	bool				uppercase		= false
) -> PrimitiveToStringResult
{
	/*
	auto DoConvert = [ &output_string_begin, output_string_end ]( const char* bool_text, i64 bool_text_length ) -> PrimitiveToStringResult
		{
			if( output_string_end <= 0 )
			{
				return internal_::MakePrimitiveToStringResult(
					0,
					PrimitiveToStringResult::Outcome::ERROR
				);
			}
			auto fill_length = std::min( bool_text_length, output_string_end );
			auto fits = output_string_end >= fill_length;

			for( i64 i = 0; i < fill_length; i++ )
			{
				output_text[ i ] = bool_text[ i ];
			}
			return internal_::MakePrimitiveToStringResult(
				fill_length,
				PrimitiveToStringResult::Outcome::SUCCESS
			);
		};

	if( input_value )
	{
		return DoConvert( uppercase ? "TRUE" : "true", 4 );
	}
	else
	{
		return DoConvert( uppercase ? "FALSE" : "false", 5 );
	}
	*/
	return internal_::MakePrimitiveToStringResult(
		0,
		PrimitiveToStringResult::Outcome::ERROR
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts integer value into text.
///
///	Appends the integer number as text into an output buffer.
///
/// @note
/// If resulting value does not fit in the output_string_begin, this function returns 0.
///
/// @tparam TextCharacterType
///	Character type of the text we want to append the converted value into.
/// 
/// @tparam ValueType
///	Value type we want to convert.
/// 
/// @param output_text
///	Location of the text where we want to append to.
///
/// @param output_string_end
///	Size of the output text. This is to make sure that we don't write over the end of the output text.
///
/// @param input_value
///	Value to convert into text.
///
/// @param string_format
///	Value text string representation. See IntegerToStringConversionFormat.
///
/// @param add_literal_prefix
///	Should "0x", "0o" or "0b" prefix be added to the output text in case when string_format is HEX, OCTAL or BINARY.
///
/// @param uppercase
///	Should the output text be in uppercase if letters are used, eg. HEX 0xaa or 0xAA.
///
/// @return
/// Result describing if the operation was successful and how many characters were written.
template<
	typename	TextCharacterType,
	typename	ValueType
>
static constexpr auto DoIntegerToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	IntegerToStringConversionFormat	string_format			= IntegerToStringConversionFormat::DECIMAL,
	bool 							add_prefix			= false,
	bool 							uppercase			= true,
	i64								zerofill_to_length	= 0
) -> PrimitiveToStringResult
{
	/*
	using UnsignedValueType = std::make_unsigned_t<ValueType>;

	if( output_string_end <= 0 )
	{
		return internal_::MakePrimitiveToStringResult(
			0,
			PrimitiveToStringResult::Outcome::ERROR
		);
	}

	if( input_value == 0 )
	{
		output_text[ 0 ] = static_cast<TextCharacterType>( '0' );
		return internal_::MakePrimitiveToStringResult(
			1,
			PrimitiveToStringResult::Outcome::SUCCESS
		);
	}

	auto output_it = output_text;
	auto output_string_end = output_text + output_string_end;

	UnsignedValueType input_value_unsigned = static_cast<UnsignedValueType>( input_value );
	if( input_value < 0 )
	{
		*output_it = static_cast<TextCharacterType>( '-' );
		++output_it;
		input_value_unsigned = static_cast<UnsignedValueType>( -input_value );
	}

	if( add_literal_prefix )
	{
		if( string_format == IntegerToStringConversionFormat::BINARY )
		{
			if( output_it + 2 >= output_string_end )
			{
				return internal_::MakePrimitiveToStringResult(
					output_it - output_string_begin,
					PrimitiveToStringResult::Outcome::INCOMPLETE
				);
			}
			*output_it = static_cast<TextCharacterType>( '0' );
			++output_it;
			*output_it = static_cast<TextCharacterType>( 'b' );
			++output_it;
		}
		else if( string_format == IntegerToStringConversionFormat::OCTAL )
		{
			if( output_it + 2 >= output_string_end )
			{
				return internal_::MakePrimitiveToStringResult(
					output_it - output_string_begin,
					PrimitiveToStringResult::Outcome::INCOMPLETE
				);
			}
			*output_it = static_cast<TextCharacterType>( '0' );
			++output_it;
			*output_it = static_cast<TextCharacterType>( 'o' );
			++output_it;
		}
		else if( string_format == IntegerToStringConversionFormat::HEX )
		{
			if( output_it + 2 >= output_string_end )
			{
				return internal_::MakePrimitiveToStringResult(
					output_it - output_string_begin,
					PrimitiveToStringResult::Outcome::INCOMPLETE
				);
			}
			*output_it = static_cast<TextCharacterType>( '0' );
			++output_it;
			*output_it = static_cast<TextCharacterType>( 'x' );
			++output_it;
		}
	}

	auto numeric_begin = output_it;
	constexpr auto ascii_numbers_start = static_cast<TextCharacterType>( '0' );
	auto ascii_alphabet_start = uppercase ? static_cast<TextCharacterType>( 'A' ) : static_cast<TextCharacterType>( 'a' );
	auto base = static_cast<UnsignedValueType>( string_format );

	while( input_value_unsigned > 0 )
	{
		if( output_it >= output_string_end )
		{
			return internal_::MakePrimitiveToStringResult(
				output_it - output_string_begin,
				PrimitiveToStringResult::Outcome::INCOMPLETE
			);
		}
		auto remainder = input_value_unsigned % base;
		auto digit = ascii_numbers_start + remainder;
		if( remainder > 9 ) digit = ascii_alphabet_start + remainder - 10;
		*output_it = digit;
		++output_it;
		input_value_unsigned /= base;
	}

	// Reverse the text
	for( auto i = numeric_begin, j = output_it - 1; i < j; ++i, --j )
	{
		auto tmp = *i;
		*i = *j;
		*j = tmp;
	}

	return internal_::MakePrimitiveToStringResult(
		output_it - output_string_begin,
		PrimitiveToStringResult::Outcome::SUCCESS
	);
	*/
	return internal_::MakePrimitiveToStringResult(
		0,
		PrimitiveToStringResult::Outcome::ERROR
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts a floating point value into text.
/// 
/// @tparam TextContainerType
///	Container type of the text we want to append the converted value into.
/// 
/// @tparam ValueType
///	Value type we want to convert.
/// 
/// @param append_to
///	Container to append to. Value is added at the end.
/// 
/// @param value
///	Value to convert into text.
///				
/// @param string_format
///	Value text representation. See FloatToStringConversionFormat.
///
/// @param precision
///	Number of digits after the decimal point. -1 means automatic precision.
///
/// @result
/// Result describing if the operation was successful and how many characters were written.
template<
	typename	TextCharacterType,
	typename	ValueType
>
static constexpr auto DoFloatToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	FloatToStringConversionFormat	string_format			= FloatToStringConversionFormat::GENERAL,
	i64 							precision				= -1
) -> PrimitiveToStringResult
{
	// OPTIMIZATION: Could optimize this function to not use std::to_chars. std::to_chars uses char array but this function works in any character type.
	// Which means that we have to use an internal buffer and do an extra copy operation for c16 and c32 types.

	/*
	if( !(
		std::underlying_type_t<conversion::FloatToStringConversionFormat>( string_format & conversion::FloatToStringConversionFormat::GENERAL ) ||
		std::underlying_type_t<conversion::FloatToStringConversionFormat>( string_format & conversion::FloatToStringConversionFormat::SCIENTIFIC ) ||
		std::underlying_type_t<conversion::FloatToStringConversionFormat>( string_format & conversion::FloatToStringConversionFormat::FIXED ) ||
		std::underlying_type_t<conversion::FloatToStringConversionFormat>( string_format & conversion::FloatToStringConversionFormat::HEX )
		) )
	{
		// Invalid float to text conversion format, fall back to general.
		string_format = conversion::FloatToStringConversionFormat::GENERAL;
	}

	if( output_string_end <= 0 )
	{
		return internal_::MakePrimitiveToStringResult(
			0,
			PrimitiveToStringResult::Outcome::ERROR
		);
	}

	auto output_it = output_text;
	auto output_string_end = output_text + output_string_end;

	auto is_negative = std::signbit( input_value );
	if( is_negative )
	{
		*output_it = static_cast<TextCharacterType>( '-' );
		++output_it;
		input_value = -input_value;
	}

	if( add_literal_prefix && ( string_format == ( string_format & conversion::FloatToStringConversionFormat::HEX ) ) )
	{
		if( output_it + 2 >= output_string_end )
		{
			return internal_::MakePrimitiveToStringResult(
				output_it - output_string_begin,
				PrimitiveToStringResult::Outcome::INCOMPLETE
			);
		}
		*output_it = static_cast<TextCharacterType>( '0' );
		++output_it;
		*output_it = static_cast<TextCharacterType>( 'x' );
		++output_it;
	}

	if( output_it + 2 >= output_string_end )
	{
		return internal_::MakePrimitiveToStringResult(
			output_it - output_string_begin,
			PrimitiveToStringResult::Outcome::INCOMPLETE
		);
	}

	if constexpr( utility::SameAs<TextCharacterType, char> || utility::SameAs<TextCharacterType, char8_t> )
	{
		// Can work directly in destination char array, no need for heap memory allocation.
		auto result = std::to_chars( static_cast<char*>( &output_it ), static_cast<char*>( &output_string_end ), input_value, std::chars_format( string_format ) );
		if( result.ec != std::errc() )
		{
			return internal_::MakePrimitiveToStringResult(
				output_it - output_string_begin,
				PrimitiveToStringResult::Outcome::INCOMPLETE
			);
		}

		return internal_::MakePrimitiveToStringResult(
			result.ptr - output_string_begin,
			PrimitiveToStringResult::Outcome::SUCCESS
		);
	}
	else
	{
		// Output is not an 8 bit char array, so we'll need to allocate a temporary buffer and copy the result into destination after.
		constexpr i64 buffer_size = 128;
		char buffer[ buffer_size ];
		auto buffer_end		= buffer + buffer_size;
		auto result = std::to_chars( buffer, buffer_end, input_value, std::chars_format( string_format ) );
		if( result.ec != std::errc() )
		{
			// did not fit into stack buffer, give up.
			return internal_::MakePrimitiveToStringResult(
				output_it - output_string_begin,
				PrimitiveToStringResult::Outcome::ERROR
			);
		}

		i64 buffer_written_length = result.ptr - buffer;
		i64 output_length_remaining = output_string_end - output_it;
		auto copy_length = std::min( buffer_written_length, output_length_remaining );
		auto fits = buffer_written_length <= output_length_remaining;
		for( i64 i = 0; i < copy_length; i++ )
		{
			output_text[ i ] = buffer[ i ];
		}
		output_it += copy_length;

		return internal_::MakePrimitiveToStringResult(
			output_it - output_string_begin,
			fits ? PrimitiveToStringResult::Outcome::SUCCESS : PrimitiveToStringResult::Outcome::INCOMPLETE
		);
	}
	*/
	return internal_::MakePrimitiveToStringResult(
		0,
		PrimitiveToStringResult::Outcome::ERROR
	);
}



} // namespace internal_
} // namespace conversion
} // namespace bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::BoolToString_char(
	char*	output_string_begin,
	char*	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult
{
	return DoBoolToString( output_string_begin, output_string_end, input_value, uppercase );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::BoolToString_c8(
	c8*		output_string_begin,
	c8*		output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult
{
	return DoBoolToString( output_string_begin, output_string_end, input_value, uppercase );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::BoolToString_c16(
	c16*	output_string_begin,
	c16*	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult
{
	return DoBoolToString( output_string_begin, output_string_end, input_value, uppercase );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::BoolToString_c32(
	c32*	output_string_begin,
	c32*	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult
{
	return DoBoolToString( output_string_begin, output_string_end, input_value, uppercase );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::IntegerToString_char(
	char*								output_string_begin,
	char*								output_string_end,
	i64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::IntegerToString_c8(
	c8*									output_string_begin,
	c8*									output_string_end,
	i64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::IntegerToString_c16(
	c16*								output_string_begin,
	c16*								output_string_end,
	i64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::IntegerToString_c32(
	c32*								output_string_begin,
	c32*								output_string_end,
	i64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::UnsignedIntegerToString_char(
	char*								output_string_begin,
	char*								output_string_end,
	u64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::UnsignedIntegerToString_c8(
	c8*									output_string_begin,
	c8*									output_string_end,
	u64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::UnsignedIntegerToString_c16(
	c16*								output_string_begin,
	c16*								output_string_end,
	u64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::UnsignedIntegerToString_c32(
	c32*								output_string_begin,
	c32*								output_string_end,
	u64									input_value,
	IntegerToStringConversionFormat		string_format,
	bool								add_prefix,
	bool								uppercase,
	i64									zerofill_to_length
) -> PrimitiveToStringResult
{
	return DoIntegerToString( output_string_begin, output_string_end, input_value, string_format, add_prefix, uppercase, zerofill_to_length );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::FloatToString_char(
	char*							output_string_begin,
	char*							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	string_format,
	i64								precision
) -> PrimitiveToStringResult
{
	return DoFloatToString( output_string_begin, output_string_end, input_value, string_format, precision );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::FloatToString_c8(
	c8*								output_string_begin,
	c8*								output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	string_format,
	i64								precision
) -> PrimitiveToStringResult
{
	return DoFloatToString( output_string_begin, output_string_end, input_value, string_format, precision );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::FloatToString_c16(
	c16*							output_string_begin,
	c16*							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	string_format,
	i64								precision
) -> PrimitiveToStringResult
{
	return DoFloatToString( output_string_begin, output_string_end, input_value, string_format, precision );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::FloatToString_c32(
	c32*							output_string_begin,
	c32*							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	string_format,
	i64								precision
) -> PrimitiveToStringResult
{
	return DoFloatToString( output_string_begin, output_string_end, input_value, string_format, precision );
}
