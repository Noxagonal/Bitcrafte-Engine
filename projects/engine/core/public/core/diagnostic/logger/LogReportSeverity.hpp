#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <stdint.h>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Nature and severity of the reported action.
/// 
/// @see
/// vk2d::PFN_VK2D_ReportFunction()
enum class LogReportSeverity : u32
{
	NONE					= 0,		///< Not valid severity value, used to detect invalid severity values.
	VERBOSE,							///< Reports everything, usually too much information.
	INFO,								///< Useful to know what the application is doing.
	PERFORMANCE_WARNING,				///< Serious bottlenecks in performance somewhere, you should check it out.
	WARNING,							///< Failed to load a resource so something might be missing but can still continue with visual defects.
	ERROR,								///< Error that still allows the application to continue running, error was handled but something might not work properly.
	CRITICAL_ERROR,						///< Critical error, application has no option but to terminate immediately.
	DEBUG					= 0x1000,	///< This is a special debug report and it is meant for displaying debug information. It is always logged and displayed in development builds and always disabled in shipping builds.
};



} // diagnostic
} // bc
