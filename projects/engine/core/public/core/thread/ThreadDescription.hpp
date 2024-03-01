#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/thread/Thread.hpp>

#include <cstdint>
#include <atomic>
#include <memory>
#include <thread>



namespace bc {
namespace thread {



using ThreadIdentifier = u64;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class WorkerThreadState : u32
{
	UNINITIALIZED		= 0,
	RUNNING,
	IDLE,
	INITIALIZATION_ERROR,
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Contains details about a thread.
///
/// This is primarily used to hide some implementation details from Thread class, to prevent clutter.
class ThreadDescription
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadDescription() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadDescription(
		const ThreadDescription				&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ThreadDescription(
		ThreadDescription					&&	other
	)
	{
		SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadDescription						&	operator=(
		const ThreadDescription				&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ThreadDescription						&	operator=(
		ThreadDescription					&&	other
	)
	{
		SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<Thread>							pool_thread;
	std::thread									stl_thread;
	std::atomic<WorkerThreadState>				state					= WorkerThreadState::UNINITIALIZED;
	std::atomic_bool							should_exit				= false;
	std::atomic_bool							ready_to_join			= false;
	ThreadIdentifier							thread_id				= 0;

private:

	inline void									SwapOther(
		ThreadDescription					&&	other
	)
	{
		std::swap( this->pool_thread, other.pool_thread );
		std::swap( this->stl_thread, other.stl_thread );
		AtomicSwap( this->state, other.state );
		AtomicSwap( this->ready_to_join, other.ready_to_join );
		std::swap( this->thread_id, other.thread_id );
	}

	// TODO: Move this to utilities.
	template<typename Type>
	void										AtomicSwap(
		std::atomic<Type>					&	left,
		std::atomic<Type>					&	right
	)
	{
		Type left_expected	= left.load();
		Type right_expected	= right.load();

		while( !left.compare_exchange_weak( left_expected, right_expected ) ||
			!right.compare_exchange_weak( right_expected, left_expected ) )
		{
			// Retry if the compare_exchange fails (values have changed)
			left_expected	= left.load();
			right_expected	= right.load();
		}
	}
};



} // thread
} // bc
