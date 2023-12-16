
cmake_minimum_required(VERSION 3.22)



# Add editor component.
#
# First parameter must be the editor component name. Eg. compositor
#
# Second parameter must be the editor component output type, library or executable. It must be one of the following:
# OBJECT_LIBRARY : no usable library files are produced, but compiled object files are used directly.
# STATIC_LIBRARY : editor component produces a static library.
# SHARED_LIBRARY : editor component produces a shared library, .dll or .so file.
#
# Remaining parameters must be after <PUBLIC/PRIVATE>ADDITIONAL_INCLUDE_FOLDERS, <PUBLIC/PRIVATE>LIBRARY_DEPENDENCIES and GENERAL_DEPENDENCIES keywords.
# PRIVATE_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the project.
# PUBLIC_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the project.
# PRIVATE_LIBRARY_DEPENDENCIES: Adds dependency to these editor libraries to the project.
# PUBLIC_LIBRARY_DEPENDENCIES: Adds dependency to these editor libraries to the project.
# GENERAL_DEPENDENCIES: Adds general dependency to these targets that must compile before this library.
#
# Usage example:
# add_editor_component(editor
#	EXECUTABLE
#	PRIVATE_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PRIVATE_INCLUDE_FOLDERS_HERE}
#	PUBLIC_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PUBLIC_INCLUDE_FOLDERS_HERE}
#	PRIVATE_LIBRARY_DEPENDENCIES
#		editor_tools
#	PUBLIC_LIBRARY_DEPENDENCIES
#		engine
#	GENERAL_DEPENDENCIES
#		code_inspector
# )
function(add_editor_component
	EDITOR_COMPONENT_NAME
	EDITOR_COMPONENT_OUTPUT_TYPE
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
		CREATE_EDITOR_COMPONENT
		"${option_args}"
		"${one_value_args}"
		"${multi_value_args}"
	)

	add_bitcrafte_target(
		${EDITOR_COMPONENT_NAME}
		${EDITOR_COMPONENT_OUTPUT_TYPE}
		ADDITIONAL_SOURCES					${CREATE_EDITOR_COMPONENT_ADDITIONAL_SOURCES}
		PRIVATE_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_EDITOR_COMPONENT_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS}
		PUBLIC_ADDITIONAL_INCLUDE_FOLDERS	${CREATE_EDITOR_COMPONENT_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS}
		PRIVATE_LIBRARY_DEPENDENCIES		${CREATE_EDITOR_COMPONENT_PRIVATE_LIBRARY_DEPENDENCIES}
		PUBLIC_LIBRARY_DEPENDENCIES			${CREATE_EDITOR_COMPONENT_PUBLIC_LIBRARY_DEPENDENCIES}
		GENERAL_DEPENDENCIES				${CREATE_EDITOR_COMPONENT_GENERAL_DEPENDENCIES}
	)
	
	set_target_properties(${EDITOR_COMPONENT_NAME}
		PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/editor/"
			LIBRARY_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/editor/"
			RUNTIME_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/bin/editor/"
			FOLDER							"projects/editor"
	)

endfunction()

# Add all subdirectories automatically.
get_subdirectories(SUB_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
	if(NOT ${SUB_DIRECTORY} STREQUAL "-template-")
		add_subdirectory(${SUB_DIRECTORY})
	endif()
endforeach()
