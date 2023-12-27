
#include <rhi_vulkan/PreCompiledHeader.hpp>
#include <rhi_vulkan/vk/queue/Queue.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::Queue::Submit(
	const bc::List<VkCommandBuffer>							&	command_buffers,
	const bc::List<Pair<VkSemaphore, VkPipelineStageFlags>>	&	wait_semaphores,
	const bc::List<VkSemaphore>								&	signal_semaphores,
	VkFence														fence
)
{
	List<VkSemaphore>			wait_sem( wait_semaphores.Size() );
	List<VkPipelineStageFlags>	wait_flags( wait_semaphores.Size() );
	for( size_t i = 0; i < wait_semaphores.Size(); ++i )
	{
		wait_sem[ i ]	= wait_semaphores[ i ].first;
		wait_flags[ i ]	= wait_semaphores[ i ].second;
	}

	VkSubmitInfo submit_info {};
	submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext					= nullptr;
	submit_info.waitSemaphoreCount		= uint32_t( wait_sem.Size() );
	submit_info.pWaitSemaphores			= wait_sem.Data();
	submit_info.pWaitDstStageMask		= wait_flags.Data();
	submit_info.commandBufferCount		= uint32_t( command_buffers.Size() );
	submit_info.pCommandBuffers			= command_buffers.Data();
	submit_info.signalSemaphoreCount	= uint32_t( signal_semaphores.Size() );
	submit_info.pSignalSemaphores		= signal_semaphores.Data();

	std::lock_guard<std::mutex> lock_guard( *queue_mutex );
	BAssertVkResult( vkQueueSubmit(
		queue,
		1,
		&submit_info,
		fence
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::Queue::Submit(
	const bc::List<SubmitInfo>		&	submit_infos,
	VkFence								fence )
{
	List<List<VkSemaphore>>				wait_sem( submit_infos.Size() );
	List<List<VkPipelineStageFlags>>	wait_flags( submit_infos.Size() );
	for( size_t s = 0; s < submit_infos.Size(); ++s )
	{
		wait_sem[ s ].Resize( submit_infos[ s ].wait_semaphores.Size() );
		wait_flags[ s ].Resize( submit_infos[ s ].wait_semaphores.Size() );
		for( size_t i = 0; i < wait_sem[ i ].Size(); ++i )
		{
			wait_sem[ s ][ i ]		= submit_infos[ s ].wait_semaphores[ i ].first;
			wait_flags[ s ][ i ]	= submit_infos[ s ].wait_semaphores[ i ].second;
		}
	}

	List<VkSubmitInfo> vk_submit_infos( submit_infos.Size() );
	for( size_t i = 0; i < submit_infos.Size(); ++i )
	{
		vk_submit_infos[ i ].sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vk_submit_infos[ i ].pNext					= nullptr;
		vk_submit_infos[ i ].waitSemaphoreCount		= uint32_t( wait_sem[ i ].Size() );
		vk_submit_infos[ i ].pWaitSemaphores		= wait_sem[ i ].Data();
		vk_submit_infos[ i ].pWaitDstStageMask		= wait_flags[ i ].Data();
		vk_submit_infos[ i ].commandBufferCount		= uint32_t( submit_infos[ i ].command_buffers.Size() );
		vk_submit_infos[ i ].pCommandBuffers		= submit_infos[ i ].command_buffers.Data();
		vk_submit_infos[ i ].signalSemaphoreCount	= uint32_t( submit_infos[ i ].signal_semaphores.Size() );
		vk_submit_infos[ i ].pSignalSemaphores		= submit_infos[ i ].signal_semaphores.Data();
	}

	std::lock_guard<std::mutex> lock_guard( *queue_mutex );
	BAssertVkResult( vkQueueSubmit(
		queue,
		uint32_t( vk_submit_infos.Size() ),
		vk_submit_infos.Data(),
		fence
	) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::rhi::Queue::Present(
	const bc::List<VkSemaphore>				&	wait_semaphores,
	const bc::List<SwapchainPresentInfo>		&	swapchains
)
{
	List<VkSwapchainKHR> vk_swapchains( swapchains.Size() );
	List<uint32_t> vk_swapchain_indices( swapchains.Size() );
	List<VkResult> vk_swapchain_results( swapchains.Size() );

	for( size_t i = 0; i < swapchains.Size(); ++i )
	{
		vk_swapchains[ i ]			= swapchains[ i ].swapchain;
		vk_swapchain_indices[ i ]	= swapchains[ i ].image_index;
		vk_swapchain_results[ i ]	= VK_SUCCESS;
	}

	VkPresentInfoKHR vk_present_info {};
	vk_present_info.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vk_present_info.pNext				= nullptr;
	vk_present_info.waitSemaphoreCount	= uint32_t( wait_semaphores.Size() );
	vk_present_info.pWaitSemaphores		= wait_semaphores.Data();
	vk_present_info.swapchainCount		= uint32_t( vk_swapchains.Size() );
	vk_present_info.pSwapchains			= vk_swapchains.Data();
	vk_present_info.pImageIndices		= vk_swapchain_indices.Data();
	vk_present_info.pResults			= vk_swapchain_results.Data();

	std::lock_guard<std::mutex> lock_guard( *queue_mutex );
	BAssertVkResult( vkQueuePresentKHR(
		queue,
		&vk_present_info
	) );

	for( auto swapchain_present_individual_result : vk_swapchain_results )
	{
		BAssertVkResult( swapchain_present_individual_result );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkQueue bc::rhi::Queue::GetQueue() const
{
	return queue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t bc::rhi::Queue::GetQueueFamilyIndex() const
{
	return queue_family_index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkBool32 bc::rhi::Queue::SupportsPresentation() const
{
	return supports_presentation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const VkQueueFamilyProperties & bc::rhi::Queue::GetQueueFamilyProperties() const
{
	return queue_family_properties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t bc::rhi::Queue::GetBasedOn() const
{
	return based_on;
}
