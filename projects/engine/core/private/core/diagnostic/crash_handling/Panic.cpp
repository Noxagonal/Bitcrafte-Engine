
#include <core/PreCompiledHeader.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[noreturn]]
void bc::diagnostic::internal::PanicInternal(
	const char						*	message_ptr,
	size_t								message_length,
	bc::diagnostic::SourceLocation		source_location
)
{
	// TODO: Log message.
	std::abort();
}
