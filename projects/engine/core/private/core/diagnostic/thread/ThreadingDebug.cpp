
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/thread/ThreadingDebug.hpp>
#include <core/CoreComponent.hpp>
#include <core/diagnostic/logger/Logger.hpp>



#if BITCRAFTE_DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::thread::id main_thread;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::internal::SetMainThreadForDebug()
{
	main_thread = std::this_thread::get_id();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::AssertMainThread(
	diagnostic::SourceLocation source_location
)
{
	if( main_thread != std::this_thread::get_id() )
	{
		auto report = diagnostic::MakePrintRecord( U"Non-main thread entered main-thread-only scope" );
		report += diagnostic::MakePrintRecord( U"\n" );
		report += diagnostic::MakePrintRecord_SourceLocation( source_location ).AddIndent();
		GetCore()->GetLogger()->LogCriticalError( report );

		std::abort();
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::mutex										access_tracker_map_mutex;
bc::Map<bc::internal::SimpleText, uint32_t>		access_tracker_map;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::SingleThreadAccessScopeTracker::SingleThreadAccessScopeTracker(
	bc::diagnostic::SourceLocation source_location
)
{
	position_key.Reserve( 64 );
	position_key.Append( bc::internal::SimpleTextView( source_location.GetFile() ) );
	position_key.Append( bc::internal::SimpleTextView( source_location.GetFunction() ) );
	conversion::PrimitiveToText( position_key, source_location.GetLine() );

	std::lock_guard<std::mutex> lock_guard( access_tracker_map_mutex );
	if( access_tracker_map[ position_key ] )
	{
		if( thread_id != std::this_thread::get_id() )
		{
			auto report = diagnostic::MakePrintRecord( U"2 or more threads entered single thread only scope, only a single thread should enter this scope at a time\n" );
			report += diagnostic::MakePrintRecord_SourceLocation( source_location ).AddIndent();
			GetCore()->GetLogger()->LogCriticalError( report );

			std::abort();
		}
	}
	else
	{
		thread_id = std::this_thread::get_id();
	}
	++access_tracker_map[ position_key ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::SingleThreadAccessScopeTracker::~SingleThreadAccessScopeTracker()
{
	std::lock_guard<std::mutex> lock_guard( access_tracker_map_mutex );
	--access_tracker_map[ position_key ];
}

#endif
