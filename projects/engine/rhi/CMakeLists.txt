
cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/BitCrafteEngineComponent.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/RHI.cmake")



# Add RHI component.
#
# First parameter must be the engine component name. Eg. compositor
#
# Second parameter must be the engine component output type, library or executable. It must be one of the following:
# OBJECT_LIBRARY : no usable library files are produced, but compiled object files are used directly.
# STATIC_LIBRARY : engine component produces a static library.
# SHARED_LIBRARY : engine component produces a shared library, .dll or .so file.
#
# Remaining parameters must be after <PUBLIC/PRIVATE>ADDITIONAL_INCLUDE_FOLDERS, <PUBLIC/PRIVATE>LIBRARY_DEPENDENCIES and GENERAL_DEPENDENCIES keywords.
# PRIVATE_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the project.
# PUBLIC_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the project.
# PRIVATE_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the project.
# PUBLIC_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the project.
# GENERAL_DEPENDENCIES: Adds general dependency to these targets that must compile before this library.
#
# Usage example:
# add_engine_rhi_component(compositor
#	OBJECT_LIBRARY
#	PRIVATE_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PRIVATE_INCLUDE_FOLDERS_HERE}
#	PUBLIC_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PUBLIC_INCLUDE_FOLDERS_HERE}
#	PRIVATE_LIBRARY_DEPENDENCIES
#		core
#	PUBLIC_LIBRARY_DEPENDENCIES
#		glfw
#	GENERAL_DEPENDENCIES
#		code_inspector
# )
function(add_engine_rhi_component
	ENGINE_RHI_COMPONENT_NAME
	ENGINE_RHI_COMPONENT_OUTPUT_TYPE
)
	set(option_args)
	set(one_value_args)
	set(multi_value_args
		ADDITIONAL_SOURCES
		PRIVATE_ADDITIONAL_INCLUDE_FOLDERS
		PUBLIC_ADDITIONAL_INCLUDE_FOLDERS
		PRIVATE_LIBRARY_DEPENDENCIES
		PUBLIC_LIBRARY_DEPENDENCIES
		GENERAL_DEPENDENCIES
	)

	cmake_parse_arguments(
		PARSE_ARGV 1
		CREATE_ENGINE_RHI_COMPONENT
		"${option_args}"
		"${one_value_args}"
		"${multi_value_args}"
	)

	add_bitcrafte_target(
		${ENGINE_RHI_COMPONENT_NAME}
		${ENGINE_RHI_COMPONENT_OUTPUT_TYPE}
		ADDITIONAL_SOURCES					${CREATE_ENGINE_RHI_COMPONENT_ADDITIONAL_SOURCES}
		PRIVATE_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_ENGINE_RHI_COMPONENT_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS}
		PUBLIC_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_ENGINE_RHI_COMPONENT_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS}
		PRIVATE_LIBRARY_DEPENDENCIES		${CREATE_ENGINE_RHI_COMPONENT_PRIVATE_LIBRARY_DEPENDENCIES}
		PUBLIC_LIBRARY_DEPENDENCIES			${CREATE_ENGINE_RHI_COMPONENT_PUBLIC_LIBRARY_DEPENDENCIES}
		GENERAL_DEPENDENCIES				${CREATE_ENGINE_RHI_COMPONENT_GENERAL_DEPENDENCIES}
	)
	
	set_target_properties(${ENGINE_RHI_COMPONENT_NAME}
		PROPERTIES
			FOLDER							"projects/engine/rhi"
	)
endfunction()



# add rhi components.
add_subdirectory("rhi")
set(RHI_LIST)
get_supported_rhi_list(RHI_LIST)
foreach(RHI IN LISTS RHI_LIST)
	add_subdirectory("rhi_${RHI}")
endforeach()
