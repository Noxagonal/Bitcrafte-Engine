

# Editor

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/BitCrafteEditorTarget.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

add_editor_component(${THIS_TARGET_NAME}
	EXECUTABLE
	PUBLIC_LIBRARY_DEPENDENCIES
		engine
)
