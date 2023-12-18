
#include <core/diagnostic/exception/Exception.hpp>

#include <assert.h>



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



#if BITCRAFTE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Throw [[noreturn]] (
	const Exception			&	exception
)
{
	throw exception;
}



#endif // BITCRAFTE_DEVELOPMENT_BUILD
