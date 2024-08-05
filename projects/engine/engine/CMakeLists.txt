
# Engine

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/BitCrafteEngineComponent.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/WindowManager.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/RHI.cmake")



get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

if(BITCRAFTE_BUILD_SHARED_ENGINE)
	set(ENGINE_OUTPUT_TYPE SHARED_LIBRARY)
else()
	set(ENGINE_OUTPUT_TYPE STATIC_LIBRARY)
endif()



get_supported_window_manager_list(WINDOW_MANAGER_LIST)
get_window_manager_component_dependencies(WINDOW_MANAGER_COMPONENT_DEPENDENCIES ${WINDOW_MANAGER_LIST})

get_supported_rhi_list(RHI_LIST)
get_rhi_component_dependencies(RHI_COMPONENT_DEPENDENCIES ${RHI_LIST})



# Make the engine target
add_engine_component(${THIS_TARGET_NAME}
	${ENGINE_OUTPUT_TYPE}
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		rhi
		window_manager
		scene
	PRIVATE_LIBRARY_DEPENDENCIES
		${WINDOW_MANAGER_COMPONENT_DEPENDENCIES}
		${RHI_COMPONENT_DEPENDENCIES}
)
