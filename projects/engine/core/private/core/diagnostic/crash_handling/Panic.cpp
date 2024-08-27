
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal_::PanicInternal [[noreturn]] ( SourceLocation source_location )
{
	// TODO: Invoke a crash handler.
	std::abort();
}
