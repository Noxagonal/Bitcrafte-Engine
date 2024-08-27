#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/utility/concepts/ContainerConcepts.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/conversion/text/primitives/PrimitiveToStringConversion.hpp>
#include <core/conversion/text/utf/UTFConversion.hpp>
#include <core/memory/pod_auto_buffer/PODAutoBuffer.hpp>

#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto MakePrintRecord(
	const c32*			c_string,
	i64					length,
	PrintRecordTheme	theme		= PrintRecordTheme::DEFAULT
) noexcept -> PrintRecord;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<i64 CharacterArraySize>
auto MakePrintRecord(
	const c32( &c_string_literal )[ CharacterArraySize ],
	PrintRecordTheme theme = PrintRecordTheme::DEFAULT
) noexcept -> PrintRecord
{
	return MakePrintRecord(
		c_string_literal,
		CharacterArraySize ? ( c_string_literal[ CharacterArraySize - 1 ] == '\0' ? CharacterArraySize - 1 : CharacterArraySize ) : 0,
		theme
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainer TextContainerType>
auto MakePrintRecord(
	const TextContainerType&	text,
	PrintRecordTheme			theme	= PrintRecordTheme::DEFAULT
) noexcept -> PrintRecord
{
	using TextType = std::decay_t<TextContainerType>;
	using TextCharacterType = typename TextType::ContainedCharacterType;

	static_assert( std::is_same_v<TextCharacterType, c32>, "PrintRecord only supports utf-32 characters" );

	return MakePrintRecord( text.Data(), text.Size(), theme );
}



namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ArgumentValueType>
auto MakePrintRecord_RawArgument(
	const c32*					c_string_argument_description,
	i64							argument_description_length,
	const ArgumentValueType&	argument_value
) noexcept -> PrintRecord
	requires( !utility::SameAs<ArgumentValueType, PrintRecordTheme> ) // Prevent PrintRecordTheme from being used as argument value
{
	auto record = MakePrintRecord( c_string_argument_description, argument_description_length );
	record += MakePrintRecord( U": \"" );
	if constexpr( utility::TextContainerView<ArgumentValueType> )
	{
		auto buffer = memory::PODAutoBuffer<c32>( argument_value.Size() );
		auto result = conversion::UTFConvert<c32>(
			buffer.Data(),
			buffer.Data() + buffer.Size(),
			argument_value.Data(),
			argument_value.Data() + argument_value.Size()
		);
		if( result.outcome != conversion::UTFConvertResult::Outcome::SUCCESS )
		{
			record += MakePrintRecord( U"[UTF conversion error]" );
			return record;
		}
		record += MakePrintRecord( buffer.Data(), result.code_unit_count );
	}
	else if constexpr( utility::Fundamental<ArgumentValueType> )
	{
		[ &record, argument_value ]()
			{
				constexpr auto buffer_size = 32;
				c32 buffer[ buffer_size ];

				auto result = conversion::PrimitiveToString( buffer, buffer + buffer_size, argument_value );
				if( result.outcome == conversion::PrimitiveToStringResult::Outcome::SUCCESS )
				{
					record += MakePrintRecord( buffer, result.length );
					return;
				}
				if( result.outcome == conversion::PrimitiveToStringResult::Outcome::INCOMPLETE )
				{
					// Try again with a large heap buffer.
					auto heap_buffer = memory::PODAutoBuffer<c32>( 1024 );
					result = conversion::PrimitiveToString( heap_buffer.Data(), heap_buffer.Data() + heap_buffer.Size(), argument_value );
					if( result.outcome != conversion::PrimitiveToStringResult::Outcome::SUCCESS )
					{
						record += MakePrintRecord( U"[Primitive to text conversion error]" );
						return;
					}
					record += MakePrintRecord( heap_buffer.Data(), result.length );
					return;
				}

				record += MakePrintRecord( U"[Primitive to text conversion error]" );
			}( );
	}
	else
	{
		assert( 0 && "Unsupported argument type" );
	}
	record += MakePrintRecord( U"\"" );
	return record;
}

} // namespace internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	i64			CharacterArraySize,
	typename	ArgumentValueType
>
auto MakePrintRecord_Argument(
	const c32( &c_string_argument_description )[ CharacterArraySize ],
	const ArgumentValueType& argument_value
) noexcept -> PrintRecord
	requires( !utility::SameAs<ArgumentValueType, PrintRecordTheme> ) // Prevent PrintRecordTheme from being used as argument value
{
	return internal_::MakePrintRecord_RawArgument(
		c_string_argument_description,
		CharacterArraySize ? ( c_string_argument_description[ CharacterArraySize - 1 ] == '\0' ? CharacterArraySize - 1 : CharacterArraySize ) : 0,
		argument_value
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	ArgumentType,
	typename					ArgumentValueType
>
auto MakePrintRecord_Argument(
	const ArgumentType&			text_argument_description,
	const ArgumentValueType&	argument_value
) noexcept -> PrintRecord
	requires( !utility::SameAs<ArgumentValueType, PrintRecordTheme> ) // Prevent PrintRecordTheme from being used as argument value
{
	return internal_::MakePrintRecord_RawArgument(
		text_argument_description.Data(),
		text_argument_description.Size(),
		argument_value
	);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::Fundamental	ArgumentType,
	typename				ArgumentValueType
>
auto MakePrintRecord_Argument(
	const ArgumentType			value_argument_description,
	const ArgumentValueType&	argument_value
) noexcept -> PrintRecord
	requires( !utility::SameAs<ArgumentValueType, PrintRecordTheme> ) // Prevent PrintRecordTheme from being used as argument value
{
	constexpr auto conversion_error_text = U"[Primitive to text conversion error]";
	constexpr auto conversion_error_text_length = std::char_traits<c32>::length( conversion_error_text );

	constexpr auto buffer_size = 64;
	c32 buffer[ buffer_size ];
	auto conversion_length = i64( 0 );
	auto conversion_result = conversion::PrimitiveToString(
		buffer,
		buffer + buffer_size,
		value_argument_description
	);
	conversion_length = conversion_result.length;
	if( conversion_result.outcome != conversion::PrimitiveToStringResult::Outcome::SUCCESS )
	{
		std::copy( conversion_error_text, conversion_error_text + conversion_error_text_length, buffer );
		conversion_length = conversion_error_text_length;
	}

	return internal_::MakePrintRecord_RawArgument(
		buffer,
		conversion_length,
		argument_value
	);
}



namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename ArgumentDescriptionType,
	typename ArgumentValueType,
	typename ...RestTypePack
>
void MakePrintRecord_ArgumentList_Collector(
	PrintRecord&					out_buffer,
	const ArgumentDescriptionType&	argument_description,
	const ArgumentValueType&		argument_value,
	const RestTypePack&				...argument_pack
) noexcept
{
	out_buffer += MakePrintRecord( U"\n" );
	out_buffer += MakePrintRecord_Argument( argument_description, argument_value );
	if constexpr( sizeof...( argument_pack ) > 0 ) MakePrintRecord_ArgumentList_Collector( out_buffer, argument_pack... );
}



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Converts a collection of arguments into easy to read text.
///
/// @tparam ArgumentTypePack
/// Argument types.
///
/// @param argument_pack
/// Arguments must be in pairs of 2, where the first is the description as a C style string, and the second can be a
/// C style string, or any primitive data type. For example:
/// @code
/// MakePrintRecord_ArgumentList( U"Description", 5 );
/// @endcode
///
/// @return
/// A new print record.
template<typename ...ArgumentTypePack>
auto MakePrintRecord_ArgumentList( const ArgumentTypePack& ...argument_pack ) noexcept -> PrintRecord
{
	static_assert( sizeof...( argument_pack ) > 0, "No arguments given" );
	static_assert( sizeof...( argument_pack ) % 2 == 0, "Argument list must be a pair of values, ['text', 'value'], argument pack size was odd" );

	auto buffer = PrintRecord {};
	internal_::MakePrintRecord_ArgumentList_Collector( buffer, argument_pack... );
	return buffer;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	i64			CharacterArraySize,
	typename	...ArgumentTypePack
>
auto MakePrintRecord_AssertText(
	const c32( &c_string_title )[ CharacterArraySize ],
	const ArgumentTypePack& ...argument_pack
) noexcept -> PrintRecord
{
	static_assert( sizeof...( argument_pack ) % 2 == 0, "Argument list must be a pair of values, ['text', 'value'], argument pack size was odd" );

	auto record = MakePrintRecord( c_string_title );
	if constexpr( sizeof...( argument_pack ) )
	{
		record += MakePrintRecord_ArgumentList( argument_pack... ).AddIndent();
	}
	return record;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto MakePrintRecord_SourceLocation( const SourceLocation& source_location ) noexcept -> PrintRecord;



} // diagnostic
} // bc
