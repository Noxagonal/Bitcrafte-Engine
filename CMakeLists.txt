
# Bitcrafte Engine root

cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

include(CheckIPOSupported)

project(Bitcrafte CXX)



################################################################
# Common paths to different folders
################################################################
set(BITCRAFTE_EXTERNAL_SUBMODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")

set(BITCRAFTE_UTILITIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utilities")
set(BITCRAFTE_PROJECTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/projects")



################################################################
# Set global project settings.
################################################################

# Set C++ and C standards
set(CMAKE_CXX_STANDARD				23)
set(CMAKE_C_STANDARD				17)

set(CMAKE_CXX_STANDARD_REQUIRED		ON)
set(CMAKE_CXX_EXTENSIONS			OFF)
set(CMAKE_C_STANDARD_REQUIRED		ON)
set(CMAKE_C_EXTENSIONS				OFF)

# Disable in source build.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# This is not a library and not a final product so forcing installing locally for now.
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

# Enable folder separators for targets, (DO NOT CHANGE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Use multiple cores when compiling, not automatically
# enabled in Visual Studio. 
if(MSVC)
	add_compile_options(
		"/MP"
	)
endif()



################################################################
# Options, variables, lists, files.
################################################################
option(BITCRAFTE_BUILD_EDITOR
	"Build and include editor target"
	ON
)
option(BITCRAFTE_BUILD_ENGINE
	"Build and include engine modules, also disables editor and runtime targets"
	ON
)
option(BITCRAFTE_BUILD_SHARED_ENGINE
	"Build engine as a shared library instead of a static library"
	ON
)

# Check and enforce interprocedural optimization (IPO/LTO)
check_ipo_supported(RESULT IPO_SUPPORT_RESULT OUTPUT IPO_SUPPORT_OUTPUT)
if(NOT IPO_SUPPORT_RESULT)
  message(FATAL_ERROR "Interprocedural optimization is required but not supported: ${IPO_SUPPORT_OUTPUT}")
endif()



################################################################
# Project prerequisites
################################################################
add_subdirectory("submodules" EXCLUDE_FROM_ALL)



################################################################
# Project files
################################################################
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		${UTILITIES_INCLUDE}
)

add_subdirectory("projects")
