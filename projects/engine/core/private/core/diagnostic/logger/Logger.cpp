
#include <core/PreCompiledHeader.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Logger::Logger(
	const LoggerCreateInfo & logger_create_info
) :
	create_info( logger_create_info )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogVerbose(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::VERBOSE, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogInfo(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::INFO, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogPerformanceWarning(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::PERFORMANCE_WARNING, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogWarning(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::WARNING, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogError(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::ERROR, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogCriticalError(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::CRITICAL_ERROR, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogDebug(
	const PrintRecord	&	message
)
{
	Log( LogReportSeverity::DEBUG, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::Log(
	LogReportSeverity		report_severity,
	const PrintRecord	&	message
)
{
	auto log_entry = LogEntry {};
	log_entry.severity		= report_severity;
	log_entry.message		= message;
	log_entry.message.Finalize();

	PushLogEntry( log_entry );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::Log(
	LogReportSeverity		report_severity,
	const Exception		&	exception
)
{
	auto current_exception_in_chain = &exception;

	auto log_entry = LogEntry {};
	log_entry.severity = report_severity;

	while( true )
	{
		log_entry.message += current_exception_in_chain->GetMessage();

		current_exception_in_chain = current_exception_in_chain->GetNextException();
		if( !current_exception_in_chain ) break;

		log_entry.message += diagnostic::MakePrintRecord( U"\n\n" );
		log_entry.message += diagnostic::MakePrintRecord( U"Next chained exception:" );
		log_entry.message += diagnostic::MakePrintRecord( U"\n" );
	}

	log_entry.message.Finalize();
	PushLogEntry( log_entry );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const bc::diagnostic::Logger::LogEntryList & bc::diagnostic::Logger::GetLogHistory()
{
	return log_history;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::PushLogEntry(
	const LogEntry		&	log_entry
)
{
	BHardAssert( log_entry.message.IsFinalized(), "Log entry must be finalized before calling this function" );

#if !BITCRAFTE_DEVELOPMENT_BUILD
	if( log_entry.severity == ReportSeverity::DEBUG ) return;
#endif

	auto lock_guard = std::lock_guard( log_mutex );

	if( std::to_underlying( log_entry.severity ) < std::to_underlying( LogReportSeverity::CRITICAL_ERROR ) &&
		std::to_underlying( log_entry.severity ) < std::to_underlying( create_info.report_severity ) )
	{
		return;
	}

	log_history.PushBack( log_entry );

	if( std::to_underlying( log_entry.severity ) < std::to_underlying( LogReportSeverity::ERROR ) &&
		std::to_underlying( log_entry.severity ) < std::to_underlying( create_info.display_severity ) )
	{
		return;
	}

	// TODO: Update callbacks here, if we introduced any.

	if( !create_info.print_to_system_console ) return;
	SystemConsolePrint( log_entry.message );
}
