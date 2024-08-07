
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/instance/VulkanInstance.hpp>
#include <core/CoreComponent.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/platform/PlatformHeadersInclude.hpp>
#undef ERROR
#include <rhi_vulkan/platform/PlatformSpecificExtensionNames.hpp>

#include <core/application/ApplicationInfo.hpp>
#include <core/diagnostic/logger/Logger.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkBool32 VKAPI_PTR VulkanDebugUtilsMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT			messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT					messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT*		pCallbackData,
	void*											pUserData
)
{
	auto rhi_vulkan_impl = reinterpret_cast<bc::rhi::RHIVulkanImpl*>( pUserData );
	assert( rhi_vulkan_impl );

	auto report_severity	= bc::diagnostic::LogReportSeverity::NONE;
	switch( messageSeverity )
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		report_severity		= bc::diagnostic::LogReportSeverity::VERBOSE;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		report_severity		= bc::diagnostic::LogReportSeverity::INFO;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		report_severity		= bc::diagnostic::LogReportSeverity::WARNING;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		report_severity		= bc::diagnostic::LogReportSeverity::ERROR;
		break;
	default:
		report_severity		= bc::diagnostic::LogReportSeverity::INFO;
		assert( 0 );
		break;
	}

	bc::Text32 message_type;
	switch( messageTypes )
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		message_type = U"GENERAL";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		message_type = U"VALIDATION";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		message_type = U"PERFORMANCE";
		break;
	default:
		message_type = U"<UNKNOWN>";
		assert( 0 );
		break;
	}

	auto print_report = bc::diagnostic::MakePrintRecord_Argument( U"Vulkan debug message", message_type );
	print_report += bc::diagnostic::MakePrintRecord( U"\n" );
	print_report += bc::diagnostic::MakePrintRecord( bc::conversion::ToUTF32( bc::TextView( pCallbackData->pMessage ) ) ).AddIndent();
	bc::GetCore()->GetLogger()->Log(
		report_severity,
		print_report
	);

	return VK_FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanInstance::VulkanInstance(
	RHIVulkanImpl & rhi_vulkan_impl
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	enabled_extension_names.PushBack( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME );
	enabled_extension_names.PushBack( VK_KHR_SURFACE_EXTENSION_NAME );
	enabled_extension_names += GetPlatformSpecificInstanceExtensionNames( rhi_vulkan_impl );

	// Vulkan validation
	if( rhi_vulkan_impl.GetDebugSettings().debug_enabled )
	{
		enabled_layer_names.PushBack( "VK_LAYER_KHRONOS_validation" );
		enabled_extension_names.PushBack( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
	}

	// Setup debug utils messenger
	auto minimum_debug_level_value = std::to_underlying( rhi_vulkan_impl.GetDebugSettings().minimum_debug_level );
	auto debug_utils_message_severity = VkDebugUtilsMessageSeverityFlagsEXT {};
	debug_utils_message_severity |= std::to_underlying( RHIDebugLevel::VERBOSE )	>= minimum_debug_level_value ? VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT : 0;
	debug_utils_message_severity |= std::to_underlying( RHIDebugLevel::INFO )		>= minimum_debug_level_value ? VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT : 0;
	debug_utils_message_severity |= std::to_underlying( RHIDebugLevel::WARNING )	>= minimum_debug_level_value ? VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT : 0;
	debug_utils_message_severity |= std::to_underlying( RHIDebugLevel::ERROR )		>= minimum_debug_level_value ? VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT : 0;

	// TODO: Give an option to adjust these somewhere.
	auto debug_utils_message_type = VkDebugUtilsMessageTypeFlagsEXT {};
	debug_utils_message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
	debug_utils_message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debug_utils_message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_utils_message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;

	auto debug_utils_messenger_create_info = VkDebugUtilsMessengerCreateInfoEXT {};
	debug_utils_messenger_create_info.sType				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_utils_messenger_create_info.pNext				= nullptr;
	debug_utils_messenger_create_info.flags				= 0;
	debug_utils_messenger_create_info.messageSeverity	= debug_utils_message_severity;
	debug_utils_messenger_create_info.messageType		= debug_utils_message_type;
	debug_utils_messenger_create_info.pfnUserCallback	= VulkanDebugUtilsMessengerCallback;
	debug_utils_messenger_create_info.pUserData			= &rhi_vulkan_impl;



	// Validation features
	List<VkValidationFeatureEnableEXT> enabled_validation_features {
		VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
		VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
		VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
	};
	List<VkValidationFeatureDisableEXT> disabled_validation_features {
	};

	auto validation_features = VkValidationFeaturesEXT {};
	validation_features.sType							= VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	validation_features.pNext							= &debug_utils_messenger_create_info;
	validation_features.enabledValidationFeatureCount	= u32( enabled_validation_features.Size() );
	validation_features.pEnabledValidationFeatures		= enabled_validation_features.Data();
	validation_features.disabledValidationFeatureCount	= u32( disabled_validation_features.Size() );
	validation_features.pDisabledValidationFeatures		= disabled_validation_features.Data();



	// Set layer and extension names to a struct we can give to VkInstanceCreateInfo struct directly
	auto enabled_layer_names_c_str = List<const char*> {};
	auto enabled_extension_names_c_str = List<const char*> {};
	for( auto & n : enabled_layer_names )
	{
		enabled_layer_names_c_str.PushBack( n.ToCStr() );
	}
	for( auto & n : enabled_extension_names )
	{
		enabled_extension_names_c_str.PushBack( n.ToCStr() );
	}



	// Set up Vulkan instance
	Text engine_name = application::EngineName;
	Text8 application_name = bc::conversion::ToUTF8( rhi_vulkan_impl.GetApplicationInfo().application_name);
	auto application_info = VkApplicationInfo {};
	application_info.sType					= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pNext					= nullptr;
	application_info.pApplicationName		= reinterpret_cast<const char*>( application_name.ToCStr() );
	application_info.applicationVersion		= rhi_vulkan_impl.GetApplicationInfo().application_version.ToVulkanPacked();
	application_info.pEngineName			= engine_name.ToCStr();
	application_info.engineVersion			= application::EngineVersion.ToVulkanPacked();
	application_info.apiVersion				= VK_API_VERSION_1_3;
	
	auto create_info = VkInstanceCreateInfo {};
	create_info.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pNext						= rhi_vulkan_impl.GetDebugSettings().debug_enabled ? &validation_features : nullptr;
	create_info.flags						= 0;
	create_info.pApplicationInfo			= &application_info;
	create_info.enabledLayerCount			= u32( enabled_layer_names_c_str.Size() );
	create_info.ppEnabledLayerNames			= enabled_layer_names_c_str.Data();
	create_info.enabledExtensionCount		= u32( enabled_extension_names_c_str.Size() );
	create_info.ppEnabledExtensionNames		= enabled_extension_names_c_str.Data();
	BAssertVkResult( vkCreateInstance(
		&create_info,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
		&vk_instance
	) );

	GetCore()->GetLogger()->LogVerbose( U"Vulkan instance created" );

	// Vulkan validation
	if( rhi_vulkan_impl.GetDebugSettings().debug_enabled )
	{
		vkCreateDebugUtilsMessengerEXT_func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr( vk_instance, "vkCreateDebugUtilsMessengerEXT" );
		assert( vkCreateDebugUtilsMessengerEXT_func );

		vkDestroyDebugUtilsMessengerEXT_func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( vk_instance, "vkDestroyDebugUtilsMessengerEXT" );
		assert( vkDestroyDebugUtilsMessengerEXT_func );

		BAssertVkResult( vkCreateDebugUtilsMessengerEXT_func(
			vk_instance,
			&debug_utils_messenger_create_info,
			rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
			&debug_utils_messenger
		) );
	}

	physical_devices = FetchPhysicalDevices();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanInstance::~VulkanInstance()
{
	if( debug_utils_messenger )
	{
		vkDestroyDebugUtilsMessengerEXT_func(
			vk_instance,
			debug_utils_messenger,
			rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
		);
		debug_utils_messenger = VK_NULL_HANDLE;
	}

	vkDestroyInstance(
		vk_instance,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::List<bc::rhi::VulkanPhysicalDevice> bc::rhi::VulkanInstance::FetchPhysicalDevices() const
{
	auto physical_device_count = u32 {};
	BAssertVkResult( vkEnumeratePhysicalDevices( vk_instance, &physical_device_count, nullptr ) );
	auto vk_physical_devices = List<VkPhysicalDevice>( physical_device_count );
	BAssertVkResult( vkEnumeratePhysicalDevices( vk_instance, &physical_device_count, vk_physical_devices.Data() ) );

	auto result = List<VulkanPhysicalDevice> {};
	result.Reserve( vk_physical_devices.Size() );
	for( u64 i = 0; i < vk_physical_devices.Size(); i++ )
	{
		result.EmplaceBack( rhi_vulkan_impl, vk_physical_devices[ i ] );
	}
	return result;
}
