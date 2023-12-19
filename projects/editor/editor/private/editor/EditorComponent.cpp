
#include "PreCompiledHeader.hpp"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::editor::EditorComponent::EditorComponent()
{
	// TODO: All of this is temporary, create info and its settings should be thought out properly first.
	auto core_create_info = CoreComponentCreateInfo {};
	core_create_info.logger_create_info.report_severity			= bc::diagnostic::LogReportSeverity::VERBOSE;
	core_create_info.logger_create_info.display_severity		= bc::diagnostic::LogReportSeverity::VERBOSE;
	core_create_info.logger_create_info.print_to_system_console	= true;

	core = std::make_unique<CoreComponent>( core_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::editor::EditorComponent::~EditorComponent()
{
	core = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::editor::EditorComponent::Run()
{

}
