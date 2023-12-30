
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/buffer/VulkanBuffer.hpp>

#include <rhi_vulkan/rhi_vulkan_impl/RHIVulkanImpl.hpp>
#include <rhi_vulkan/rhi_memory_pool/RHIMemoryPool.hpp>
#include <rhi_vulkan/vk/device/VulkanDevice.hpp>
#include <rhi_vulkan/vk/physical_device/VulkanPhysicalDevice.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanBuffer::VulkanBuffer(
	RHIVulkanImpl					&	rhi_vulkan_impl,
	const VulkanBufferCreateInfo	&	create_info
) :
	rhi_vulkan_impl( rhi_vulkan_impl )
{
	auto IsSparse = [ &create_info ]()
		{
			return
				create_info.buffer_flags & VK_BUFFER_CREATE_SPARSE_BINDING_BIT ||
				create_info.buffer_flags & VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT ||
				create_info.buffer_flags & VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;
		};

	auto CreateBuffer = [ &create_info, &rhi_vulkan_impl ](
		VkDeviceSize	size
		)
		{
			auto buffer_create_info = VkBufferCreateInfo {};
			buffer_create_info.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffer_create_info.pNext					= nullptr;
			buffer_create_info.flags					= create_info.buffer_flags;
			buffer_create_info.size						= size;
			buffer_create_info.usage					= create_info.buffer_usage_flags;
			buffer_create_info.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
			buffer_create_info.queueFamilyIndexCount	= 0;
			buffer_create_info.pQueueFamilyIndices		= nullptr;

			auto result = VkBuffer {};
			BAssertVkResult( vkCreateBuffer(
				rhi_vulkan_impl.GetVulkanDevice(),
				&buffer_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );
			return result;
		};

	auto AllocateMemory = [ &rhi_vulkan_impl ](
		VkBuffer		buffer,
		bool			is_host_visible
		)
		{
			auto & memory_pool = rhi_vulkan_impl.GetRHIMemoryPool();
			auto result = memory_pool.AllocateBufferMemory(
				buffer,
				is_host_visible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);
			return result;
		};

	auto BindBufferMemory = [ &rhi_vulkan_impl ](
		VkBuffer				buffer,
		RHIPoolMemoryHandle	&	memory_handle
		)
		{
			auto bind_info = VkBindBufferMemoryInfo {};
			bind_info.sType			= VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
			bind_info.pNext			= nullptr;
			bind_info.buffer		= buffer;
			bind_info.memory		= memory_handle.GetVulkanMemory();
			bind_info.memoryOffset	= memory_handle.GetOffset();
			vkBindBufferMemory2(
				rhi_vulkan_impl.GetVulkanDevice(),
				1,
				&bind_info
			);
		};

	auto CreateBufferView = [ &create_info, &rhi_vulkan_impl ](
		VkBuffer			buffer,
		VkDeviceSize		offset,
		VkDeviceSize		size
		)
		{
			auto buffer_view_create_info = VkBufferViewCreateInfo {};
			buffer_view_create_info.sType		= VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
			buffer_view_create_info.pNext		= nullptr;
			buffer_view_create_info.flags		= 0;
			buffer_view_create_info.buffer		= buffer;
			buffer_view_create_info.format		= create_info.buffer_format;
			buffer_view_create_info.offset		= offset;
			buffer_view_create_info.range		= size;
			auto result = VkBufferView {};
			BAssertVkResult( vkCreateBufferView(
				rhi_vulkan_impl.GetVulkanDevice(),
				&buffer_view_create_info,
				rhi_vulkan_impl.GetMainThreadAllocationCallbacks(),
				&result
			) );
			return result;
		};

	format			= create_info.buffer_format;

	is_sparse		= IsSparse();
	size			= create_info.buffer_size;
	is_host_visible	= create_info.is_host_visible;

	vk_buffer = CreateBuffer( size );

	buffer_memory_handle = AllocateMemory(
		vk_buffer,
		is_host_visible
	);

	BindBufferMemory( vk_buffer, buffer_memory_handle );

	if( create_info.create_buffer_view )
	{
		vk_image_view = CreateBufferView( vk_buffer, 0, size );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::rhi::VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBufferView(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_image_view,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	vkDestroyBuffer(
		rhi_vulkan_impl.GetVulkanDevice(),
		vk_buffer,
		rhi_vulkan_impl.GetMainThreadAllocationCallbacks()
	);
	buffer_memory_handle = nullptr;
}
