
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <core/diagnostic/print_record/PrintRecordFactory.hpp>
#include <core/memory/raw/RawMemory.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Exception::Exception(
	const PrintRecord&		print_record,
	const SourceLocation&	source_location,
	const StackTrace&		stack_trace
) noexcept
:
	message { print_record },
	source_location { source_location },
	stack_trace { stack_trace },
	next { nullptr }
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Exception::Exception( const Exception& other ) noexcept
{
	this->Copy( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Exception::~Exception() noexcept
{
	this->ClearNext();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::operator=( const Exception& other ) noexcept -> Exception&
{
	this->Copy( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::GetMessage() const noexcept -> const PrintRecord&
{
	return this->message;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::GetSourceLocation() const noexcept -> const SourceLocation&
{
	return this->source_location;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::GetStackTrace() const noexcept -> const StackTrace&
{
	return this->stack_trace;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::GetNextException() const noexcept -> const Exception*
{
	return this->next;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Exception::SetNextException( const Exception& next_exception ) noexcept
{
	assert( std::addressof( next_exception ) != this && "next_exception was this exception" );
	if( std::addressof( next_exception ) == this ) return;

	// This will allocate and construct the exception chain recursively.
	this->next = memory::AllocateMemory<Exception>( 1, alignof( Exception ) );
	new( this->next ) Exception( next_exception );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Exception::IsEmpty() const noexcept -> bool
{
	return this->message.IsEmpty() && this->stack_trace.IsEmpty() && this->source_location.IsEmpty() && this->next == nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Exception::ClearNext() noexcept
{
	if( this->next )
	{
		// This will destruct the next exception, which will destroy the next exception in the chain, recursively.
		this->next->~Exception();
		memory::FreeMemory( this->next, 1 );
		this->next = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Exception::Copy( const Exception& other ) noexcept
{
	this->message			= other.message;
	this->source_location	= other.source_location;
	this->stack_trace		= other.stack_trace;
	if( other.next ) this->SetNextException( *other.next );
}



namespace bc {
namespace diagnostic {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ExceptionThrowPanic [[noreturn]] ( const Exception& exception )
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

	// TODO: Log the exception.

	Panic( exception.GetSourceLocation() );
}

} // namespace internal_
} // namespace diagnostic
} // namespace bc



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] ( const Exception& exception )
{
	throw exception;
}

#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] ( const Exception& exception )
{
	internal_::ExceptionThrowPanic( exception, source_location );
}

#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD
