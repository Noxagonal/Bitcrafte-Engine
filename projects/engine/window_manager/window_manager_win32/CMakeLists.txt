
# Window manager Win32

cmake_minimum_required(VERSION 3.22)

get_auto_target_name(THIS_TARGET_NAME)

add_engine_window_manager_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window_manager
)

set_window_manager_component_defines(${THIS_TARGET_NAME})
