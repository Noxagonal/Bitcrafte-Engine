
# Scene

cmake_minimum_required(VERSION 3.22)

get_auto_target_name(THIS_TARGET_NAME)

add_engine_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
)
