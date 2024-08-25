#pragma once

#include <core/containers/backend/ContainerBase.hpp>

#include <charconv>
#include <cctype>



namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept TextToBoolConvertible = std::is_same_v<ValueType, bool>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept TextToIntegerConvertible =
std::is_same_v<ValueType, i8> ||
std::is_same_v<ValueType, u8> ||
std::is_same_v<ValueType, i16> ||
std::is_same_v<ValueType, u16> ||
std::is_same_v<ValueType, i32> ||
std::is_same_v<ValueType, u32> ||
std::is_same_v<ValueType, i64> ||
std::is_same_v<ValueType, u64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept TextToFloatingConvertible = std::is_floating_point_v<ValueType>;



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts text into boolean value.
///
/// If text is a number other than 0, set value to true. If text reads "true", set value to true. Everything else sets value to
/// false, including if the text is empty.
///
/// @tparam TextContainerType
///	Generic container type of the text we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param out_value
///	Reference to value we want to set.
///
/// @param in_text
///	Text which contains the value we want to convert.
/// 
/// @return
/// Number of characters read from the text.
template<
	utility::TextContainerView			TextContainerType,
	internal_::TextToBoolConvertible	ValueType
>
constexpr auto TextToPrimitive(
	ValueType&					out_value,
	const TextContainerType&	in_text
) -> i64
{
	using CharT = typename TextContainerType::ContainedCharacterType;

	if( in_text.IsEmpty() ) {
		out_value = false;
		return {};
	}

	// Check for numeric value.
	if( in_text.Front() >= CharT( '0' ) && in_text.Front() <= CharT( '9' ) ) {
		bool is_numeric_true_value = false;
		auto it = in_text.begin();

		while( it != in_text.end() ) {
			if( *it < CharT( '0' ) || *it > CharT( '9' ) ) {
				break;
			}
			if( *it > CharT( '0' ) ) {
				is_numeric_true_value = true;
			}
			++it;
		}

		out_value = is_numeric_true_value;
		return it.GetIndex();
	}

	// Check for string value
	if( in_text == "true" ) {
		out_value = true;
		return 4;
	}
	if( in_text == "false" ) {
		out_value = false;
		return 5;
	}
	if( in_text == "on" ) {
		out_value = true;
		return 2;
	}
	if( in_text == "off" ) {
		out_value = false;
		return 3;
	}

	// TODO: Throw in development mode.

	out_value = {};
	return {};
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how integer numbers are represented as text.
///
/// @note
/// AUTOMATIC option automatically parses values starting with "0o" as octal, "0b" as binary and "0x" as hexadecimal. Other
/// options do not automatically parse the text and expects the text to be the value portion without any prefix. Eg. You might
/// want to read text that is "5FA0", which does not contain "0x" but you know is hexadecimal.
enum class TextToIntegerConversionFormat : u32
{
	AUTOMATIC	= 0,		///< Detect "0b", "0o" and "0x" beginning texts as binary, octal and hex respectively.
	BINARY		= 2,		///< Base-2
	OCTAL		= 8,		///< Base-8
	DECIMAL		= 10,		///< Base-10
	HEX			= 16,		///< Base-16
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts text to integer type.
/// 
/// @tparam TextContainerType
///	Generic container type of the text we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param out_value
///	Reference to value we want to set.
/// 
/// @param in_text
///	Text which contains the value we want to convert.
/// 
/// @param text_format
///	Number base to convert the number from. See TextToIntegerConversionFormat for more info. You can use other types too. For
/// example if you want to read value that was encoded with Base-5, you can just give this parameter
/// static_cast<TextToIntegerConversionFormat>( 5 ).
/// 
/// @return
/// Number of characters read from the text.
template<
	utility::TextContainerView					TextContainerType,
	internal_::TextToIntegerConvertible			ValueType
>
constexpr auto TextToPrimitive(
	ValueType&						out_value,
	const TextContainerType&		in_text,
	TextToIntegerConversionFormat	text_format		= TextToIntegerConversionFormat::AUTOMATIC
) -> i64
{
	using CharacterType			= typename TextContainerType::ContainedCharacterType;
	using ASCIIType				= typename TextContainerType::template ThisContainerFullType<char>;
	using ASCIIViewType			= typename ASCIIType::template ThisViewType<true>;

	auto FixEndPosition = []( ASCIIViewType in_text_view, i64 end_position ) -> i64
	{
		// On success, std::from_chars either returns pointer to the first non-number, or a pointer to
		// the last number, depending on if the text contained only numbers. Here we fix it so that
		// end position always points one past the last number.
		if( end_position < in_text_view.Size() ) {
			if( in_text_view[ end_position ] >= '0' && in_text_view[ end_position ] <= '9' ) {
				++end_position;
			}
		}
		return end_position;
	};

	// Check and convert to ascii type if needed, char and char32_t are considered fixed lenght.
	ASCIIType		in_buffer;
	ASCIIViewType	in_buffer_view;
	{
		if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
			in_buffer_view = in_text;

		} else {
			in_buffer.Resize( in_text.Size() );
			for( i64 i = 0; i < in_text.Size(); i++ ) {
				in_buffer[ i ] = char( std::tolower( in_text[ i ] ) );
			}
			in_buffer_view = in_buffer;
		}
	}

	// Remove preceeding "+" symbols.
	while( !in_buffer_view.IsEmpty() && in_buffer_view.Front() == CharacterType( '+' ) ) {
		in_buffer_view = in_buffer_view.SubText( 1 );
	}

	// TODO: TextToPrimitive does not understand negative numbers if conversion format is deduced from the text.
	// Negative type should be allowed before prefix. Eg. "-0x30" does not actually work. "0x-30" does work but shouldn't.

	// If automatic type selection was used, auto-detect type from the text.
	if( text_format == TextToIntegerConversionFormat::AUTOMATIC ) {
		text_format = TextToIntegerConversionFormat::DECIMAL;

		if( in_buffer_view.Size() >= 2 &&
			in_buffer_view[ 0 ] == CharacterType( '0' ) ) {

			auto second_char = std::tolower( in_buffer_view[ 1 ] );

			if( second_char == CharacterType( 'x' ) ) {
				text_format = TextToIntegerConversionFormat::HEX;
				in_buffer_view = in_buffer_view.SubText( 2 );

			} else if( second_char == CharacterType( 'b' ) ) {
				text_format = TextToIntegerConversionFormat::BINARY;
				in_buffer_view = in_buffer_view.SubText( 2 );

			} else if( second_char == CharacterType( 'o' ) ) {
				text_format = TextToIntegerConversionFormat::OCTAL;
				in_buffer_view = in_buffer_view.SubText( 2 );
			}
		}
	}

	// Convert to lower-case letters if conversion format was higher than decimal,
	// only do this for single byte characters as other types are already converted to lower.
	if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
		if( std::underlying_type_t<TextToIntegerConversionFormat>( text_format ) >
			std::underlying_type_t<TextToIntegerConversionFormat>( TextToIntegerConversionFormat::DECIMAL ) ) {
			for( i64 i = 0; i < in_buffer.Size(); i++ ) {
				in_buffer[ i ] = tolower( in_buffer[ i ] );
			}
		}
	}

	auto result = std::from_chars(
		in_buffer_view.Data(),
		in_buffer_view.Data() + in_buffer_view.Size(),
		out_value,
		int( text_format )
	);
	if( result.ec == std::errc() ) {
		i64 end_position = result.ptr - in_buffer_view.Data();
		return FixEndPosition( in_buffer_view, end_position );
	}
	if( result.ec == std::errc::result_out_of_range ) {

		// TODO: Throw in development mode.
		out_value = {};
		return {};

		// Value was larger than this primitive type can hold, set the value to maximum.
		out_value = std::numeric_limits<CharacterType>::max();

		i64 end_position = result.ptr - in_buffer_view.Data();
		return end_position;
	}

	// TODO: Throw in development mode.

	out_value = {};
	return {};
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how floating point numbers are represented as text.
///
///	Unlike FloatToTextConversionFormat, these are not flags and may not be used together. Scientific notation or fixed type is
/// always resolved automatically, hex is resolved only if type is left at AUTOMATIC and value starts with "0x".
enum class TextToFloatConversionFormat : u32
{
	AUTOMATIC	= 0,	///< Automatically resolves if the value is decimal or hex. (Default)
	DECIMAL,			///< Allows scientific notations as well as fixed representation, but not hex.
	HEX,				///< Same as DECIMAL but considers text in hexadecimal representation (must not have "0x" prefix)
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts text to integer type.
/// 
/// @tparam TextContainerType
///	Generic container type of the text we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param[out] out_value
///	Reference to value we want to set.
/// 
/// @param in_text
///	Text which contains the value we want to convert.
/// 
/// @param text_format
///	Value text representation. See TextToFloatConversionFormat.
///
/// @return
/// Number of characters processed.
template<
	utility::TextContainerView				TextContainerType,
	internal_::TextToFloatingConvertible	ValueType
>
constexpr auto TextToPrimitive(
	ValueType&						out_value,
	const TextContainerType&		in_text,
	TextToFloatConversionFormat		text_format		= TextToFloatConversionFormat::AUTOMATIC
) -> i64
{
	using CharacterType			= typename TextContainerType::CharacterType;
	using ASCIIType				= typename TextContainerType::template NonViewBaseType<char>;
	using ASCIIViewType			= typename ASCIIType::ViewType;

	auto FixEndPosition = []( ASCIIViewType in_text_view, i64 end_position ) -> i64
	{
		// On success, std::from_chars either returns pointer to the first non-number, or a pointer to
		// the last number, depending on if the text contained only numbers. Here we fix it so that
		// end position always points one past the last number.
		if( end_position < in_text_view.Size() ) {
			if( in_text_view[ end_position ] >= '0' && in_text_view[ end_position ] <= '9' ) {
				++end_position;
			}
		}
		return end_position;
	};

	// Check and convert to ascii type if needed, char and char32_t are considered fixed lenght.
	ASCIIType		in_buffer;
	ASCIIViewType	in_buffer_view;
	{
		if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
			in_buffer_view = in_text;

		} else {
			in_buffer.Resize( in_text.Size() );
			for( i64 i = 0; i < in_text.Size(); i++ ) {
				in_buffer[ i ] = char( std::tolower( in_text[ i ] ) );
			}
			in_buffer_view = in_buffer;
		}
	}

	// Remove preceeding "+" symbols.
	while( !in_buffer_view.IsEmpty() && in_buffer_view.Front() == CharacterType( '+' ) ) {
		in_buffer_view = in_buffer_view.SubText( 1 );
	}

	// TODO: TextToPrimitive does not understand negative numbers if conversion format is deduced from the text.
	// Negative type should be allowed before prefix. Eg. "-0x30" does not actually work. "0x-30" does work but shouldn't.

	// If automatic type selection was used, auto-detect type from the text.
	if( text_format == TextToFloatConversionFormat::AUTOMATIC ) {
		if( in_buffer_view.Size() >= 2 ) {
			text_format = TextToFloatConversionFormat::DECIMAL;
			if( in_buffer_view[ 0 ] == CharacterType( '0' ) && std::tolower( in_buffer_view[ 1 ] ) == CharacterType( 'x' ) ) {
				in_buffer_view = in_buffer_view.SubText( 2 );
				text_format = TextToFloatConversionFormat::HEX;
			}
		}
	}

	// Convert to lower-case letters if conversion format was hexadecimal,
	// only do this for single byte characters as other types are already converted to lower.
	if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
		if( std::underlying_type_t<TextToFloatConversionFormat>( text_format ) &
			std::underlying_type_t<TextToFloatConversionFormat>( TextToFloatConversionFormat::HEX ) ) {
			for( i64 i = 0; i < in_buffer.Size(); i++ ) {
				in_buffer[ i ] = tolower( in_buffer[ i ] );
			}
		}
	}

	{
		auto chars_format = std::chars_format::general;
		if( text_format == TextToFloatConversionFormat::HEX ) {
			chars_format |= std::chars_format::hex;
		}

		auto result = std::from_chars(
			in_buffer_view.Data(),
			in_buffer_view.Data() + in_buffer_view.Size(),
			out_value,
			chars_format
		);

		if( result.ec == std::errc() ) {
			i64 end_position = result.ptr - in_buffer_view.Data();
			return FixEndPosition( in_buffer_view, end_position );
		}
	}

	// TODO: Throw in development mode.

	out_value = {};
	return {};
}



} // conversion
} // bc
