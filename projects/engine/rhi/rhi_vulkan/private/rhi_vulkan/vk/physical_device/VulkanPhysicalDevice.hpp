#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>

#include <vulkan/vulkan.h>



namespace bc {
namespace rhi {



class RHIVulkanImpl;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VulkanPhysicalDevice
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct QueueFamilyInfo
	{
		List<VkQueueFamilyProperties2>						queue_family_properties;
		List<bool>											can_present;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice(
		RHIVulkanImpl									&	rhi_vulkan_impl,
		VkPhysicalDevice									vk_physical_device
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice(
		const VulkanPhysicalDevice						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice(
		VulkanPhysicalDevice							&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~VulkanPhysicalDevice();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice								&	operator=(
		const VulkanPhysicalDevice						&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanPhysicalDevice								&	operator=(
		VulkanPhysicalDevice							&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const VkPhysicalDeviceMemoryProperties2		&	GetMemoryProperties() const { return vk_memory_properties; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const VkPhysicalDeviceProperties2			&	GetProperties() const { return vk_properties; }
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const VkPhysicalDeviceFeatures2				&	GetFeatures() const { return vk_features; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline const QueueFamilyInfo						&	GetQueueFamilyProperties() const { return queue_family_properties; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline operator VkPhysicalDevice() const { return vk_physical_device; }

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<bool>												GetPhysicalDeviceQueuePresentationSupport(
		ListView<VkQueueFamilyProperties2>					queue_family_properties
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VkPhysicalDevice										vk_physical_device					= VK_NULL_HANDLE;

	VkPhysicalDeviceMemoryProperties2						vk_memory_properties;
	VkPhysicalDeviceProperties2								vk_properties;
	VkPhysicalDeviceFeatures2								vk_features;
	QueueFamilyInfo											queue_family_properties;

	RHIVulkanImpl										*	rhi_vulkan_impl;
};



} // rhi
} // bc
