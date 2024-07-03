
# RHI Vulkan

cmake_minimum_required(VERSION 3.22)

find_package(Vulkan REQUIRED)

get_auto_target_name(THIS_TARGET_NAME)

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
set_window_manager_component_defines(${THIS_TARGET_NAME})
