#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BAssertVkResult( m_result ) BAssert( ( m_result ) == VK_SUCCESS, bc::diagnostic::MakePrintRecord_Argument( U"Vulkan result assertion failed", string_VkResult( m_result ) ) )



} // rhi
} // bc
