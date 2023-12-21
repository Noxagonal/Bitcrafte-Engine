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
	/// @return
	/// true if created successful, false to terminate entire thread pool. ThreadEnd() is ran afterwards regardless so you can rely
	/// that for cleanup.
	virtual	bool									ThreadBegin()			= 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Run when thread pool terminates, this is ran in thread only after all tasks have completed.
	///
	///	This function is run even if ThreadBegin() returns false, in which case thread pool will terminate immediately. You can rely
	/// on this function for cleanup if something in ThreadBegin() goes wrong and you need to bail out.
	virtual void									ThreadEnd()				= 0;
};



} // thread
} // bc
