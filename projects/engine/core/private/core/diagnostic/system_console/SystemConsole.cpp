
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/system_console/SystemConsole.hpp>

#include <iostream>
#include <sstream>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex needed to make sense of messages if multiple threads are printing messages at the same time.
std::mutex print_mutex;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * PrintRecordColorToCharANSIForegroundColorCode(
	bc::diagnostic::PrintRecordColor color
)
{
	switch( color )
	{
	case bc::diagnostic::PrintRecordColor::BLACK:				return "30";
	case bc::diagnostic::PrintRecordColor::DARK_BLUE:			return "34";
	case bc::diagnostic::PrintRecordColor::DARK_GREEN:			return "32";
	case bc::diagnostic::PrintRecordColor::DARK_CYAN:			return "36";
	case bc::diagnostic::PrintRecordColor::DARK_RED:			return "31";
	case bc::diagnostic::PrintRecordColor::DARK_MAGENTA:		return "35";
	case bc::diagnostic::PrintRecordColor::DARK_YELLOW:			return "33";
	case bc::diagnostic::PrintRecordColor::GRAY:				return "37";
	case bc::diagnostic::PrintRecordColor::DARK_GRAY:			return "90";
	case bc::diagnostic::PrintRecordColor::BLUE:				return "94";
	case bc::diagnostic::PrintRecordColor::GREEN:				return "92";
	case bc::diagnostic::PrintRecordColor::CYAN:				return "96";
	case bc::diagnostic::PrintRecordColor::RED:					return "91";
	case bc::diagnostic::PrintRecordColor::MAGENTA:				return "95";
	case bc::diagnostic::PrintRecordColor::YELLOW:				return "93";
	case bc::diagnostic::PrintRecordColor::WHITE:				return "97";
	case bc::diagnostic::PrintRecordColor::DEFAULT:				return "37";
	}
	assert( 0 );
	return "37";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * PrintRecordColorToCharANSIBackgroundColorCode(
	bc::diagnostic::PrintRecordColor color
)
{
	switch( color )
	{
	case bc::diagnostic::PrintRecordColor::BLACK:				return "40";
	case bc::diagnostic::PrintRecordColor::DARK_BLUE:			return "44";
	case bc::diagnostic::PrintRecordColor::DARK_GREEN:			return "42";
	case bc::diagnostic::PrintRecordColor::DARK_CYAN:			return "46";
	case bc::diagnostic::PrintRecordColor::DARK_RED:			return "41";
	case bc::diagnostic::PrintRecordColor::DARK_MAGENTA:		return "45";
	case bc::diagnostic::PrintRecordColor::DARK_YELLOW:			return "43";
	case bc::diagnostic::PrintRecordColor::GRAY:				return "47";
	case bc::diagnostic::PrintRecordColor::DARK_GRAY:			return "100";
	case bc::diagnostic::PrintRecordColor::BLUE:				return "104";
	case bc::diagnostic::PrintRecordColor::GREEN:				return "102";
	case bc::diagnostic::PrintRecordColor::CYAN:				return "106";
	case bc::diagnostic::PrintRecordColor::RED:					return "101";
	case bc::diagnostic::PrintRecordColor::MAGENTA:				return "105";
	case bc::diagnostic::PrintRecordColor::YELLOW:				return "103";
	case bc::diagnostic::PrintRecordColor::WHITE:				return "107";
	case bc::diagnostic::PrintRecordColor::DEFAULT:				return "40";
	}
	assert( 0 );
	return "40";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * PrintRecordColorToWCharANSIForegroundColorCode(
	bc::diagnostic::PrintRecordColor color
)
{
	switch( color )
	{
	case bc::diagnostic::PrintRecordColor::BLACK:				return L"30";
	case bc::diagnostic::PrintRecordColor::DARK_BLUE:			return L"34";
	case bc::diagnostic::PrintRecordColor::DARK_GREEN:			return L"32";
	case bc::diagnostic::PrintRecordColor::DARK_CYAN:			return L"36";
	case bc::diagnostic::PrintRecordColor::DARK_RED:			return L"31";
	case bc::diagnostic::PrintRecordColor::DARK_MAGENTA:		return L"35";
	case bc::diagnostic::PrintRecordColor::DARK_YELLOW:			return L"33";
	case bc::diagnostic::PrintRecordColor::GRAY:				return L"37";
	case bc::diagnostic::PrintRecordColor::DARK_GRAY:			return L"90";
	case bc::diagnostic::PrintRecordColor::BLUE:				return L"94";
	case bc::diagnostic::PrintRecordColor::GREEN:				return L"92";
	case bc::diagnostic::PrintRecordColor::CYAN:				return L"96";
	case bc::diagnostic::PrintRecordColor::RED:					return L"91";
	case bc::diagnostic::PrintRecordColor::MAGENTA:				return L"95";
	case bc::diagnostic::PrintRecordColor::YELLOW:				return L"93";
	case bc::diagnostic::PrintRecordColor::WHITE:				return L"97";
	case bc::diagnostic::PrintRecordColor::DEFAULT:				return L"37";
	}
	assert( 0 );
	return L"37";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * PrintRecordColorToWCharANSIBackgroundColorCode(
	bc::diagnostic::PrintRecordColor color
)
{
	switch( color )
	{
	case bc::diagnostic::PrintRecordColor::BLACK:				return L"40";
	case bc::diagnostic::PrintRecordColor::DARK_BLUE:			return L"44";
	case bc::diagnostic::PrintRecordColor::DARK_GREEN:			return L"42";
	case bc::diagnostic::PrintRecordColor::DARK_CYAN:			return L"46";
	case bc::diagnostic::PrintRecordColor::DARK_RED:			return L"41";
	case bc::diagnostic::PrintRecordColor::DARK_MAGENTA:		return L"45";
	case bc::diagnostic::PrintRecordColor::DARK_YELLOW:			return L"43";
	case bc::diagnostic::PrintRecordColor::GRAY:				return L"47";
	case bc::diagnostic::PrintRecordColor::DARK_GRAY:			return L"100";
	case bc::diagnostic::PrintRecordColor::BLUE:				return L"104";
	case bc::diagnostic::PrintRecordColor::GREEN:				return L"102";
	case bc::diagnostic::PrintRecordColor::CYAN:				return L"106";
	case bc::diagnostic::PrintRecordColor::RED:					return L"101";
	case bc::diagnostic::PrintRecordColor::MAGENTA:				return L"105";
	case bc::diagnostic::PrintRecordColor::YELLOW:				return L"103";
	case bc::diagnostic::PrintRecordColor::WHITE:				return L"107";
	case bc::diagnostic::PrintRecordColor::DEFAULT:				return L"40";
	}
	assert( 0 );
	return L"40";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::basic_ostream<char8_t> & operator<<(
	std::basic_ostream<char8_t>					&	out,
	const bc::internal_::SimpleTextView8			&	in
)
{
	std::basic_ostringstream<char8_t> ss;
	for( auto c : in )
	{
		ss << c;
	}
	return out << ss.str();
}



#if defined( BITCRAFTE_PLATFORM_WINDOWS )

#include <Windows.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal_::SystemConsolePrintRawUTF8(
	const char8_t						*	raw_text,
	u64										raw_text_length,
	bc::diagnostic::PrintRecordColor		foreground_color,
	bc::diagnostic::PrintRecordColor		background_color
)
{
	auto text = bc::internal_::SimpleTextView8( raw_text, raw_text_length );
	auto text_as_utf16 = conversion::ToUTF16( text );

	auto lock_guard = std::lock_guard( print_mutex );

#if BUILD_OPTIONS_WINDOWS_USE_ANSI_TERMINAL_CONTROLS

	constexpr wchar_t ESC		= L'\x1b';
	constexpr wchar_t CSI[]		= L"\x1b[";
	constexpr wchar_t RES[]		= L"\x1b[0m";

	std::wcout
		<< CSI
		<< PrintRecordColorToWCharANSIForegroundColorCode( foreground_color )
		<< L";"
		<< PrintRecordColorToWCharANSIBackgroundColorCode( background_color )
		<< L"m"
		<< reinterpret_cast<wchar_t*>( text_as_utf16.ToCStr() )
		<< RES;

#else

	HANDLE std_out_handle = GetStdHandle( STD_OUTPUT_HANDLE );
	if( std_out_handle == INVALID_HANDLE_VALUE )
	{
		return;
	}

	WORD old_attributes = 7; // Default black background, Gray color text.
	{
		CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info {};
		if( GetConsoleScreenBufferInfo( std_out_handle, &console_screen_buffer_info ) )
		{
			old_attributes = console_screen_buffer_info.wAttributes;
		}
	}

	WORD foreground_color_as_int	= ( foreground_color == PrintRecordColor::DEFAULT ) ? ( ( old_attributes & WORD( 0x000F ) ) ) : ( WORD( foreground_color ) );
	WORD background_color_as_int	= ( background_color == PrintRecordColor::DEFAULT ) ? ( ( old_attributes & WORD( 0x00F0 ) ) ) : ( WORD( background_color ) << 4 );
	WORD new_attributes				= background_color_as_int | foreground_color_as_int;

	SetConsoleTextAttribute( std_out_handle, new_attributes );

	static_assert( sizeof( wchar_t ) == 2, "Wrong wchar_t size, this is Windows only." );

	std::wcout << reinterpret_cast<const wchar_t*>( text_as_utf16.ToCStr() );

	SetConsoleTextAttribute( std_out_handle, old_attributes );

	#endif // BUILD_OPTIONS_WINDOWS_USE_ANSI_TERMINAL_CONTROLS
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal_::SetupSystemConsole(
	u64 characters_per_line_num
)
{
	HANDLE std_out_handle = GetStdHandle( STD_OUTPUT_HANDLE );
	if( std_out_handle == INVALID_HANDLE_VALUE )
	{
		// TODO: Figure out what to do if we cannot aquire a handle to system console.
		return;
	}

	// Set font to 'Consolas'.
	// This is purely to make command line more readable, it'll fail if ran in the new Windows terminal but we don't have to
	// care if it fails in that situation as Windows terminal is readable enough.
	{
		auto cfi = CONSOLE_FONT_INFOEX {};
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 18;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s( cfi.FaceName, L"Consolas" );
		SetCurrentConsoleFontEx( std_out_handle, FALSE, &cfi );
	}

	auto window_info = CONSOLE_SCREEN_BUFFER_INFO {};
	GetConsoleScreenBufferInfo(
		std_out_handle,
		&window_info
	);

	auto console_buffer_size = COORD { SHORT( characters_per_line_num ), window_info.dwSize.Y };
	SetConsoleScreenBufferSize(
		std_out_handle,
		console_buffer_size
	);

	auto new_console_dimensions		= window_info.srWindow;
	new_console_dimensions.Right	= new_console_dimensions.Left	+ SHORT( characters_per_line_num );
	SetConsoleWindowInfo(
		std_out_handle,
		TRUE,
		&new_console_dimensions
	);

	// Console mode also fails for Windows terminal. We'll just skip the rest if they do.
	// This is mainly to make sure that virtual terminal works for command line.
	auto console_mode = DWORD {};
	auto got_console_mode = GetConsoleMode( std_out_handle, &console_mode );

	if( got_console_mode )
	{

#if BUILD_OPTIONS_WINDOWS_USE_ANSI_TERMINAL_CONTROLS
		console_mode |= ENABLE_PROCESSED_OUTPUT;
		console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
#endif

		if( !SetConsoleMode( std_out_handle, console_mode ) )
		{
			// TODO: Figure out what to do if we fail to set up the console mode.
		}
	}

	system( "cls" );
}



#elif defined( BITCRAFTE_PLATFORM_LINUX )



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::SetupSystemConsole(
	u64 characters_per_line_num
)
{
	auto set_characters_per_line_syscall_string = TextFormat( "stty cols {}", characters_per_line_num );
	std::system( set_characters_per_line_syscall_string );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal_::SystemConsolePrintRawUTF8(
	const char8_t						*	raw_text,
	u64										raw_text_length,
	bc::diagnostic::PrintRecordColor		foreground_color,
	bc::diagnostic::PrintRecordColor		background_color
)
{
	// TODO: Test ConsolePrint on Linux machine.

	auto text = SimpleText8( SimpleTextView8( raw_text, raw_text_length ) );

	std::lock_guard<std::mutex> lock_guard( print_mutex );

	constexpr char ESC			= '\x1b';
	constexpr char CSI[]		= "\x1b[";
	constexpr char RES[]		= "\x1b[0m";

	std::cout
		<< CSI
		<< PrintRecordColorToCharANSIForegroundColorCode( foreground_color )
		<< ";"
		<< PrintRecordColorToCharANSIBackgroundColorCode( background_color )
		<< "m"
		<< reinterpret_cast<char*>( text.ToCStr() )
		<< RES;

	//std::cout << "\033[" << foreground_color_map[ foreground_color ] << ";" << background_color_map[ background_color ] << "m"
	//	<< Text8( text ).ToCStr()
	//	<< "\033[0m";
}

#else

#error "Please add platform support!"

#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::SystemConsolePrint(
	const PrintRecord	&	print_record
)
{
	auto finalized_print_record = print_record.GetFinalized();
	for( const auto & s : finalized_print_record.GetSections() )
	{
		auto print_record_colors = diagnostic::GetPrintRecordThemeColors( s.theme );
		SystemConsolePrint( s.text, print_record_colors.foreground_color, print_record_colors.background_color );
	}
}

