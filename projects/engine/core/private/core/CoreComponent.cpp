
#include <core/PreCompiledHeader.hpp>
#include <core/CoreComponent.hpp>
#include <core/diagnostic/logger/Logger.hpp>
#include <core/thread/ThreadPool.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent		*	global_core			= nullptr;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent::CoreComponent(
	const bc::CoreComponentCreateInfo & create_info
)
{
	BHardAssert( global_core == nullptr, "More than one core instance is not allowed" );
	global_core = this;

	// TODO: Start memory pool.

	logger				= MakeUniquePtr<diagnostic::Logger>( create_info.logger_create_info );
	thread_pool			= MakeUniquePtr<thread::ThreadPool>( create_info.thread_pool_create_info );

	logger->LogVerbose( "Core component started" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent::~CoreComponent()
{
	BHardAssert( global_core == this, "Global core singleton changed in running application" );

	// Cleanup code here...

	thread_pool			= nullptr;
	logger				= nullptr;

	global_core			= nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::CoreComponent::Run()
{
	thread_pool->Run();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Logger * bc::CoreComponent::GetLogger()
{
	return logger.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::thread::ThreadPool * bc::CoreComponent::GetThreadPool()
{
	return thread_pool.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent * bc::GetCore()
{
	return global_core;
}
