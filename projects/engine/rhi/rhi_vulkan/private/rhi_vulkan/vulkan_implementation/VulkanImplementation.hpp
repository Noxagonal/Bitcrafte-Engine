#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/RHIComponentCreateInfo.hpp>



namespace bc {
namespace rhi {



class VulkanInstance;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanImplementation
{
public:
	struct ApplicationInfo
	{
		Text32													application_name					= {};
		utility::Version										application_version					= {};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanImplementation(
		const RHIComponentCreateInfo & create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanImplementation();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const VkAllocationCallbacks						*	GetMainThreadAllocationCallbacks() { return &main_thread_allocation_callbacks; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool													IsDebugEnabled() { return debug_enabled; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const ApplicationInfo							&	GetApplicationInfo() { return application_info; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkAllocationCallbacks										main_thread_allocation_callbacks;
	ApplicationInfo												application_info;
	bool														debug_enabled						= false;

	UniquePtr<VulkanInstance>									vulkan_instance;
};



} // rhi
} // bc