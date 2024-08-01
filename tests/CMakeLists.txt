
# BITCRAFTE tests root

cmake_minimum_required(VERSION 3.22)

message("========== Tests ==========")



# Create a test.
#
# first parameter must be the test name.
#
# Remaining parameters must be after <PUBLIC/PRIVATE>ADDITIONAL_INCLUDE_FOLDERS, <PUBLIC/PRIVATE>LIBRARY_DEPENDENCIES or GENERAL_DEPENDENCIES keywords.
# PRIVATE_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the test.
# PUBLIC_ADDITIONAL_INCLUDE_FOLDERS: Defines additional include folder paths included in the test.
# PRIVATE_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the test.
# PUBLIC_LIBRARY_DEPENDENCIES: Adds dependency to these engine libraries to the test.
# GENERAL_DEPENDENCIES: Adds general dependency to these targets that must compile before this library.
#
# Usage example:
# add_bitcrafte_test(test_name
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
function(add_bitcrafte_test
	BITCRAFTE_TEST_NAME
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
		CREATE_BITCRAFTE_TEST
		"${option_args}"
		"${one_value_args}"
		"${multi_value_args}"
	)

	set(BITCRAFTE_TEST_SOURCE_DIR
		"${CMAKE_CURRENT_SOURCE_DIR}"
	)

	# All files in source folder
	file(GLOB_RECURSE SOURCE_FILES
		RELATIVE "${BITCRAFTE_TEST_SOURCE_DIR}"
		CONFIGURE_DEPENDS
		"${BITCRAFTE_TEST_SOURCE_DIR}/source/*"
	)

	set(UTILS_INCLUDE
		"${BITCRAFTE_UTILITIES_DIR}/.natvis"
	)

	source_group(TREE "${BITCRAFTE_TEST_SOURCE_DIR}"
		FILES
			${SOURCE_FILES}
	)

	add_executable(${BITCRAFTE_TEST_NAME}
		${SOURCE_FILES}
		${UTILS_INCLUDE}
		${CREATE_BITCRAFTE_TEST_ADDITIONAL_SOURCES}
	)

	set_target_properties(${BITCRAFTE_TEST_NAME}
		PROPERTIES
			LINKER_LANGUAGE					CXX
			ARCHIVE_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
			LIBRARY_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
			RUNTIME_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/bin/"
			ENGINEDEVELOPMENT_POSTFIX		"_engine_dev"
			GAMEDEVELOPMENT_POSTFIX			"_dev"
			FOLDER							"tests"
			POSITION_INDEPENDENT_CODE		ON
			#INTERPROCEDURAL_OPTIMIZATION	TRUE
	)

	target_compile_definitions(${BITCRAFTE_TEST_NAME}
		PRIVATE
			"$<$<CONFIG:GameShipping>:BITCRAFTE_GAME_SHIPPING_BUILD=1>"
			"$<$<CONFIG:GameDevelopment>:BITCRAFTE_GAME_DEVELOPMENT_BUILD=1>"
			"$<$<CONFIG:EngineDevelopment>:BITCRAFTE_GAME_DEVELOPMENT_BUILD=1>"
			"$<$<CONFIG:EngineDevelopment>:BITCRAFTE_ENGINE_DEVELOPMENT_BUILD=1>"
	)

	target_link_libraries(${BITCRAFTE_TEST_NAME}
		PRIVATE
			gtest_main
	)

	if(DEFINED CREATE_BITCRAFTE_TEST_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS)
		target_include_directories(${BITCRAFTE_TEST_NAME}
			PRIVATE
				${CREATE_BITCRAFTE_TEST_PRIVATE_ADDITIONAL_INCLUDE_FOLDERS}
		)
	endif()
	if(DEFINED CREATE_BITCRAFTE_TEST_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS)
		target_include_directories(${BITCRAFTE_TEST_NAME}
			PUBLIC
				${CREATE_BITCRAFTE_TEST_PUBLIC_ADDITIONAL_INCLUDE_FOLDERS}
		)
	endif()

	if(DEFINED CREATE_BITCRAFTE_TEST_PRIVATE_LIBRARY_DEPENDENCIES)
		target_link_libraries(${BITCRAFTE_TEST_NAME}
			PRIVATE
				${CREATE_BITCRAFTE_TEST_PRIVATE_LIBRARY_DEPENDENCIES}
		)
	endif()
	if(DEFINED CREATE_BITCRAFTE_TEST_PUBLIC_LIBRARY_DEPENDENCIES)
		target_link_libraries(${BITCRAFTE_TEST_NAME}
			PUBLIC
				${CREATE_BITCRAFTE_TEST_PUBLIC_LIBRARY_DEPENDENCIES}
		)
	endif()

	if(DEFINED CREATE_BITCRAFTE_TEST_GENERAL_DEPENDENCIES)
		add_dependencies(${BITCRAFTE_TEST_NAME}
			${CREATE_BITCRAFTE_TEST_GENERAL_DEPENDENCIES}
		)
	endif()

	add_test(
		NAME 		${BITCRAFTE_TEST_NAME}
		COMMAND 	${BITCRAFTE_TEST_NAME}
	)

endfunction()

# Get a target name automatically from current source folder name.
function(get_auto_test_target_name
	OUT_RESULT
)
	cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME AUTO_TARGET_NAME)
	SET(${OUT_RESULT} "test_${AUTO_TARGET_NAME}" PARENT_SCOPE)
endfunction()



# Add the tests.
add_subdirectory("engine")
