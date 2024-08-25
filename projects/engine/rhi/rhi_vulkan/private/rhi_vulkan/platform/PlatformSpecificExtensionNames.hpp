
#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>



namespace bc {
namespace rhi {

class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto GetPlatformSpecificInstanceExtensionNames( RHIVulkanImpl& rhi_vulkan_impl ) -> List<Text>;



} // rhi
} // bc
