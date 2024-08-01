
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
bc::diagnostic::PrintRecord bc::diagnostic::MakePrintRecord(
	bc::internal_::SimpleTextView32		text,
	bc::diagnostic::PrintRecordTheme	theme
)
{
	using namespace bc::diagnostic;

	auto record = PrintRecord {};
	auto new_section = PrintRecordSection {};
	new_section.text = text;
	new_section.theme = theme;
	record += new_section;
	return record;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
bc::diagnostic::PrintRecord bc::diagnostic::MakePrintRecord(
	bc::internal_::SimpleTextView32 text
)
{
	return MakePrintRecord( text, PrintRecordTheme::DEFAULT );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
bc::diagnostic::PrintRecord bc::diagnostic::MakePrintRecord_SourceLocation(
	const SourceLocation 	&	source_location
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