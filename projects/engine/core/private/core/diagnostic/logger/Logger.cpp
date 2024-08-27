
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/logger/Logger.hpp>
#include <core/diagnostic/logger/LogReportSeverityToOther.hpp>
#include <core/diagnostic/system_console/SystemConsole.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>

#include <string>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Logger::Logger( const LoggerCreateInfo& logger_create_info ) noexcept
	:
	create_info( logger_create_info )
{
	log_history.Allocate( create_info.log_history_size, alignof( LogEntry ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Logger::~Logger() noexcept
{
	DestructAllHistoryLogEntries();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogVerbose( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::VERBOSE, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogInfo( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::INFO, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogPerformanceWarning( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::PERFORMANCE_WARNING, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogWarning( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::WARNING, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogError( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::ERROR, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogCriticalError( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::CRITICAL_ERROR, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::LogDebug( const PrintRecord& message ) noexcept
{
	Log( LogReportSeverity::DEBUG, message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::Log(
	LogReportSeverity		report_severity,
	const PrintRecord&		message
) noexcept
{
	if( this->create_info.disabled ) [[unlikely]] return;

	auto log_entry = LogEntry {};
	log_entry.severity		= report_severity;
	log_entry.message		= message;

	PushLogEntry( log_entry );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::Log(
	LogReportSeverity		report_severity,
	const Exception&		exception
) noexcept
{
	if( this->create_info.disabled ) [[unlikely]] return;

	auto current_exception_in_chain = &exception;

	auto log_entry = LogEntry {};
	log_entry.severity = report_severity;

	// TODO: Do formatting on both Log functions.

	u64 exception_counter = 0;
	while( current_exception_in_chain )
	{
		auto exception_message = MakePrintRecord_Argument( U"Exception", exception_counter );
		exception_message += MakePrintRecord( U"\n" );

		auto message_body = current_exception_in_chain->GetMessage();
		message_body.AddIndent();
		exception_message += message_body;

		log_entry.message += exception_message;

		current_exception_in_chain = current_exception_in_chain->GetNextException();

		++exception_counter;
	}

	PushLogEntry( log_entry );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::Logger::GetLogHistory() const noexcept -> memory::MemoryRange<LogEntry>
{
	return { log_history.Data(), log_history_size };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::PushLogEntry( const LogEntry& log_entry ) noexcept
{
	// Ignore debug logs in shipping builds.
#if !BITCRAFTE_GAME_DEVELOPMENT_BUILD
	if( log_entry.severity == LogReportSeverity::DEBUG ) return;
#endif

	auto lock_guard = std::lock_guard( log_mutex );

	if( std::to_underlying( log_entry.severity ) < std::to_underlying( LogReportSeverity::CRITICAL_ERROR ) &&
		std::to_underlying( log_entry.severity ) < std::to_underlying( create_info.minimum_report_severity ) )
	{
		return;
	}

	AddLogEntryToHistory( log_entry );

	if( std::to_underlying( log_entry.severity ) < std::to_underlying( LogReportSeverity::ERROR ) &&
		std::to_underlying( log_entry.severity ) < std::to_underlying( create_info.minimum_display_severity ) )
	{
		return;
	}

	// TODO: Update callbacks here, if we introduced any.

	if( !create_info.print_to_system_console ) return;

	auto console_print_complete_message = MakePrintRecord( U"\n\n" );

	auto log_entry_severity_string = LogReportSeverityToCString( log_entry.severity );
	auto log_entry_severity_string_length = std::char_traits<c32>::length( log_entry_severity_string );
 	console_print_complete_message += MakePrintRecord(
		log_entry_severity_string,
		log_entry_severity_string_length,
		LogReportSeverityToPrintRecordTheme( log_entry.severity )
	);

	console_print_complete_message += MakePrintRecord( U"\n" );

	auto console_print_body = log_entry.message;
	console_print_body.AddIndent();

	console_print_complete_message += console_print_body;
	SystemConsolePrint( console_print_complete_message );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::AddLogEntryToHistory( const LogEntry & log_entry ) noexcept
{
	// Log history elements are not initially initialized, just the memory is allocated.

	if( log_history_size < log_history.count )
	{
		auto log_entry_index = log_history_size;
		log_history_size++;
		CopyConstructHistoryLogEntry( log_entry_index, log_entry );
	}
	else
	{
		// Rolling over the log history size capacity, we'll need to shift all the entries down by one.
		// OPTIMIZATION: We could avoid copying the elements one by one if we use a ring buffer here instead, we'd need
		// to return a custom iterator from GetLogHistory() however.
		for( i64 i = 0; i < log_history_size - 1; ++i )
		{
			DestructHistoryLogEntry( i );
			MoveConstructHistoryLogEntry( i, std::move( log_history.data[ i + 1 ] ) );
		}
		DestructHistoryLogEntry( log_history_size - 1 );
		CopyConstructHistoryLogEntry( log_history_size - 1, log_entry );
	}

	log_recorded_total++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::CopyConstructHistoryLogEntry( i64 at_index, const LogEntry & log_entry ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < log_history.count, U"Log history index out of bounds" );

	new( &log_history.data[ at_index ] ) LogEntry( log_entry );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::MoveConstructHistoryLogEntry( i64 at_index, LogEntry && log_entry ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < log_history.count, U"Log history index out of bounds" );

	new( &log_history.data[ at_index ] ) LogEntry( std::move( log_entry ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::DestructHistoryLogEntry( i64 at_index ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < log_history_size, U"Log history index out of bounds" );

	log_history.data[ at_index ].~LogEntry();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::Logger::DestructAllHistoryLogEntries() noexcept
{
	for( i64 i = 0; i < log_history_size; ++i )
	{
		auto log_entry_address = &log_history.data[ i ];
		log_entry_address->~LogEntry();
	}
}
