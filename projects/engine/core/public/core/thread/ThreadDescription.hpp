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
	ThreadDescription( const ThreadDescription& other ) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ThreadDescription( ThreadDescription&& other )
	{
		auto lock_guard = std::lock_guard( other.modify_mutex );
		Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( const ThreadDescription& other ) -> ThreadDescription& = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( ThreadDescription&& other ) -> ThreadDescription&
	{
		std::lock( modify_mutex, other.modify_mutex );
		auto my_lock_guard = std::lock_guard( modify_mutex, std::adopt_lock );
		auto other_lock_guard = std::lock_guard( other.modify_mutex, std::adopt_lock );

		Swap( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UniquePtr<Thread>				pool_thread;
	std::thread						stl_thread;
	std::atomic<WorkerThreadState>	state			= WorkerThreadState::UNINITIALIZED;
	std::atomic_bool				should_exit		= false;
	std::atomic_bool				ready_to_join	= false;
	ThreadIdentifier				thread_id		= 0;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::mutex						modify_mutex;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void Swap( ThreadDescription& other ) noexcept
	{
		std::swap( this->pool_thread, other.pool_thread );
		std::swap( this->stl_thread, other.stl_thread );
		AtomicSwap( this->state, other.state );
		AtomicSwap( this->ready_to_join, other.ready_to_join );
		std::swap( this->thread_id, other.thread_id );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Swaps two atomic values.
	///
	/// @warning
	/// Internal use only. Assumes that the mutexes are locked and does not verify that atomic exhanges are actually atomic.
	template<typename Type>
	void AtomicSwap(
		std::atomic<Type>&	left,
		std::atomic<Type>&	right
	) noexcept
	{
		auto left_value = left.load();
		auto right_value = right.load();
		left.store( right_value );
		right.store( left_value );
	}
};



} // thread
} // bc
