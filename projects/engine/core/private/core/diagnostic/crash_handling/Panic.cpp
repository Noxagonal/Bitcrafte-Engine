
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal_::PanicInternal [[noreturn]] (
	const char32_t					*	message_ptr,
	u64									message_length,
	bc::diagnostic::SourceLocation		source_location
)
{
	// TODO: Log message.
	std::abort();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Panic [[noreturn]] (
	const PrintRecord	&	message,
	SourceLocation			source_location
)
{
	// TODO: Log message.
	std::abort();
}