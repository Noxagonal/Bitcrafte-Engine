
#pragma once

#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/concepts/ContainerConcepts.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>



namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept BoolToStringConvertible = utility::SameAs<ValueType, bool>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept IntegerToStringConvertible =
	utility::SameAs<ValueType, i8> ||
	utility::SameAs<ValueType, i16> ||
	utility::SameAs<ValueType, i32> ||
	utility::SameAs<ValueType, i64>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept UnsignedIntegerToStringConvertible =
	utility::SameAs<ValueType, u8> ||
	utility::SameAs<ValueType, u16> ||
	utility::SameAs<ValueType, u32> ||
	utility::SameAs<ValueType, u64>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept FloatToStringConvertible = std::is_floating_point_v<ValueType>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ValueType>
concept PrimitiveToStringConvertible =
	BoolToStringConvertible<ValueType> ||
	IntegerToStringConvertible<ValueType> ||
	UnsignedIntegerToStringConvertible<ValueType> ||
	FloatToStringConvertible<ValueType>;



} // namespace internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how integer numbers should be represented as text.
enum class IntegerToStringConversionFormat : u8
{
	BINARY		= 2,		///< Base-2
	OCTAL		= 8,		///< Base-8
	DECIMAL		= 10,		///< Base-10
	HEX			= 16,		///< Base-16
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Tells how floating point numbers should be represented as text.
enum class FloatToStringConversionFormat : u8
{
	GENERAL		= 1 << 0,	///< Uses fixed format for smaller values and scientific format for larger values. (Default)
	SCIENTIFIC	= 1 << 1,	///< Output scientific notation, use mantissa/exponent.
	FIXED		= 1 << 2,	///< Always use fixed format, regardless how big the value is.
	HEX			= 1 << 3,	///< Output as hexadecimal floating point. (lowercase hexadecimal letters, does not append "0x")
};
inline auto operator|( FloatToStringConversionFormat lhs, FloatToStringConversionFormat rhs ) -> FloatToStringConversionFormat
{
	return FloatToStringConversionFormat( std::to_underlying( lhs ) | std::to_underlying( rhs ) );
}
inline auto operator&( FloatToStringConversionFormat lhs, FloatToStringConversionFormat rhs ) -> FloatToStringConversionFormat
{
	return FloatToStringConversionFormat( std::to_underlying( lhs ) & std::to_underlying( rhs ) );
}
inline auto operator|=( FloatToStringConversionFormat& lhs, FloatToStringConversionFormat rhs ) -> FloatToStringConversionFormat
{
	lhs = lhs | rhs;
	return lhs;
}
inline auto operator&=( FloatToStringConversionFormat& lhs, FloatToStringConversionFormat rhs ) -> FloatToStringConversionFormat
{
	lhs = lhs & rhs;
	return lhs;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PrimitiveToStringResult
{
	enum class Outcome : u8
	{
		// TODO: Do UNDETERMINED on other results as well.
		UNDETERMINED = 0,	///< Conversion was not run, primarily used for debugging as this is 0 by default.
		SUCCESS,			///< The entire value was converted into a string.
		INCOMPLETE,			///< Value was not converted into text because lack of space in output buffer.
		ERROR,				///< Conversion had errors, or invalid input.
	};
	i64		length;			///< Length of the resulting text.
	Outcome	outcome;		///< Conversion outcome, indicates if the conversion was successful.
};



namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline auto MakePrimitiveToStringResult(
	i64									length,
	PrimitiveToStringResult::Outcome	outcome
) -> PrimitiveToStringResult
{
	return PrimitiveToStringResult{ length, outcome };
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto BoolToString_char(
	char*	output_string_begin,
	char* 	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto BoolToString_c8(
	c8*		output_string_begin,
	c8* 	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto BoolToString_c16(
	c16*	output_string_begin,
	c16*	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto BoolToString_c32(
	c32*	output_string_begin,
	c32* 	output_string_end,
	bool	input_value,
	bool	uppercase
) -> PrimitiveToStringResult;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto IntegerToString_char(
	char*							output_string_begin,
	char* 							output_string_end,
	i64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto IntegerToString_c8(
	c8*								output_string_begin,
	c8* 							output_string_end,
	i64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto IntegerToString_c16(
	c16*							output_string_begin,
	c16* 							output_string_end,
	i64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto IntegerToString_c32(
	c32*							output_string_begin,
	c32* 							output_string_end,
	i64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto UnsignedIntegerToString_char(
	char*							output_string_begin,
	char* 							output_string_end,
	u64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto UnsignedIntegerToString_c8(
	c8*								output_string_begin,
	c8* 							output_string_end,
	u64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto UnsignedIntegerToString_c16(
	c16*							output_string_begin,
	c16*							output_string_end,
	u64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto UnsignedIntegerToString_c32(
	c32*							output_string_begin,
	c32* 							output_string_end,
	u64								input_value,
	IntegerToStringConversionFormat	format,
	bool 							add_prefix,
	bool 							uppercase,
	i64								zerofill_to_length
) -> PrimitiveToStringResult;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto FloatToString_char(
	char*							output_string_begin,
	char* 							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	format,
	i64 							precision
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto FloatToString_c8(
	c8*								output_string_begin,
	c8*	 							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	format,
	i64 							precision
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto FloatToString_c16(
	c16*							output_string_begin,
	c16* 							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	format,
	i64 							precision
) -> PrimitiveToStringResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
constexpr auto FloatToString_c32(
	c32*							output_string_begin,
	c32* 							output_string_end,
	f64								input_value,
	FloatToStringConversionFormat	format,
	i64 							precision
) -> PrimitiveToStringResult;



} // namespace internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter				TextCharacterType,
	internal_::BoolToStringConvertible	ValueType
>
constexpr auto BoolToString(
	TextCharacterType*	output_string_begin,
	TextCharacterType*	output_string_end,
	ValueType			input_value,
	bool				uppercase		= false
) -> PrimitiveToStringResult
{
	if constexpr( utility::SameAs<TextCharacterType, char> )
	{
		return internal_::BoolToString_char(
			output_string_begin,
			output_string_end,
			input_value,
			uppercase
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c8> )
	{
		return internal_::BoolToString_c8(
			output_string_begin,
			output_string_end,
			input_value,
			uppercase
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c16> )
	{
		return internal_::BoolToString_c16(
			output_string_begin,
			output_string_end,
			input_value,
			uppercase
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c32> )
	{
		return internal_::BoolToString_c32(
			output_string_begin,
			output_string_end,
			input_value,
			uppercase
		);
	}
	else
	{
		return internal_::MakePrimitiveToStringResult( 0, PrimitiveToStringResult::Outcome::ERROR );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter					TextCharacterType,
	internal_::IntegerToStringConvertible	ValueType
>
constexpr auto IntegerToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	IntegerToStringConversionFormat	format			= IntegerToStringConversionFormat::DECIMAL,
	bool 							add_prefix			= false,
	bool 							uppercase			= true,
	i64								zerofill_to_length	= 0
) -> PrimitiveToStringResult
{
	if constexpr( utility::SameAs<TextCharacterType, char> )
	{
		return internal_::IntegerToString_char(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c8> )
	{
		return internal_::IntegerToString_c8(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c16> )
	{
		return internal_::IntegerToString_c16(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c32> )
	{
		return internal_::IntegerToString_c32(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else
	{
		return internal_::MakePrimitiveToStringResult( 0, PrimitiveToStringResult::Outcome::ERROR );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter							TextCharacterType,
	internal_::UnsignedIntegerToStringConvertible	ValueType
>
constexpr auto UnsignedIntegerToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	IntegerToStringConversionFormat	format				= IntegerToStringConversionFormat::DECIMAL,
	bool 							add_prefix			= false,
	bool 							uppercase			= true,
	i64								zerofill_to_length	= 0
) -> PrimitiveToStringResult
{
	if constexpr( utility::SameAs<TextCharacterType, char> )
	{
		return internal_::UnsignedIntegerToString_char(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c8> )
	{
		return internal_::UnsignedIntegerToString_c8(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c16> )
	{
		return internal_::UnsignedIntegerToString_c16(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c32> )
	{
		return internal_::UnsignedIntegerToString_c32(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			add_prefix,
			uppercase,
			zerofill_to_length
		);
	}
	else
	{
		return internal_::MakePrimitiveToStringResult( 0, PrimitiveToStringResult::Outcome::ERROR );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter				TextCharacterType,
	internal_::FloatToStringConvertible	ValueType
>
constexpr auto FloatToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	FloatToStringConversionFormat	format				= FloatToStringConversionFormat::GENERAL,
	i64 							precision			= -1
) -> PrimitiveToStringResult
{
	if constexpr( utility::SameAs<TextCharacterType, char> )
	{
		return internal_::FloatToString_char(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			precision
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c8> )
	{
		return internal_::FloatToString_c8(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			precision
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c16> )
	{
		return internal_::FloatToString_c16(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			precision
		);
	}
	else if constexpr( utility::SameAs<TextCharacterType, c32> )
	{
		return internal_::FloatToString_c32(
			output_string_begin,
			output_string_end,
			input_value,
			format,
			precision
		);
	}
	else
	{
		return internal_::MakePrimitiveToStringResult( 0, PrimitiveToStringResult::Outcome::ERROR );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter				TextCharacterType,
	internal_::BoolToStringConvertible	ValueType
>
constexpr auto PrimitiveToString(
	TextCharacterType*	output_string_begin,
	TextCharacterType*	output_string_end,
	ValueType			input_value,
	bool				uppercase		= false
) -> PrimitiveToStringResult
{
	return BoolToString(
		output_string_begin,
		output_string_end,
		input_value,
		uppercase
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter					TextCharacterType,
	internal_::IntegerToStringConvertible	ValueType
>
constexpr auto PrimitiveToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	IntegerToStringConversionFormat	format				= IntegerToStringConversionFormat::DECIMAL,
	bool 							add_prefix			= false,
	bool 							uppercase			= true,
	i64								zerofill_to_length	= 0
) -> PrimitiveToStringResult
{
	return IntegerToString(
		output_string_begin,
		output_string_end,
		input_value,
		format,
		add_prefix,
		uppercase,
		zerofill_to_length
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter							TextCharacterType,
	internal_::UnsignedIntegerToStringConvertible	ValueType
>
constexpr auto PrimitiveToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	IntegerToStringConversionFormat	format				= IntegerToStringConversionFormat::DECIMAL,
	bool 							add_prefix			= false,
	bool 							uppercase			= true,
	i64								zerofill_to_length	= 0
) -> PrimitiveToStringResult
{
	return UnsignedIntegerToString(
		output_string_begin,
		output_string_end,
		input_value,
		format,
		add_prefix,
		uppercase,
		zerofill_to_length
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter					TextCharacterType,
	internal_::FloatToStringConvertible		ValueType
>
constexpr auto PrimitiveToString(
	TextCharacterType*				output_string_begin,
	TextCharacterType*				output_string_end,
	ValueType						input_value,
	FloatToStringConversionFormat	format				= FloatToStringConversionFormat::GENERAL,
	i64 							precision			= -1
) -> PrimitiveToStringResult
{
	return FloatToString(
		output_string_begin,
		output_string_end,
		input_value,
		format,
		precision
	);
}



} // namespace conversion
} // namespace bc
