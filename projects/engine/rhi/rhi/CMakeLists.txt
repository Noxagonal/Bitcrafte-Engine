
# RHI Base

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/RHI.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

add_engine_rhi_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
)



get_supported_rhi_list(RHI_LIST)
message(STATUS "Supported RHIs: ${RHI_LIST}")

set_target_rhi_component_defines(${THIS_TARGET_NAME} ${RHI_LIST})
