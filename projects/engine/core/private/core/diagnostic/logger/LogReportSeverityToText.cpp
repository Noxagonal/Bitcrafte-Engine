
#include <core/diagnostic/logger/LogReportSeverityToText.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::TextView32 bc::diagnostic::LogReportSeverityToText(
	bc::diagnostic::LogReportSeverity e
)
{
	switch( e )
	{
	case bc::diagnostic::LogReportSeverity::NONE:
		return U"NONE";
	case bc::diagnostic::LogReportSeverity::VERBOSE:
		return U"VERBOSE";
	case bc::diagnostic::LogReportSeverity::INFO:
		return U"INFO";
	case bc::diagnostic::LogReportSeverity::PERFORMANCE_WARNING:
		return U"PERFORMANCE_WARNING";
	case bc::diagnostic::LogReportSeverity::WARNING:
		return U"WARNING";
	case bc::diagnostic::LogReportSeverity::ERROR:
		return U"ERROR";
	case bc::diagnostic::LogReportSeverity::CRITICAL_ERROR:
		return U"CRITICAL_ERROR";
	case bc::diagnostic::LogReportSeverity::DEBUG:
		return U"DEBUG";
	}
	assert( 0 );

	return U"<Unknown LogReportSeverity>";
}
