
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[noreturn]]
void bc::diagnostic::internal_::PanicInternal(
	const char						*	message_ptr,
	u64									message_length,
	bc::diagnostic::SourceLocation		source_location
)
{
	// TODO: Log message.
	std::abort();
}
