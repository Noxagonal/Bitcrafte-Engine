
# Window manager Wayland

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/WindowManager.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})



find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-protocols)

add_subdirectory(generated_wayland_protocols)

add_engine_window_manager_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PRIVATE_LIBRARY_DEPENDENCIES
		${WAYLAND_LIBRARIES}
		generated_wayland_protocols
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window_manager
)

if (NOT WAYLAND_FOUND)
	message(FATAL_ERROR "Wayland development files not found. Please install wayland-client development package.")
endif()

get_supported_window_manager_list(WINDOW_MANAGER_LIST)
set_target_window_manager_component_defines(${THIS_TARGET_NAME} ${WINDOW_MANAGER_LIST})
