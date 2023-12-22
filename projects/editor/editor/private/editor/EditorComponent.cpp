
#include "PreCompiledHeader.hpp"
#include <core/diagnostic/logger/Logger.hpp>
#include <core/thread/ThreadPool.hpp>



class StandardEditorThread : public bc::thread::Thread
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void					ThreadBegin() override
	{

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void					ThreadEnd() noexcept override
	{

	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::editor::EditorComponent::EditorComponent()
{
	// TODO: All of this is temporary, create info and its settings should be thought out properly first.
	auto core_create_info = CoreComponentCreateInfo {};
	core_create_info.logger_create_info.minimum_report_severity			= bc::diagnostic::LogReportSeverity::VERBOSE;
	core_create_info.logger_create_info.minimum_display_severity		= bc::diagnostic::LogReportSeverity::VERBOSE;
	core_create_info.logger_create_info.print_to_system_console	= true;

	core = std::make_unique<CoreComponent>( core_create_info );
	auto thread_pool = core->GetThreadPool();
	thread_pool->AddThread<StandardEditorThread>();
	thread_pool->AddThread<StandardEditorThread>();
	thread_pool->AddThread<StandardEditorThread>();
	thread_pool->AddThread<StandardEditorThread>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::editor::EditorComponent::~EditorComponent()
{
	core = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::editor::EditorComponent::Run()
{
	try
	{
		while( true )
		{
			core->Run();
		}
	}
	catch( const bc::diagnostic::Exception & exception )
	{
		core->GetLogger()->Log( bc::diagnostic::LogReportSeverity::CRITICAL_ERROR, exception );
		return;
	}
}
