#pragma once

#include <core/diagnostic/logger/LogReportSeverity.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct LogEntry
{
	LogReportSeverity			severity				= LogReportSeverity::NONE;
	PrintRecord					message;
};



} // diagnostic
} // bc
