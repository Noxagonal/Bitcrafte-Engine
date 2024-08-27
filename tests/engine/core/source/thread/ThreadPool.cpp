
#include <gtest/gtest.h>

#include <core/CoreComponent.hpp>
#include <core/thread/ThreadPool.hpp>



namespace core {
namespace thread {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ThreadPool, BasicInit )
{
	{
		auto core_create_info = bc::CoreComponentCreateInfo {};
		core_create_info.logger_create_info.disabled = true;
		auto core = std::make_unique<bc::CoreComponent>( core_create_info );
		auto thread_pool = core->GetThreadPool();

		class TestThread : public bc::thread::Thread
		{
		public:
			TestThread() = default;
			void ThreadBegin() override {}
			void ThreadEnd() noexcept override {}
		};

		auto t1 = thread_pool->AddThread<TestThread>();
		auto t2 = thread_pool->AddThread<TestThread>();
		auto t3 = thread_pool->AddThread<TestThread>();
		auto t4 = thread_pool->AddThread<TestThread>();
		auto t5 = thread_pool->AddThread<TestThread>();
		auto t6 = thread_pool->AddThread<TestThread>();
		auto t7 = thread_pool->AddThread<TestThread>();
		auto t8 = thread_pool->AddThread<TestThread>();
		thread_pool->RemoveThread( t1 );
		thread_pool->RemoveThread( t2 );
		thread_pool->RemoveThread( t3 );
		thread_pool->RemoveThread( t4 );
		thread_pool->RemoveThread( t5 );
		thread_pool->RemoveThread( t6 );
		thread_pool->RemoveThread( t7 );
		thread_pool->RemoveThread( t8 );
		
		t1 = thread_pool->AddThread<TestThread>();
		t2 = thread_pool->AddThread<TestThread>();
		t3 = thread_pool->AddThread<TestThread>();
		t4 = thread_pool->AddThread<TestThread>();
		t5 = thread_pool->AddThread<TestThread>();
		t6 = thread_pool->AddThread<TestThread>();
		t7 = thread_pool->AddThread<TestThread>();
		t8 = thread_pool->AddThread<TestThread>();

		EXPECT_NE( thread_pool->GetThreadSystemID( t1 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t2 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t3 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t4 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t5 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t6 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t7 ), std::thread::id() );
		EXPECT_NE( thread_pool->GetThreadSystemID( t8 ), std::thread::id() );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ThreadPool, ThrowAtThreadConstruction )
{
	{
		auto core_create_info = bc::CoreComponentCreateInfo {};
		core_create_info.logger_create_info.disabled = true;
		auto core = std::make_unique<bc::CoreComponent>( core_create_info );
		auto thread_pool = core->GetThreadPool();

		class ThrowingThread : public bc::thread::Thread
		{
		public:
			ThrowingThread() = default;
			void ThreadBegin() override {
				bc::diagnostic::Throw( bc::diagnostic::Exception( U"Test" ) );
			}
			void ThreadEnd() noexcept override {}
		};

		EXPECT_THROW( thread_pool->AddThread<ThrowingThread>(), bc::diagnostic::Exception );
		EXPECT_THROW( thread_pool->AddThread<ThrowingThread>(), bc::diagnostic::Exception );
	}
};



} // thread
} // core
