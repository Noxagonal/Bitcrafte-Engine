
# Bitcrafte Engine root

cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

include(CheckIPOSupported)

project(Bitcrafte CXX)



################################################################
# Set configurations
################################################################
set(CMAKE_CONFIGURATION_TYPES "GameShipping;GameDevelopment;EngineDevelopment" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_GAMESHIPPING "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_SHIPPING "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_SHIPPING "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS_GAMEDEVELOPMENT "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_EXE_LINKER_FLAGS_GAMEDEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
set(CMAKE_SHARED_LINKER_FLAGS_GAMEDEVELOPMENT "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")

set(CMAKE_CXX_FLAGS_ENGINEDEVELOPMENT "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_ENGINEDEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_ENGINEDEVELOPMENT "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")

# Set 'EngineDevelopment' as the default build type if none is specified
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Setting default build type to 'EngineDevelopment' as none was specified.")
	set(CMAKE_BUILD_TYPE "EngineDevelopment" CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui, ccmake
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endif()




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

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build/install/${CMAKE_BUILD_TYPE}/")

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
option(BITCRAFTE_BUILD_DOCUMENTATION
	"Build documentation"
	OFF
)
option(BITCRAFTE_BUILD_TESTS
	"Build tests"
	OFF
)
option(BITCRAFTE_INSTALL_GAME_SHIPPING
	"Enable installation for Game Shipping"
	OFF
)
option(BITCRAFTE_INSTALL_GAME_DEVELOPMENT
	"Enable installation for Game Development"
	OFF
)
option(BITCRAFTE_INSTALL_ENGINE_DEVELOPMENT
	"Enable installation for Engine Development"
	OFF
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

if(${BITCRAFTE_BUILD_DOCUMENTATION})
	add_subdirectory("documentation")
endif()

if(${BITCRAFTE_BUILD_TESTS})
	add_subdirectory("tests")
endif()



################################################################
# Installation targets
################################################################
# These targets can be used to package the project automatically. It works by recursively calling CMake on this project.
# This allows for automatic build settings per configuration in all build system without using external tools.
# Generated files are put in 'build/install_targets/<configuration>' folder and installed into 'build/install/<configuration>'.
# This leaves the current build directory clean of installation related generated files, but does take up more disk space
# and requires recompilation of the entire project.

# If installation is already enabled, these targets will not be created, to prevent accidental infinite recursion.
if(NOT ${BITCRAFTE_INSTALL_GAME_SHIPPING} AND NOT ${BITCRAFTE_INSTALL_GAME_DEVELOPMENT} AND NOT ${BITCRAFTE_INSTALL_ENGINE_DEVELOPMENT})
	add_custom_target(InstallGameShipping
		COMMAND ${CMAKE_COMMAND} "-S${CMAKE_SOURCE_DIR}" "-B${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameShipping" -DBITCRAFTE_INSTALL_GAME_SHIPPING=ON -DCMAKE_BUILD_TYPE=GameShipping
		COMMAND ${CMAKE_COMMAND} --build "${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameShipping" --target install --config GameShipping
		COMMENT "Reconfiguring in '${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameShipping' and installing to '${CMAKE_SOURCE_DIR}/build/install/InstallGameShipping'"
	)
	set_target_properties(InstallGameShipping
		PROPERTIES
			FOLDER							"install"
	)

	add_custom_target(InstallGameDevelopment
		COMMAND ${CMAKE_COMMAND} "-S${CMAKE_SOURCE_DIR}" "-B${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameDevelopment" -DBITCRAFTE_INSTALL_GAME_DEVELOPMENT=ON -DCMAKE_BUILD_TYPE=GameDevelopment
		COMMAND ${CMAKE_COMMAND} --build "${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameDevelopment" --target install --config GameDevelopment
		COMMENT "Reconfiguring in '${CMAKE_SOURCE_DIR}/build/install_targets/InstallGameDevelopment' and installing to '${CMAKE_SOURCE_DIR}/build/install/InstallGameDevelopment'"
	)
	set_target_properties(InstallGameDevelopment
		PROPERTIES
			FOLDER							"install"
	)

	add_custom_target(InstallEngineDevelopment
		COMMAND ${CMAKE_COMMAND} "-S${CMAKE_SOURCE_DIR}" "-B${CMAKE_SOURCE_DIR}/build/install_targets/InstallEngineDevelopment" -DBITCRAFTE_INSTALL_ENGINE_DEVELOPMENT=ON -DCMAKE_BUILD_TYPE=EngineDevelopment
		COMMAND ${CMAKE_COMMAND} --build "${CMAKE_SOURCE_DIR}/build/install_targets/InstallEngineDevelopment" --target install --config EngineDevelopment
		COMMENT "Reconfiguring in '${CMAKE_SOURCE_DIR}/build/install_targets/InstallEngineDevelopment' and installing to '${CMAKE_SOURCE_DIR}/build/install/InstallEngineDevelopment'"
	)
	set_target_properties(InstallEngineDevelopment
		PROPERTIES
			FOLDER							"install"
	)
endif()
