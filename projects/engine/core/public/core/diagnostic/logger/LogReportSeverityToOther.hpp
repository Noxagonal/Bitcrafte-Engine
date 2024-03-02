#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/logger/LogReportSeverity.hpp>
#include <core/diagnostic/print_record/PrintRecordTheme.hpp>
#include <core/containers/simple/SimpleText.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
bc::internal_::SimpleTextView32			LogReportSeverityToText(
	LogReportSeverity					e
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
PrintRecordTheme						LogReportSeverityToPrintRecordTheme(
	LogReportSeverity					e
);



} // diagnostic
} // bc
