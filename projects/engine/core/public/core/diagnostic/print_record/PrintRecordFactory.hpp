#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include "PrintRecord.hpp"

#include <core/conversion/text/text_format/TextFormat.hpp>
#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline PrintRecord											MakePrintRecord(
	SimpleTextView32										text,
	PrintRecordTheme										theme					= PrintRecordTheme::DEFAULT
)
{
	auto record = PrintRecord {};
	auto new_section = PrintRecordSection {};
	new_section.text = text;
	new_section.theme = theme;
	record += new_section;
	return record;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												ArgumentTextType,
	typename												ArgumentValueType
>
PrintRecord													MakePrintRecord_Argument(
	const ArgumentTextType								&	text,
	const ArgumentValueType								&	argument
)
{
	auto record = MakePrintRecord( text::TextFormat( U"{}", text ) );
	record += MakePrintRecord( U": \"" );
	record += MakePrintRecord( text::TextFormat( U"{}", argument ) );
	record += MakePrintRecord( U"\"" );
	return record;
}

namespace internal {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												ArgumentTextType,
	typename												ArgumentValueType,
	typename												...RestTypePack
>
void														MakePrintRecord_ArgumentList_Collector(
	PrintRecord											&	out_buffer,
	const ArgumentTextType								&	text,
	const ArgumentValueType								&	argument,
	const RestTypePack									&	...argument_pack
)
{
	out_buffer += MakePrintRecord_Argument( text, argument );
	if constexpr( sizeof...( argument_pack ) > 0 ) MakePrintRecord_ArgumentList_Collector( out_buffer, argument_pack... );
}

} // internal

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
	internal::MakePrintRecord_ArgumentList_Collector( buffer, argument_pack... );
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename												...ArgumentTypePack
>
PrintRecord													MakePrintRecord_AssertText(
	SimpleTextView32										title,
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
inline PrintRecord											MakePrintRecord_SourceLocation(
	const SourceLocation								&	source_location
)
{
	auto record = MakePrintRecord_Argument( U"File", source_location.GetFile() );
	record += MakePrintRecord( U"\n" );
	record += MakePrintRecord_Argument( U"Function", source_location.GetFunction() );
	record += MakePrintRecord( U"\n" );
	record += MakePrintRecord_Argument( U"Line", source_location.GetLine() );
	record += MakePrintRecord( U"\n" );
	record += MakePrintRecord_Argument( U"Column", source_location.GetColumn() );
	return record;
}



} // diagnostic
} // bc
