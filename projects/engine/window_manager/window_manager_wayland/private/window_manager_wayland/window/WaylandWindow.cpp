
#include <window_manager_wayland/PreCompiledHeader.hpp>

#include <window_manager_wayland/wayland_manager/WaylandManager.hpp>
#include <window_manager_wayland/window/WaylandWindow.hpp>



bc::window_manager::WaylandWindow::Settings::Settings(
	const bc::window_manager::WindowCreateInfo & window_create_info
)
{
	is_decorated			= window_create_info.decorated;
	show_minimize_button	= window_create_info.has_minimize_button;
	show_maximize_button	= window_create_info.has_maximize_button;
	is_minimized			= window_create_info.initially_minimized;
	is_maximized			= window_create_info.initially_maximized;
	allow_drag_resize		= window_create_info.allow_drag_resize;
	allow_file_drop			= window_create_info.allow_file_drop;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandWindow::WaylandWindow(
	WaylandManager			&	wayland_manager,
	const WindowCreateInfo	&	window_create_info
) :
	Window( window_create_info ),
	settings( window_create_info ),
	wayland_manager( wayland_manager )
{
	// TODO: Create wayland window.

//	platform_handles.hWnd		= NULL;
//	platform_handles.hInstance	= win32_manager.GetWindowClass().hInstance;
//
//	style = settings.MakeWindowStyle();
//
//	// Create a window
//	platform_handles.hWnd = CreateWindowExW(
//		0,
//		win32_manager.GetWindowClass().lpszClassName,
//		L"My Window",
//		style,
//		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
//		NULL, NULL, NULL, NULL
//	);
//
//	if( platform_handles.hWnd == NULL )
//	{
//		MessageBoxW( NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK );
//		BHardAssert( 0, "Window registration failed" );
//	}
//
//	// Set user data to this so we can use this window class later.
//	SetWindowLongPtr( platform_handles.hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );
//
//	// Show and update the window
//	ShowWindow( platform_handles.hWnd, SW_SHOWNORMAL );
//	UpdateWindow( platform_handles.hWnd );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::window_manager::WaylandWindow::~WaylandWindow()
{
	wayland_manager.NotifyWindowBeingDestroyed( this );

	// TODO: Destroy wayland window.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::WaylandWindow::GetDecorated()
{
	// TODO: Get window decorations

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::SetDecorated(
	bool is_decorated
)
{
	// TODO: Set window decorations

//	if( is_decorated )
//	{
//		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
//	}
//	else
//	{
//		style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
//	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::window_manager::WaylandWindow::GetResizeable()
{
	// TODO: Get window resizeable
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::SetResizeable(
	bool is_decorated
)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::WaylandWindow::Update()
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * bc::window_manager::WaylandWindow::GetPlatformSpecificHandles()
{
	return &platform_handles;
}
