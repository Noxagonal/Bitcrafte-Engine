#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/thread/ThreadPoolCreateInfo.hpp>
#include <core/thread/ThreadDescription.hpp>
#include <core/thread/Task.hpp>

#include <core/utility/concepts/LambdaConcepts.hpp>

#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>

#include <thread>
#include <concepts>



namespace bc {
namespace thread {

struct ThreadDescription;
class ThreadSharedData;
class Thread;
class Task;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Contains a pool threads that are kept running until this object is destroyed.
///
/// Thread pool should always be used when using multithreading in engine as it is more efficient than creating and destroying
/// threads constantly. Tasks can also be scheduled, waited and ordered easily without using external fences or worrying about
/// joining threads later.
class BITCRAFTE_ENGINE_API ThreadPool
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Starts worker threads so tasks can be scheduled.
	///
	/// @param create_info
	/// Thread pool create info tells how the thread pool should be constructed.
	/// 
	/// @param thread_resources
	/// This initializes thread private resources and tells how many threads should be created.
	ThreadPool(
		const ThreadPoolCreateInfo						&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual													~ThreadPool();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<
		typename											ThreadType,
		typename											...ThreadConstructorArgumentsTypePack
	>
	ThreadIdentifier										AddThread(
		ThreadConstructorArgumentsTypePack				&&	...constructor_arguments
	)
	{
		static_assert( std::is_base_of_v<Thread, ThreadType>, "Thread type must be derived from bc::thread::Thread" );
		auto thread_description = MakeUniquePtr<ThreadDescription>();
		thread_description->pool_thread		= MakeUniquePtr<ThreadType>( std::forward<ThreadConstructorArgumentsTypePack>( constructor_arguments )... );
		return DoAddThread( std::move( thread_description ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													RemoveThread(
		ThreadIdentifier									thread_id
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam ThreadType
	///	Thread type we're locking this task to.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param dependencies
	///	Sets which tasks must run before the task we're currently submitting.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											ThreadType,
		typename											TaskType,
		typename											...TaskConstructorArgumentTypePack
	>
	TaskIdentifier											ScheduleTaskToThreadTypeWithDependencies(
		const List<TaskIdentifier>						&	dependencies,
		TaskConstructorArgumentTypePack					&&	...task_constructor_arguments
	) requires( std::is_base_of_v<Task, TaskType> )
	{
		auto unique_task = MakeUniquePtr<TaskType>( std::forward<TaskConstructorArgumentTypePack>( task_constructor_arguments )... );
		unique_task->locked_to_threads	= GetTaskThreadLockIDs<ThreadType>();
		unique_task->dependencies		= dependencies;
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param dependencies
	///	Sets which tasks must run before the task we're currently submitting.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											TaskType,
		typename											...TaskConstructorArgumentTypePack
	>
	TaskIdentifier											ScheduleTaskWithDependencies(
		const List<TaskIdentifier>						&	dependencies,
		TaskConstructorArgumentTypePack					&&	...task_constructor_arguments
	) requires( std::is_base_of_v<Task, TaskType> )
	{
		auto unique_task = MakeUniquePtr<TaskType>( std::forward<TaskConstructorArgumentTypePack>( task_constructor_arguments )... );
		unique_task->dependencies		= dependencies;
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam ThreadType
	///	Thread type we're locking this task to.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											ThreadType,
		typename											TaskType,
		typename											...TaskConstructorArgumentTypePack
	>
	TaskIdentifier											ScheduleTaskToThreadType(
		TaskConstructorArgumentTypePack					&&	...task_constructor_arguments
	) requires( std::is_base_of_v<Task, TaskType> )
	{
		auto unique_task = MakeUniquePtr<TaskType>( std::forward<TaskConstructorArgumentTypePack>( task_constructor_arguments )... );
		unique_task->locked_to_threads	= GetTaskThreadLockIDs<ThreadType>();
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											TaskType,
		typename											...TaskConstructorArgumentTypePack
	>
	TaskIdentifier											ScheduleTask(
		TaskConstructorArgumentTypePack					&&	...task_constructor_arguments
	) requires( std::is_base_of_v<Task, TaskType> )
	{
		auto unique_task = MakeUniquePtr<TaskType>( std::forward<TaskConstructorArgumentTypePack>( task_constructor_arguments )... );
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam ThreadType
	///	Task type we're scheduling.
	///
	/// @tparam LambdaType
	///	Lambda type we're scheduling.
	///
	/// @param lambda_function
	///	Lambda function we're submitting.
	///
	/// @param dependencies
	///	Sets which tasks must run before the task we're currently submitting.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename ThreadType,
		typename LambdaType
	>
	TaskIdentifier											ScheduleLambdaTaskToThreadTypeWithDependencies(
		const List<TaskIdentifier>						&	dependencies,
		LambdaType										&&	lambda_function
	)
	{
		static_assert(
			utility::IsLambdaAcceptingParameters<LambdaType, Task&> || utility::IsLambdaAcceptingParameters<LambdaType>,
			"Task lambda must accept reference to a task or nothing, Eg. '[](){}' or '[]( Task & task ){}"
		);
		static_assert(
			utility::IsLambdaReturningVoid<LambdaType, Task&> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult, Task&> ||
			utility::IsLambdaReturningVoid<LambdaType> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult>,
			"Lambda task must return task state or nothing, Eg. '[]( Task & task ){}' or '[]( Task & task ){ return TaskExecutionResult::FINISHED; }'"
		);

		auto unique_task = MakeUniquePtr<LambdaTask<LambdaType>>( std::move( lambda_function ) );
		unique_task->locked_to_threads	= GetTaskThreadLockIDs<ThreadType>();
		unique_task->dependencies		= dependencies;
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param dependencies
	///	Sets which tasks must run before the task we're currently submitting.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											LambdaType
	>
	TaskIdentifier											ScheduleLambdaTaskWithDependencies(
		const List<TaskIdentifier>						&	dependencies,
		LambdaType										&&	lambda_function
	)
	{
		static_assert(
			utility::IsLambdaAcceptingParameters<LambdaType, Task&> || utility::IsLambdaAcceptingParameters<LambdaType>,
			"Task lambda must accept reference to a task or nothing, Eg. '[](){}' or '[]( Task & task ){}"
		);
		static_assert(
			utility::IsLambdaReturningVoid<LambdaType, Task&> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult, Task&> ||
			utility::IsLambdaReturningVoid<LambdaType> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult>,
			"Lambda task must return task state or nothing, Eg. '[]( Task & task ){}' or '[]( Task & task ){ return TaskExecutionResult::FINISHED; }'"
		);

		auto unique_task = MakeUniquePtr<LambdaTask<LambdaType>>( std::move( lambda_function ) );
		unique_task->dependencies		= dependencies;
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam ThreadType
	///	Thread type we're locking this task to.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											ThreadType,
		typename											LambdaType
	>
	TaskIdentifier											ScheduleLambdaTaskToThreadType(
		LambdaType										&&	lambda_function
	)
	{
		static_assert(
			utility::IsLambdaAcceptingParameters<LambdaType, Task&> || utility::IsLambdaAcceptingParameters<LambdaType>,
			"Task lambda must accept reference to a task or nothing, Eg. '[](){}' or '[]( Task & task ){}"
		);
		static_assert(
			utility::IsLambdaReturningVoid<LambdaType, Task&> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult, Task&> ||
			utility::IsLambdaReturningVoid<LambdaType> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult>,
			"Lambda task must return task state or nothing, Eg. '[]( Task & task ){}' or '[]( Task & task ){ return TaskExecutionResult::FINISHED; }'"
		);

		auto unique_task = MakeUniquePtr<LambdaTask<LambdaType>>();
		unique_task->locked_to_threads	= GetTaskThreadLockIDs<ThreadType>( std::move( lambda_function ) );
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Schedules a new task to be run on a thread.
	///
	/// @tparam TaskType
	///	Task type we're scheduling.
	///
	/// @tparam ...TaskConstructorArgumentTypePack
	/// Task constructor argument types.
	///
	/// @param ...task_constructor_arguments
	/// Arguments passed to task constructor.
	///
	/// @return
	/// Unique id to the task process which can be waited upon by other submits.
	template<
		typename											LambdaType
	>
	TaskIdentifier											ScheduleLambdaTask(
		LambdaType										&&	lambda_function
	)
	{
		static_assert(
			utility::IsLambdaAcceptingParameters<LambdaType, Task&> || utility::IsLambdaAcceptingParameters<LambdaType>,
			"Task lambda must accept reference to a task or nothing, Eg. '[](){}' or '[]( Task & task ){}"
		);
		static_assert(
			utility::IsLambdaReturningVoid<LambdaType, Task&> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult, Task&> ||
			utility::IsLambdaReturningVoid<LambdaType> || utility::IsLambdaReturningType<LambdaType, TaskExecutionResult>,
			"Lambda task must return task state or nothing, Eg. '[]( Task & task ){}' or '[]( Task & task ){ return TaskExecutionResult::FINISHED; }'"
		);

		auto unique_task = MakeUniquePtr<LambdaTask<LambdaType>>( std::move( lambda_function ) );
		return DoAddTask( std::move( unique_task ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the number fo tasks currently queued to be run in threads.
	///
	/// @note
	/// Running tasks are included.
	///
	/// @return
	/// Number of tasks queued.
	u64														GetTaskQueueCount() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the number fo tasks currently running in threads.
	///
	/// @return
	/// Number of tasks being executed.
	u64														GetTaskRunningCount() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the STL thread id from thread pool index.
	/// 
	/// @param thread_id
	///	Identifier of the thread.
	/// 
	/// @return
	/// STL thread id. Returned id will be empty if thread was not found.
	std::thread::id											GetThreadSystemID(
		ThreadIdentifier									thread_id
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													Run();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Waits until thread pool has no work left to do.
	///
	/// @warning
	/// This can add up to a millisecond of wasted time per call and is primarily meant to be used only when shutting down.
	void													WaitIdle();

private:

	template<typename LambdaType>
	class LambdaTask : public Task
	{
	public:
		LambdaTask(
			LambdaType										lambda_function
		) :
			lambda_function( lambda_function )
		{}

		virtual TaskExecutionResult							operator() (
			Thread										&	thread
		) override
		{
			constexpr bool non_void1 = utility::IsLambdaSignature<LambdaType, void>;
			constexpr bool non_void2 = utility::IsLambdaSignature<LambdaType, void, Task&>;
			constexpr bool non_void3 = utility::IsLambdaSignature<LambdaType, TaskExecutionResult>;
			constexpr bool non_void4 = utility::IsLambdaSignature<LambdaType, TaskExecutionResult, Task&>;

			if constexpr( utility::IsLambdaSignature<LambdaType, void> )
			{
				lambda_function();
				return TaskExecutionResult::FINISHED;
			}
			else if constexpr( utility::IsLambdaSignature<LambdaType, void, Task&> )
			{
				lambda_function();
				return TaskExecutionResult::FINISHED;
			}
			else if constexpr( utility::IsLambdaSignature<LambdaType, TaskExecutionResult> )
			{
				return lambda_function();
			}
			else if constexpr( utility::IsLambdaSignature<LambdaType, TaskExecutionResult, Task&> )
			{
				return lambda_function();
			}
			else
			{
				lambda_function();
			}
		}

	private:
		LambdaType											lambda_function;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TaskIdentifier											DoAddTask(
		UniquePtr<Task>									&&	new_task
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadIdentifier										DoAddThread(
		UniquePtr<ThreadDescription>					&&	thread_description
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													DoRemoveThread(
		ThreadIdentifier									thread_id
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													CheckAndHandleThreadThrow();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void													EvacuateThreads();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename ThreadType>
	List<ThreadIdentifier>									GetTaskThreadLockIDs()
	{
		List<ThreadIdentifier> ret;
		for( u64 i = 0; i < thread_description_list.Size(); i++ )
		{
			if( dynamic_cast<ThreadType*>( thread_description_list[ i ]->pool_thread.Get() ) != nullptr )
			{
				ret.PushBack( i );
			}
		}
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadPoolCreateInfo									create_info					= {};

	std::thread::id											main_thread_id;

	UniquePtr<ThreadSharedData>								thread_shared_data;
	List<UniquePtr<ThreadDescription>>						thread_description_list;

	std::atomic<TaskIdentifier>								task_id_counter				= 0;
	std::atomic<ThreadIdentifier>							thread_id_counter			= 0;

	std::atomic_bool										shutting_down				= {};
};



} // thread
} // bc
