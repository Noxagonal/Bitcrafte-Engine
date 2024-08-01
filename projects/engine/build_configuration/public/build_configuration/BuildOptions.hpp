#pragma once

#include <stdint.h>



////////////////////////////////////////////////////////////////
// ENGINE
////////////////////////////////////////////////////////////////

// TODO: These need to be put into somewhere else.
#define BITCRAFTE_ENGINE_NAME														"Bitcrafte Engine"
#define BITCRAFTE_ENGINE_VERSION_MAJOR												0
#define BITCRAFTE_ENGINE_VERSION_MINOR												1
#define BITCRAFTE_ENGINE_VERSION_PATCH												0



////////////////////////////////////////////////////////////////
// VULKAN
////////////////////////////////////////////////////////////////

#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION									1
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_SEVERITY_VERBOSE					0
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_SEVERITY_INFO						0
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_SEVERITY_WARNING					1
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_SEVERITY_ERROR						1

#define BITCRAFTE_BUILD_OPTION_VULKAN_GPU_ASSISTED_VALIDATION						1

#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_API_DUMP							0
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_API_DUMP_FILE_PATH					"vulkan_api_dump"
#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_API_DUMP_FILE_EXTENSION			"txt"

#define BITCRAFTE_BUILD_OPTION_VULKAN_VALIDATION_BEST_PRACTICES						0

// Size in bytes of a single allocation from the GPU or host, linear = buffers, linear images, all host resources.
// Non-linear resources are optimal images stored inside the GPU in a way what is fastest for GPU to access.
#define BITCRAFTE_VULKAN_MEMORY_POOL_SINGLE_LINEAR_ALLOCATION_SIZE					( 1024ll * 1024 * 64 )
#define BITCRAFTE_VULKAN_MEMORY_POOL_SINGLE_NON_LINEAR_ALLOCATION_SIZE				( 1024ll * 1024 * 256 )

#define BITCRAFTE_VULKAN_DESCRIPTOR_AUTO_POOL_CATEGORY_MINIMUM_COMPATIBILITY		( 0.85 )
#define BITCRAFTE_VULKAN_DESCRIPTOR_AUTO_POOL_ALLOCATION_BATCH_SIZE					( 256 )

// NVidia only.
#define BITCRAFTET_BUILD_OPTION_VULKAN_COMMAND_BUFFER_CHECKMARKS					1



////////////////////////////////////////////////////////////////
// EXPERIMENTAL
////////////////////////////////////////////////////////////////

#define BITCRAFTE_USE_VIRTUAL_TERMINAL_FOR_CONSOLE_PRINTING							0
