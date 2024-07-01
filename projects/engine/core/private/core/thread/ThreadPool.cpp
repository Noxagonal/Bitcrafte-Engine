
#include <core/PreCompiledHeader.hpp>

#include <core/thread/ThreadPool.hpp>
#include <core/thread/ThreadSharedData.hpp>
#include <core/thread/Thread.hpp>
#include <core/thread/ThreadDescription.hpp>
#include <core/thread/Task.hpp>

#include <core/CoreComponent.hpp>
#include <core/diagnostic/logger/Logger.hpp>

#include <core/diagnostic/print_record/PrintRecordFactory.hpp>

#include <core/diagnostic/system_console/SystemConsole.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ThreadPoolWorker(
	bc::thread::ThreadDescription	*	thread_description,
	bc::thread::ThreadSharedData	*	thread_shared_data
)
{
	using namespace bc::thread;

	assert( thread_description );
	assert( thread_shared_data );

	auto CleanUpThreadForTermination = [ thread_description ](
		WorkerThreadState new_thread_state
		)
		{
			thread_description->state			= new_thread_state;
			thread_description->pool_thread->ThreadEnd();
			thread_description->ready_to_join	= true;
		};

	auto ReportException = [ thread_shared_data, thread_description ]( const bc::diagnostic::Exception & exception )
		{
			// If a thread task raises an exception, we exit the application. It won't happen
			// immediately however and other threads might run parallel to this thread and need
			// to be cleaned up, best we can do here is to signal the exception.
			//
			// Cleanup is done by the main thread whenever the main thread tries to add a new
			// task or do a general update. It may take a little while however. Once the main
			// thread notices that there was an exception, EvacuateThreads() is called which
			// will join all threads and manually clears the task list.

			auto lock_guard = std::lock_guard( thread_shared_data->thread_exception_mutex );

			if( thread_shared_data->thread_exception_raised ) return;
			thread_shared_data->thread_exception_raised		= true;

			thread_shared_data->thread_exception			= exception;
			thread_shared_data->thread_exception_id			= thread_description->thread_id;
			thread_shared_data->threads_should_exit			= true;
			thread_shared_data->thread_wakeup.notify_all();
		};

	auto MakeExceptionFromStlException = []( const std::exception & exception )
		{
			auto report = bc::diagnostic::MakePrintRecord( U"stl exception thrown in thread" );
			report += bc::diagnostic::MakePrintRecord( U"\n" );
			report += bc::diagnostic::MakePrintRecord_Argument( U"stl exception message", exception.what() ).AddIndent();
			return bc::diagnostic::MakeException( report );
		};

	auto thread_start_result = [ ReportException, MakeExceptionFromStlException, thread_description ]() -> bool
		{
			try
			{
				thread_description->pool_thread->ThreadBegin();
			}
			catch( const bc::diagnostic::Exception & e )
			{
				ReportException( e );
				return false;
			}
			catch( const std::exception & e )
			{
				ReportException( MakeExceptionFromStlException( e ) );
				return false;
			}
			catch( ... )
			{
				ReportException( bc::diagnostic::MakeException( "Unknown exception thrown in thread" ) );
				return false;
			}
			return true;
		}();
	if( !thread_start_result )
	{
		CleanUpThreadForTermination( WorkerThreadState::INITIALIZATION_ERROR );
		return;
	}

	thread_description->state				= WorkerThreadState::RUNNING;

	while( !thread_shared_data->threads_should_exit && !thread_description->should_exit )
	{
		thread_description->state = WorkerThreadState::RUNNING;

		bool found_work			= false;
		if( auto task			= thread_shared_data->FindWork( *thread_description ) )
		{
			// There's more work to be done, let the other threads know about it too.
			thread_shared_data->thread_wakeup.notify_all();

			auto task_execution_result = TaskExecutionResult::ERROR;

			try
			{
				task_execution_result = task->ThreadRun( *thread_description->pool_thread );
			}
			catch( const bc::diagnostic::Exception & e )
			{
				// Breaking makes sure we finish this thread here, this makes sure TaskComplete()
				// below is not called. If TaskComplete() would be called, the task is removed from
				// the task list and tasks that might depend on this task may run, we need to make
				// sure all depending tasks do have an opportunity to run before exiting the
				// application.

				ReportException( e );
				break;
			}
			catch( const std::exception & e )
			{
				ReportException( MakeExceptionFromStlException( e ) );
				break;
			}
			catch( ... )
			{
				ReportException( bc::diagnostic::MakeException( "Unknown exception thrown in thread" ) );
				break;
			}

			switch( task_execution_result )
			{
			case bc::thread::TaskExecutionResult::PAUSED:
				// Push the task to the bottom of the list.
				thread_shared_data->RescheduleTask( task );
				break;

			case bc::thread::TaskExecutionResult::FINISHED:
				thread_shared_data->TaskCompleted( task );
				break;

			case bc::thread::TaskExecutionResult::ERROR:
				bc::GetCore()->GetLogger()->LogWarning(
					bc::diagnostic::MakePrintRecord_AssertText(
						U"Thread task failed",
						U"Task id", task->GetTaskId(),
						U"Thread id", thread_description->thread_id
					)
				);
				thread_shared_data->TaskCompleted( task );
				break;
			}

			found_work = true;
		}

		// If we found work previously there might be more waiting, check until we run out of tasks.
		if( !found_work )
		{
			std::unique_lock<std::mutex> unique_lock( thread_shared_data->thread_wakeup_mutex );
			thread_description->state = WorkerThreadState::IDLE;
			thread_shared_data->thread_wakeup.wait_for( unique_lock, std::chrono::milliseconds( 10 ) );	// periodically wake up and check for work
		}
	}

	CleanUpThreadForTermination( WorkerThreadState::IDLE );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::thread::ThreadPool::ThreadPool(
	const bc::thread::ThreadPoolCreateInfo & create_info
)
{
	main_thread_id = std::this_thread::get_id();

	thread_shared_data = MakeUniquePtr<ThreadSharedData>();
	thread_description_list.Reserve( 32 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::thread::ThreadPool::~ThreadPool()
{
	shutting_down = true;

	auto CheckAndReportThreadException = [ this ]()
		{
			auto lock_guard = std::lock_guard( thread_shared_data->thread_exception_mutex );

			if( thread_shared_data->thread_exception_raised && !thread_shared_data->thread_exception_handled )
			{
				EvacuateThreads();
				if( !thread_shared_data->thread_exception.IsEmpty() )
				{
					GetCore()->GetLogger()->Log( diagnostic::LogReportSeverity::CRITICAL_ERROR, thread_shared_data->thread_exception );
				}
				thread_shared_data->thread_exception_handled = true;
			}
		};

	CheckAndReportThreadException();

	// Wait for all the work to be done.
	while( !thread_shared_data->IsTaskListEmpty() )
	{
		thread_shared_data->thread_wakeup.notify_all();
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		CheckAndReportThreadException();
	}
	assert( thread_shared_data->IsTaskListEmpty() );

	CheckAndReportThreadException();

	if( !thread_shared_data->thread_exception_raised )
	{
		// Signal all threads to exit.
		thread_shared_data->threads_should_exit	= true;

		// Wait for all threads to be ready for joining.
		while( !std::all_of( thread_description_list.begin(), thread_description_list.end(), []( auto & thread_description ) -> bool
			{
				return thread_description->ready_to_join.load();
			} ) )
		{

			// Keep signaling all threads until they're ready to join.
			thread_shared_data->thread_wakeup.notify_all();
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}

	// All threads ready to join
	for( auto & t : thread_description_list )
	{
		t->stl_thread.join();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::RemoveThread(
	ThreadIdentifier thread_id
)
{
	DoRemoveThread( thread_id );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::u64 bc::thread::ThreadPool::GetTaskQueueCount() const
{
	auto lock_guard = std::lock_guard( thread_shared_data->task_list_mutex );

	return thread_shared_data->task_list.Size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::u64 bc::thread::ThreadPool::GetTaskRunningCount() const
{
	auto lock_guard = std::lock_guard( thread_shared_data->task_list_mutex );

	auto result = u64 {};
	for( auto & t : thread_shared_data->task_list )
	{
		if( t->state == TaskState::RUNNING ) ++result;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::thread::id bc::thread::ThreadPool::GetThreadSystemID(
	ThreadIdentifier thread_id
) const
{
	auto thread_description = std::find_if( thread_description_list.begin(), thread_description_list.end(), [ thread_id ]( auto & t ) { return t->thread_id == thread_id; } );
	if( thread_description == thread_description_list.end() ) return {};
	return ( *thread_description )->stl_thread.get_id();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::Run()
{
	thread_shared_data->thread_wakeup.notify_all();

	if( thread_shared_data->thread_exception_raised )
	{
		CheckAndHandleThreadThrow();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::WaitIdle()
{
	while( !thread_shared_data->IsTaskListEmpty() )
	{
		thread_shared_data->thread_wakeup.notify_all();

		if( thread_shared_data->thread_exception_raised )
		{
			CheckAndHandleThreadThrow();
		}

		// TODO: Instead of waiting for a time, wait for a signal from any of the worker threads to finish execution.
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
	assert( thread_shared_data->IsTaskListEmpty() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::u64 bc::thread::ThreadPool::DoAddTask(
	UniquePtr<Task>	&&	new_task
)
{
	BHardAssert( !shutting_down, "Failed to schedule task, trying to add tasks while shutting down the thread pool" );
	if( thread_shared_data->thread_exception_raised )
	{
		return 0;
	}
	CheckAndHandleThreadThrow();

	auto task_id = new_task->task_id = ++task_id_counter;
	thread_shared_data->AddTask( std::move( new_task ) );
	thread_shared_data->thread_wakeup.notify_one();

	return task_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::thread::ThreadIdentifier bc::thread::ThreadPool::DoAddThread(
	UniquePtr<ThreadDescription> && thread_description
)
{
	BHardAssert( !shutting_down, "Cannot add thread, trying to add threads while shutting down the thread pool" );
	BHardAssert( GetTaskQueueCount() == 0, "Cannot remove thread, threads cannot be removed when there are any tasks queued" );
	BHardAssert( std::this_thread::get_id() == main_thread_id, "Cannot add thread, threads can only be added by the main thread" );

	thread_description->state			= WorkerThreadState::UNINITIALIZED;
	thread_description->ready_to_join	= false;
	thread_description->thread_id		= ++thread_id_counter;
	thread_description->stl_thread		= std::thread(
		ThreadPoolWorker,
		thread_description.Get(),
		thread_shared_data.Get()
	);

	auto thread_description_ptr = thread_description.Get();
	thread_description_list.PushBack( std::move( thread_description ) );

	// wait for the thread to start.
	while( thread_description_ptr->state == WorkerThreadState::UNINITIALIZED )
	{
		std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );
	}
	if( thread_description_ptr->state == WorkerThreadState::INITIALIZATION_ERROR )
	{
		auto exception = diagnostic::MakeException( "Failed to start thread pool thread" );
		exception.SetNextException( thread_shared_data->thread_exception );

		EvacuateThreads();
		diagnostic::Throw( exception );
	}

	return thread_description_ptr->thread_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::DoRemoveThread(
	ThreadIdentifier thread_id
)
{
	BHardAssert( !shutting_down, "Cannot remove thread, trying to remove threads while shutting down the thread pool" );
	BHardAssert( GetTaskQueueCount() == 0, "Cannot remove thread, threads cannot be removed when there are any tasks queued" );
	BHardAssert( std::this_thread::get_id() == main_thread_id, "Cannot remove thread, threads can only be removed by the main thread" );

	// Find the thread from thread description list.
	auto thread = std::find_if( thread_description_list.begin(), thread_description_list.end(),
		[ thread_id ]( auto & thread_description )
		{
			return thread_description->thread_id == thread_id;
		}
	);
	if( thread == thread_description_list.end() ) return;

	// Signal the thread to exit, keep waking up threads until the thread is ready to join.
	( *thread )->should_exit = true;
	while( !( *thread )->ready_to_join )
	{
		thread_shared_data->thread_wakeup.notify_all();
		std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );
	}
	( *thread )->stl_thread.join();

	thread_description_list.Erase( thread );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::CheckAndHandleThreadThrow()
{
	if( std::this_thread::get_id() != main_thread_id ) return;

	auto lock_guard = std::unique_lock( thread_shared_data->thread_exception_mutex );

	if( thread_shared_data->thread_exception_handled.load() ) return;

	if( thread_shared_data->thread_exception_raised && !thread_shared_data->thread_exception_handled )
	{
		thread_shared_data->thread_exception_handled = true;
		lock_guard.unlock();
		EvacuateThreads();
		auto record = diagnostic::MakePrintRecord_Argument( U"Exception thrown in thread", thread_shared_data->thread_exception_id.load() );
		record += diagnostic::MakePrintRecord( U"\n" );
		auto exception = diagnostic::MakeException( record );
		exception.SetNextException( thread_shared_data->thread_exception );
		diagnostic::Throw( exception );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadPool::EvacuateThreads()
{
	BHardAssert( std::this_thread::get_id() == main_thread_id, "Cannot evacuate threads, threads can only be evacuated by the main thread" );

	thread_shared_data->threads_should_exit = true;
	while( std::any_of( thread_description_list.begin(), thread_description_list.end(), [](const auto & thread_description) { return !thread_description->ready_to_join.load(); }) )
	{
		thread_shared_data->thread_wakeup.notify_all();
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	// All threads ready to join
	for( auto & t : thread_description_list )
	{
		t->stl_thread.join();
	}

	thread_description_list.Clear();
	thread_shared_data->task_list.Clear();
}
