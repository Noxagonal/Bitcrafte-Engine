
# Window manager Wayland: generated protocols

cmake_minimum_required(VERSION 3.22)

include("${CMAKE_SOURCE_DIR}/cmake/CCompilerSettings.cmake")

set_c_compiler_settings()

add_library(generated_wayland_protocols STATIC)

target_sources(generated_wayland_protocols
	PRIVATE
		xdg-shell-protocol.c
	INTERFACE
		xdg-shell-client-protocol.h
)

target_include_directories(generated_wayland_protocols
	INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}"
)

set_default_target_properties(generated_wayland_protocols)
