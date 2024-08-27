
#include <core/PreCompiledHeader.hpp>
#include <core/conversion/text/primitives/StringToPrimitiveConversion.hpp>

#include <cmath>
#include <charconv>



namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <
	utility::Boolean				ValueType,
	utility::TextCharacter			TextCharacterType
>
static constexpr auto DoStringToBool(
	ValueType&						output_value,
	const TextCharacterType*		input_string_begin,
	const TextCharacterType*		input_string_end
) -> ::bc::conversion::StringToPrimitiveResult
{
	return MakeStringToPrimitiveResult(
		0,
		StringToPrimitiveResult::Outcome::ERROR
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <
	utility::SignedIntegral			ValueType,
	utility::TextCharacter			TextCharacterType
>
static constexpr auto DoStringToInteger(
	ValueType&						output_value,
	const TextCharacterType*		input_string_begin,
	const TextCharacterType*		input_string_end,
	StringToIntegerConversionFormat	format
) -> ::bc::conversion::StringToPrimitiveResult
{
	return MakeStringToPrimitiveResult(
		0,
		StringToPrimitiveResult::Outcome::ERROR
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <
	utility::UnsignedIntegral		ValueType,
	utility::TextCharacter			TextCharacterType
>
static constexpr auto DoStringToUnsignedInteger(
	ValueType&						output_value,
	const TextCharacterType*		input_string_begin,
	const TextCharacterType*		input_string_end,
	StringToIntegerConversionFormat	format
) -> ::bc::conversion::StringToPrimitiveResult
{
	return MakeStringToPrimitiveResult(
		0,
		StringToPrimitiveResult::Outcome::ERROR
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <
	utility::FloatingPoint			ValueType,
	utility::TextCharacter			TextCharacterType
>
static constexpr auto DoStringToFloat(
	ValueType&						output_value,
	const TextCharacterType*		input_string_begin,
	const TextCharacterType*		input_string_end,
	StringToFloatConversionFormat	format
) -> ::bc::conversion::StringToPrimitiveResult
{
	return MakeStringToPrimitiveResult(
		0,
		StringToPrimitiveResult::Outcome::ERROR
	);
}



} // namespace internal_
} // namespace conversion
} // namespace bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToBool_char(
	bool&			output_value,
	const char*		input_string_begin,
	const char*		input_string_end
) -> StringToPrimitiveResult
{
	return DoStringToBool(
		output_value,
		input_string_begin,
		input_string_end
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToBool_c8(
	bool&			output_value,
	const c8*		input_string_begin,
	const c8*		input_string_end
) -> StringToPrimitiveResult
{
	return DoStringToBool(
		output_value,
		input_string_begin,
		input_string_end
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToBool_c16(
	bool&			output_value,
	const c16*		input_string_begin,
	const c16*		input_string_end
) -> StringToPrimitiveResult
{
	return DoStringToBool(
		output_value,
		input_string_begin,
		input_string_end
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToBool_c32(
	bool&			output_value,
	const c32*		input_string_begin,
	const c32*		input_string_end
) -> StringToPrimitiveResult
{
	return DoStringToBool(
		output_value,
		input_string_begin,
		input_string_end
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToInteger_char(
	i64&							output_value,
	const char*						input_string_begin,
	const char*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToInteger_c8(
	i64&							output_value,
	const c8*						input_string_begin,
	const c8*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToInteger_c16(
	i64&							output_value,
	const c16*						input_string_begin,
	const c16*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToInteger_c32(
	i64&							output_value,
	const c32*						input_string_begin,
	const c32*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToUnsignedInteger_char(
	u64&							output_value,
	const char*						input_string_begin,
	const char*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToUnsignedInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToUnsignedInteger_c8(
	u64&							output_value,
	const c8*						input_string_begin,
	const c8*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToUnsignedInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToUnsignedInteger_c16(
	u64&							output_value,
	const c16*						input_string_begin,
	const c16*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToUnsignedInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToUnsignedInteger_c32(
	u64&							output_value,
	const c32*						input_string_begin,
	const c32*						input_string_end,
	StringToIntegerConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToUnsignedInteger(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToFloat_char(
	f64&							output_value,
	const char*						input_string_begin,
	const char*						input_string_end,
	StringToFloatConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToFloat(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToFloat_c8(
	f64&							output_value,
	const c8*						input_string_begin,
	const c8*						input_string_end,
	StringToFloatConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToFloat(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToFloat_c16(
	f64&							output_value,
	const c16*						input_string_begin,
	const c16*						input_string_end,
	StringToFloatConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToFloat(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr auto bc::conversion::internal_::StringToFloat_c32(
	f64&							output_value,
	const c32*						input_string_begin,
	const c32*						input_string_end,
	StringToFloatConversionFormat	format
) -> StringToPrimitiveResult
{
	return DoStringToFloat(
		output_value,
		input_string_begin,
		input_string_end,
		format
	);
}
