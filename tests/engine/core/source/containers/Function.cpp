
#include <gtest/gtest.h>

#include <core/containers/Function.hpp>
#include <functional>



namespace core {
namespace containers {



static void FunctionContainer_Function_Simple() {}
struct FunctionContainer_Struct_Simple { float Function() {} };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, BasicInit )
{
	using A = bc::Function<void()>;

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
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, AssignFunction )
{
	using A = bc::Function<decltype( FunctionContainer_Function_Simple )>;

	A a1( &FunctionContainer_Function_Simple );
	EXPECT_FALSE( a1.IsEmpty() );

	A b1 { &FunctionContainer_Function_Simple };
	EXPECT_FALSE( b1.IsEmpty() );

	A c1 = &FunctionContainer_Function_Simple;
	EXPECT_FALSE( c1.IsEmpty() );

	A d1 = A( &FunctionContainer_Function_Simple );
	EXPECT_FALSE( d1.IsEmpty() );

	A e1 = A { &FunctionContainer_Function_Simple };
	EXPECT_FALSE( e1.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, AssignMemberFunction )
{
	using A = bc::Function<decltype( &FunctionContainer_Struct_Simple::Function )>;

	A a1( &FunctionContainer_Struct_Simple::Function );
	EXPECT_FALSE( a1.IsEmpty() );

	A b1 { &FunctionContainer_Struct_Simple::Function };
	EXPECT_FALSE( b1.IsEmpty() );

	A c1 = &FunctionContainer_Struct_Simple::Function;
	EXPECT_FALSE( c1.IsEmpty() );

	A d1 = A( &FunctionContainer_Struct_Simple::Function );
	EXPECT_FALSE( d1.IsEmpty() );

	A e1 = A { &FunctionContainer_Struct_Simple::Function };
	EXPECT_FALSE( e1.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, AssignCallableObject )
{
	struct { void operator()() {} } callable;

	using A = bc::Function<decltype( callable )>;

	A a1( callable );
	EXPECT_FALSE( a1.IsEmpty() );

	A b1 { callable };
	EXPECT_FALSE( b1.IsEmpty() );

	A c1 = callable;
	EXPECT_FALSE( c1.IsEmpty() );

	A d1 = A( callable );
	EXPECT_FALSE( d1.IsEmpty() );

	A e1 = A { callable };
	EXPECT_FALSE( e1.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, AssignLambda )
{
	auto simple_lambda = []() {};

	using A = bc::Function<decltype( simple_lambda )>;
	//using A = bc::Function<void()>;

	A a1( simple_lambda );
	EXPECT_FALSE( a1.IsEmpty() );

	A b1 { simple_lambda };
	EXPECT_FALSE( b1.IsEmpty() );

	A c1 = simple_lambda;
	EXPECT_FALSE( c1.IsEmpty() );

	A d1 = A( simple_lambda );
	EXPECT_FALSE( d1.IsEmpty() );

	A e1 = A { simple_lambda };
	EXPECT_FALSE( e1.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, Clear )
{
	struct Class { void MemberFunction() {} };
	struct { void operator()() {} } callable_object;
	auto lambda = []() {};
	using A = bc::Function<decltype( FunctionContainer_Function_Simple )>;
	using B = bc::Function<decltype( &Class::MemberFunction )>;
	using C = bc::Function<decltype( lambda )>;
	using D = bc::Function<decltype( callable_object )>;

	{
		A f = &FunctionContainer_Function_Simple;
		EXPECT_FALSE( f.IsEmpty() );

		f.Clear();
		EXPECT_TRUE( f.IsEmpty() );
	}
	{
		B f = &Class::MemberFunction;
		EXPECT_FALSE( f.IsEmpty() );

		f.Clear();
		EXPECT_TRUE( f.IsEmpty() );
	}
	{
		C f = lambda;
		EXPECT_FALSE( f.IsEmpty() );

		f.Clear();
		EXPECT_TRUE( f.IsEmpty() );
	}
	{
		D f = callable_object;
		EXPECT_FALSE( f.IsEmpty() );

		f.Clear();
		EXPECT_TRUE( f.IsEmpty() );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, Copy )
{
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, Move )
{
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( FunctionContainer, SelfAssignment )
{
};



} // containers
} // core
