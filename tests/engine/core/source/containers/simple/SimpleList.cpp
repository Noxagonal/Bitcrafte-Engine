
#include <gtest/gtest.h>

#include <core/containers/simple/SimpleList.hpp>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( SimpleListContainer, BasicInit )
{
	using A = bc::SimpleList<uint32_t>;
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
TEST( SimpleListContainer, ConstructWithSize )
{
	using A = bc::SimpleList<uint32_t>;
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
TEST( SimpleListContainer, IndexOperator )
{
	using A = bc::SimpleList<uint32_t>;
	A a { 5, 10, 20 };

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );

	ASSERT_DEBUG_DEATH( auto test = a[ 3 ], "" );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( SimpleListContainer, FrontBack )
{
	using A = bc::SimpleList<uint32_t>;
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
TEST( SimpleListContainer, PushBack )
{
	using A = bc::SimpleList<uint32_t>;
	using B = bc::SimpleList<PushObject>;

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
TEST( SimpleListContainer, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::SimpleList<Simple>;
	A a;
	A b {};
	A c = {};
	A d = bc::SimpleList<Simple>();
	A e = bc::SimpleList<Simple> {};

	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 0 );
	EXPECT_EQ( d.Size(), 0 );
	EXPECT_EQ( e.Size(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( SimpleListContainer, StructureInitializerListInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::SimpleList<Simple>;
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
TEST( SimpleListContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::SimpleList<Simple>;
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
TEST( SimpleListContainer, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::SimpleList<Simple>;
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
TEST( SimpleListContainer, MoveableOnlyStructure )
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

	using A = bc::SimpleList<MoveableOnly>;
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
class SimpleList_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	SimpleList_CopyableOnly_Control() noexcept = default;
	SimpleList_CopyableOnly_Control( const SimpleList_CopyableOnly_Control & other ) noexcept { ++copy_counter; }
	SimpleList_CopyableOnly_Control( SimpleList_CopyableOnly_Control && other ) noexcept { ++move_counter; }

	SimpleList_CopyableOnly_Control & operator=( const SimpleList_CopyableOnly_Control & other ) noexcept { ++copy_counter; return *this; }
	SimpleList_CopyableOnly_Control & operator=( SimpleList_CopyableOnly_Control && other ) noexcept { ++move_counter; return *this; }
};
int32_t SimpleList_CopyableOnly_Control::copy_counter		= 0;
int32_t SimpleList_CopyableOnly_Control::move_counter		= 0;

class SimpleList_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	SimpleList_CopyableOnly() noexcept = default;
	SimpleList_CopyableOnly( const SimpleList_CopyableOnly & other ) noexcept { ++copy_counter; }
	SimpleList_CopyableOnly( SimpleList_CopyableOnly && other ) noexcept = delete;

	SimpleList_CopyableOnly & operator=( const SimpleList_CopyableOnly & other ) noexcept { ++copy_counter; return *this; }
	SimpleList_CopyableOnly & operator=( SimpleList_CopyableOnly && other ) noexcept = delete;
};
int32_t SimpleList_CopyableOnly::copy_counter		= 0;

TEST( SimpleListContainer, SimpleList_CopyableOnlyStructure )
{
	SimpleList_CopyableOnly_Control::copy_counter	= 0;
	SimpleList_CopyableOnly_Control::move_counter	= 0;

	SimpleList_CopyableOnly::copy_counter			= 0;

	using A = bc::SimpleList<SimpleList_CopyableOnly>;
	using B = bc::SimpleList<SimpleList_CopyableOnly_Control>;
	A a; a.Reserve( 16 );
	a.PushBack( SimpleList_CopyableOnly() );
	a.PushBack( SimpleList_CopyableOnly() );
	a.PushBack( SimpleList_CopyableOnly() );
	EXPECT_EQ( SimpleList_CopyableOnly::copy_counter, 3 );

	B ac; ac.Reserve( 16 );
	ac.PushBack( SimpleList_CopyableOnly_Control() );
	ac.PushBack( SimpleList_CopyableOnly_Control() );
	ac.PushBack( SimpleList_CopyableOnly_Control() );
	EXPECT_EQ( SimpleList_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( SimpleList_CopyableOnly_Control::move_counter, 3 );

	A b = a;
	EXPECT_EQ( SimpleList_CopyableOnly::copy_counter, 6 );

	B bc = ac;
	EXPECT_EQ( SimpleList_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( SimpleList_CopyableOnly_Control::move_counter, 3 );

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

	EXPECT_EQ( SimpleList_CopyableOnly::copy_counter, 6 );
	EXPECT_EQ( SimpleList_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( SimpleList_CopyableOnly_Control::move_counter, 3 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleList_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	SimpleList_CtorDtorCounted() noexcept { ++constructed_counter; data = constructed_counter; }
	SimpleList_CtorDtorCounted( const SimpleList_CtorDtorCounted & other ) noexcept { ++constructed_counter; data = constructed_counter; }
	SimpleList_CtorDtorCounted( SimpleList_CtorDtorCounted && other ) noexcept { ++constructed_counter; data = constructed_counter; }
	~SimpleList_CtorDtorCounted() noexcept { --constructed_counter; }
	SimpleList_CtorDtorCounted & operator=( const SimpleList_CtorDtorCounted & other ) noexcept = default;
	SimpleList_CtorDtorCounted & operator=( SimpleList_CtorDtorCounted && other ) noexcept = default;
};
int32_t SimpleList_CtorDtorCounted::constructed_counter	= 0;

TEST( SimpleListContainer, CtorDtorCounter )
{
	SimpleList_CtorDtorCounted::constructed_counter		= 0;

	using A = bc::SimpleList<SimpleList_CtorDtorCounted>;
	A a;

	a.PushBack( {} );
	a.PushBack( SimpleList_CtorDtorCounted() );
	a.PushBack( SimpleList_CtorDtorCounted {} );
	EXPECT_EQ( a.Size(), 3 );

	// a.PushFront( {} );
	// a.PushFront( SimpleList_CtorDtorCounted() );
	// a.PushFront( SimpleList_CtorDtorCounted {} );
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
	EXPECT_EQ( SimpleList_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( SimpleListContainer, SelfAssignment )
{
	{
		using A = bc::SimpleList<uint32_t>;
		using AView = A::ThisViewType<true>;

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
		SimpleList_CtorDtorCounted::constructed_counter		= 0;

		using A = bc::SimpleList<SimpleList_CtorDtorCounted>;
		using AView = A::ThisViewType<true>;

		A a( 5 );
		a.Resize( 10 );
		a.Reserve( 50 );
		a.PushBack( {} );
		for( size_t i = 0; i < 5; i++ )
		{
			a.PushBack( SimpleList_CtorDtorCounted() );
			a = a;
			a.Append( a );
		}
		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( SimpleList_CtorDtorCounted::constructed_counter, 0 );
	}
}



} // containers
} // core
