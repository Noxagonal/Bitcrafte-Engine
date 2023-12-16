

# Editor

cmake_minimum_required(VERSION 3.22)

get_auto_target_name(THIS_TARGET_NAME)

add_editor_component(${THIS_TARGET_NAME}
	EXECUTABLE
	PUBLIC_LIBRARY_DEPENDENCIES
		engine
)
