#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {

class PrintRecord;

namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
void								PanicInternal [[noreturn]] (
	const char32_t				*	message_ptr,
	u64								message_length,
	SourceLocation					source_location
);



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<u64 MessageLength>
void								Panic [[noreturn]] (
	const char32_t					( &message )[ MessageLength ],
	SourceLocation					source_location					= SourceLocation::Current()
)
{
	internal_::PanicInternal( message, MessageLength ? ( message[ MessageLength - 1 ] == U'\0' ? MessageLength - 1 : MessageLength ) : MessageLength, source_location );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
void								Panic [[noreturn]] (
	const PrintRecord			&	message,
	SourceLocation					source_location					= SourceLocation::Current()
);



} // diagnostic
} // bc
