
# Window manager Win32

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/WindowManager.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

add_engine_window_manager_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window_manager
)

get_supported_window_manager_list(WINDOW_MANAGER_LIST)
set_target_window_manager_component_defines(${THIS_TARGET_NAME} ${WINDOW_MANAGER_LIST})
