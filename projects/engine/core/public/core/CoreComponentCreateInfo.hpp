#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/logger/LoggerCreateInfo.hpp>
#include <core/thread/ThreadPoolCreateInfo.hpp>



namespace bc {



struct CoreComponentCreateInfo
{
	diagnostic::LoggerCreateInfo	logger_create_info;
	thread::ThreadPoolCreateInfo	thread_pool_create_info;
};



} // bc
