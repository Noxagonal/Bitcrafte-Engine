
# RHI Vulkan

cmake_minimum_required(VERSION 3.22)

find_package(Vulkan REQUIRED)

get_auto_target_name(THIS_TARGET_NAME)

add_engine_rhi_component(${THIS_TARGET_NAME}
	OBJECT_LIBRARY
	PUBLIC_LIBRARY_DEPENDENCIES
		core
		window
		rhi
	PRIVATE_LIBRARY_DEPENDENCIES
		Vulkan::Vulkan
)