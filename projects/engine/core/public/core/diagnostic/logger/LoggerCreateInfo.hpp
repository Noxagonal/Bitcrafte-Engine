#pragma once

#include <core/diagnostic/logger/LogReportSeverity.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct LoggerCreateInfo
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Severity level that is logged.
	///
	/// By default this is VERBOSE, meaning that everything gets recorded. If set to another level, eg. WARNING, then only warnings,
	/// errors and anything higher is logged. This is usually kept at VERBOSE for development builds, however, for performance
	/// reasons, this may be set to ERROR for shipping builds.
	///
	/// @note
	/// Default: @c VERBOSE
	///
	/// @note
	/// Setting report severity level higher than CRITICAL_ERROR is ignored, critical errors will always be reported, even in
	/// shipping builds.
	LogReportSeverity			report_severity					= LogReportSeverity::VERBOSE;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Severity level that is displayed.
	///
	/// By default this is WARNING, meaning that only warnings, errors, critical errors and above are ever displayed. For more info,
	/// this may be set to a lower level, eg. VERBOSE, to display everything. This is usually too much information and usually used
	/// for debugging some nasty bugs, or try to understand the path that led to the error.
	///
	/// @note
	/// Default: @c WARNING
	///
	/// @note
	/// Setting display report severity level higher than ERROR is ignored, errors will always be displayed.
	///
	/// @note
	/// If printing to system console, and display severity is updated, the already printed messages in system console are not
	/// updated, only new logs in the system console follow the new display severity.
	LogReportSeverity			display_severity				= LogReportSeverity::WARNING;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Tells if the reported logs should be printed to system console.
	///
	/// By default this is "true", meaning new logs will be written to system console, if the system console is visible. If the
	/// system console window is hidden, new logs are not added to the system console.
	///
	/// @note
	/// Default: @c true
	///
	bool						print_to_system_console			= true;
};



} // diagnostic
} // bc
