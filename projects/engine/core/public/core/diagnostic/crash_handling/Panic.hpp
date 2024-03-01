#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[[noreturn]]
BITCRAFTE_ENGINE_API
void								PanicInternal(
	const char					*	message_ptr,
	u64								message_length,
	SourceLocation					source_location
);



} // internal



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<u64 MessageLength>
[[noreturn]]
void								Panic(
	const char						( &message )[ MessageLength ],
	SourceLocation					source_location					= SourceLocation::Current()
)
{
	internal::PanicInternal( message, MessageLength ? ( message[ MessageLength - 1 ] == '\0' ? MessageLength - 1 : MessageLength ) : MessageLength, source_location );
}



} // diagnostic
} // bc
