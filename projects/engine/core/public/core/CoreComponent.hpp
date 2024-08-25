#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/containers/UniquePtr.hpp>
#include "CoreComponentCreateInfo.hpp"



namespace bc {
namespace diagnostic { class Logger; }
namespace thread { class ThreadPool; }



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API CoreComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CoreComponent( const CoreComponentCreateInfo& create_info );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CoreComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetLogger() -> diagnostic::Logger*;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetThreadPool() -> thread::ThreadPool*;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<diagnostic::Logger>	logger;
	UniquePtr<thread::ThreadPool>	thread_pool;
};



CoreComponent												*	GetCore();



} // bc
