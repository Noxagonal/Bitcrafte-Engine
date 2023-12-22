#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace thread {



class ThreadPool;
class Thread;
class ThreadSharedData;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// This is a unique thread resource and each thread will have exactly one.
///
///	This class is meant to be inherited from and extended to suit the specific need for the thread. Resources in this class
/// never need a mutex as single thread access is guaranteed. Tasks can also be locked to only run on this thread using this
/// specific thread resource, this means that this is the perfect place for memory managers that only ever allocate and
/// deallocate on a single thread like Vulkan memory pools and Vulkan descriptor pools.
class Thread
{
	friend class ThreadPool;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline virtual									~Thread() {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Run in thread start before it processes anything else.
	///
	/// @note
	/// If an exception is thrown in this function, the whole thread pool will be evacuated and an exception is forwarded in the
	/// main thread. ThreadEnd() is always called so you may use that to do any potential cleanup first.
	virtual	void									ThreadBegin()					= 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Run when thread is about to terminate.
	///
	/// @note
	///	This function is run even if ThreadBegin() throws. You can rely on this function for cleanup.
	///
	/// @note
	/// This is a cleanup function and should not throw, any major errors can be left to the OS to deal with.
	virtual void									ThreadEnd() noexcept			= 0;
};



} // thread
} // bc
