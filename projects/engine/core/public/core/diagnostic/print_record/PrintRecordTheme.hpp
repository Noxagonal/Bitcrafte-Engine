#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <stdint.h>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Text or background color used for a log.
enum class PrintRecordColor : u8
{
	BLACK			= 0,
	DARK_BLUE		= 1,
	DARK_GREEN		= 2,
	DARK_CYAN		= 3,
	DARK_RED		= 4,
	DARK_MAGENTA	= 5,
	DARK_YELLOW		= 6,
	GRAY			= 7,
	DARK_GRAY		= 8,
	BLUE			= 9,
	GREEN			= 10,
	CYAN			= 11,
	RED				= 12,
	MAGENTA			= 13,
	YELLOW			= 14,
	WHITE			= 15,
	DEFAULT			= 255
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Print record theme enables consistent look for each situation.
///
/// @see
/// GetSystemConsoleThemeColors()
enum class PrintRecordTheme : u32
{
	DEFAULT			= 0,

	// General
	DANGER,
	CAUTION,

	// Result
	SUCCESS,
	PERFORMANCE_WARNING,
	WARNING,
	ERROR,
	CRITICAL_ERROR,

	// Reporting
	VERBOSE,
	INFO,
	DEBUG,

	// Custom
	TITLE,
	BRIGHT,
	PARAMETER,
	ARGUMENT,
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PrintRecordThemeColors
{
	PrintRecordColor	foreground_color	= PrintRecordColor::DEFAULT;
	PrintRecordColor	background_color	= PrintRecordColor::DEFAULT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get console colors from theme.
///
/// @return
/// Console colors representing the theme. Foreground color is first, then background color.
BITCRAFTE_ENGINE_API
auto GetPrintRecordThemeColors( PrintRecordTheme theme ) -> PrintRecordThemeColors;



} // diagnostic
} // bc
