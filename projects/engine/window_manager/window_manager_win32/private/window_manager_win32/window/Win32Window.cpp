
#include <window_manager_win32/PreCompiledHeader.hpp>

#include <window_manager_win32/win32_manager/Win32Manager.hpp>
#include <window_manager_win32/window/Win32Window.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Win32Window::Win32Window(
	Win32Manager&				win32_manager,
	const WindowCreateInfo&		window_create_info
) :
	Window( window_create_info ),
	win32_manager( win32_manager )
{
	platform_handles.hWnd		= NULL;
	platform_handles.hInstance	= win32_manager.GetWindowClass().hInstance;

	style = MakeWindowStyleFromSettings( settings );

	// Create a window
	platform_handles.hWnd = CreateWindowExW(
		0,
		win32_manager.GetWindowClass().lpszClassName,
		L"My Window",
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, NULL, NULL
	);

	if( platform_handles.hWnd == NULL )
	{
		MessageBoxW( NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK );
		BHardAssert( 0, "Window registration failed" );
	}

	// Set user data to this so we can use this window class later.
	SetWindowLongPtr( platform_handles.hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

	// Show and update the window
	ShowWindow( platform_handles.hWnd, SW_SHOWNORMAL );
	UpdateWindow( platform_handles.hWnd );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::Win32Window::~Win32Window()
{
	win32_manager.NotifyWindowBeingDestroyed( this );
	DestroyWindow( platform_handles.hWnd );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::Win32Window::GetDecorated() const -> bool
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Window::SetDecorated( bool is_decorated )
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
auto bc::window_manager::Win32Window::GetResizeable() const -> bool
{
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Window::SetResizeable( bool is_resizeable )
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::Win32Window::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::Win32Window::GetPlatformSpecificHandles() const -> const WindowManagerPlatformHandlesBase*
{
	return &platform_handles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::window_manager::Win32Window::MakeWindowStyleFromSettings( const Settings& from_settings ) const -> DWORD
{
	static_assert( sizeof( Settings ) == 8, "If this is not true, this function needs to be updated" );

	return
		( from_settings.is_decorated ? ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ) : 0 ) |
		( from_settings.is_visible ? ( WS_VISIBLE ) : 0 ) |
		( from_settings.show_minimize_button && from_settings.is_decorated ? ( WS_MINIMIZEBOX ) : 0 ) |
		( from_settings.show_maximize_button && from_settings.is_decorated ? ( WS_MAXIMIZEBOX ) : 0 ) |
		( from_settings.allow_drag_resize ? ( WS_THICKFRAME ) : 0 ) |
		( from_settings.allow_file_drop ? ( WS_EX_ACCEPTFILES ) : 0 ) |
		( from_settings.is_maximized ? ( WS_MAXIMIZE ) : 0 ) |
		( from_settings.is_minimized ? ( WS_MINIMIZE ) : 0 );
}
