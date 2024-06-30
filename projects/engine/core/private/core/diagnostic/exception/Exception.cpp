
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/exception/Exception.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::Exception::Exception(
	const Exception		&	other
)
{
	this->CopyOther( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::Exception & bc::diagnostic::Exception::operator=(
	const Exception		&	other
)
{
	this->CopyOther( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const bc::diagnostic::PrintRecord & bc::diagnostic::Exception::GetMessage() const noexcept
{
	return this->message;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const bc::diagnostic::SourceLocation & bc::diagnostic::Exception::GetSourceLocation() const noexcept
{
	return this->source_location;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const bc::diagnostic::StackTrace & bc::diagnostic::Exception::GetStackTrace() const noexcept
{
	return this->stack_trace;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const bc::diagnostic::Exception * bc::diagnostic::Exception::GetNextException() const noexcept
{
	return this->next.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr void bc::diagnostic::Exception::SetNextException(
	const Exception		&	next_exception
	)
{
	assert( std::addressof( next_exception ) != this && "next_exception was this exception" );
	if( std::addressof( next_exception ) == this ) return;
	this->next = bc::internal_::MakeSimpleUniquePtr<Exception>( next_exception );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bool bc::diagnostic::Exception::IsEmpty() const noexcept
{
	return this->message.IsEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr void bc::diagnostic::Exception::CopyOther(const Exception &other)
{
	this->message			= other.message;
	this->source_location	= other.source_location;
	this->stack_trace		= other.stack_trace;
	if( !other.next.IsEmpty() ) SetNextException( *other.next );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Exception bc::diagnostic::MakeException(
	const PrintRecord		&	print_record,
	const SourceLocation	&	source_location,
	const StackTrace		&	stack_trace
)
{
	auto exception = Exception{};

	exception.message			= print_record;
	exception.source_location	= source_location;
	exception.stack_trace		= stack_trace;

	return exception;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[noreturn]]
void ExceptionThrowPanic(
	const bc::diagnostic::Exception 	&	exception,
	bc::diagnostic::SourceLocation			source_location					= bc::diagnostic::SourceLocation::Current()
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
	const bc::diagnostic::Exception					&	exception,
	bc::diagnostic::SourceLocation						source_location
)
{
	throw exception;
}

#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::diagnostic::Exception					&	exception,
	bc::diagnostic::SourceLocation						source_location
)
{
	ExceptionThrowPanic( exception, source_location );
}

#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::diagnostic::PrintRecord				&	print_record,
	const bc::diagnostic::SourceLocation			&	source_location
)
{
	Throw(
		MakeException(
			print_record,
			source_location,
			StackTrace::Current( 1 )
		)
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const bc::internal_::SimpleTextView32				message,
	const bc::diagnostic::SourceLocation			&	source_location
)
{
	Throw(
		MakeException(
			PrintRecord( message ),
			source_location,
			StackTrace::Current( 1 )
		)
	);
}
