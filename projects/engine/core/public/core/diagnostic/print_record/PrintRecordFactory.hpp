#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/print_record/PrintRecord.hpp>

#include <core/conversion/text/text_format/TextFormat.hpp>
#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PrintRecord													MakePrintRecord(
	bc::internal_::SimpleTextView32							text,
	PrintRecordTheme										theme
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PrintRecord													MakePrintRecord(
	bc::internal_::SimpleTextView32							text
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainer								TextContainerType>
PrintRecord													MakePrintRecord(
	TextContainerType										text,
	PrintRecordTheme										theme
) requires ( !std::is_same_v<std::decay_t<TextContainerType>, bc::internal_::SimpleTextView32> && !std::is_same_v<std::decay_t<TextContainerType>, bc::internal_::SimpleText32> )
{
	static_assert( !std::is_same_v<TextContainerType, bc::internal_::SimpleTextView32> );
	return MakePrintRecord( text::TextFormat( bc::internal_::SimpleTextView32( U"{}" ), text ), theme );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextContainer								TextContainerType>
PrintRecord													MakePrintRecord(
	TextContainerType										text
) requires ( !std::is_same_v<std::decay_t<TextContainerType>, bc::internal_::SimpleTextView32> && !std::is_same_v<std::decay_t<TextContainerType>, bc::internal_::SimpleText32> )
{
	return MakePrintRecord( text::TextFormat( bc::internal_::SimpleTextView32( U"{}" ), text ), PrintRecordTheme::DEFAULT );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												ArgumentDescriptionType,
	typename												ArgumentValueType
>
PrintRecord													MakePrintRecord_Argument(
	const ArgumentDescriptionType						&	argument_description,
	const ArgumentValueType								&	argument_value
) requires( !std::is_same_v<ArgumentValueType, PrintRecordTheme> ) // Prevent PrintRecordTheme from being used as argument
{
	auto record = MakePrintRecord( text::TextFormat( U"{}", argument_description ) );
	record += MakePrintRecord( U": \"" );
	record += MakePrintRecord( text::TextFormat( U"{}", argument_value ) );
	record += MakePrintRecord( U"\"" );
	return record;
}

namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												ArgumentDescriptionType,
	typename												ArgumentValueType,
	typename												...RestTypePack
>
void														MakePrintRecord_ArgumentList_Collector(
	PrintRecord											&	out_buffer,
	const ArgumentDescriptionType						&	argument_description,
	const ArgumentValueType								&	argument_value,
	const RestTypePack									&	...argument_pack
)
{
	out_buffer += MakePrintRecord_Argument( argument_description, argument_value );
	if constexpr( sizeof...( argument_pack ) > 0 ) MakePrintRecord_ArgumentList_Collector( out_buffer, argument_pack... );
}

} // internal_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												...ArgumentTypePack
>
PrintRecord													MakePrintRecord_ArgumentList(
	const ArgumentTypePack								&	...argument_pack
)
{
	static_assert( sizeof...( argument_pack ) > 0, "No arguments given" );
	static_assert( sizeof...( argument_pack ) % 2 == 0, "Argument list must be a pair of values, 'text' : 'value', argument pack size was odd" );

	auto buffer = PrintRecord {};
	internal_::MakePrintRecord_ArgumentList_Collector( buffer, argument_pack... );
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												...ArgumentTypePack
>
PrintRecord													MakePrintRecord_AssertText(
	bc::internal_::SimpleTextView32							title,
	const ArgumentTypePack								&	...argument_pack
)
{
	static_assert( sizeof...( argument_pack ) % 2 == 0, "Argument list must be a pair of values, 'text' : 'value', argument pack size was odd" );

	auto record = MakePrintRecord( title );
	if constexpr( sizeof...( argument_pack ) )
	{
		record += MakePrintRecord( U"\n" );
		record += MakePrintRecord_ArgumentList( argument_pack... ).AddIndent();
	}
	return record;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PrintRecord													MakePrintRecord_SourceLocation(
	const SourceLocation								&	source_location
);



} // diagnostic
} // bc
