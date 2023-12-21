
cmake_minimum_required(VERSION 3.22)



################################################################
# Update library submodules.
################################################################
find_package(Git REQUIRED QUIET)

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
else()
	message(FATAL_ERROR "Git repository not found, unable to download dependencies automatically")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
#if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glm/CMakeLists.txt")
#    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()



#if(BITCRAFTE_BUILD_ENGINE)
#	################################
#	# GLFW 3
#	################################
#	set(BUILD_SHARED_LIBS		OFF	CACHE BOOL	"We need static library.")
#	set(GLFW_BUILD_EXAMPLES		OFF	CACHE BOOL	"No need for examples.")
#	set(GLFW_BUILD_TESTS		OFF	CACHE BOOL	"No need for tests.")
#	set(GLFW_BUILD_DOCS			OFF	CACHE BOOL	"No need for docs.")
#	set(GLFW_VULKAN_STATIC		OFF	CACHE BOOL	"We plan to link with static vulkan library.")
#	add_subdirectory("glfw")
#
#	set_target_properties(glfw
#		PROPERTIES
#			ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/lib"
#			LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/lib"
#			RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/bin"
#			FOLDER						"third_party/glfw"
#			POSITION_INDEPENDENT_CODE	ON
#	)
#
#	set_target_properties(uninstall
#		PROPERTIES
#			ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/lib"
#			LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/lib"
#			RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_CURRENT_BINARY_DIR}/bin"
#			FOLDER						"third_party/glfw"
#			POSITION_INDEPENDENT_CODE	ON
#	)
#endif()



# Set google test projects to their own folders
set_target_properties(gtest			PROPERTIES FOLDER "submodules/googletest")
set_target_properties(gtest_main	PROPERTIES FOLDER "submodules/googletest")
set_target_properties(gmock			PROPERTIES FOLDER "submodules/googletest")
set_target_properties(gmock_main	PROPERTIES FOLDER "submodules/googletest")
