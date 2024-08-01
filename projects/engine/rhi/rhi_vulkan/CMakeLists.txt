
# RHI Vulkan

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/BitCrafteEngineComponent.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/RHI.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/WindowManager.cmake")



find_package(Vulkan REQUIRED)

get_auto_target_name(THIS_TARGET_NAME)
begin_bitcrafte_target(${THIS_TARGET_NAME})

add_engine_rhi_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		rhi
		window_manager
	PRIVATE_LIBRARY_DEPENDENCIES
		Vulkan::Vulkan
)

# Set window manager component defines like BITCRAFTE_WINDOW_MANAGER_WIN32 or
# BITCRAFTE_WINDOW_MANAGER_WAYLAND. These are needed by vulkan window system integration.
get_supported_window_manager_list(WINDOW_MANAGER_LIST)
set_target_window_manager_component_defines(${THIS_TARGET_NAME} ${WINDOW_MANAGER_LIST})
