
# Window manager XCB

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/WindowManager.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})



# Find XCB
find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11 x11-xcb)
pkg_check_modules(XCB REQUIRED xcb xcb-keysyms xcb-icccm xcb-ewmh)

if(X11_FOUND)
	message(STATUS "Found X11: ${X11_LIBRARIES}")
else()
	message(FATAL_ERROR "X11 not found")
endif()

if(XCB_FOUND)
	message(STATUS "Found XCB: ${XCB_LIBRARIES}")
else()
	message(FATAL_ERROR "XCB not found")
endif()



add_engine_window_manager_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PRIVATE_LIBRARY_DEPENDENCIES
		${X11_LIBRARIES}
		${XCB_LIBRARIES}
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window_manager
)

get_supported_window_manager_list(WINDOW_MANAGER_LIST)
set_target_window_manager_component_defines(${THIS_TARGET_NAME} ${WINDOW_MANAGER_LIST})
