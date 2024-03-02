
# Engine

cmake_minimum_required(VERSION 3.22)

get_auto_target_name(THIS_TARGET_NAME)


if(BITCRAFTE_BUILD_SHARED_ENGINE)
	set(ENGINE_OUTPUT_TYPE SHARED_LIBRARY)
else()
	set(ENGINE_OUTPUT_TYPE STATIC_LIBRARY)
endif()



get_window_manager_component_dependencies(BITCRAFTE_ENGINE_WINDOW_MANAGER_COMPONENT_DEPENDENCIES)
get_rhi_component_dependencies(BITCRAFTE_ENGINE_RHI_COMPONENT_DEPENDENCIES)

# Make the engine target
add_engine_component(${THIS_TARGET_NAME}
	${ENGINE_OUTPUT_TYPE}
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		rhi
		window_manager
		scene
	PRIVATE_LIBRARY_DEPENDENCIES
		${BITCRAFTE_ENGINE_WINDOW_MANAGER_COMPONENT_DEPENDENCIES}
		${BITCRAFTE_ENGINE_RHI_COMPONENT_DEPENDENCIES}
)

set_window_manager_component_defines(${THIS_TARGET_NAME})
set_rhi_component_defines(${THIS_TARGET_NAME})
