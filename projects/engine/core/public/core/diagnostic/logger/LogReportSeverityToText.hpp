#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include "LogReportSeverity.hpp"
#include <core/containers/Text.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
TextView32					LogReportSeverityToText(
	LogReportSeverity		e
);



} // diagnostic
} // bc
