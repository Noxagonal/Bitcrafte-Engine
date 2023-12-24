
#include <editor/PreCompiledHeader.hpp>
#include <core/diagnostic/logger/Logger.hpp>
#include <core/thread/ThreadPool.hpp>

#include <engine/EngineComponent.hpp>
#include <window/WindowComponent.hpp>
#include <window/window/Window.hpp>

#include <iostream>



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

	auto engine_create_info = bc::engine::EngineComponentCreateInfo {};
	engine = MakeUniquePtr<bc::engine::EngineComponent>( engine_create_info );

	auto window_create_info = bc::window::WindowCreateInfo {};
	main_window = engine->GetWindowComponent()->CreateWindow( window_create_info );

	main_window->events.CloseRequested.RegisterCallback( [ this ]()
		{
			should_close = true;
		}
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::editor::EditorComponent::~EditorComponent()
{
	main_window = nullptr;
	core = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::editor::EditorComponent::Run()
{
	try
	{
		while( !should_close )
		{
			core->Run();
			engine->Run();

			std::this_thread::sleep_for( std::chrono::milliseconds( 15 ) );
		}
	}
	catch( const bc::diagnostic::Exception & exception )
	{
		core->GetLogger()->Log( bc::diagnostic::LogReportSeverity::CRITICAL_ERROR, exception );
		return;
	}
}
