
#include <window/PreCompiledHeader.hpp>
#include "WindowsWindow.hpp"

#if BITCRAFTE_PLATFORM_WINDOWS

#include <window/os_window_system/windows/WindowsWindowSystem.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD bc::window::WindowsWindow::Settings::MakeWindowStyle() const
{
	return
		( is_decorated ? ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ) : 0 ) |
		( is_visible ? ( WS_VISIBLE ) : 0 ) |
		( show_minimize_button && is_decorated ? ( WS_MINIMIZEBOX ) : 0 ) |
		( show_maximize_button && is_decorated ? ( WS_MAXIMIZEBOX ) : 0 ) |
		( allow_drag_resize ? ( WS_THICKFRAME ) : 0 ) |
		( allow_file_drop ? ( WS_EX_ACCEPTFILES ) : 0 ) |
		( is_maximized ? ( WS_MAXIMIZE ) : 0 ) |
		( is_minimized ? ( WS_MINIMIZE ) : 0 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowsWindow::WindowsWindow(
	WindowComponent			&	window_component,
	WindowsWindowSystem		&	windows_window_system,
	const WindowCreateInfo	&	window_create_info
) :
	Window(
		window_component,
		window_create_info
	),
	windows_window_system( windows_window_system )
{
	settings	= MakeSettingsFromCreateInfo( window_create_info );
	style		= settings.MakeWindowStyle();

	// Create a window
	window_handle = CreateWindowExW(
		0,
		windows_window_system.GetWindowClass().lpszClassName,
		L"My Window",
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, NULL, NULL
	);

	if( window_handle == NULL )
	{
		MessageBoxW( NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK );
		BHardAssert( 0, "Window registration failed" );
	}

	// Set user data to this so we can use this window class later.
	SetWindowLongPtr( window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

	// Show and update the window
	ShowWindow( window_handle, SW_SHOWNORMAL );
	UpdateWindow( window_handle );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowsWindow::~WindowsWindow()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window::WindowsWindow::GetDecorated()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowsWindow::SetDecorated(
	bool is_decorated
)
{
	if( is_decorated )
	{
		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
	}
	else
	{
		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
	}
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window::WindowsWindow::GetResizeable()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowsWindow::SetResizeable(
	bool is_decorated
)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window::WindowsWindow::Update()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window::WindowsWindow::Settings bc::window::WindowsWindow::MakeSettingsFromCreateInfo(
	const WindowCreateInfo & window_create_info
)
{
	auto result = Settings {};

	result.is_decorated				= window_create_info.decorated;
	result.show_minimize_button		= window_create_info.has_minimize_button;
	result.show_maximize_button		= window_create_info.has_maximize_button;
	result.is_minimized				= window_create_info.initially_minimized;
	result.is_maximized				= window_create_info.initially_maximized;
	result.allow_drag_resize		= window_create_info.allow_drag_resize;
	result.allow_file_drop			= window_create_info.allow_file_drop;

	return result;
}



#endif // BITCRAFTE_PLATFORM_WINDOWS
