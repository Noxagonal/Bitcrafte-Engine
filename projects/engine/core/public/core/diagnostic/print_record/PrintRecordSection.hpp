#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/print_record/PrintRecordTheme.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PrintRecordSection
{
	PrintRecordTheme						theme					= PrintRecordTheme::DEFAULT;
	SimpleText32							text;
	size_t									indent					= {};
};



} // diagnostic
} // bc
