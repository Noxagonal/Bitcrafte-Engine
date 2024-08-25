
#include <core/PreCompiledHeader.hpp>
#include <core/CoreComponent.hpp>
#include <core/diagnostic/logger/Logger.hpp>
#include <core/thread/ThreadPool.hpp>

#include <locale>
#include <clocale>
#include <iostream>



namespace bc {
namespace internal_ {

void SetGlobalLocale()
{
	// Set the C global locale to UTF-8
	std::setlocale( LC_ALL, "en_US.UTF-8" );

	// Set the C++ global locale to UTF-8
	std::locale::global( std::locale( "en_US.UTF-8" ) );

	// Set the C++ output streams to use UTF-8
	std::cout.imbue( std::locale() );

	// Platform dependent, Windows uses utf-16 by default, Linux uses utf-32 by default.
	// std::wcout.imbue(std::locale());
};

} // namespace internal_
} // namespace bc



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent*	global_core			= nullptr;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent::CoreComponent( const bc::CoreComponentCreateInfo & create_info )
{
	BHardAssert( global_core == nullptr, "More than one core instance is not allowed" );
	global_core = this;

	internal_::SetGlobalLocale();

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
auto bc::CoreComponent::GetLogger() -> diagnostic::Logger*
{
	return logger.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::CoreComponent::GetThreadPool() -> thread::ThreadPool*
{
	return thread_pool.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::GetCore() -> bc::CoreComponent*
{
	return global_core;
}
