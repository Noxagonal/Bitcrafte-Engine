
cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/DirectoryOps.cmake")



if(NOT ${BITCRAFTE_INSTALL_GAME_SHIPPING})
	add_all_subdirectories()
endif()
