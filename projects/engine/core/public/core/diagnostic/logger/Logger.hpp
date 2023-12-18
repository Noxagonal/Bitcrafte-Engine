#pragma once

#include <core/diagnostic/logger/LogReportSeverity.hpp>
#include <core/containers/simple/SimpleList.hpp>

#include "LoggerCreateInfo.hpp"
#include "LogEntry.hpp"

#include <mutex>



namespace bc {
namespace diagnostic {



class Exception;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class that keeps a list of reports in memory.
class BITCRAFTE_ENGINE_API Logger
{
public:

	using LogEntryList = SimpleList<LogEntry>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Logger(
		const LoggerCreateInfo				&	logger_create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogVerbose(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogInfo(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogPerformanceWarning(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogWarning(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogError(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogCriticalError(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										LogDebug(
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										Log(
		LogReportSeverity						report_severity,
		const PrintRecord					&	message
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										Log(
		LogReportSeverity						report_severity,
		const Exception						&	exception
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const LogEntryList						&	GetLogHistory();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void										PushLogEntry(
		const LogEntry						&	log_entry
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::mutex									log_mutex;
	LoggerCreateInfo							create_info				= {};
	LogEntryList								log_history;
};



} // diagnostic
} // bc