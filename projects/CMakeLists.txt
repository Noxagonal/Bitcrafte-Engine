
cmake_minimum_required(VERSION 3.22)



# Add bitcrafte target.
#
# First parameter must be the target name. Eg. editor
#
# Second parameter must be the target output type, library or executable. It must be one of the following:
# OBJECT_LIBRARY : no usable library files are produced, but compiled object files are used directly.
# STATIC_LIBRARY : target produces a static library.
# SHARED_LIBRARY : target produces a shared library, .dll or .so file.
# EXECUTABLE : target produces an executable.
#
# Remaining parameters must be after <PUBLIC/PRIVATE>ADDITIONAL_INCLUDE_FOLDERS, <PUBLIC/PRIVATE>LIBRARY_DEPENDENCIES and GENERAL_DEPENDENCIES keywords.
# PRIVATE_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the target.
# PUBLIC_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the target.
# PRIVATE_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the target.
# PUBLIC_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the target.
# GENERAL_DEPENDENCIES: Adds general dependency to these targets that must compile before this library.
#
# Usage example:
# add_bitcrafte_target(compositor
#	OBJECT_LIBRARY
#	PRIVATE_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PRIVATE_INCLUDE_FOLDERS_HERE}
#	PUBLIC_ADDITIONAL_INCLUDE_FOLDERS
#		${ADDITIONAL_PUBLIC_INCLUDE_FOLDERS_HERE}
#	PRIVATE_LIBRARY_DEPENDENCIES
#		glfw
#	PUBLIC_LIBRARY_DEPENDENCIES
#		core
#	GENERAL_DEPENDENCIES
#		code_inspector
# )
function(add_bitcrafte_target
	BITCRAFTE_TARGET_NAME
	BITCRAFTE_TARGET_OUTPUT_TYPE
)
	message("========== ${BITCRAFTE_TARGET_NAME} ==========")

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
		CREATE_BITCRAFTE_TARGET
		"${option_args}"
		"${one_value_args}"
		"${multi_value_args}"
	)
	
	# All files in public folder
	file(GLOB_RECURSE PUBLIC_SOURCE_FILES
		RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/public/${BITCRAFTE_TARGET_NAME}/*"
	)

	# All files in private folder
	file(GLOB_RECURSE PRIVATE_SOURCE_FILES
		RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/private/${BITCRAFTE_TARGET_NAME}/*"
	)

	# All files in resource folder
	file(GLOB_RECURSE PRIVATE_RESOURCE_FILES
		RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/resource/${BITCRAFTE_TARGET_NAME}/*"
	)

	set(UTILITIES_INCLUDE
		"${BITCRAFTE_UTILITIES_DIR}/.natvis"
	)

	#source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	#	FILES
	#		${PUBLIC_SOURCE_FILES}
	#		${PRIVATE_SOURCE_FILES}
	#)

	# Set source group for public files
	foreach(SOURCE_FILE IN ITEMS ${PUBLIC_SOURCE_FILES})
		get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
		file(RELATIVE_PATH SOURCE_FILE_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/public/${BITCRAFTE_TARGET_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE_PATH}")
		source_group("public/${SOURCE_FILE_RELATIVE_PATH}" FILES "${SOURCE_FILE}")
	endforeach()

	# Set source group for private files
	foreach(SOURCE_FILE IN ITEMS ${PRIVATE_SOURCE_FILES})
		get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
		file(RELATIVE_PATH SOURCE_FILE_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/private/${BITCRAFTE_TARGET_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE_PATH}")
		source_group("private/${SOURCE_FILE_RELATIVE_PATH}" FILES "${SOURCE_FILE}")
	endforeach()
	
	foreach(SOURCE_FILE IN ITEMS ${PRIVATE_RESOURCE_FILES})
		get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
		file(RELATIVE_PATH SOURCE_FILE_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resource/${BITCRAFTE_TARGET_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE_PATH}")
		source_group("resource/${SOURCE_FILE_RELATIVE_PATH}" FILES "${SOURCE_FILE}")
	endforeach()

	source_group("utilities"
		FILES ${UTILITIES_INCLUDE}
	)

	if(${BITCRAFTE_TARGET_OUTPUT_TYPE} STREQUAL "EXECUTABLE")
		add_executable(${BITCRAFTE_TARGET_NAME})
	elseif(${BITCRAFTE_TARGET_OUTPUT_TYPE} STREQUAL "SHARED_LIBRARY")
		add_library(${BITCRAFTE_TARGET_NAME} SHARED)
	elseif(${BITCRAFTE_TARGET_OUTPUT_TYPE} STREQUAL "STATIC_LIBRARY")
		add_library(${BITCRAFTE_TARGET_NAME} STATIC)
	elseif(${BITCRAFTE_TARGET_OUTPUT_TYPE} STREQUAL "OBJECT_LIBRARY")
		add_library(${BITCRAFTE_TARGET_NAME} OBJECT)
	else()
		if(${BITCRAFTE_TARGET_OUTPUT_TYPE})
			message(FATAL_ERROR
				"Target '${BITCRAFTE_TARGET_NAME}' output type was invalid.
				'${BITCRAFTE_TARGET_OUTPUT_TYPE}' was given.
				Accepted types: 'EXECUTABLE', 'SHARED_LIBRARY', 'STATIC_LIBRARY', OBJECT_LIBRARY
				"
			)
		else()
			message(FATAL_ERROR
				"Target '${BITCRAFTE_TARGET_NAME}' output type not given. Accepted types: 'EXECUTABLE', 'SHARED_LIBRARY', 'STATIC_LIBRARY', OBJECT_LIBRARY"
			)
		endif()
	endif()

	target_sources(${BITCRAFTE_TARGET_NAME}
		PUBLIC
			${PUBLIC_SOURCE_FILES}
		PRIVATE
			${PRIVATE_SOURCE_FILES}
			${PRIVATE_RESOURCE_FILES}
			${UTILITIES_INCLUDE}
			${CREATE_BITCRAFTE_TARGET_ADDITIONAL_SOURCES}
	)

	set_target_properties(${BITCRAFTE_TARGET_NAME}
		PROPERTIES
			LINKER_LANGUAGE					CXX
			ARCHIVE_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
			LIBRARY_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
			RUNTIME_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/bin/"
			DEBUG_POSTFIX					"_d"
			FOLDER							"projects"
			POSITION_INDEPENDENT_CODE		ON
			INTERPROCEDURAL_OPTIMIZATION	TRUE
	)

	target_precompile_headers(${BITCRAFTE_TARGET_NAME}
		PRIVATE
			"${CMAKE_CURRENT_SOURCE_DIR}/private/${BITCRAFTE_TARGET_NAME}/PreCompiledHeader.hpp"
	)

	if(DEFINED CREATE_BITCRAFTE_TARGET_GENERAL_DEPENDENCIES)
		add_dependencies(${BITCRAFTE_TARGET_NAME}
			${CREATE_BITCRAFTE_TARGET_GENERAL_DEPENDENCIES}
		)
	endif()

	target_include_directories(${BITCRAFTE_TARGET_NAME}
		PUBLIC
			"${CMAKE_CURRENT_SOURCE_DIR}/public/"
		PRIVATE
			"${CMAKE_CURRENT_SOURCE_DIR}/private/"
			"${BITCRAFTE_GENERATED_SOURCES_DIR}/"
	)

	if(DEFINED CREATE_BITCRAFTE_TARGET_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS)
		target_include_directories(${BITCRAFTE_TARGET_NAME}
			PRIVATE
				${CREATE_BITCRAFTE_TARGET_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS}
		)
	endif()
	if(DEFINED CREATE_BITCRAFTE_TARGET_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS)
		target_include_directories(${BITCRAFTE_TARGET_NAME}
			PUBLIC
				${CREATE_BITCRAFTE_TARGET_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS}
		)
	endif()

	if(DEFINED CREATE_BITCRAFTE_TARGET_PRIVATE_LIBRARY_DEPENDENCIES)
		target_link_libraries(${BITCRAFTE_TARGET_NAME}
			PRIVATE
				${CREATE_BITCRAFTE_TARGET_PRIVATE_LIBRARY_DEPENDENCIES}
		)
	endif()
	if(DEFINED CREATE_BITCRAFTE_TARGET_PUBLIC_LIBRARY_DEPENDENCIES)
		target_link_libraries(${BITCRAFTE_TARGET_NAME}
			PUBLIC
				${CREATE_BITCRAFTE_TARGET_PUBLIC_LIBRARY_DEPENDENCIES}
		)
	endif()

	install(
		DIRECTORY			"${CMAKE_CURRENT_SOURCE_DIR}/resource/"
		DESTINATION			"install"
	)

endfunction()

# Get a list of subdirectories
function(get_subdirectories
	OUT_RESULT
	CURRENT_DIRECTORY
)
	file(GLOB CHILD_DIRECTORIES
		RELATIVE "${CURRENT_DIRECTORY}"
		CONFIGURE_DEPENDS
		"${CURRENT_DIRECTORY}/*"
	)
	SET(DIRECTORY_LIST "")
	FOREACH(CHILD_DIRECTORY ${CHILD_DIRECTORIES})
		IF(IS_DIRECTORY ${CURRENT_DIRECTORY}/${CHILD_DIRECTORY})
			LIST(APPEND DIRECTORY_LIST ${CHILD_DIRECTORY})
		ENDIF()
	ENDFOREACH()
	SET(${OUT_RESULT} ${DIRECTORY_LIST} PARENT_SCOPE)
endfunction()

# Get a target name automatically from current source folder name.
function(get_auto_target_name
	OUT_RESULT
)
	cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME AUTO_TARGET_NAME)
	SET(${OUT_RESULT} ${AUTO_TARGET_NAME} PARENT_SCOPE)
endfunction()



if(BITCRAFTE_BUILD_ENGINE)
	add_subdirectory("engine")
endif()

if(BITCRAFTE_BUILD_EDITOR AND BITCRAFTE_BUILD_ENGINE)
	add_subdirectory("editor")
endif()
