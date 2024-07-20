
# Core

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/BitCrafteEngineComponent.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

add_engine_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		build_configuration
)
