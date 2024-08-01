
# Documentation generator

# All required dependencies to compile documentation:
# - Doxygen

cmake_minimum_required(VERSION 3.22)



################################################################
# Doxygen
################################################################
find_package(Doxygen REQUIRED)



################################################################
# Settings and files that will be placed in Doxyfile
################################################################
set(DOXYFILE_STRIP_FROM_PATH
	${PROJECT_SOURCE_DIR}
)
set(DOXYFILE_SOURCE_DIR
	"${PROJECT_SOURCE_DIR}/projects/"
)
set(DOXYFILE_PAGES_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/pages/"
)



################################################################
# General settings
################################################################
set(DOXYGEN_OUTPUT_DIR
	"${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen"
)
set(DOXYGEN_INDEX_FILE
	"${DOXYGEN_OUTPUT_DIR}/xml/index.xml"
)
set(DOXYFILE_IN
	"${CMAKE_CURRENT_SOURCE_DIR}/doxygen/bitcrafte.Doxyfile.in"
)
set(DOXYFILE_OUT
	"${CMAKE_CURRENT_BINARY_DIR}/doxygen/bitcrafte.Doxyfile"
)

################################################################
# Create doxygen project
################################################################
#Replace variables inside @@ with the current values
configure_file(
	${DOXYFILE_IN}
	${DOXYFILE_OUT}
	@ONLY
)

file(MAKE_DIRECTORY
	${DOXYGEN_OUTPUT_DIR}
)

file(GLOB_RECURSE DOXYFILE_PAGES
	CONFIGURE_DEPENDS
	"${DOXYFILE_PAGES_DIR}/*.md"
)
add_custom_target(documentation)
add_custom_command(
	TARGET
		documentation
	PRE_BUILD
	DEPENDS
		${FILES_SOURCE}
		${DOXYFILE_PAGES}
	COMMAND
		${DOXYGEN_EXECUTABLE}
		${DOXYFILE_OUT}
	MAIN_DEPENDENCY
		${DOXYFILE_OUT}
		${DOXYFILE_IN}
	COMMENT
		"Generating Doxygen Documentation"
	VERBATIM
)



################################################################
# Documentation install
################################################################
install(
	DIRECTORY
		"${DOXYGEN_OUTPUT_DIR}/html/"
	DESTINATION
		"install/documentation"
)
