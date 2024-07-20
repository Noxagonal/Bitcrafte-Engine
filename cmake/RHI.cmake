
if(NOT DEFINED BITCRAFTE_RHI_INCLUDED)
	set(BITCRAFTE_RHI_INCLUDED TRUE)


	# Get a list of supported RHIs on the current platform.
	function(get_supported_rhi_list
		OUT_LIST
	)
		set(RHI_LIST)
		if(WIN32)
			list(APPEND RHI_LIST "vulkan")
		elseif(UNIX AND NOT APPLE)
			list(APPEND RHI_LIST "vulkan")
		elseif(APPLE)
			list(APPEND RHI_LIST "metal") # Consider using MoltenVK instead to save on rewriting code.
		else()
			message(FATAL_ERROR "Please add platform rhi component here")
		endif()

		set(${OUT_LIST} ${RHI_LIST} PARENT_SCOPE)
	endfunction()


	# Get RHI component dependencies which can be used to add a library dependency to a target.
	function(get_rhi_component_dependencies
		OUT_LIST
	)
		set(RHI_LIST ${ARGN})
		set(DEPENDENCY_LIST)
		foreach(RHI IN LISTS RHI_LIST)
			if(${RHI} STREQUAL "d3d12")
				list(APPEND DEPENDENCY_LIST rhi_d3d12)
			endif()
			if(${RHI} STREQUAL "vulkan")
				list(APPEND DEPENDENCY_LIST rhi_vulkan)
			endif()
			if(${RHI} STREQUAL "metal")
				list(APPEND DEPENDENCY_LIST rhi_metal)
			endif()
		endforeach()
		set(${OUT_LIST} ${DEPENDENCY_LIST} PARENT_SCOPE)
	endfunction()


	# Set RHI compile definitions for a specific target.
	function(set_target_rhi_component_defines
		THIS_TARGET_NAME
	)
		set(RHI_LIST ${ARGN})
		foreach(RHI IN LISTS RHI_LIST)
			if(${RHI} STREQUAL "d3d12")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_RHI_D3D12=1
				)
			endif()
			if(${RHI} STREQUAL "vulkan")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_RHI_VULKAN=1
				)
			endif()
			if(${RHI} STREQUAL "metal")
				target_compile_definitions(${THIS_TARGET_NAME}
					PRIVATE
						BITCRAFTE_RHI_METAL=1
				)
			endif()
		endforeach()
	endfunction()


endif()
