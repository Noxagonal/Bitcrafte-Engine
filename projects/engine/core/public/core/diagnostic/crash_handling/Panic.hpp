#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>



namespace bc {
namespace diagnostic {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
void PanicInternal [[noreturn]] ( SourceLocation source_location );



} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Panic [[noreturn]] ( SourceLocation source_location = SourceLocation::Current() )
{
	internal_::PanicInternal( source_location );
}



} // diagnostic
} // bc
