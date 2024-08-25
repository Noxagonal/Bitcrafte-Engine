#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/logger/LogReportSeverity.hpp>
#include <core/diagnostic/print_record/PrintRecordTheme.hpp>
#include <core/containers/simple/SimpleText.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto LogReportSeverityToText( LogReportSeverity e ) -> bc::internal_::SimpleTextView32;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto LogReportSeverityToPrintRecordTheme( LogReportSeverity e ) -> PrintRecordTheme;



} // diagnostic
} // bc
