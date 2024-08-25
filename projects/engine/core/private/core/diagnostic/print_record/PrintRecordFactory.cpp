
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto bc::diagnostic::MakePrintRecord(
	bc::internal_::SimpleTextView32		text,
	bc::diagnostic::PrintRecordTheme	theme
) -> PrintRecord
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
auto bc::diagnostic::MakePrintRecord( bc::internal_::SimpleTextView32 text ) -> PrintRecord
{
	return MakePrintRecord( text, PrintRecordTheme::DEFAULT );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto bc::diagnostic::MakePrintRecord_SourceLocation( const SourceLocation & source_location ) -> PrintRecord
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
