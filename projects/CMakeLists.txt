
cmake_minimum_required(VERSION 3.22)



if(BITCRAFTE_BUILD_ENGINE)
	add_subdirectory("engine")
endif()

if(BITCRAFTE_BUILD_EDITOR AND BITCRAFTE_BUILD_ENGINE)
	add_subdirectory("editor")
endif()
