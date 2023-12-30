
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecordTheme.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::PrintRecordThemeColors bc::diagnostic::GetPrintRecordThemeColors(
	bc::diagnostic::PrintRecordTheme		theme
)
{
	auto result = PrintRecordThemeColors {};

	switch( theme )
	{
	case bc::diagnostic::PrintRecordTheme::DEFAULT:
		return PrintRecordThemeColors { PrintRecordColor::DEFAULT, PrintRecordColor::DEFAULT };

	case bc::diagnostic::PrintRecordTheme::DANGER:
		return PrintRecordThemeColors { PrintRecordColor::RED, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::CAUTION:
		return PrintRecordThemeColors { PrintRecordColor::YELLOW, PrintRecordColor::DEFAULT };

	case bc::diagnostic::PrintRecordTheme::SUCCESS:
		return PrintRecordThemeColors { PrintRecordColor::GREEN, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::PERFORMANCE_WARNING:
		return PrintRecordThemeColors { PrintRecordColor::DARK_YELLOW, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::WARNING:
		return PrintRecordThemeColors { PrintRecordColor::YELLOW, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::ERROR:
		return PrintRecordThemeColors { PrintRecordColor::RED, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::CRITICAL_ERROR:
		return PrintRecordThemeColors { PrintRecordColor::WHITE, PrintRecordColor::RED };

	case bc::diagnostic::PrintRecordTheme::VERBOSE:
		return PrintRecordThemeColors { PrintRecordColor::DARK_GRAY, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::INFO:
		return PrintRecordThemeColors { PrintRecordColor::DARK_GREEN, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::DEBUG:
		return PrintRecordThemeColors { PrintRecordColor::WHITE, PrintRecordColor::GREEN };

	case bc::diagnostic::PrintRecordTheme::TITLE:
		return PrintRecordThemeColors { PrintRecordColor::BLUE, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::BRIGHT:
		return PrintRecordThemeColors { PrintRecordColor::WHITE, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::PARAMETER:
		return PrintRecordThemeColors { PrintRecordColor::CYAN, PrintRecordColor::DEFAULT };
	case bc::diagnostic::PrintRecordTheme::ARGUMENT:
		return PrintRecordThemeColors { PrintRecordColor::YELLOW, PrintRecordColor::DEFAULT };
	}

	assert( 0 );
	return result;
}
