
#include <gtest/gtest.h>
#include <TestsCommon.hpp>

#include <core/containers/simple/SimpleLinearContainer.hpp>
#include <core/assertion/Assert.hpp>



namespace containers {
namespace array {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, BasicInit )
{
	using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
	A a;
	EXPECT_EQ( a.Size(), 0 );

	A b {};
	EXPECT_EQ( b.Size(), 0 );

	A c = {};
	EXPECT_EQ( c.Size(), 0 );

	A d = A();
	EXPECT_EQ( d.Size(), 0 );

	A e = A {};
	EXPECT_EQ( e.Size(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, ConstructWithSize )
{
	using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
	A a;
	EXPECT_EQ( a.Size(), 0 );

	A b( 1 );
	EXPECT_EQ( b.Size(), 1 );

	A c { 5, 10 };
	EXPECT_EQ( c.Size(), 2 );

	A d = { 5, 10, 20 };
	EXPECT_EQ( d.Size(), 3 );

	A e = A( 4 );
	EXPECT_EQ( e.Size(), 4 );

	A f = A { 5, 10, 20, 50, 100 };
	EXPECT_EQ( f.Size(), 5 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, IndexOperator )
{
	using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
	A a { 5, 10, 20 };

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );

	ASSERT_DEBUG_DEATH( auto test = a[ 3 ], "" );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, FrontBack )
{
	using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
	{
		A a;

		ASSERT_DEBUG_DEATH( auto t = a.Front(), "" );
		ASSERT_DEBUG_DEATH( auto t = a.Back(), "" );
	}
	{
		A b = { 5 };
		EXPECT_EQ( b.Front(), 5 );
		EXPECT_EQ( b.Back(), 5 );

		A c = { 5, 10 };
		EXPECT_EQ( c.Front(), 5 );
		EXPECT_EQ( c.Back(), 10 );

		A d = { 5, 10, 15 };
		EXPECT_EQ( d.Front(), 5 );
		EXPECT_EQ( d.Back(), 15 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PushObject
{
public:
	PushObject() noexcept = default;
	PushObject( size_t value ) noexcept : value( value ) {}
	PushObject( const PushObject & other ) noexcept = default;
	PushObject( PushObject && other ) noexcept = default;

	PushObject & operator=( const PushObject & other ) noexcept = default;
	PushObject & operator=( PushObject && other ) noexcept = default;

	bool operator==( const PushObject & other ) const noexcept { return value == other.value; }

	size_t value = 0;
};

static_assert( std::is_nothrow_constructible_v<PushObject, size_t>, "Test object must be nothrow constructible" );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, PushBack )
{
	using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
	using B = qup::_internal::SimpleLinearContainerBase<PushObject>;

	A a;
	a.PushBack( 5 );
	a.PushBack( 10 );
	a.PushBack( 20 );

	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );

	B b;
	b.PushBack( 5 );
	b.PushBack( 10 );
	b.PushBack( 20 );

	EXPECT_EQ( b[ 0 ], 5 );
	EXPECT_EQ( b[ 1 ], 10 );
	EXPECT_EQ( b[ 2 ], 20 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::_internal::SimpleLinearContainerBase<Simple>;
	A a;
	A b {};
	A c = {};
	A d = qup::_internal::SimpleLinearContainerBase<Simple>();
	A e = qup::_internal::SimpleLinearContainerBase<Simple> {};

	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 0 );
	EXPECT_EQ( d.Size(), 0 );
	EXPECT_EQ( e.Size(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, StructureInitializerListInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::_internal::SimpleLinearContainerBase<Simple>;
	A a { { 5, 10, 20.0f, 50.0 }, {} };

	EXPECT_EQ( a.Size(), 2 );

	EXPECT_EQ( a[ 0 ].v1, 5 );
	EXPECT_EQ( a[ 0 ].v2, 10 );
	EXPECT_FLOAT_EQ( a[ 0 ].v3, 20.0f );
	EXPECT_DOUBLE_EQ( a[ 0 ].v4, 50.0 );

	EXPECT_EQ( a[ 1 ].v1, 0 );
	EXPECT_EQ( a[ 1 ].v2, 0 );
	EXPECT_FLOAT_EQ( a[ 1 ].v3, 0.0f );
	EXPECT_DOUBLE_EQ( a[ 1 ].v4, 0.0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::_internal::SimpleLinearContainerBase<Simple>;
	A a { { 5, 10, 20.0f, 50.0 }, {} };
	A b = a;

	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( b.Size(), 2 );

	EXPECT_EQ( a[ 0 ].v1, b[ 0 ].v1 );
	EXPECT_EQ( a[ 0 ].v2, b[ 0 ].v2 );
	EXPECT_FLOAT_EQ( a[ 0 ].v3, b[ 0 ].v3 );
	EXPECT_DOUBLE_EQ( a[ 0 ].v4, b[ 0 ].v4 );

	EXPECT_EQ( a[ 1 ].v1, b[ 1 ].v1 );
	EXPECT_EQ( a[ 1 ].v2, b[ 1 ].v2 );
	EXPECT_FLOAT_EQ( a[ 1 ].v3, b[ 1 ].v3 );
	EXPECT_DOUBLE_EQ( a[ 1 ].v4, b[ 1 ].v4 );

	b[ 0 ].v1 = 600;
	EXPECT_EQ( a[ 0 ].v1, 5 );
	EXPECT_EQ( b[ 0 ].v1, 600 );

	A c { { 700 } };
	c = b;
	EXPECT_EQ( b[ 0 ].v1, c[ 0 ].v1 );
	EXPECT_EQ( b[ 0 ].v2, c[ 0 ].v2 );
	EXPECT_FLOAT_EQ( b[ 0 ].v3, c[ 0 ].v3 );
	EXPECT_DOUBLE_EQ( b[ 0 ].v4, c[ 0 ].v4 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::_internal::SimpleLinearContainerBase<Simple>;
	A a1 { { 5, 10, 20.0f, 50.0 }, {} };
	A a2 = a1;
	A b1 = std::move( a1 );
	A b2; b2 = std::move( a2 );

	EXPECT_EQ( a1.Size(), 0 );
	EXPECT_EQ( a2.Size(), 0 );
	EXPECT_EQ( b1.Size(), 2 );
	EXPECT_EQ( b2.Size(), 2 );

	EXPECT_EQ( b1[ 0 ].v1, 5 );
	EXPECT_EQ( b1[ 0 ].v2, 10 );
	EXPECT_FLOAT_EQ( b1[ 0 ].v3, 20.0f );
	EXPECT_DOUBLE_EQ( b1[ 0 ].v4, 50.0 );

	EXPECT_EQ( b2[ 0 ].v1, 5 );
	EXPECT_EQ( b2[ 0 ].v2, 10 );
	EXPECT_FLOAT_EQ( b2[ 0 ].v3, 20.0f );
	EXPECT_DOUBLE_EQ( b2[ 0 ].v4, 50.0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, MoveableOnlyStructure )
{
	struct MoveableOnly
	{
		MoveableOnly() noexcept = default;
		MoveableOnly( size_t value ) noexcept : v1( value ) {};
		MoveableOnly( const MoveableOnly & other ) noexcept = delete;
		MoveableOnly( MoveableOnly && other ) noexcept = default;
		MoveableOnly & operator=( const MoveableOnly & other ) noexcept = delete;
		MoveableOnly & operator=( MoveableOnly && other ) noexcept = default;
		size_t v1 = {};
	};

	using A = qup::_internal::SimpleLinearContainerBase<MoveableOnly>;
	A a;
	a.PushBack( MoveableOnly( 5 ) );
	a.PushBack( MoveableOnly( 10 ) );
	a.PushBack( MoveableOnly( 20 ) );

	// Expecting compile time error for this:
	// A b = a;

	A b = std::move( a );
	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 3 );

	A c; c = std::move( b );
	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 3 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleLinearContainer_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	SimpleLinearContainer_CopyableOnly_Control() noexcept = default;
	SimpleLinearContainer_CopyableOnly_Control( const SimpleLinearContainer_CopyableOnly_Control & other ) noexcept { ++copy_counter; }
	SimpleLinearContainer_CopyableOnly_Control( SimpleLinearContainer_CopyableOnly_Control && other ) noexcept { ++move_counter; }

	SimpleLinearContainer_CopyableOnly_Control & operator=( const SimpleLinearContainer_CopyableOnly_Control & other ) noexcept { ++copy_counter; }
	SimpleLinearContainer_CopyableOnly_Control & operator=( SimpleLinearContainer_CopyableOnly_Control && other ) noexcept { ++move_counter; }
};
int32_t SimpleLinearContainer_CopyableOnly_Control::copy_counter		= 0;
int32_t SimpleLinearContainer_CopyableOnly_Control::move_counter		= 0;

class SimpleLinearContainer_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	SimpleLinearContainer_CopyableOnly() noexcept = default;
	SimpleLinearContainer_CopyableOnly( const SimpleLinearContainer_CopyableOnly & other ) noexcept { ++copy_counter; }
	SimpleLinearContainer_CopyableOnly( SimpleLinearContainer_CopyableOnly && other ) noexcept = delete;

	SimpleLinearContainer_CopyableOnly & operator=( const SimpleLinearContainer_CopyableOnly & other ) noexcept { ++copy_counter; }
	SimpleLinearContainer_CopyableOnly & operator=( SimpleLinearContainer_CopyableOnly && other ) noexcept = delete;
};
int32_t SimpleLinearContainer_CopyableOnly::copy_counter		= 0;

TEST( CoreContainerSimpleLinearContainer, SimpleLinearContainer_CopyableOnlyStructure )
{
	SimpleLinearContainer_CopyableOnly_Control::copy_counter	= 0;
	SimpleLinearContainer_CopyableOnly_Control::move_counter	= 0;

	SimpleLinearContainer_CopyableOnly::copy_counter			= 0;

	using A = qup::_internal::SimpleLinearContainerBase<SimpleLinearContainer_CopyableOnly>;
	using B = qup::_internal::SimpleLinearContainerBase<SimpleLinearContainer_CopyableOnly_Control>;
	A a;
	a.PushBack( SimpleLinearContainer_CopyableOnly() );
	a.PushBack( SimpleLinearContainer_CopyableOnly() );
	a.PushBack( SimpleLinearContainer_CopyableOnly() );

	B ac;
	ac.PushBack( SimpleLinearContainer_CopyableOnly_Control() );
	ac.PushBack( SimpleLinearContainer_CopyableOnly_Control() );
	ac.PushBack( SimpleLinearContainer_CopyableOnly_Control() );

	A b = a;
	B bc = ac;

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( b.Size(), 3 );

	EXPECT_EQ( ac.Size(), 3 );
	EXPECT_EQ( bc.Size(), 3 );

	A c = std::move( b );
	B cc = std::move( bc );

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 3 );

	EXPECT_EQ( ac.Size(), 3 );
	EXPECT_EQ( bc.Size(), 0 );
	EXPECT_EQ( cc.Size(), 3 );

	EXPECT_GT( SimpleLinearContainer_CopyableOnly::copy_counter, SimpleLinearContainer_CopyableOnly_Control::copy_counter );
	EXPECT_EQ( SimpleLinearContainer_CopyableOnly::copy_counter, SimpleLinearContainer_CopyableOnly_Control::copy_counter + SimpleLinearContainer_CopyableOnly_Control::move_counter );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleLinearContainer_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	SimpleLinearContainer_CtorDtorCounted() noexcept { ++constructed_counter; data = constructed_counter; }
	SimpleLinearContainer_CtorDtorCounted( const SimpleLinearContainer_CtorDtorCounted & other ) noexcept { ++constructed_counter; data = constructed_counter; }
	SimpleLinearContainer_CtorDtorCounted( SimpleLinearContainer_CtorDtorCounted && other ) noexcept { ++constructed_counter; data = constructed_counter; }
	~SimpleLinearContainer_CtorDtorCounted() noexcept { --constructed_counter; }
	SimpleLinearContainer_CtorDtorCounted & operator=( const SimpleLinearContainer_CtorDtorCounted & other ) noexcept = default;
	SimpleLinearContainer_CtorDtorCounted & operator=( SimpleLinearContainer_CtorDtorCounted && other ) noexcept = default;
};
int32_t SimpleLinearContainer_CtorDtorCounted::constructed_counter	= 0;

TEST( CoreContainerSimpleLinearContainer, CtorDtorCounter )
{
	SimpleLinearContainer_CtorDtorCounted::constructed_counter		= 0;

	using A = qup::_internal::SimpleLinearContainerBase<SimpleLinearContainer_CtorDtorCounted>;
	A a;

	a.PushBack( {} );
	a.PushBack( SimpleLinearContainer_CtorDtorCounted() );
	a.PushBack( SimpleLinearContainer_CtorDtorCounted {} );
	EXPECT_EQ( a.Size(), 3 );

	// a.PushFront( {} );
	// a.PushFront( SimpleLinearContainer_CtorDtorCounted() );
	// a.PushFront( SimpleLinearContainer_CtorDtorCounted {} );
	// EXPECT_EQ( a.Size(), 6 );

	a.EmplaceBack();
	// a.EmplaceFront();
	// EXPECT_EQ( a.Size(), 8 );
	EXPECT_EQ( a.Size(), 4 );

	a.PopBack();
	// EXPECT_EQ( a.Size(), 7 );
	EXPECT_EQ( a.Size(), 3 );

	// a.PopFront();
	// EXPECT_EQ( a.Size(), 6 );

	a.Resize( 50 );
	EXPECT_EQ( a.Size(), 50 );

	// a.Erase( a.begin() );
	// EXPECT_EQ( a.Size(), 49 );
	//
	// a.Erase( a.end() - 1 );
	// EXPECT_EQ( a.Size(), 48 );
	//
	// a.Erase( a.begin() + 10 );
	// EXPECT_EQ( a.Size(), 47 );
	//
	// a.Erase( a.begin(), a.end() - 5 );
	// EXPECT_EQ( a.Size(), 5 );

	a.Clear();

	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( SimpleLinearContainer_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerSimpleLinearContainer, SelfAssignment )
{
	{
		using A = qup::_internal::SimpleLinearContainerBase<uint32_t>;
		using AView = A::ViewType;

		A original { 5, 10, 20, 50, 200 };
		{
			A a = original;
			a = a;
			EXPECT_EQ( a.Size(), 5 );
			EXPECT_EQ( a, original );
		}
		{
			A a = original;
			a = AView( a );
			EXPECT_EQ( a.Size(), 5 );
			EXPECT_EQ( a, original );
		}
		{
			A a = original;
			a = AView( a.Data() + 1, a.Size() - 2 );

			A comp { 10, 20, 50 };
			EXPECT_EQ( a.Size(), 3 );
			EXPECT_EQ( a, comp );
		}
		{
			A a = original;
			a.Append( a );

			A comp { 5, 10, 20, 50, 200, 5, 10, 20, 50, 200 };
			EXPECT_EQ( a.Size(), 10 );
			EXPECT_EQ( a, comp );
		}
	}
	{
		SimpleLinearContainer_CtorDtorCounted::constructed_counter		= 0;

		using A = qup::_internal::SimpleLinearContainerBase<SimpleLinearContainer_CtorDtorCounted>;
		using AView = A::ViewType;
		A a( 5 );
		a.Resize( 10 );
		a.Reserve( 50 );
		a.PushBack( {} );
		for( size_t i = 0; i < 5; i++ ) {
			a.PushBack( SimpleLinearContainer_CtorDtorCounted() );
			a = a;
			a.Append( a );
		}
		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( SimpleLinearContainer_CtorDtorCounted::constructed_counter, 0 );
	}
}



} // array
} // containers
