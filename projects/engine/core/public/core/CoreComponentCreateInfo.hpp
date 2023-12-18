#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/logger/LoggerCreateInfo.hpp>



namespace bc {



struct CoreComponentCreateInfo
{
	diagnostic::LoggerCreateInfo					logger_create_info;
};



} // bc
