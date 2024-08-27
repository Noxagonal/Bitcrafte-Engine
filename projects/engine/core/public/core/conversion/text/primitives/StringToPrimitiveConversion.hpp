#pragma once

#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/concepts/ContainerConcepts.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>



namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept StringToBoolConvertible = utility::SameAs<ValueType, bool>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept StringToIntegerConvertible =
	utility::SameAs<ValueType, i8> ||
	utility::SameAs<ValueType, i16> ||
	utility::SameAs<ValueType, i32> ||
	utility::SameAs<ValueType, i64>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept StringToUnsignedIntegerConvertible =
	utility::SameAs<ValueType, u8> ||
	utility::SameAs<ValueType, u16> ||
	utility::SameAs<ValueType, u32> ||
	utility::SameAs<ValueType, u64>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept StringToFloatConvertible = std::is_floating_point_v<ValueType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept StringToPrimitiveConvertible =
	StringToBoolConvertible<ValueType> ||
	StringToIntegerConvertible<ValueType> ||
	StringToUnsignedIntegerConvertible<ValueType> ||
	StringToFloatConvertible<ValueType>;



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how integer numbers are represented as string.
///
/// @note
/// AUTOMATIC option automatically parses values starting with "0o" as octal, "0b" as binary and "0x" as hexadecimal. Other
/// options do not automatically parse the string and expects the string to be the value portion without any prefix. Eg. You
/// might want to read string that is "5FA0", which does not contain "0x" but you know ahead of time that it is hexadecimal.
enum class StringToIntegerConversionFormat : u8
{
	AUTOMATIC	= 0,		///< Detect "0b", "0o" and "0x" beginning strings as binary, octal and hex respectively.
	BINARY		= 2,		///< Base-2
	OCTAL		= 8,		///< Base-8
	DECIMAL		= 10,		///< Base-10
	HEX			= 16,		///< Base-16
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how floating point numbers are represented as a string.
///
/// @note
///	Scientific notation or fixed type is always resolved automatically, hex is resolved only if type is left at AUTOMATIC and
/// value starts with "0x".
enum class StringToFloatConversionFormat : u8
{
	AUTOMATIC	= 0,	///< Automatically resolves if the value is decimal or hex. (Default)
	DECIMAL,			///< Allows scientific notations as well as fixed representation, but not hex.
	HEX,				///< Same as DECIMAL but considers string in hexadecimal representation (must not have "0x" prefix)
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StringToPrimitiveResult
{
	enum class Outcome : u8
	{
		SUCCESS		= 0,	///< The entire value was converted into a string.
		INVALID,			///< Value is not a number, or input string is invalid.
		ERROR,				///< Conversion had errors, or invalid input.
	};
	i64		length;			///< Length of how much was read from the string and how much the string should advance.
	Outcome	outcome;		///< Conversion outcome, indicates if the conversion was successful.
};



namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline auto MakeStringToPrimitiveResult(
	i64									length,
	StringToPrimitiveResult::Outcome	outcome
) -> StringToPrimitiveResult
{
	return StringToPrimitiveResult { length, outcome };
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToBool_char(
	bool&			output_value,
	const char*		input_string_begin,
	const char* 	input_string_end
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToBool_c8(
	bool&			output_value,
	const c8*		input_string_begin,
	const c8* 		input_string_end
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToBool_c16(
	bool&			output_value,
	const c16*		input_string_begin,
	const c16* 		input_string_end
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToBool_c32(
	bool&			output_value,
	const c32*		input_string_begin,
	const c32*		input_string_end
) -> StringToPrimitiveResult;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToInteger_char(
	i64&								output_value,
	const char*							input_string_begin,
	const char* 						input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToInteger_c8(
	i64&								output_value,
	const c8*							input_string_begin,
	const c8* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToInteger_c16(
	i64&								output_value,
	const c16*							input_string_begin,
	const c16* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToInteger_c32(
	i64&								output_value,
	const c32*							input_string_begin,
	const c32* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToUnsignedInteger_char(
	u64&								output_value,
	const char*							input_string_begin,
	const char* 						input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToUnsignedInteger_c8(
	u64&								output_value,
	const c8*							input_string_begin,
	const c8* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToUnsignedInteger_c16(
	u64&								output_value,
	const c16*							input_string_begin,
	const c16* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToUnsignedInteger_c32(
	u64&								output_value,
	const c32*							input_string_begin,
	const c32* 							input_string_end,
	StringToIntegerConversionFormat		format
) -> StringToPrimitiveResult;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToFloat_char(
	f64&								output_value,
	const char*							input_string_begin,
	const char* 						input_string_end,
	StringToFloatConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToFloat_c8(
	f64&								output_value,
	const c8*							input_string_begin,
	const c8* 							input_string_end,
	StringToFloatConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToFloat_c16(
	f64&								output_value,
	const c16*							input_string_begin,
	const c16* 							input_string_end,
	StringToFloatConversionFormat		format
) -> StringToPrimitiveResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto StringToFloat_c32(
	f64&								output_value,
	const c32*							input_string_begin,
	const c32* 							input_string_end,
	StringToFloatConversionFormat		format
) -> StringToPrimitiveResult;



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextCharacter StringCharacterType>
constexpr auto StringToBool(
	bool&						output_value,
	const StringCharacterType*	input_string_begin,
	const StringCharacterType*	input_string_end
) -> StringToPrimitiveResult
{
	if constexpr( utility::SameAs<StringCharacterType, char> )
	{
		return internal_::StringToBool_char( output_value, input_string_begin, input_string_end );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c8> )
	{
		return internal_::StringToBool_c8( output_value, input_string_begin, input_string_end );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c16> )
	{
		return internal_::StringToBool_c16( output_value, input_string_begin, input_string_end );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c32> )
	{
		return internal_::StringToBool_c32( output_value, input_string_begin, input_string_end );
	}
	return internal_::MakeStringToPrimitiveResult( 0, StringToPrimitiveResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextCharacter StringCharacterType>
constexpr auto StringToInteger(
	i64&								output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToIntegerConversionFormat		format				= StringToIntegerConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	if constexpr( utility::SameAs<StringCharacterType, char> )
	{
		return internal_::StringToInteger_char( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c8> )
	{
		return internal_::StringToInteger_c8( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c16> )
	{
		return internal_::StringToInteger_c16( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c32> )
	{
		return internal_::StringToInteger_c32( output_value, input_string_begin, input_string_end, format );
	}
	return internal_::MakeStringToPrimitiveResult( 0, StringToPrimitiveResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextCharacter StringCharacterType>
constexpr auto StringToUnsignedInteger(
	u64&								output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToIntegerConversionFormat		format				= StringToIntegerConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	if constexpr( utility::SameAs<StringCharacterType, char> )
	{
		return internal_::StringToUnsignedInteger_char( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c8> )
	{
		return internal_::StringToUnsignedInteger_c8( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c16> )
	{
		return internal_::StringToUnsignedInteger_c16( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c32> )
	{
		return internal_::StringToUnsignedInteger_c32( output_value, input_string_begin, input_string_end, format );
	}
	return internal_::MakeStringToPrimitiveResult( 0, StringToPrimitiveResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextCharacter StringCharacterType>
constexpr auto StringToFloat(
	f64&								output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToFloatConversionFormat		format				= StringToFloatConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	if constexpr( utility::SameAs<StringCharacterType, char> )
	{
		return internal_::StringToFloat_char( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c8> )
	{
		return internal_::StringToFloat_c8( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c16> )
	{
		return internal_::StringToFloat_c16( output_value, input_string_begin, input_string_end, format );
	}
	else if constexpr( utility::SameAs<StringCharacterType, c32> )
	{
		return internal_::StringToFloat_c32( output_value, input_string_begin, input_string_end, format );
	}
	return internal_::MakeStringToPrimitiveResult( 0, StringToPrimitiveResult::Outcome::ERROR );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	internal_::StringToBoolConvertible	ValueType,
	utility::TextCharacter				StringCharacterType
>
constexpr auto StringToPrimitive(
	ValueType&					output_value,
	const StringCharacterType*	input_string_begin,
	const StringCharacterType*	input_string_end
) -> StringToPrimitiveResult
{
	return StringToBool( output_value, input_string_begin, input_string_end );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	internal_::StringToIntegerConvertible	ValueType,
	utility::TextCharacter					StringCharacterType
>
constexpr auto StringToPrimitive(
	ValueType&							output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToIntegerConversionFormat		format				= StringToIntegerConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	return StringToInteger( output_value, input_string_begin, input_string_end, format );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	internal_::StringToUnsignedIntegerConvertible	ValueType,
	utility::TextCharacter							StringCharacterType
>
constexpr auto StringToPrimitive(
	ValueType&							output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToIntegerConversionFormat		format				= StringToIntegerConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	return StringToUnsignedInteger( output_value, input_string_begin, input_string_end, format );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	internal_::StringToFloatConvertible	ValueType,
	utility::TextCharacter				StringCharacterType
>
constexpr auto StringToPrimitive(
	ValueType&							output_value,
	const StringCharacterType*			input_string_begin,
	const StringCharacterType*			input_string_end,
	StringToFloatConversionFormat		format				= StringToFloatConversionFormat::AUTOMATIC
) -> StringToPrimitiveResult
{
	return StringToFloat( output_value, input_string_begin, input_string_end, format );
}



/*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts string into boolean value.
///
/// If string is a number other than 0, set value to true. If string reads "true", set value to true. Everything else sets value to
/// false, including if the string is empty.
///
/// @tparam StringContainerType
///	Generic container type of the string we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param out_value
///	Reference to value we want to set.
///
/// @param in_string
///	String which contains the value we want to convert.
/// 
/// @return
/// Number of characters read from the string.
template<
	utility::TextCharacter				StringCharacterType,
	internal_::StringToBoolConvertible	ValueType
>
constexpr auto StringToPrimitive(
	ValueType&					output_value,
	const StringCharacterType*	input_string,
	i64							input_string_size
) -> i64
{
	using CharT = typename StringContainerType::ContainedCharacterType;

	if( in_string.IsEmpty() ) {
		out_value = false;
		return {};
	}

	// Check for numeric value.
	if( in_string.Front() >= CharT( '0' ) && in_string.Front() <= CharT( '9' ) ) {
		bool is_numeric_true_value = false;
		auto it = in_string.begin();

		while( it != in_string.end() ) {
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
	if( in_string == "true" ) {
		out_value = true;
		return 4;
	}
	if( in_string == "false" ) {
		out_value = false;
		return 5;
	}
	if( in_string == "on" ) {
		out_value = true;
		return 2;
	}
	if( in_string == "off" ) {
		out_value = false;
		return 3;
	}

	// TODO: Throw in development mode.

	out_value = {};
	return {};
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts string to integer type.
/// 
/// @tparam StringContainerType
///	Generic container type of the string we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param out_value
///	Reference to value we want to set.
/// 
/// @param in_string
///	String which contains the value we want to convert.
/// 
/// @param format
///	Number base to convert the number from. See StringToIntegerConversionFormat for more info. You can use other types too. For
/// example if you want to read value that was encoded with Base-5, you can just give this parameter
/// static_cast<StringToIntegerConversionFormat>( 5 ).
/// 
/// @return
/// Number of characters read from the string.
template<
	utility::TextCharacter					StringCharacterType,
	internal_::StringToIntegerConvertible	ValueType
>
constexpr auto StringToPrimitive(
	ValueType&						output_value,
	const StringCharacterType*		input_string,
	i64								input_string_size,
	StringToIntegerConversionFormat	format		= StringToIntegerConversionFormat::AUTOMATIC
) -> i64
{
	using CharacterType			= typename StringContainerType::ContainedCharacterType;
	using ASCIIType				= typename StringContainerType::template ThisContainerFullType<char>;
	using ASCIIViewType			= typename ASCIIType::template ThisViewType<true>;

	auto FixEndPosition = []( ASCIIViewType in_string_view, i64 end_position ) -> i64
	{
		// On success, std::from_chars either returns pointer to the first non-number, or a pointer to
		// the last number, depending on if the string contained only numbers. Here we fix it so that
		// end position always points one past the last number.
		if( end_position < in_string_view.Size() ) {
			if( in_string_view[ end_position ] >= '0' && in_string_view[ end_position ] <= '9' ) {
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
			in_buffer_view = in_string;

		} else {
			in_buffer.Resize( in_string.Size() );
			for( i64 i = 0; i < in_string.Size(); i++ ) {
				in_buffer[ i ] = char( std::tolower( in_string[ i ] ) );
			}
			in_buffer_view = in_buffer;
		}
	}

	// Remove preceeding "+" symbols.
	while( !in_buffer_view.IsEmpty() && in_buffer_view.Front() == CharacterType( '+' ) ) {
		in_buffer_view = in_buffer_view.SubString( 1 );
	}

	// TODO: StringToPrimitive does not understand negative numbers if conversion format is deduced from the string.
	// Negative type should be allowed before prefix. Eg. "-0x30" does not actually work. "0x-30" does work but shouldn't.

	// If automatic type selection was used, auto-detect type from the string.
	if( format == StringToIntegerConversionFormat::AUTOMATIC ) {
		format = StringToIntegerConversionFormat::DECIMAL;

		if( in_buffer_view.Size() >= 2 &&
			in_buffer_view[ 0 ] == CharacterType( '0' ) ) {

			auto second_char = std::tolower( in_buffer_view[ 1 ] );

			if( second_char == CharacterType( 'x' ) ) {
				format = StringToIntegerConversionFormat::HEX;
				in_buffer_view = in_buffer_view.SubString( 2 );

			} else if( second_char == CharacterType( 'b' ) ) {
				format = StringToIntegerConversionFormat::BINARY;
				in_buffer_view = in_buffer_view.SubString( 2 );

			} else if( second_char == CharacterType( 'o' ) ) {
				format = StringToIntegerConversionFormat::OCTAL;
				in_buffer_view = in_buffer_view.SubString( 2 );
			}
		}
	}

	// Convert to lower-case letters if conversion format was higher than decimal,
	// only do this for single byte characters as other types are already converted to lower.
	if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
		if( std::underlying_type_t<StringToIntegerConversionFormat>( format ) >
			std::underlying_type_t<StringToIntegerConversionFormat>( StringToIntegerConversionFormat::DECIMAL ) ) {
			for( i64 i = 0; i < in_buffer.Size(); i++ ) {
				in_buffer[ i ] = tolower( in_buffer[ i ] );
			}
		}
	}

	auto result = std::from_chars(
		in_buffer_view.Data(),
		in_buffer_view.Data() + in_buffer_view.Size(),
		out_value,
		int( format )
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
/// Converts string to integer type.
/// 
/// @tparam StringContainerType
///	Generic container type of the string we want to convert from.
/// 
/// @tparam ValueType
///	Value type we want to convert into.
/// 
/// @param[out] out_value
///	Reference to value we want to set.
/// 
/// @param in_string
///	String which contains the value we want to convert.
/// 
/// @param format
///	Value string representation. See StringToFloatConversionFormat.
///
/// @return
/// Number of characters processed.
template<
	utility::TextCharacter					StringCharacterType,
	internal_::StringToFloatConvertible		ValueType
>
constexpr auto StringToPrimitive(
	ValueType&						output_value,
	const StringCharacterType*		input_string,
	i64								input_string_size,
	StringToFloatConversionFormat		format		= StringToFloatConversionFormat::AUTOMATIC
) -> i64
{
	using CharacterType			= typename StringContainerType::CharacterType;
	using ASCIIType				= typename StringContainerType::template NonViewBaseType<char>;
	using ASCIIViewType			= typename ASCIIType::ViewType;

	auto FixEndPosition = []( ASCIIViewType in_string_view, i64 end_position ) -> i64
	{
		// On success, std::from_chars either returns pointer to the first non-number, or a pointer to
		// the last number, depending on if the string contained only numbers. Here we fix it so that
		// end position always points one past the last number.
		if( end_position < in_string_view.Size() ) {
			if( in_string_view[ end_position ] >= '0' && in_string_view[ end_position ] <= '9' ) {
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
			in_buffer_view = in_string;

		} else {
			in_buffer.Resize( in_string.Size() );
			for( i64 i = 0; i < in_string.Size(); i++ ) {
				in_buffer[ i ] = char( std::tolower( in_string[ i ] ) );
			}
			in_buffer_view = in_buffer;
		}
	}

	// Remove preceeding "+" symbols.
	while( !in_buffer_view.IsEmpty() && in_buffer_view.Front() == CharacterType( '+' ) ) {
		in_buffer_view = in_buffer_view.SubString( 1 );
	}

	// TODO: StringToPrimitive does not understand negative numbers if conversion format is deduced from the string.
	// Negative type should be allowed before prefix. Eg. "-0x30" does not actually work. "0x-30" does work but shouldn't.

	// If automatic type selection was used, auto-detect type from the string.
	if( format == StringToFloatConversionFormat::AUTOMATIC ) {
		if( in_buffer_view.Size() >= 2 ) {
			format = StringToFloatConversionFormat::DECIMAL;
			if( in_buffer_view[ 0 ] == CharacterType( '0' ) && std::tolower( in_buffer_view[ 1 ] ) == CharacterType( 'x' ) ) {
				in_buffer_view = in_buffer_view.SubString( 2 );
				format = StringToFloatConversionFormat::HEX;
			}
		}
	}

	// Convert to lower-case letters if conversion format was hexadecimal,
	// only do this for single byte characters as other types are already converted to lower.
	if constexpr( std::is_same_v<char, CharacterType> || std::is_same_v<char8_t, CharacterType> ) {
		if( std::underlying_type_t<StringToFloatConversionFormat>( format ) &
			std::underlying_type_t<StringToFloatConversionFormat>( StringToFloatConversionFormat::HEX ) ) {
			for( i64 i = 0; i < in_buffer.Size(); i++ ) {
				in_buffer[ i ] = tolower( in_buffer[ i ] );
			}
		}
	}

	{
		auto chars_format = std::chars_format::general;
		if( format == StringToFloatConversionFormat::HEX ) {
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

*/

} // conversion
} // bc
