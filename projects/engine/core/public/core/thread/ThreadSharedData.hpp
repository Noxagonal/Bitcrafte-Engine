#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/List.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <core/thread/ThreadDescription.hpp>
#include <core/thread/Task.hpp>

#include <atomic>
#include <mutex>
#include <memory>
#include <condition_variable>



namespace bc {
namespace thread {



class Task;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This is the main method of inter-thread communication.
class BITCRAFTE_ENGINE_API ThreadSharedData
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Finds more work in work queue that hasn't started processing yet.
	///
	/// @param thread_description
	/// Pointer to thread description, which contains details about an individual thread, a pointer to thread instance itself, its
	/// index and potentially other information that is not directly stored in Thread class.
	///
	/// @return
	/// Task that needs to be run by worker thread.
	Task 							*	FindWork(
		ThreadDescription			&	thread_description
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Signals the task completion.
	/// 
	/// @param task
	///	Pointer to task that was completed.
	void								TaskCompleted(
		Task						*	task
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool								IsTaskListEmpty();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void								AddTask(
		UniquePtr<Task>				&&	new_task
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Reschedules the task to be executed again later.
	///
	///	Task dependencies are not removed.
	///
	/// @param task
	///	Pointer to task that is being resecheduled.
	void								RescheduleTask(
		Task						*	task
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::mutex							thread_wakeup_mutex;
	std::condition_variable				thread_wakeup;

	std::atomic_bool					threads_should_exit;

	std::mutex							thread_exception_mutex;
	std::atomic_bool					thread_exception_raised				= false;
	std::atomic_bool					thread_exception_handled			= false;
	std::atomic<ThreadIdentifier>		thread_exception_id					= 0;
	diagnostic::Exception				thread_exception;

	mutable std::mutex					task_list_mutex;
	List<UniquePtr<Task>>				task_list;
};



} // thread
} // bc
