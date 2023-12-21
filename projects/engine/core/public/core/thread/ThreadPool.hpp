#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/thread/ThreadPoolCreateInfo.hpp>
#include <core/thread/ThreadDescription.hpp>
#include <core/thread/Task.hpp>

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

namespace internal {



// TODO: Move these concepts into utilities.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if the thread lambda returns void.
template<
	typename LambdaType,
	typename ...LambdaParamsT
>
concept IsLambdaReturningVoid = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	{ lambda( std::forward<LambdaParamsT>( params )... ) } -> std::same_as<void>;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if the thread lambda returns something other than void.
template<
	typename LambdaType,
	typename ...LambdaParamsT
>
concept IsLambdaReturningNonVoid = !IsLambdaReturningVoid<LambdaType, LambdaParamsT...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if the thread lambda returns a specific type.
template<
	typename LambdaType,
	typename LambdaReturnT,
	typename ...LambdaParamsT
>
concept IsLambdaReturningType = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	{ lambda( std::forward<LambdaParamsT>( params )... ) } -> std::same_as<LambdaReturnT>;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Concept to check if the thread lambda accepts specific parameters.
template<
	typename LambdaType,
	typename ...LambdaParamsT
>
concept IsLambdaAcceptingParameters = requires(
	LambdaType			lambda,
	LambdaParamsT		...params
)
{
	lambda( std::forward<LambdaParamsT>( params )... );
};



} // internal



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
		return AddTask( std::move( unique_task ) );
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
		return AddTask( std::move( unique_task ) );
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
		return AddTask( std::move( unique_task ) );
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
		return AddTask( MakeUniquePtr<TaskType>( std::forward<TaskConstructorArgumentTypePack>( task_constructor_arguments )... ) );
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
	TaskIdentifier											ScheduleTaskWithDependencies(
		const List<TaskIdentifier>						&	dependencies,
		LambdaType										&&	lambda_function
	)
	{
		static_assert(
			internal::IsLambdaAcceptingParameters<LambdaType, Task&>,
			"Task lambda must accept reference to a task"
		);
		static_assert(
			internal::IsLambdaReturningVoid<LambdaType, Task&> || internal::IsLambdaReturningType<LambdaType, TaskState, Task&>,
			"Lambda task must return task state or void"
		);

		auto lambda_task = MakeUniquePtr<LambdaTask<LambdaType>>( std::forward<LambdaType>( lambda_function ) );

		return ScheduleTask<ThreadType, LambdaTask<LambdaType>>(
			std::move( lambda_task ),
			dependencies
		);
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
	size_t													GetTaskQueueCount() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the number fo tasks currently running in threads.
	///
	/// @return
	/// Number of tasks being executed.
	size_t													GetTaskRunningCount() const;

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

		virtual TaskState									operator() (
			Thread										&	thread
		) override
		{
			if constexpr( internal::IsLambdaReturningNonVoid<LambdaType, Task&> )
			{
				return lambda_function( *this );
			}
			else
			{
				lambda_function( *this );
				return TaskState::FINISHED;
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
		for( size_t i = 0; i < thread_description_list.Size(); i++ )
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
