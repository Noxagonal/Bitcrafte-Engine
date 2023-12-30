
# Vext engine test

cmake_minimum_required(VERSION 3.22)

# Add all subdirectories automatically.
get_subdirectories(SUB_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
	if(NOT ${SUB_DIRECTORY} STREQUAL "-template-")
		add_subdirectory(${SUB_DIRECTORY})
	endif()
endforeach()
