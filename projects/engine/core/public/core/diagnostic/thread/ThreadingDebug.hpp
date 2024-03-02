#pragma once

#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>

#include <thread>



namespace bc {
namespace diagnostic {



#if BITCRAFTE_DEBUG



namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void												SetMainThreadForDebug();

} // internal_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BITCRAFTE_ENGINE_API
void												AssertMainThread(
	SourceLocation									source_location					= SourceLocation::Current()
);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BITCRAFTE_ENGINE_API SingleThreadAccessScopeTracker
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SingleThreadAccessScopeTracker(
		SourceLocation								source_location					= SourceLocation::Current()
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~SingleThreadAccessScopeTracker();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bc::internal_::SimpleText						position_key;
	std::thread::id									thread_id;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BITCRAFTE_ASSERT_MAIN_THREAD_SCOPE ::bc::diagnostic::AssertMainThread();
#define BITCRAFTE_ASSERT_SINGLE_THREAD_SCOPE ::bc::diagnostic::ThreadAccessScopeTracker m_internal_thread_scope_access_tracker_##__LINE__()



#else



namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SetMainThreadForDebug()
{};

} // internal_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BITCRAFTE_ASSERT_MAIN_THREAD_SCOPE
#define BITCRAFTE_ASSERT_SINGLE_THREAD_SCOPE



#endif



} // diagnostic
} // bc
