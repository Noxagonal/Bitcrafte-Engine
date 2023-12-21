
#include <gtest/gtest.h>

#include <TestsCommon.hpp>
#include <core/event/Event.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreEvent, BasicInitialize )
{
	using A = qup::Event<>;

	A a;
	EXPECT_EQ( a.GetObserverCount(), 0 );
	EXPECT_EQ( a.GetObservingCount(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreEvent, Observers )
{
	using A = qup::Event<>;
	{
		A a;
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 0 );

		// Create b and register observer a.
		A b;
		b.RegisterObserver( &a );
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 1 );
		EXPECT_EQ( b.GetObserverCount(), 1 );
		EXPECT_EQ( b.GetObservingCount(), 0 );

		// Create c and register observer b.
		A c;
		c.RegisterObserver( &b );
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 1 );
		EXPECT_EQ( b.GetObserverCount(), 1 );
		EXPECT_EQ( b.GetObservingCount(), 1 );
		EXPECT_EQ( c.GetObserverCount(), 1 );
		EXPECT_EQ( c.GetObservingCount(), 0 );

		// Unregister b from c.
		c.UnRegisterObserver( &b );
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 1 );
		EXPECT_EQ( b.GetObserverCount(), 1 );
		EXPECT_EQ( b.GetObservingCount(), 0 );
		EXPECT_EQ( c.GetObserverCount(), 0 );
		EXPECT_EQ( c.GetObservingCount(), 0 );

		// Unregister b from c the second time, this should pass.
		c.UnRegisterObserver( &b );
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 1 );
		EXPECT_EQ( b.GetObserverCount(), 1 );
		EXPECT_EQ( b.GetObservingCount(), 0 );
		EXPECT_EQ( c.GetObserverCount(), 0 );
		EXPECT_EQ( c.GetObservingCount(), 0 );
	}
	// auto unregister on destruction. destroying the observer event.
	{
		A a;
		{
			A b;
			b.RegisterObserver( &a );
			EXPECT_EQ( a.GetObserverCount(), 0 );
			EXPECT_EQ( a.GetObservingCount(), 1 );
			EXPECT_EQ( b.GetObserverCount(), 1 );
			EXPECT_EQ( b.GetObservingCount(), 0 );
		}
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 0 );
	}
	// auto unregister on destruction. destroying the observing event.
	{
		A a;
		{
			A b;
			a.RegisterObserver( &b );
			EXPECT_EQ( a.GetObserverCount(), 1 );
			EXPECT_EQ( a.GetObservingCount(), 0 );
			EXPECT_EQ( b.GetObserverCount(), 0 );
			EXPECT_EQ( b.GetObservingCount(), 1 );
		}
		EXPECT_EQ( a.GetObserverCount(), 0 );
		EXPECT_EQ( a.GetObservingCount(), 0 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreEvent, Callbacks )
{
	using A = qup::Event<>;

	int32_t a_call_counter = 0;
	int32_t b_call_counter = 0;

	A a;
	a.RegisterCallback( [&a_call_counter]()
		{
			++a_call_counter;
		}
	);

	a.Signal();
	EXPECT_EQ( a_call_counter, 1 );

	a.RegisterCallback( [&a_call_counter]()
		{
			++a_call_counter;
		}
	);

	// Make sure both callbacks are called.
	a.Signal();
	EXPECT_EQ( a_call_counter, 3 );



	// Make sure signals also call callbacks from observers.
	A b;
	a.RegisterObserver( &b );
	b.RegisterCallback( [&b_call_counter]()
		{
			++b_call_counter;
		}
	);

	a.Signal();
	EXPECT_EQ( a_call_counter, 5 );
	EXPECT_EQ( b_call_counter, 1 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreEvent, Ordering )
{
	using A = qup::Event<>;

	enum class First
	{
		UNDECIDED,
		A,
		B
	};
	auto first = First::UNDECIDED;

	A a;
	A b;
	a.RegisterObserver( &b );
	a.RegisterCallback( [&first]()
		{
			if( first == First::UNDECIDED ) {
				first = First::A;
			}
		}
	);
	b.RegisterCallback( [&first]()
		{
			if( first == First::UNDECIDED ) {
				first = First::B;
			}
		}
	);

	// Expecting callbacks to run first.
	a.Signal();
	EXPECT_EQ( first, First::A );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreEvent, CyclicOrDoubleDependency )
{
	using A = qup::Event<>;

	A a;
	A b;
	A c;
	A d;

	a.RegisterObserver( &b );
	b.RegisterObserver( &c );
	c.RegisterObserver( &d );

	EXPECT_THROW( b.RegisterObserver( &a ), qup::exception::Exception );
	EXPECT_THROW( a.RegisterObserver( &c ), qup::exception::Exception );
}
