
#include <gtest/gtest.h>

#include <core/containers/Function.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <functional>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Basic test functions.
void TestFunction_void_void() {};
void TestFunction_void_u32( bc::u32 value ) {};
bc::u32 TestFunction_u32_void() { return 0; };
bc::u32 TestFunction_u32_u32( bc::u32 value ) { return value; };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, BasicInit )
{
	using namespace bc;

	using A = Function<void()>;

	A a;
	EXPECT_TRUE( a.IsEmpty() );

	A b {};
	EXPECT_TRUE( b.IsEmpty() );

	A c = {};
	EXPECT_TRUE( c.IsEmpty() );

	A d = A();
	EXPECT_TRUE( d.IsEmpty() );

	A e = A {};
	EXPECT_TRUE( e.IsEmpty() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, RegularFunctionInit )
{
	using namespace bc;

	{
		auto a = Function<void()>( TestFunction_void_void );
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( u32 )>( TestFunction_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<u32()>( TestFunction_u32_void );
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<u32( u32 )>( TestFunction_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function<void()>( &TestFunction_void_void );
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( u32 )>( &TestFunction_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<u32()>( &TestFunction_u32_void );
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<u32( u32 )>( &TestFunction_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function<void()>{ TestFunction_void_void };
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( u32 )>{ TestFunction_void_u32 };
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<u32()>{ TestFunction_u32_void };
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<u32( u32 )>{ TestFunction_u32_u32 };
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function<void()>{ &TestFunction_void_void };
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( u32 )>{ &TestFunction_void_u32 };
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<u32()>{ &TestFunction_u32_void };
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<u32( u32 )>{ &TestFunction_u32_u32 };
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		Function<void()> a = TestFunction_void_void;
		EXPECT_FALSE( a.IsEmpty() );

		Function<void( u32 )> b = TestFunction_void_u32;
		EXPECT_FALSE( b.IsEmpty() );

		Function<u32()> c = TestFunction_u32_void;
		EXPECT_FALSE( c.IsEmpty() );

		Function<u32( u32 )> d = TestFunction_u32_u32;
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		Function<void()> a = &TestFunction_void_void;
		EXPECT_FALSE( a.IsEmpty() );

		Function<void( u32 )> b = &TestFunction_void_u32;
		EXPECT_FALSE( b.IsEmpty() );

		Function<u32()> c = &TestFunction_u32_void;
		EXPECT_FALSE( c.IsEmpty() );

		Function<u32( u32 )> d = &TestFunction_u32_u32;
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function( TestFunction_void_void );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( a )::Signature, void()> ) );

		auto b = Function( TestFunction_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( b )::Signature, void( u32 )> ) );

		auto c = Function( TestFunction_u32_void );
		EXPECT_FALSE( c.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( c )::Signature, u32()> ) );

		auto d = Function( TestFunction_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( d )::Signature, bc::u32( u32 )> ) );
	}
	{
		auto a = Function( &TestFunction_void_void );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( a )::Signature, void()> ) );

		auto b = Function( &TestFunction_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( b )::Signature, void( u32 )> ) );

		auto c = Function( &TestFunction_u32_void );
		EXPECT_FALSE( c.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( c )::Signature, u32()> ) );

		auto d = Function( &TestFunction_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( d )::Signature, bc::u32( bc::u32 )> ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, CallableObjectInit )
{
	using namespace bc;

	struct Callable_void_void { void operator()() {} } callable_void_void;
	struct Callable_void_u32 { void operator()( u32 value ) {} } callable_void_u32;
	struct Callable_u32_void { u32 operator()() { return 0; } } callable_u32_void;
	struct Callable_u32_u32 { u32 operator()( u32 value ) { return value; } } callable_u32_u32;

	{
		auto a = Function<void()>( callable_void_void );
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( bc::u32 )>( callable_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<bc::u32()>( callable_u32_void );
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<bc::u32( bc::u32 )>( callable_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		Function<void()> a = callable_void_void;
		EXPECT_FALSE( a.IsEmpty() );

		Function<void( bc::u32 )> b = callable_void_u32;
		EXPECT_FALSE( b.IsEmpty() );

		Function<bc::u32()> c = callable_u32_void;
		EXPECT_FALSE( c.IsEmpty() );

		Function<bc::u32( bc::u32 )> d = callable_u32_u32;
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function( callable_void_void );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( a )::Signature, void()> ) );

		auto b = Function( callable_void_u32 );
		EXPECT_FALSE( b.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( b )::Signature, void( u32 )> ) );

		auto c = Function( callable_u32_void );
		EXPECT_FALSE( c.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( c )::Signature, u32()> ) );

		auto d = Function( callable_u32_u32 );
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( d )::Signature, u32( u32 )> ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, LambdaInit )
{
	using namespace bc;

	{
		auto a = Function<void()>( []() {} );
		EXPECT_FALSE( a.IsEmpty() );

		auto b = Function<void( u32 )>( []( u32 value ) {} );
		EXPECT_FALSE( b.IsEmpty() );

		auto c = Function<u32()>( []() { return 0; } );
		EXPECT_FALSE( c.IsEmpty() );

		auto d = Function<u32( u32 )>( []( u32 value ) { return value; } );
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		Function<void()> a = []() {};
		EXPECT_FALSE( a.IsEmpty() );

		Function<void( u32 )> b = []( u32 value ) {};
		EXPECT_FALSE( b.IsEmpty() );

		Function<u32()> c = []() { return 0; };
		EXPECT_FALSE( c.IsEmpty() );

		Function<u32( u32 )> d = []( u32 value ) { return value; };
		EXPECT_FALSE( d.IsEmpty() );
	}
	{
		auto a = Function( []() {} );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( a )::Signature, void()> ) );

		auto b = Function( []( u32 value ) {} );
		EXPECT_FALSE( b.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( b )::Signature, void( u32 )> ) );

		auto c = Function( []() { return u32( 0 ); } );
		EXPECT_FALSE( c.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( c )::Signature, u32()> ) );

		auto d = Function( []( u32 value ) { return value; } );
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( ( std::is_same_v<decltype( d )::Signature, u32( u32 )> ) );
	}
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
TEST( FunctionContainer, FunctorConstructorAndOperatorCallCounts )
{
	using namespace bc;
	struct Callable_ConstructorCallsDetection
	{
		i32 value = 0;
		Callable_ConstructorCallsDetection() { value += 1; };
		Callable_ConstructorCallsDetection( const Callable_ConstructorCallsDetection & other ) { value = other.value + 10; }
		Callable_ConstructorCallsDetection( Callable_ConstructorCallsDetection && other ) { value = other.value + 100; };
		Callable_ConstructorCallsDetection & operator=( const Callable_ConstructorCallsDetection & other ) { value = other.value + 1000; return *this; }
		Callable_ConstructorCallsDetection & operator=( Callable_ConstructorCallsDetection && other ) { value = other.value + 10000; return *this; }
		i32 operator()() { return value; }
	};

	{
		Callable_ConstructorCallsDetection callable_constructor_calls_detection_std;
		EXPECT_EQ( callable_constructor_calls_detection_std(), 1 );

		// Functor should be move constructed.
		auto std_a = Function( Callable_ConstructorCallsDetection() );
		EXPECT_EQ( std_a(), 101 );

		// Functor should be copy constructed.
		auto std_b = Function( callable_constructor_calls_detection_std );
		EXPECT_EQ( std_b(), 11 );

		// Functor should be move constructed.
		auto std_c = Function( std::move( callable_constructor_calls_detection_std ) );
		EXPECT_EQ( std_c(), 101 );
	}
	{
		auto std_a = Function( Callable_ConstructorCallsDetection() );
		EXPECT_EQ( std_a(), 101 );

		// Functor should not change its state when Function is move constructed.
		auto std_b = Function( std::move( std_a ) );
		EXPECT_EQ( std_b(), 101 );

		// Functor should be copy constructed when Function is copy constructed.
		auto std_c = Function( std_b );
		EXPECT_EQ( std_c(), 111 );
	}
	{
		// Functor should be copy constructed when Function is copied.
		auto std_a1 = Function( Callable_ConstructorCallsDetection() ); // Constructed and moved, Add 101
		auto std_a2 = Function( Callable_ConstructorCallsDetection() ); // Constructed and moved, Add 101
		std_a2 = std_a1; // Copy constructed, Add 10
		EXPECT_EQ( std_a2(), 111 );

		// Functor should not change its state when Function is moved.
		auto std_b1 = Function( Callable_ConstructorCallsDetection() ); // Add 101
		auto std_b2 = Function( Callable_ConstructorCallsDetection() ); // Add 101
		std_b2 = std::move( std_b1 ); // No change
		EXPECT_EQ( std_b2(), 101 );
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

	struct CopyThrow {};
	struct MoveThrow {};

	struct Callable_ThrowDuringFunctorCopyConstruction
	{
		Callable_ThrowDuringFunctorCopyConstruction() = default;
		Callable_ThrowDuringFunctorCopyConstruction( const Callable_ThrowDuringFunctorCopyConstruction & ) { throw CopyThrow {}; }
		Callable_ThrowDuringFunctorCopyConstruction( Callable_ThrowDuringFunctorCopyConstruction && ) {};
		void operator()() {}
	} callable_throw_during_functor_copy_construction;
	static_assert( !std::is_trivially_copyable_v<Callable_ThrowDuringFunctorCopyConstruction> );

	{
		EXPECT_THROW( auto a = Function( callable_throw_during_functor_copy_construction ), CopyThrow );
	}
	{
		auto a = Function<void()>();
		EXPECT_THROW( a = Function( callable_throw_during_functor_copy_construction ), CopyThrow );
	}
	{
		auto a = Function<void()>();
		auto b = Function<void()>();
		EXPECT_NO_THROW( a = b );
	}
	{
		auto a = Function<void()>();
		auto b = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		EXPECT_THROW( a = b, CopyThrow );
	}
	{
		auto a = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		auto b = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		EXPECT_THROW( a = b, CopyThrow );
	}
	{
		auto a = Function<void()>();
		auto b = Function<void()>();
		EXPECT_NO_THROW( a = std::move( b ) );
	}
	{
		auto a = Function<void()>();
		auto b = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		EXPECT_NO_THROW( a = std::move( b ) );
	}
	{
		auto a = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		auto b = Function<void()>( Callable_ThrowDuringFunctorCopyConstruction{} );
		EXPECT_NO_THROW( a = std::move( b ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, ThrowDuringFunctorCopyAssignment )
{
	using namespace bc;

	struct CopyThrow {};
	struct MoveThrow {};

	struct Callable_ThrowDuringFunctorCopyAssignment
	{
		Callable_ThrowDuringFunctorCopyAssignment() = default;
		Callable_ThrowDuringFunctorCopyAssignment( const Callable_ThrowDuringFunctorCopyAssignment & ) = default;
		Callable_ThrowDuringFunctorCopyAssignment( Callable_ThrowDuringFunctorCopyAssignment && ) = default;
		Callable_ThrowDuringFunctorCopyAssignment & operator=( const Callable_ThrowDuringFunctorCopyAssignment & ) { throw CopyThrow {}; }
		Callable_ThrowDuringFunctorCopyAssignment & operator=( Callable_ThrowDuringFunctorCopyAssignment && ) { throw MoveThrow {}; };
		void operator()() {}
	} callable_throw_during_functor_copy_assignment;
	static_assert( !std::is_trivially_copyable_v<Callable_ThrowDuringFunctorCopyAssignment> );

	{
		auto a = Function( callable_throw_during_functor_copy_assignment );
		auto b = Function<decltype( a )::Signature>();
		// No copy or move operator should be called for a functor.
		EXPECT_NO_THROW( b = a );
	}
	{
		auto a = Function( callable_throw_during_functor_copy_assignment );
		auto b = Function<decltype( a )::Signature>();
		// No copy or move operator should be called for a functor.
		EXPECT_NO_THROW( b = std::move( a ) );
	}
}



} // containers
} // core
