
# Window manager Wayland

cmake_minimum_required(VERSION 3.22)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)

get_auto_target_name(THIS_TARGET_NAME)

add_engine_window_manager_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PRIVATE_LIBRARY_DEPENDENCIES
		${WAYLAND_LIBRARIES}
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window_manager
)

if (NOT WAYLAND_FOUND)
    message(FATAL_ERROR "Wayland development files not found. Please install wayland-client development package.")
else()
    message(STATUS "Wayland found: ${WAYLAND_VERSION}")
endif()

set_window_manager_component_defines(${THIS_TARGET_NAME})
