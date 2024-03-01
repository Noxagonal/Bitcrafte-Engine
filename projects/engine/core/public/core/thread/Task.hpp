#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/containers/List.hpp>

#include <core/thread/TaskState.hpp>
#include <core/thread/ThreadDescription.hpp>

#include <thread>
#include <atomic>
#include <assert.h>



namespace bc {
namespace thread {



class ThreadPool;
class ThreadSharedData;
class Thread;

using TaskIdentifier = u64;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class TaskExecutionResult
{
	PAUSED,
	FINISHED,
	ERROR,
};
inline TaskState				TaskExecutionResultToTaskState(
	TaskExecutionResult			e
)
{
	switch( e )
	{
	case bc::thread::TaskExecutionResult::PAUSED:
		return TaskState::PAUSED;
	case bc::thread::TaskExecutionResult::FINISHED:
		return TaskState::FINISHED;
	case bc::thread::TaskExecutionResult::ERROR:
		return TaskState::ERROR;
	}
	assert( 0 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Task represent a piece of work that will be completed by the thread pool at some point in time in the future.
class Task
{
	friend class ThreadPool;
	friend class ThreadSharedData;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline 											Task()
	{
		state = TaskState::NOT_STARTED;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Task(
		const Task								&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Task(
		Task									&&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual inline									~Task()
	{}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Returns threads this task is allowed to run on.
	/// 
	/// @return
	/// Reference to an array of indices which represent threads that this task is allowed to run on.
	inline const List<ThreadIdentifier>			&	GetThreadLocks() const
	{
		return locked_to_threads;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Checks and returns wether this task is allowed to run on any thread.
	/// 
	/// @return
	/// true if this task must run on specific threads or false if this task can run on any thread.
	inline bool										IsThreadLocked() const
	{
		return !!locked_to_threads.Size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Task id is an unique identifier to a specific task.
	///
	///	Identifiers are always unique for all tasks and an identifier is never recycled.
	/// 
	/// @return
	/// Task unique identifier.
	inline TaskIdentifier							GetTaskId() const
	{
		return task_id;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Task dependencies are unique identifiers to other threads that must fully complete before this task.
	/// 
	/// @return
	/// An array of unique identifiers to tasks that must complete before this task.
	inline const List<u64>						&	GetDependencies() const
	{
		return dependencies;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds a new dependency to this task.
	///
	/// @warning
	/// This is meant to be called from within a running task. Do not call from other threads or places.
	/// 
	/// @warning
	/// Adding dependency at runtime does not pause the current thread, instead you should return TaskState::CONTINUED which will
	/// reschedule this task afterwards. Execution does not continue where we returned but restarts the entire task at a later time,
	/// it is the user's responsibility to track task state.
	///
	/// @param task_id
	///	Id of the task we want this task to depend on.
	inline void										AddDependencyAtRuntime(
		TaskIdentifier								task_id
	)
	{
		BAssert( state == TaskState::RUNNING, "Cannot call AddDependencyAtRuntime on a task that is not currently running, this function must be called from within the running task" );
		BAssert( running_thread_system_id == std::this_thread::get_id(), "Cannot call AddDependencyAtRuntime from a different thread from which the task is running, this function must be called from within the running task" );
		dependencies.PushBack( task_id );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the current state of the task.
	/// 
	/// @return
	/// State of the task.
	inline TaskState								GetState() const
	{
		return state.load();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the current state of this task.
	///
	///	Tasks may be paused and continued later, this tells if the task has finished or if it is pending more work.
	///
	/// @return
	/// Task state.
	inline TaskState								GetTaskState() const
	{
		return state.load();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used by the thread process to actually run this task.
	/// 
	/// @param thread
	///	Pool thread that is running this task.
	///
	/// @return
	/// Result of the execution.
	virtual TaskExecutionResult						operator()(
		Thread									&	thread
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Runs the task. This is used by the running thread to execute the task and should not be called directly.
	///
	/// @note
	/// Internal use.
	///
	/// @param thread
	///	Pool thread that is running this task.
	///
	/// @return
	/// Result of the execution.
	inline TaskExecutionResult						ThreadRun(
		Thread									&	thread
	)
	{
		assert( state == TaskState::RUNNING );
		return operator()( thread );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<ThreadIdentifier>							locked_to_threads			= {};
	TaskIdentifier									task_id						= {};
	List<TaskIdentifier>							dependencies				= {};

	ThreadIdentifier								running_thread_id			= {};
	std::thread::id									running_thread_system_id	= {};

	std::atomic<TaskState>							state;
};




} // thread
} // bc
