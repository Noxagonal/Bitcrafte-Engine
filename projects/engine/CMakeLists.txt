
cmake_minimum_required(VERSION 3.22)



# Add engine component.
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
# add_engine_component(compositor
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
function(add_engine_component
	ENGINE_COMPONENT_NAME
	ENGINE_COMPONENT_OUTPUT_TYPE
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
		CREATE_ENGINE_COMPONENT
		"${option_args}"
		"${one_value_args}"
		"${multi_value_args}"
	)

	add_bitcrafte_target(
		${ENGINE_COMPONENT_NAME}
		${ENGINE_COMPONENT_OUTPUT_TYPE}
		ADDITIONAL_SOURCES					${CREATE_ENGINE_COMPONENT_ADDITIONAL_SOURCES}
		PRIVATE_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_ENGINE_COMPONENT_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS}
		PUBLIC_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_ENGINE_COMPONENT_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS}
		PRIVATE_LIBRARY_DEPENDENCIES		${CREATE_ENGINE_COMPONENT_PRIVATE_LIBRARY_DEPENDENCIES}
		PUBLIC_LIBRARY_DEPENDENCIES			${CREATE_ENGINE_COMPONENT_PUBLIC_LIBRARY_DEPENDENCIES}
		GENERAL_DEPENDENCIES				${CREATE_ENGINE_COMPONENT_GENERAL_DEPENDENCIES}
	)
	
	set_target_properties(${ENGINE_COMPONENT_NAME}
		PROPERTIES
			FOLDER							"projects/engine"
	)

	if(${BITCRAFTE_BUILD_SHARED_ENGINE})
		target_compile_definitions(${ENGINE_COMPONENT_NAME}
			PRIVATE
				BITCRAFTE_ENGINE_SHARED=1
		)
	endif()

	target_compile_definitions(${ENGINE_COMPONENT_NAME}
		PRIVATE
			BITCRAFTE_ENGINE_COMPONENT=1
	)

endfunction()



# Determine which window components should be included in the built.
if(WIN32)
	if(NOT DEFINED BITCRAFTE_BUILD_WINDOW_WIN32)
	    set(BITCRAFTE_BUILD_WINDOW_WIN32 ON CACHE BOOL "Build Windows window component" FORCE)
	endif()
endif()
if(LINUX)
	if(NOT DEFINED BITCRAFTE_BUILD_WINDOW_WAYLAND)
	    set(BITCRAFTE_BUILD_WINDOW_WAYLAND ON CACHE BOOL "Build Wayland window component" FORCE)
	endif()
elseif(APPLE)
	message(FATAL_ERROR "Please add window component here")
endif()



# Determine which RHI components should be included in the built.
if(WIN32 OR LINUX)
	if(NOT DEFINED BITCRAFTE_BUILD_RHI_VULKAN)
	    set(BITCRAFTE_BUILD_RHI_VULKAN ON CACHE BOOL "Include Vulkan RHI" FORCE)
	endif()
elseif(APPLE)
	if(NOT DEFINED BITCRAFTE_BUILD_RHI_METAL)
	    set(BITCRAFTE_BUILD_RHI_METAL ON CACHE BOOL "Include Metal RHI" FORCE)
	endif()
endif()



# Add all subdirectories automatically.
get_subdirectories(SUB_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
	if(NOT ${SUB_DIRECTORY} STREQUAL "-template-")
		add_subdirectory(${SUB_DIRECTORY})
	endif()
endforeach()
