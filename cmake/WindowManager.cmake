
if(NOT DEFINED BITCRAFTE_WINDOW_MANAGER_INCLUDED)
	set(BITCRAFTE_WINDOW_MANAGER_INCLUDED TRUE)


	# Get a list of supported window managers on the current platform.
	function(get_supported_window_manager_list
		OUT_LIST
	)
		set(WINDOW_MANAGER_LIST)
		if(WIN32)
			list(APPEND WINDOW_MANAGER_LIST "win32")
		elseif(UNIX AND NOT APPLE)
			if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
				list(APPEND WINDOW_MANAGER_LIST "xlib")
				list(APPEND WINDOW_MANAGER_LIST "wayland")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
				list(APPEND WINDOW_MANAGER_LIST "xlib")
				list(APPEND WINDOW_MANAGER_LIST "wayland")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
				list(APPEND WINDOW_MANAGER_LIST "xlib")
				list(APPEND WINDOW_MANAGER_LIST "wayland")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
				list(APPEND WINDOW_MANAGER_LIST "android")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
				list(APPEND WINDOW_MANAGER_LIST "xlib")
				# TODO: Check support for Wayland.
			elseif(CMAKE_SYSTEM_NAME STREQUAL "Haiku")
				list(APPEND WINDOW_MANAGER_LIST "haiku")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
				list(APPEND WINDOW_MANAGER_LIST "photon")
			else()
				message(FATAL_ERROR "Please add unix window manager component here")
			endif()
		elseif(APPLE)
			if(CMAKE_SYSTEM_NAME STREQUAL "MacOS")
				list(APPEND WINDOW_MANAGER_LIST "macos")
			elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
				list(APPEND WINDOW_MANAGER_LIST "ios")
			else()
				message(FATAL_ERROR "Please add Apple platform window manager component here")
			endif()
		else()
			message(FATAL_ERROR "Please add platform window manager component here")
		endif()
		set(${OUT_LIST} ${WINDOW_MANAGER_LIST} PARENT_SCOPE)
	endfunction()


	# Get window manager component dependencies which can be used to add a library dependency to a target.
	function(get_window_manager_component_dependencies
		OUT_LIST
	)
		set(WINDOW_MANAGER_LIST ${ARGN})
		set(DEPENDENCY_LIST)
		foreach(WINDOW_MANAGER IN LISTS WINDOW_MANAGER_LIST)
			if(${WINDOW_MANAGER} STREQUAL "win32")
				list(APPEND DEPENDENCY_LIST window_manager_win32)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "xlib")
				list(APPEND DEPENDENCY_LIST window_manager_xlib)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "xcb")
				list(APPEND DEPENDENCY_LIST window_manager_xcb)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "wayland")
				list(APPEND DEPENDENCY_LIST window_manager_wayland)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "android")
				list(APPEND DEPENDENCY_LIST window_manager_android)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "macos")
				list(APPEND DEPENDENCY_LIST window_manager_macos)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "haiku")
				list(APPEND DEPENDENCY_LIST window_manager_haiku)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "photon")
				list(APPEND DEPENDENCY_LIST window_manager_photon)
			endif()
		endforeach()
		set(${OUT_LIST} ${DEPENDENCY_LIST} PARENT_SCOPE)
	endfunction()


	# Set window manager compile definitions for a specific target.
	function(set_target_window_manager_component_defines
		THIS_TARGET_NAME
	)
		set(WINDOW_MANAGER_LIST ${ARGN})
		foreach(WINDOW_MANAGER IN LISTS WINDOW_MANAGER_LIST)
			if(${WINDOW_MANAGER} STREQUAL "win32")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_WIN32=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "xlib")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_XLIB=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "xcb")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_XCB=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "wayland")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_WAYLAND=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "android")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_ANDROID=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "cocoa")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_COCOA=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "haiku")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_HAIKU=1
				)
			endif()
			if(${WINDOW_MANAGER} STREQUAL "photon")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_WINDOW_MANAGER_PHOTON=1
				)
			endif()
		endforeach()
	endfunction()


endif()
