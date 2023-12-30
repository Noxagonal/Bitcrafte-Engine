
#pragma once

#include <core/containers/backend/ContainerBase.hpp>

#include <charconv>



namespace bc {
namespace conversion {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr size_t primitive_to_text_conversion_integer_reserve_space = 64;
constexpr size_t primitive_to_text_conversion_floating_reserve_space = 32;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept BoolToTextConvertible = std::is_same_v<ValueType, bool>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept IntegerToTextConvertible =
std::is_same_v<ValueType, int8_t> ||
std::is_same_v<ValueType, uint8_t> ||
std::is_same_v<ValueType, int16_t> ||
std::is_same_v<ValueType, uint16_t> ||
std::is_same_v<ValueType, int32_t> ||
std::is_same_v<ValueType, uint32_t> ||
std::is_same_v<ValueType, int64_t> ||
std::is_same_v<ValueType, uint64_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept FloatingToTextConvertible = std::is_floating_point_v<ValueType>;



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept PrimitiveToTextConvertibleValue =
internal::BoolToTextConvertible<ValueType> ||
internal::IntegerToTextConvertible<ValueType> ||
internal::FloatingToTextConvertible<ValueType>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts boolean into text.
///
///	Appends either "true" or "false" depending on input, not "1" or "0".
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
template<
	utility::TextContainer						TextContainerType,
	internal::BoolToTextConvertible				ValueType
>
constexpr size_t								PrimitiveToText(
	TextContainerType						&	append_to,
	const ValueType								value
)
{
	if( value ) {
		append_to.Append( "true" );
		return 4;
	} else {
		append_to.Append( "false" );
		return 5;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how integer numbers should be represented as text.
enum class IntegerToTextConversionFormat
{
	BINARY		= 2,		///< Base-2 (Does not append "0b")
	OCTAL		= 8,		///< Base-8 (Does not append "0o")
	DECIMAL		= 10,		///< Base-10
	HEX			= 16,		///< Base-16 (Does not append "0x")
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts an integer into text.
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
/// @param text_format
///	Number base to convert the number into. See IntegerToTextConversionFormat for more info. You can also use other types. For
/// example if you want Base-5 you can just give this parameter static_cast<IntegerToTextConversionFormat>( 5 ).
template<
	utility::TextContainer						TextContainerType,
	internal::IntegerToTextConvertible			ValueType
>
constexpr size_t								PrimitiveToText(
	TextContainerType						&	append_to,
	const ValueType								value,
	IntegerToTextConversionFormat				text_format		= IntegerToTextConversionFormat::DECIMAL
)
{
	using ContainerBaseCharType = typename TextContainerType::template ThisContainerFullType<char>;
	using TextContainerCharacterType = typename TextContainerType::ContainedCharacterType;

	size_t write_length;

	if constexpr( std::is_same_v<TextContainerCharacterType, char> ) {
		auto original_size	= append_to.Size();
		append_to.Resize( original_size + internal::primitive_to_text_conversion_integer_reserve_space );
		auto begin			= append_to.Data() + original_size;
		auto end			= append_to.Data() + ( original_size + internal::primitive_to_text_conversion_integer_reserve_space );
		auto result			= std::to_chars( begin, end, value, int( text_format ) );

		if( result.ec == std::errc() ) {
			// Success
			end = result.ptr;
		} else {
			// Failure
			end = begin;
			assert( 0 && "Should not get here with integer values" );
		}
		write_length		= end - begin;
		append_to.Resize( original_size + write_length );

	} else {
		ContainerBaseCharType buffer;
		// Text8 buffer;
		buffer.Resize( internal::primitive_to_text_conversion_integer_reserve_space );
		auto begin			= buffer.Data();
		auto end			= buffer.Data() + internal::primitive_to_text_conversion_integer_reserve_space;
		auto result			= std::to_chars( begin, end, value, int( text_format ) );

		if( result.ec == std::errc() ) {
			// Success
			end = result.ptr;
		} else {
			// Failure
			end = begin;
			assert( 0 && "Should not get here with integer values" );
		}
		write_length		= end - begin;
		buffer.Resize( write_length );
		append_to.Append( buffer );
	}

	return write_length;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how floating point numbers should be represented as text.
/// 
///	These can be used together. Eg SCIENTIFIC | HEX;
enum class FloatToTextConversionFormat : uint32_t
{
	GENERAL			= static_cast<uint32_t>( std::chars_format::general ),		///< Uses fixed format for smaller values and scientific format for larger values. (Default)
	SCIENTIFIC		= static_cast<uint32_t>( std::chars_format::scientific ),	///< Output scientific notation, use mantissa/exponent.
	FIXED			= static_cast<uint32_t>( std::chars_format::fixed ),		///< Always use fixed format, regardless how big the value is.
	HEX				= static_cast<uint32_t>( std::chars_format::hex ),			///< Output as hexadecimal floating point. (lowercase hexadecimal letters, does not append "0x")
};
inline FloatToTextConversionFormat operator|( FloatToTextConversionFormat a, FloatToTextConversionFormat b )
{
	return FloatToTextConversionFormat(
		std::underlying_type_t<FloatToTextConversionFormat>( a ) |
		std::underlying_type_t<FloatToTextConversionFormat>( b )
	);
}
inline FloatToTextConversionFormat operator&( FloatToTextConversionFormat a, FloatToTextConversionFormat b )
{
	return FloatToTextConversionFormat(
		std::underlying_type_t<FloatToTextConversionFormat>( a ) &
		std::underlying_type_t<FloatToTextConversionFormat>( b )
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
/// @param text_format
///	Value text representation. See FloatToTextConversionFormat.
template<
	utility::TextContainer						TextContainerType,
	internal::FloatingToTextConvertible			ValueType
>
constexpr size_t								PrimitiveToText(
	TextContainerType						&	append_to,
	const ValueType								value,
	FloatToTextConversionFormat					text_format		= FloatToTextConversionFormat::GENERAL
)
{
	using ContainerBaseCharType = typename TextContainerType::template ThisContainerFullType<char>;
	using TextContainerCharacterType = typename TextContainerType::ContainedCharacterType;

	size_t write_length = 0;
	auto original_size	= append_to.Size();

	auto Convert = [original_size, value, text_format]( ContainerBaseCharType & append_to, size_t reserve_space ) -> std::to_chars_result
	{
		append_to.Resize( original_size + reserve_space );
		auto begin					= append_to.Data() + original_size;
		auto end					= append_to.Data() + ( original_size + reserve_space );
		auto editable_text_format	= text_format;

		if( !(
			std::underlying_type_t<conversion::FloatToTextConversionFormat>( editable_text_format & conversion::FloatToTextConversionFormat::GENERAL ) ||
			std::underlying_type_t<conversion::FloatToTextConversionFormat>( editable_text_format & conversion::FloatToTextConversionFormat::SCIENTIFIC ) ||
			std::underlying_type_t<conversion::FloatToTextConversionFormat>( editable_text_format & conversion::FloatToTextConversionFormat::FIXED ) ||
			std::underlying_type_t<conversion::FloatToTextConversionFormat>( editable_text_format & conversion::FloatToTextConversionFormat::HEX )
			) ) {

			// Invalid float to text conversion format, fall back to general.
			editable_text_format = conversion::FloatToTextConversionFormat::GENERAL;
		}

		return std::to_chars( begin, end, value, std::chars_format( editable_text_format ) );
	};

	if constexpr( std::is_same_v<TextContainerCharacterType, char> ) {

		auto result = Convert( append_to, internal::primitive_to_text_conversion_floating_reserve_space );

		if( result.ec != std::errc() ) {
			// Failure, try again with much larger buffer. This will be enough to store
			// doubles in fixed size. Double max can be represented in 309 characters.
			append_to.Resize( original_size );
			result = Convert( append_to, 310 );
			assert( result.ec == std::errc() );
		}

		//write_length = result.ptr - ( append_to.Data() + append_to.Size() );
		auto final_size = result.ptr - append_to.Data();
		append_to.Resize( final_size );

		write_length = final_size - original_size;

	} else {
		ContainerBaseCharType buffer;
		// Text8 buffer;
		auto result = Convert( buffer, internal::primitive_to_text_conversion_floating_reserve_space );

		if( result.ec != std::errc() ) {
			// Failure, try again with much larger buffer. This will be enough to store
			// doubles in fixed size. Double max can be represented in 309 characters.
			append_to.Resize( original_size );
			result = Convert( buffer, 310 );
			assert( result.ec == std::errc() );
		}

		write_length = result.ptr - buffer.Data();
		buffer.Resize( write_length );
		append_to.Append( buffer );
	}

	return write_length;
}



} // conversion
} // bc
