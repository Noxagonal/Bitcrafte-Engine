
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/exception/Exception.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[noreturn]]
void ExceptionThrowPanic(
	const bc::diagnostic::Exception&	exception,
	bc::diagnostic::SourceLocation		source_location		= bc::diagnostic::SourceLocation::Current()
)
{
	using namespace bc::diagnostic;

	auto print_record = MakePrintRecord( U"Panic: Exception thrown in shipping build is unrecoverable", PrintRecordTheme::CRITICAL_ERROR );
	print_record += MakePrintRecord( U"\n" );

	auto exception_counter = bc::u32( 0 );
	auto current_exception = &exception;
	while( current_exception )
	{
		print_record += MakePrintRecord_Argument( U"Exception", exception_counter );
		print_record += MakePrintRecord( U"\n" );
		print_record += PrintRecord( exception.GetMessage() ).AddIndent();
		print_record += MakePrintRecord( U"\n\n" );
		current_exception = current_exception->GetNextException();
		++exception_counter;
	}

	Panic( print_record, source_location );
}



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::diagnostic::Exception&	exception,
	bc::diagnostic::SourceLocation		source_location
)
{
	throw exception;
}

#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::diagnostic::Exception&	exception,
	bc::diagnostic::SourceLocation		source_location
)
{
	ExceptionThrowPanic( exception, source_location );
}

#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::diagnostic::PrintRecord&		print_record,
	const bc::diagnostic::SourceLocation&	source_location
)
{
	Throw(
		Exception{
			print_record,
			source_location,
			StackTrace::Current( 1 )
		}
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::internal_::SimpleTextView32	message,
	const bc::diagnostic::SourceLocation&	source_location
)
{
	Throw(
		Exception{
			PrintRecord( message ),
			source_location,
			StackTrace::Current( 1 )
		}
	);
}
