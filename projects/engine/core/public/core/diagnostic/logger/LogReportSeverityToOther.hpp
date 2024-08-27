#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/logger/LogReportSeverity.hpp>
#include <core/diagnostic/print_record/PrintRecordTheme.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto LogReportSeverityToCString( LogReportSeverity e ) noexcept -> const c32*;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
auto LogReportSeverityToPrintRecordTheme( LogReportSeverity e ) noexcept -> PrintRecordTheme;



} // diagnostic
} // bc
