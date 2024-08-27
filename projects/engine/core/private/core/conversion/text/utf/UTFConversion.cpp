
#include <core/PreCompiledHeader.hpp>

#include <core/conversion/text/utf/UTFConversion.hpp>



/*
namespace bc {
namespace conversion {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InputType>
static auto UTF_NarrowingConversion(
	c8*					output_text,
	i64					output_text_size,
	const InputType*	input_text,
	i64					input_text_size
) -> UTFConvertResult
{
	auto state = mbstate_t {};
	char mb_character_result[ MB_LEN_MAX ];

	auto output_it = output_text;
	i64 code_unit_count = 0;
	i64 code_point_count = 0;

	for( i64 i = 0; i < input_text_size; ++i )
	{
		size_t conversion_result;
		if constexpr( utility::SameAs<InputType, c16> )
		{
			conversion_result = std::c16rtomb( mb_character_result, input_text[ i ], &state );
		}
		else if constexpr( utility::SameAs<InputType, c32> )
		{
			conversion_result = std::c32rtomb( mb_character_result, input_text[ i ], &state );
		}

		if( conversion_result == size_t( -1 ) ) [[unlikely]]
		{
			return MakeUTFConvertResult(
				code_unit_count,
				code_point_count,
				UTFConvertResult::Outcome::ERROR
			);
		}
		// TODO: Figure out if this is just multi-code-unit utf-16, in which case we should not ignore this.
		if( conversion_result == 0 ) [[unlikely]] continue;

		code_unit_count += conversion_result;
		i64 remaining_out_length = output_text_size - code_unit_count;
		if( remaining_out_length < conversion_result )
		{
			return MakeUTFConvertResult(
				code_unit_count,
				code_point_count,
				UTFConvertResult::Outcome::INCOMPLETE
			);
		}
		for( size_t c = 0; c < conversion_result; ++c )
		{
			*output_it = mb_character_result[ c ];
			++output_it;
		}
		++code_point_count;
	}

	return MakeUTFConvertResult(
		code_unit_count,
		code_point_count,
		UTFConvertResult::Outcome::SUCCESS
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename OutputType>
static auto UTF_WideningConversion(
	OutputType*	output_text,
	i64			output_text_size,
	const c8*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	auto state = std::mbstate_t {};
	c16 character_result = {};
	i64 code_point_count = 0;
	i64 code_unit_count = 0;
	auto input_text_it	= input_text;
	auto input_text_end	= input_text + input_text_size;

	while( true )
	{
		size_t conversion_result;
		if constexpr( utility::SameAs<OutputType, c16> )
		{
			conversion_result = std::mbrtoc16( &character_result, input_text_it, input_text_end - input_text_it + 1, &state );
		}
		else if constexpr( utility::SameAs<OutputType, c32> )
		{
			conversion_result = std::mbrtoc32( &character_result, input_text_it, input_text_end - input_text_it + 1, &state );
		}

		if( conversion_result == 0 )
		{
			return MakeUTFConvertResult(
				code_unit_count,
				code_point_count,
				UTFConvertResult::Outcome::SUCCESS
			);
		}
		else if( conversion_result == size_t( -3 ) ) [[unlikely]]
			{
				// Surrogate pair
				//++input_text_it;
				++code_unit_count;
				continue;
			}
		else if( conversion_result == size_t( -2 ) || conversion_result == size_t( -1 ) ) [[unlikely]]
			{
				// Invalid or truncated input
				return MakeUTFConvertResult(
					code_unit_count,
					code_point_count,
					UTFConvertResult::Outcome::ERROR
				);
			}
		else
		{
			input_text_it += conversion_result;
			++code_unit_count;
			++code_point_count;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename OutputType,
	typename InputType
>
static auto UTF_NarrowingThenWideningConversion(
	OutputType*			output_text,
	i64					output_text_size,
	const InputType*	input_text,
	i64					input_text_size
) -> UTFConvertResult
{
	auto narrowing_state	= mbstate_t {};
	auto widening_state		= mbstate_t {};

	auto input_text_it		= input_text;
	auto output_text_it		= output_text;

	i64 code_unit_count		= 0;
	i64 code_point_count	= 0;

	char mb_character_result[ MB_LEN_MAX ];
	for( i64 i = 0; i < input_text_size; ++i )
	{
		size_t narrowing_conversion_result;
		if constexpr( utility::SameAs<InputType, c16> )
		{
			narrowing_conversion_result = std::c16rtomb( mb_character_result, input_text[ i ], &narrowing_state );
		}
		else if constexpr( utility::SameAs<InputType, c32> )
		{
			narrowing_conversion_result = std::c32rtomb( mb_character_result, input_text[ i ], &narrowing_state );
		}

		if( narrowing_conversion_result == size_t( -1 ) ) [[unlikely]]
		{
			return MakeUTFConvertResult(
				code_unit_count,
				code_point_count,
				UTFConvertResult::Outcome::ERROR
			);
		}
		// TODO: Figure out if this is just multi-code-unit utf-16, in which case we should not ignore this.
		if( narrowing_conversion_result == 0 ) [[unlikely]] continue;
		mb_character_result[ narrowing_conversion_result ] = '\0';

		OutputType character_result = {};
		while( true )
		{
			size_t widening_conversion_result;
			if constexpr( utility::SameAs<OutputType, c16> )
			{
				widening_conversion_result = std::mbrtoc16( &character_result, mb_character_result, narrowing_conversion_result, &widening_state );
			}
			else if constexpr( utility::SameAs<OutputType, c32> )
			{
				widening_conversion_result = std::mbrtoc32( &character_result, mb_character_result, narrowing_conversion_result, &widening_state );
			}
			if( widening_conversion_result == 0 ) break;
			else if( widening_conversion_result == size_t( -3 ) ) [[unlikely]]
			{
				// Surrogate pair
				if( code_unit_count >= output_text_size )
				{
					return MakeUTFConvertResult(
						code_unit_count,
						code_point_count,
						UTFConvertResult::Outcome::INCOMPLETE
					);
				}
				*output_text_it = character_result;
				++output_text_it;
				++code_unit_count;
				continue;
			}
			else if( widening_conversion_result == size_t( -2 ) || widening_conversion_result == size_t( -1 ) ) [[unlikely]]
			{
				// Invalid or truncated input
				return MakeUTFConvertResult(
					code_unit_count,
					code_point_count,
					UTFConvertResult::Outcome::ERROR
				);
			}
			else
			{
				*output_text_it = character_result;
				input_text_it += widening_conversion_result;
				++code_unit_count;
				++code_point_count;
			}
		}
	}

	return MakeUTFConvertResult(
		code_unit_count,
		code_point_count,
		UTFConvertResult::Outcome::SUCCESS
	);
}



} // namespace internal_
} // namespace conversion
} // namespace bc
*/



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC16ToC8(
	c8*			output_string_begin,
	c8*			output_string_end,
	const c16*	source_string_begin,
	const c16*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC32ToC8(
	c8*			output_string_begin,
	c8*			output_string_end,
	const c32*	source_string_begin,
	const c32*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC8ToC16(
	c16*		output_string_begin,
	c16*		output_string_end,
	const c8*	source_string_begin,
	const c8*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC32ToC16(
	c16*		output_string_begin,
	c16*		output_string_end,
	const c32*	source_string_begin,
	const c32*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC8ToC32(
	c32*		output_string_begin,
	c32*		output_string_end,
	const c8*	source_string_begin,
	const c8*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTFC16ToC32(
	c32*		output_string_begin,
	c32*		output_string_end,
	const c16*	source_string_begin,
	const c16*	source_string_end
) -> UTFConvertResult
{
	return MakeUTFConvertResult( 0, 0, UTFConvertResult::Outcome::ERROR );
}










/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C8FromC16(
	c8*			output_text,
	i64			output_text_size,
	const c16*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_NarrowingConversion<c16>( output_text, output_text_size, input_text, input_text_size );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C8FromC32(
	c8*			output_text,
	i64			output_text_size,
	const c32*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_NarrowingConversion<c32>( output_text, output_text_size, input_text, input_text_size );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C16FromC8(
	c16*		output_text,
	i64			output_text_size,
	const c8*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_WideningConversion<c16>( output_text, output_text_size, input_text, input_text_size );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C16FromC32(
	c16*		output_text,
	i64			output_text_size,
	const c32*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_NarrowingThenWideningConversion<c16, c32>( output_text, output_text_size, input_text, input_text_size );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C32FromC8(
	c32*		output_text,
	i64			output_text_size,
	const c8*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_WideningConversion<c32>( output_text, output_text_size, input_text, input_text_size );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::conversion::internal_::UTF_C32FromC16(
	c32*		output_text,
	i64			output_text_size,
	const c16*	input_text,
	i64			input_text_size
) -> UTFConvertResult
{
	return UTF_NarrowingThenWideningConversion<c32, c16>( output_text, output_text_size, input_text, input_text_size );
}
*/
