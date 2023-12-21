
#include <core/PreCompiledHeader.hpp>
#include <core/thread/ThreadSharedData.hpp>
#include <core/thread/Task.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::thread::Task * bc::thread::ThreadSharedData::FindWork(
	bc::thread::ThreadDescription	&	thread_description
)
{
	std::lock_guard<std::mutex> lock_guard( task_list_mutex );

	auto it = task_list.begin();
	while( it != task_list.end() )
	{
		auto task = it->Get();

		// Check to see if the task is already running.
		if( task->GetState() == TaskState::RUNNING )
		{
			++it;
			continue;
		}

		// Check if this thread is allowed to run this code
		if( task->IsThreadLocked() )
		{
			if( std::none_of( task->GetThreadLocks().begin(), task->GetThreadLocks().end(),
				[ &thread_description, &task ]( ThreadIdentifier thread_lock_id )
			{
				return thread_description.thread_id == thread_lock_id;
			} ) )
			{
				++it;
				continue;
			}
		}

		// Look for dependencies, if task is depending on another task that isn't yet finished we should not execute it.
		// Finished tasks are removed from task_list.
		const auto & dependencies = task->GetDependencies();
		if( std::any_of( task_list.begin(), task_list.end(), [ &dependencies ](
			UniquePtr<Task> & t )
		{
			return std::any_of( dependencies.begin(), dependencies.end(), [ &t ]( uint64_t task_dependency )
			{
				return t->GetTaskId() == task_dependency;
			} );
		} ) )
		{
			// Task is depending on some other previous task in the queue
			++it;
			continue;
		}

		// TODO: Implement priority checking. This could be done by adding this job to a candidate list, then doing another loop
		// over the candidate list to see which task has the highest priority.

		// Task is able to be run by the calling thread, set it active and return pointer to it.
		task->state = TaskState::RUNNING;
		return task;
	}

	// No work available
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadSharedData::TaskCompleted(
	bc::thread::Task	*	task
)
{
	std::lock_guard<std::mutex> lock_guard( task_list_mutex );
	auto it = task_list.begin();
	while( it != task_list.end() )
	{
		if( it->Get() == task )
		{
			task_list.Erase( it );
			return;
		}
		++it;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bc::thread::ThreadSharedData::IsTaskListEmpty()
{
	std::lock_guard<std::mutex> lock_guard( task_list_mutex );
	return task_list.IsEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadSharedData::AddTask(
	UniquePtr<Task>	&&	new_task )
{
	assert( !thread_exception_raised );
	if( thread_exception_raised ) return;

	std::lock_guard<std::mutex> lock_guard( task_list_mutex );
	task_list.EmplaceBack( std::move( new_task ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::thread::ThreadSharedData::RescheduleTask(
	Task * task
)
{
	assert( !thread_exception_raised );
	if( thread_exception_raised ) return;

	std::lock_guard<std::mutex> lock_guard( task_list_mutex );

	auto it = task_list.begin();
	while( it != task_list.end() )
	{
		if( it->Get() != task )
		{
			++it;
			continue;
		}

		auto moved_task = std::move( *it );
		task_list.Erase( it );

		moved_task->running_thread_id	= {};
		moved_task->state				= TaskState::PAUSED;

		task_list.PushBack( std::move( moved_task ) );
		return;
	}

	assert( 0 && "Should not get here, task was not found in the task list while rescheduling it" );
}
