
#include <gtest/gtest.h>

#include <core/containers/Function.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <functional>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, BasicInit )
{
	using namespace bc;

	using A = Function<void()>;

	A a1;
	EXPECT_TRUE( a1.IsEmpty() );

	A b1 {};
	EXPECT_TRUE( b1.IsEmpty() );

	A c1 = {};
	EXPECT_TRUE( c1.IsEmpty() );

	A d1 = A();
	EXPECT_TRUE( d1.IsEmpty() );

	A e1 = A {};
	EXPECT_TRUE( e1.IsEmpty() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestFunction_void_void() {};
void TestFunction_void_u32( bc::u32 value ) {};
bc::u32 TestFunction_u32_void() { return 0; };
bc::u32 TestFunction_u32_u32( bc::u32 value ) { return value; };
TEST( FunctionContainer, RegularFunctionInit )
{
	using namespace bc;


	auto a1 = Function<void()>( TestFunction_void_void );
	EXPECT_FALSE( a1.IsEmpty() );

	auto b1 = Function<void( u32 )>( TestFunction_void_u32 );
	EXPECT_FALSE( b1.IsEmpty() );

	auto c1 = Function<u32()>( TestFunction_u32_void );
	EXPECT_FALSE( c1.IsEmpty() );

	auto d1 = Function<u32( u32 )>( TestFunction_u32_u32 );
	EXPECT_FALSE( d1.IsEmpty() );


	auto a2 = Function<void()>( &TestFunction_void_void );
	EXPECT_FALSE( a2.IsEmpty() );

	auto b2 = Function<void( u32 )>( &TestFunction_void_u32 );
	EXPECT_FALSE( b2.IsEmpty() );

	auto c2 = Function<u32()>( &TestFunction_u32_void );
	EXPECT_FALSE( c2.IsEmpty() );

	auto d2 = Function<u32( u32 )>( &TestFunction_u32_u32 );
	EXPECT_FALSE( d2.IsEmpty() );


	auto a3 = Function<void()>{ TestFunction_void_void };
	EXPECT_FALSE( a3.IsEmpty() );

	auto b3 = Function<void( u32 )>{ TestFunction_void_u32 };
	EXPECT_FALSE( b3.IsEmpty() );

	auto c3 = Function<u32()>{ TestFunction_u32_void };
	EXPECT_FALSE( c3.IsEmpty() );

	auto d3 = Function<u32( u32 )>{ TestFunction_u32_u32 };
	EXPECT_FALSE( d3.IsEmpty() );


	auto a4 = Function<void()>{ &TestFunction_void_void };
	EXPECT_FALSE( a4.IsEmpty() );

	auto b4 = Function<void( u32 )>{ &TestFunction_void_u32 };
	EXPECT_FALSE( b4.IsEmpty() );

	auto c4 = Function<u32()>{ &TestFunction_u32_void };
	EXPECT_FALSE( c4.IsEmpty() );

	auto d4 = Function<u32( u32 )>{ &TestFunction_u32_u32 };
	EXPECT_FALSE( d4.IsEmpty() );


	Function<void()> a5 = TestFunction_void_void;
	EXPECT_FALSE( a5.IsEmpty() );

	Function<void( u32 )> b5 = TestFunction_void_u32;
	EXPECT_FALSE( b5.IsEmpty() );

	Function<u32()> c5 = TestFunction_u32_void;
	EXPECT_FALSE( c5.IsEmpty() );

	Function<u32( u32 )> d5 = TestFunction_u32_u32;
	EXPECT_FALSE( d5.IsEmpty() );


	Function<void()> a6 = &TestFunction_void_void;
	EXPECT_FALSE( a6.IsEmpty() );

	Function<void( u32 )> b6 = &TestFunction_void_u32;
	EXPECT_FALSE( b6.IsEmpty() );

	Function<u32()> c6 = &TestFunction_u32_void;
	EXPECT_FALSE( c6.IsEmpty() );

	Function<u32( u32 )> d6 = &TestFunction_u32_u32;
	EXPECT_FALSE( d6.IsEmpty() );


	auto a7 = Function( TestFunction_void_void );
	EXPECT_FALSE( a7.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( a7 )::Signature, void()> ) );

	auto b7 = Function( TestFunction_void_u32 );
	EXPECT_FALSE( b7.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( b7 )::Signature, void( u32 )> ) );

	auto c7 = Function( TestFunction_u32_void );
	EXPECT_FALSE( c7.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( c7 )::Signature, u32()> ) );

	auto d7 = Function( TestFunction_u32_u32 );
	EXPECT_FALSE( d7.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( d7 )::Signature, bc::u32( u32 )> ) );


	auto a8 = Function( &TestFunction_void_void );
	EXPECT_FALSE( a8.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( a8 )::Signature, void()> ) );

	auto b8 = Function( &TestFunction_void_u32 );
	EXPECT_FALSE( b8.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( b8 )::Signature, void( u32 )> ) );

	auto c8 = Function( &TestFunction_u32_void );
	EXPECT_FALSE( c8.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( c8 )::Signature, u32()> ) );

	auto d8 = Function( &TestFunction_u32_u32 );
	EXPECT_FALSE( d8.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( d8 )::Signature, bc::u32( bc::u32 )> ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectInit )
{
	using namespace bc;

	struct Callable_void_void { void operator()() {} } callable_void_void;
	struct Callable_void_u32 { void operator()( u32 value ) {} } callable_void_u32;
	struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
	struct Callable_u32_u32 { u32 operator()( u32 value ) { return value; } } callable_u32_u32;


	auto a1 = Function<void()>( callable_void_void );
	EXPECT_FALSE( a1.IsEmpty() );

	auto b1 = Function<void( bc::u32 )>( callable_void_u32 );
	EXPECT_FALSE( b1.IsEmpty() );

	auto c1 = Function<bc::u32()>( callable_u32_void );
	EXPECT_FALSE( c1.IsEmpty() );

	auto d1 = Function<bc::u32( bc::u32 )>( callable_u32_u32 );
	EXPECT_FALSE( d1.IsEmpty() );


	Function<void()> a3 = callable_void_void;
	EXPECT_FALSE( a3.IsEmpty() );

	Function<void( bc::u32 )> b3 = callable_void_u32;
	EXPECT_FALSE( b3.IsEmpty() );

	Function<bc::u32()> c3 = callable_u32_void;
	EXPECT_FALSE( c3.IsEmpty() );

	Function<bc::u32( bc::u32 )> d3 = callable_u32_u32;
	EXPECT_FALSE( d3.IsEmpty() );


	auto a4 = Function( callable_void_void );
	EXPECT_FALSE( a4.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( a4 )::Signature, void()> ) );

	auto b4 = Function( callable_void_u32 );
	EXPECT_FALSE( b4.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( b4 )::Signature, void( u32 )> ) );

	auto c4 = Function( callable_u32_void );
	EXPECT_FALSE( c4.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( c4 )::Signature, u32()> ) );

	auto d4 = Function( callable_u32_u32 );
	EXPECT_FALSE( d4.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( d4 )::Signature, u32( u32 )> ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaInit )
{
	using namespace bc;

	auto test = std::function( []() {} );
	auto test_size = sizeof( test );

	auto a1 = Function<void()>( []() {} );
	EXPECT_FALSE( a1.IsEmpty() );

	auto b1 = Function<void( u32 )>( []( u32 value ) {} );
	EXPECT_FALSE( b1.IsEmpty() );

	auto c1 = Function<u32()>( []() { return 0; } );
	EXPECT_FALSE( c1.IsEmpty() );

	auto d1 = Function<u32( u32 )>( []( u32 value ) { return value; } );
	EXPECT_FALSE( d1.IsEmpty() );


	Function<void()> a2 = []() {};
	EXPECT_FALSE( a2.IsEmpty() );

	Function<void( u32 )> b2 = []( u32 value ) {};
	EXPECT_FALSE( b2.IsEmpty() );

	Function<u32()> c2 = []() { return 0; };
	EXPECT_FALSE( c2.IsEmpty() );

	Function<u32( bc::u32 )> d2 = []( u32 value ) { return value; };
	EXPECT_FALSE( d2.IsEmpty() );

	auto a3 = Function( []() {} );
	EXPECT_FALSE( a3.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( a3 )::Signature, void()> ) );

	auto b3 = Function( []( bc::u32 value ) {} );
	EXPECT_FALSE( b3.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( b3 )::Signature, void( u32 )> ) );

	auto c3 = Function( []() { return u32( 0 ); } );
	EXPECT_FALSE( c3.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( c3 )::Signature, u32()> ) );

	auto d3 = Function( []( bc::u32 value ) { return value; } );
	EXPECT_FALSE( d3.IsEmpty() );
	EXPECT_TRUE( ( std::is_same_v<decltype( d3 )::Signature, u32( u32 )> ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, Clear )
{
	using namespace bc;

	using A = Function<void()>;

	{
		auto a = A();
		EXPECT_TRUE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );
	}
	{
		auto a = A( TestFunction_void_void );
		EXPECT_FALSE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );
	}
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = A( callable_void_void );
		EXPECT_FALSE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );
	}
	{
		auto a = A( []() {} );
		EXPECT_FALSE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, FunctionCopyConstruct )
{
	using namespace bc;

	{
		auto a = Function( TestFunction_void_void );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_void_u32 );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_void );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_u32 );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, FunctionCopyOperator )
{
	using namespace bc;

	{
		auto a = Function( TestFunction_void_void );
		auto b = Function<void()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_void_u32 );
		auto b = Function<void( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_void );
		auto b = Function<u32()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_u32 );
		auto b = Function<u32( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectCopyConstruct )
{
	using namespace bc;
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = Function( callable_void_void );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_void_u32 { void operator()( u32 ) {} } callable_void_u32;
		auto a = Function( callable_void_u32 );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
		auto a = Function( callable_u32_void );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_u32 { u32 operator()( u32 ) { return 0; } } callable_u32_u32;
		auto a = Function( callable_u32_u32 );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectCopyOperator )
{
	using namespace bc;
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = Function( callable_void_void );
		auto b = Function<void()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_void_u32 { void operator()( u32 ) {} } callable_void_u32;
		auto a = Function( callable_void_u32 );
		auto b = Function<void( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
		auto a = Function( callable_u32_void );
		auto b = Function<u32()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_u32 { u32 operator()( u32 ) { return 0; } } callable_u32_u32;
		auto a = Function( callable_u32_u32 );
		auto b = Function<u32( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaCopyConstruct )
{
	using namespace bc;

	{
		auto a = Function( []() {} );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( u32 value ) {} );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []() { return u32( 0 ); } );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( u32 value ) { return value; } );
		auto b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaCopyOperator )
{
	using namespace bc;

	{
		auto a = Function( []() {} );
		auto b = Function<void()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( u32 value ) {} );
		auto b = Function<void( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []() { return u32( 0 ); } );
		auto b = Function<u32()>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( u32 value ) { return value; } );
		auto b = Function<u32( u32 )>();
		b = a;
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, RegularFunctionMoveConstruct )
{
	using namespace bc;

	{
		auto a = Function( TestFunction_void_void );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_void_u32 );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_void );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_u32 );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, RegularFunctionMoveOperator )
{
	using namespace bc;

	{
		auto a = Function( TestFunction_void_void );
		auto b = Function<void()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_void_u32 );
		auto b = Function<void( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_void );
		auto b = Function<u32()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_u32_u32 );
		auto b = Function<u32( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectMoveConstruct )
{
	using namespace bc;
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = Function( callable_void_void );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_void_u32 { void operator()() {} } callable_void_u32;
		auto a = Function( callable_void_u32 );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
		auto a = Function( callable_u32_void );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_u32 { u32 operator()( u32 value ) { return value; } } callable_u32_u32;
		auto a = Function( callable_u32_u32 );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectMoveOperator )
{
	using namespace bc;
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = Function( callable_void_void );
		auto b = Function<void()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_void_u32 { void operator()( u32 ) {} } callable_void_u32;
		auto a = Function( callable_void_u32 );
		auto b = Function<void( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
		auto a = Function( callable_u32_void );
		auto b = Function<u32()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		struct Callable_u32_u32 { u32 operator()( u32 value ) { return value; } } callable_u32_u32;
		auto a = Function( callable_u32_u32 );
		auto b = Function<u32( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaMoveConstruct )
{
	using namespace bc;

	{
		auto a = Function( []() {} );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( bc::u32 value ) {} );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []() { return u32( 0 ); } );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( bc::u32 value ) { return value; } );
		auto b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaMoveOperator )
{
	using namespace bc;

	{
		auto a = Function( []() {} );
		auto b = Function<void()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( bc::u32 value ) {} );
		auto b = Function<void( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []() { return u32( 0 ); } );
		auto b = Function<u32()>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = Function( []( bc::u32 value ) { return value; } );
		auto b = Function<u32( u32 )>();
		b = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallNoneExistingRegularFunction )
{
	using namespace bc;

	{
		auto a = Function<void()>();
		EXPECT_THROW( a(), diagnostic::Exception );
	}
	{
		auto a = Function<void(u32)>();
		EXPECT_THROW( a( 0 ), diagnostic::Exception );
	}
	{
		auto a = Function<u32()>();
		EXPECT_THROW( a(), diagnostic::Exception );
	}
	{
		auto a = Function<u32(u32)>();
		EXPECT_THROW( a( 0 ), diagnostic::Exception );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallNormalFunction )
{
	using namespace bc;

	{
		auto a = Function( TestFunction_void_void );
		EXPECT_NO_THROW( a() );
	}
	{
		auto a = Function( TestFunction_void_u32 );
		EXPECT_NO_THROW( a( 0 ) );
	}
	{
		auto a = Function( TestFunction_u32_void );
		EXPECT_NO_THROW( a() );
	}
	{
		auto a = Function( TestFunction_u32_u32 );
		auto b = a( 15 );
		EXPECT_EQ( b, 15 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallCallableObject )
{
	using namespace bc;
	{
		struct Callable_void_void { void operator()() {} } callable_void_void;
		auto a = Function( callable_void_void );
		EXPECT_NO_THROW( a() );
	}
	{
		struct Callable_void_u32 { void operator()( u32 value ) {} } callable_void_u32;
		auto a = Function( callable_void_u32 );
		EXPECT_NO_THROW( a( 0 ) );
	}
	{
		struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
		auto a = Function( callable_u32_void );
		EXPECT_NO_THROW( a() );
	}
	{
		struct Callable_u32_u32 { u32 operator()( u32 value ) { return value; } } callable_u32_u32;
		auto a = Function( callable_u32_u32 );
		auto b = a( 15 );
		EXPECT_EQ( b, 15 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallLambda )
{
	using namespace bc;
	{
		auto a = Function( []() {} );
		EXPECT_NO_THROW( a() );
	}
	{
		auto a = Function( []( bc::u32 value ) {} );
		EXPECT_NO_THROW( a( 0 ) );
	}
	{
		auto a = Function( []() { return u32( 0 ); } );
		EXPECT_NO_THROW( a() );
	}
	{
		auto a = Function( []( bc::u32 value ) { return value; } );
		auto b = a( 15 );
		EXPECT_EQ( b, 15 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, InitHeapAllocatedFunctor )
{
	using namespace bc;
	{
		u64 t1, t2;
		// The functor should be stored in the stack, because the capture list is smaller or equal to the Function local storage.
		auto a = Function( [ & ]( u64 value ) { t1 = value; t2 = value; } );
		EXPECT_TRUE( a.IsStoredLocally() );
	}
	{
		u64 t1, t2, t3;
		// The functor should be stored in the heap, because the capture list is larger than the Function local storage.
		auto a = Function( [ & ]( u64 value ) { t1 = value; t2 = value; t3 = value; } );
		EXPECT_FALSE( a.IsEmpty() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallHeapAllocatedFunctor )
{
	using namespace bc;
	{
		u64 t1, t2;
		auto a = Function( [ & ]( u64 value ) { t1 = value; t2 = value; } );
		EXPECT_NO_THROW( a( 42 ) );
		EXPECT_EQ( t1, 42 );
		EXPECT_EQ( t2, 42 );
	}
	{
		u64 t1, t2, t3;
		auto a = Function( [ & ]( u64 value ) { t1 = value; t2 = value; t3 = value; } );
		EXPECT_NO_THROW( a( 42 ) );
		EXPECT_EQ( t1, 42 );
		EXPECT_EQ( t2, 42 );
		EXPECT_EQ( t3, 42 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, NonTriviallyCopyableFunctor )
{
	using namespace bc;
	{
		struct Callable_NonTriviallyCopyable
		{
			Callable_NonTriviallyCopyable() = default;
			Callable_NonTriviallyCopyable( const Callable_NonTriviallyCopyable & ) {}
			Callable_NonTriviallyCopyable( Callable_NonTriviallyCopyable && ) = default;
			Callable_NonTriviallyCopyable & operator=( const Callable_NonTriviallyCopyable & ) { return *this; }
			Callable_NonTriviallyCopyable & operator=( Callable_NonTriviallyCopyable && ) = default;
			void operator()() {}
		} callable_non_trivially_copyable;
		static_assert( !std::is_trivially_copyable_v<Callable_NonTriviallyCopyable> );

		// The functor should be stored in the heap, because this is a non-trivially copyable type.
		auto a = Function( callable_non_trivially_copyable );
		EXPECT_FALSE( a.IsStoredLocally() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, FunctorConstructorCalls )
{
	using namespace bc;
	{
		struct Callable_ConstructorCallsDetection
		{
			i64 value = 0;
			Callable_ConstructorCallsDetection() { ++value; };
			Callable_ConstructorCallsDetection( const Callable_ConstructorCallsDetection & ) { value += 10; }
			Callable_ConstructorCallsDetection( Callable_ConstructorCallsDetection && ) { value += 1000; };
			Callable_ConstructorCallsDetection & operator=( const Callable_ConstructorCallsDetection & ) { value += 10000; return *this; }
			Callable_ConstructorCallsDetection & operator=( Callable_ConstructorCallsDetection && ) { value += 100000; return *this; }
			i64 operator()() { return value; }
		};

		Callable_ConstructorCallsDetection callable_constructor_calls_detection;
		EXPECT_EQ( callable_constructor_calls_detection(), 1 );

		auto a = Function( callable_constructor_calls_detection );
		auto test = a();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, ThrowDuringCall )
{
	using namespace bc;
	{
		auto a = Function( []() { throw 1; } );
		EXPECT_THROW( a(), int );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, ThrowDuringFunctorCopyConstruction )
{
	using namespace bc;
	{
		struct Callable_ThrowDuringFunctorCopyConstruction
		{
			Callable_ThrowDuringFunctorCopyConstruction() = default;
			Callable_ThrowDuringFunctorCopyConstruction( const Callable_ThrowDuringFunctorCopyConstruction & ) { throw int( 1 ); }
			Callable_ThrowDuringFunctorCopyConstruction( Callable_ThrowDuringFunctorCopyConstruction && ) { throw float( 2.0f ); };
			void operator()() {}
		} callable_throw_during_functor_copy_construction;
		static_assert( !std::is_trivially_copyable_v<Callable_ThrowDuringFunctorCopyConstruction> );

		auto a = Function( callable_throw_during_functor_copy_construction );
		EXPECT_THROW( auto b = a, int );


		// TEST:
		auto std_a = std::function( callable_throw_during_functor_copy_construction );
		EXPECT_THROW( auto std_b = std_a, int );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, ThrowDuringFunctorCopyAssignment )
{
	using namespace bc;
	{
		struct Callable_ThrowDuringFunctorCopyAssignment
		{
			Callable_ThrowDuringFunctorCopyAssignment() = default;
			Callable_ThrowDuringFunctorCopyAssignment( const Callable_ThrowDuringFunctorCopyAssignment & ) = default;
			Callable_ThrowDuringFunctorCopyAssignment( Callable_ThrowDuringFunctorCopyAssignment && ) = default;
			Callable_ThrowDuringFunctorCopyAssignment & operator=( const Callable_ThrowDuringFunctorCopyAssignment & ) { throw 1; }
			Callable_ThrowDuringFunctorCopyAssignment & operator=( Callable_ThrowDuringFunctorCopyAssignment && ) { throw 2; };
			void operator()() {}
		} callable_throw_during_functor_copy_assignment;
		static_assert( !std::is_trivially_copyable_v<Callable_ThrowDuringFunctorCopyAssignment> );

		auto a = Function( callable_throw_during_functor_copy_assignment );
		auto b = Function<decltype( a )::Signature>();
		// No copy or move operator should be called for a functor.
		EXPECT_NO_THROW( b = a );
	}
}



} // containers
} // core
