
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::MakePrintRecord(
	const c32*			c_string,
	i64					length,
	PrintRecordTheme	theme
) noexcept -> PrintRecord
{
	using namespace bc::diagnostic;

	auto record = PrintRecord {};
	auto new_section = PrintRecordSection {};
	new_section.SetMessage( c_string, length );
	new_section.SetTheme( theme );
	record += new_section;
	return record;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::MakePrintRecord_SourceLocation( const SourceLocation & source_location ) noexcept -> PrintRecord
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
