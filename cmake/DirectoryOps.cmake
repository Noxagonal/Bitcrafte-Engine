
if(NOT DEFINED DIRECTORY_OPS_INCLUDED)
	set(DIRECTORY_OPS_INCLUDED TRUE)

	
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


	# Add all subdirectories automatically. Exclude template subdirectory "-template-".
	function(add_all_subdirectories)
		# Add all subdirectories automatically.
		get_subdirectories(SUB_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
		foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
			if(NOT ${SUB_DIRECTORY} STREQUAL "-template-")
				add_subdirectory(${SUB_DIRECTORY})
			endif()
		endforeach()
	endfunction()


	# Function to copy contents of one directory to another
	function(copy_folder_contents
		SOURCE_DIR
		DESTINATION_DIR
	)
		# Ensure the destination directory exists
		file(MAKE_DIRECTORY "${DESTINATION_DIR}")

		# Get all files and directories in the source directory
		file(GLOB_RECURSE FILES "${SOURCE_DIR}/*")

		# Copy each file to the destination directory, preserving the directory structure
		foreach(FILE ${FILES})
			# Get the relative path of the file
			file(RELATIVE_PATH rel_path "${SOURCE_DIR}" "${FILE}")
			
			# Construct the full destination path
			set(DESTINATION_PATH "${DESTINATION_DIR}/${rel_path}")
			
			# Make sure the destination directory exists
			get_filename_component(DESTINATION_DIR "${DESTINATION_PATH}" DIRECTORY)
			file(MAKE_DIRECTORY "${DESTINATION_DIR}")

			# Copy the file
			file(COPY "${FILE}" DESTINATION "${DESTINATION_DIR}" FOLLOW_SYMLINK_CHAIN)
		endforeach()
	endfunction()


endif()
