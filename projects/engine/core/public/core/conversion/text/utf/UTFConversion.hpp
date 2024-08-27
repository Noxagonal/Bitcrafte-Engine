#pragma once

#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/concepts/ContainerConcepts.hpp>



namespace bc {
namespace conversion {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Result of UTF conversion.
struct UTFConvertResult
{
	enum class Outcome : u8
	{
		SUCCESS,			///< The entire string was converted.
		INCOMPLETE,			///< Partial success, not the entire text was converted because lack of space in output buffer.
		ERROR,				///< Conversion may have completed to some extent but the utf string had error that prevent further conversion.
	};

	i64		code_unit_count;    ///< Output buffer size used (e.g, number of bytes for UTF-8, shorts for UTF-16, or words for UTF-32).
	i64		code_point_count;   ///< Number of Unicode code points (characters) written to the output buffer. May be -1 if not calculated.
	Outcome	outcome;			///< Conversion outcome, indicates if the conversion was successful.
};

namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static auto MakeUTFConvertResult(
	i64							code_unit_count,
	i64							code_point_count,
	UTFConvertResult::Outcome	outcome
) -> UTFConvertResult
{
	// Update this if UTFConvertResult changes.
	return UTFConvertResult {
		code_unit_count,
		code_point_count,
		outcome
	};
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter	ToCharacterType,
	utility::TextCharacter	FromCharacterType
>
auto UTFCopy(
	ToCharacterType*			output_string_begin,
	ToCharacterType*			output_string_end,
	const FromCharacterType*	source_string_begin,
	const FromCharacterType*	source_string_end
) -> UTFConvertResult
{
	auto output_size = output_string_end - output_string_begin;
	auto source_size = source_string_end - source_string_begin;
	auto copy_size = std::min( output_size, source_size );
	for( i64 i = 0; i < copy_size; ++i )
	{
		output_string_begin[ i ] = static_cast<ToCharacterType>( source_string_begin[ i ] );
	}
	return internal_::MakeUTFConvertResult(
		copy_size,
		-1, // Code point count may differ from code unit count, but I don't think it's worth calculating it, might remove this field. Leaving as -1 as an unknown. 
		( output_size < source_size ) ? UTFConvertResult::Outcome::INCOMPLETE : UTFConvertResult::Outcome::SUCCESS
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC16ToC8(
	c8*			output_string_begin,
	c8*			output_string_end,
	const c16*	source_string_begin,
	const c16*	source_string_end
) -> UTFConvertResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC32ToC8(
	c8*			output_string_begin,
	c8*			output_string_end,
	const c32*	source_string_begin,
	const c32*	source_string_end
) -> UTFConvertResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC8ToC16(
	c16*		output_string_begin,
	c16*		output_string_end,
	const c8*	source_string_begin,
	const c8*	source_string_end
) -> UTFConvertResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC32ToC16(
	c16*		output_string_begin,
	c16*		output_string_end,
	const c32*	source_string_begin,
	const c32*	source_string_end
) -> UTFConvertResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC8ToC32(
	c32*		output_string_begin,
	c32*		output_string_end,
	const c8*	source_string_begin,
	const c8*	source_string_end
) -> UTFConvertResult;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto UTFC16ToC32(
	c32*		output_string_begin,
	c32*		output_string_end,
	const c16*	source_string_begin,
	const c16*	source_string_end
) -> UTFConvertResult;



} // namespace internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextCharacter	ToCharacterType,
	utility::TextCharacter	FromCharacterType
>
auto UTFConvert(
	ToCharacterType*			output_string_begin,
	ToCharacterType*			output_string_end,
	const FromCharacterType*	source_string_begin,
	const FromCharacterType*	source_string_end
) -> UTFConvertResult
{
	auto output_size = output_string_end - output_string_begin;
	auto source_size = source_string_end - source_string_begin;

	if( output_size <= 0 )
	{
		return internal_::MakeUTFConvertResult(
			0,
			0,
			( source_size > 0 ) ? UTFConvertResult::Outcome::SUCCESS : UTFConvertResult::Outcome::ERROR
		);
	}

	if constexpr( utility::SameAs<ToCharacterType, FromCharacterType> || utility::SameAs<FromCharacterType, char> )
	{
		// same type	-> same type
		// char			-> any type
		return internal_::UTFCopy(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c8> && utility::SameAs<FromCharacterType, c16> )
	{
		// c16 -> c8
		return internal_::UTFC16ToC8(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c8> && utility::SameAs<FromCharacterType, c32> )
	{
		// c32 -> c8
		return internal_::UTFC32ToC8(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c16> && utility::SameAs<FromCharacterType, c8> )
	{
		// c8 -> c16
		return internal_::UTFC8ToC16(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c16> && utility::SameAs<FromCharacterType, c32> )
	{
		// c32 -> c16
		return internal_::UTFC32ToC16(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c32> && utility::SameAs<FromCharacterType, c8> )
	{
		// c8 -> c32
		return internal_::UTFC8ToC32(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}
	else if constexpr( utility::SameAs<ToCharacterType, c32> && utility::SameAs<FromCharacterType, c16> )
	{
		// c16 -> c32
		return internal_::UTFC16ToC32(
			output_string_begin,
			output_string_end,
			source_string_begin,
			source_string_end
		);
	}

	return internal_::MakeUTFConvertResult(
		0,
		0,
		UTFConvertResult::Outcome::ERROR
	);
}



} // conversion
} // bc
