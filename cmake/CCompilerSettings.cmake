
if (NOT DEFINED BC_COMPILER_SETTINGS_INCLUDED)
	set(BC_COMPILER_SETTINGS_INCLUDED TRUE)


	# Allows C compiler to be used.
	macro(set_c_compiler_settings)
		set(CMAKE_C_COMPILE_OBJECT "clang <DEFINES> <INCLUDES> <FLAGS> -c <SOURCE> -o <OBJECT>")
		set(CMAKE_C_ARCHIVE_CREATE "ar rcs <TARGET> <LINK_FLAGS> <OBJECTS>")
		set(CMAKE_C_ARCHIVE_FINISH ":")
	endmacro()

	
endif()

