
#include <containers/List.hpp>

#include <gtest/gtest.h>
#include <TestsCommon.hpp>



namespace containers {
namespace list {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, BasicInit )
{
	using A = qup::List<uint32_t>;
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
TEST( CoreContainerList, ConstructWithSize )
{
	using A = qup::List<uint32_t>;
	A a;
	EXPECT_EQ( a.Size(), 0 );

	A b { 5, 10 };
	EXPECT_EQ( b.Size(), 2 );

	A c = { 5, 10, 20 };
	EXPECT_EQ( c.Size(), 3 );

	A d = A { 5, 10, 20, 50, 100 };
	EXPECT_EQ( d.Size(), 5 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, Erase )
{
	using A = qup::List<uint32_t>;
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( *( a.begin() + 0 ), 1 );
		EXPECT_EQ( *( a.begin() + 1 ), 2 );
		EXPECT_EQ( *( a.begin() + 2 ), 3 );
		EXPECT_EQ( *( a.begin() + 3 ), 4 );
		EXPECT_EQ( *( a.begin() + 4 ), 5 );
		EXPECT_EQ( *( a.begin() + 5 ), 6 );
		EXPECT_EQ( *( a.begin() + 6 ), 7 );
		EXPECT_EQ( *( a.begin() + 7 ), 8 );
		EXPECT_EQ( *( a.begin() + 8 ), 9 );

		EXPECT_THROW( a.Erase( a.end() ), qup::exception::Exception );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.end() - 1 );
		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 8 );
		EXPECT_EQ( *( a.begin() + 0 ), 1 );
		EXPECT_EQ( *( a.begin() + 1 ), 2 );
		EXPECT_EQ( *( a.begin() + 2 ), 3 );
		EXPECT_EQ( *( a.begin() + 3 ), 4 );
		EXPECT_EQ( *( a.begin() + 4 ), 5 );
		EXPECT_EQ( *( a.begin() + 5 ), 6 );
		EXPECT_EQ( *( a.begin() + 6 ), 7 );
		EXPECT_EQ( *( a.begin() + 7 ), 8 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin(), a.end() - 5 );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( *( a.begin() + 0 ), 5 );
		EXPECT_EQ( *( a.begin() + 1 ), 6 );
		EXPECT_EQ( *( a.begin() + 2 ), 7 );
		EXPECT_EQ( *( a.begin() + 3 ), 8 );
		EXPECT_EQ( *( a.begin() + 4 ), 9 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin() + 5, a.end() );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( *( a.begin() + 0 ), 0 );
		EXPECT_EQ( *( a.begin() + 1 ), 1 );
		EXPECT_EQ( *( a.begin() + 2 ), 2 );
		EXPECT_EQ( *( a.begin() + 3 ), 3 );
		EXPECT_EQ( *( a.begin() + 4 ), 4 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin() + 3, a.end() - 3 );
		EXPECT_EQ( a.Size(), 6 );
		EXPECT_EQ( *( a.begin() + 0 ), 0 );
		EXPECT_EQ( *( a.begin() + 1 ), 1 );
		EXPECT_EQ( *( a.begin() + 2 ), 2 );
		EXPECT_EQ( *( a.begin() + 3 ), 7 );
		EXPECT_EQ( *( a.begin() + 4 ), 8 );
		EXPECT_EQ( *( a.begin() + 5 ), 9 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, FrontBack )
{
	using A = qup::List<uint32_t>;

	{
		A a;

		EXPECT_THROW( auto t = a.Front(), qup::exception::Exception );
		EXPECT_THROW( auto t = a.Back(), qup::exception::Exception );
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
	PushObject( size_t value ) : value( value ) {}
	PushObject( const PushObject & other ) = default;
	PushObject( PushObject && other ) = default;

	PushObject & operator=( const PushObject & other ) = default;
	PushObject & operator=( PushObject && other ) = default;

	bool operator==( const PushObject & other ) const { return value == other.value; }

	size_t value = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, PushBack )
{
	using A = qup::List<uint32_t>;
	using B = qup::List<PushObject>;

	{
		A a;
		a.PushBack( 5 );
		a.PushBack( 10 );
		a.PushBack( 20 );

		EXPECT_EQ( *( a.begin() + 0 ), 5 );
		EXPECT_EQ( *( a.begin() + 1 ), 10 );
		EXPECT_EQ( *( a.begin() + 2 ), 20 );

		B b;
		b.PushBack( 5 );
		b.PushBack( 10 );
		b.PushBack( 20 );

		EXPECT_EQ( *( b.begin() + 0 ), 5 );
		EXPECT_EQ( *( b.begin() + 1 ), 10 );
		EXPECT_EQ( *( b.begin() + 2 ), 20 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, PushFront )
{
	using A = qup::List<uint32_t>;
	using B = qup::List<PushObject>;

	{
		A a;
		a.PushFront( 5 );
		a.PushFront( 10 );
		a.PushFront( 20 );

		EXPECT_EQ( *( a.begin() + 0 ), 20 );
		EXPECT_EQ( *( a.begin() + 1 ), 10 );
		EXPECT_EQ( *( a.begin() + 2 ), 5 );

		B b;
		b.PushFront( 5 );
		b.PushFront( 10 );
		b.PushFront( 20 );

		EXPECT_EQ( *( b.begin() + 0 ), 20 );
		EXPECT_EQ( *( b.begin() + 1 ), 10 );
		EXPECT_EQ( *( b.begin() + 2 ), 5 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, MixedPushBackPushFront )
{
	using A = qup::List<uint32_t>;
	A a;
	a.PushFront( 5 );
	a.PushBack( 10 );
	a.PushFront( 20 );
	a.PushBack( 50 );
	a.PushFront( 100 );

	EXPECT_EQ( a.Size(), 5 );
	EXPECT_EQ( *( a.begin() + 0 ), 100 );
	EXPECT_EQ( *( a.begin() + 1 ), 20 );
	EXPECT_EQ( *( a.begin() + 2 ), 5 );
	EXPECT_EQ( *( a.begin() + 3 ), 10 );
	EXPECT_EQ( *( a.begin() + 4 ), 50 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, Append )
{
	{
		using A = qup::List<uint32_t>;
		A a { 5, 10, 20 };
		A b { 50, 100, 200 };
		a.Append( b );
		a.Append( { 500 } );
		a.Append( { 1000, 2000 } );

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( *( a.begin() + 0 ), 5 );
		EXPECT_EQ( *( a.begin() + 1 ), 10 );
		EXPECT_EQ( *( a.begin() + 2 ), 20 );
		EXPECT_EQ( *( a.begin() + 3 ), 50 );
		EXPECT_EQ( *( a.begin() + 4 ), 100 );
		EXPECT_EQ( *( a.begin() + 5 ), 200 );
		EXPECT_EQ( *( a.begin() + 6 ), 500 );
		EXPECT_EQ( *( a.begin() + 7 ), 1000 );
		EXPECT_EQ( *( a.begin() + 8 ), 2000 );
	}
	{
		using A = qup::List<uint32_t>;
		A a { 5, 10, 20 };
		A b { 50, 100, 200 };
		a += b;
		a += { 500 };
		a += { 1000, 2000 };

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( *( a.begin() + 0 ), 5 );
		EXPECT_EQ( *( a.begin() + 1 ), 10 );
		EXPECT_EQ( *( a.begin() + 2 ), 20 );
		EXPECT_EQ( *( a.begin() + 3 ), 50 );
		EXPECT_EQ( *( a.begin() + 4 ), 100 );
		EXPECT_EQ( *( a.begin() + 5 ), 200 );
		EXPECT_EQ( *( a.begin() + 6 ), 500 );
		EXPECT_EQ( *( a.begin() + 7 ), 1000 );
		EXPECT_EQ( *( a.begin() + 8 ), 2000 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::List<Simple>;
	A a;
	A b {};
	A c = {};
	A d = qup::List<Simple>();
	A e = qup::List<Simple> {};

	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 0 );
	EXPECT_EQ( d.Size(), 0 );
	EXPECT_EQ( e.Size(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, StructureInitializerListInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::List<Simple>;
	A a { { 5, 10, 20.0f, 50.0 }, {} };

	EXPECT_EQ( a.Size(), 2 );

	EXPECT_EQ( ( a.begin() + 0 )->v1, 5 );
	EXPECT_EQ( ( a.begin() + 0 )->v2, 10 );
	EXPECT_FLOAT_EQ( ( a.begin() + 0 )->v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( a.begin() + 0 )->v4, 50.0 );

	EXPECT_EQ( ( a.begin() + 1 )->v1, 0 );
	EXPECT_EQ( ( a.begin() + 1 )->v2, 0 );
	EXPECT_FLOAT_EQ( ( a.begin() + 1 )->v3, 0.0f );
	EXPECT_DOUBLE_EQ( ( a.begin() + 1 )->v4, 0.0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::List<Simple>;
	A a { { 5, 10, 20.0f, 50.0 }, {} };
	A b = a;

	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( b.Size(), 2 );

	EXPECT_EQ( ( a.begin() + 0 )->v1, ( b.begin() + 0 )->v1 );
	EXPECT_EQ( ( a.begin() + 0 )->v2, ( b.begin() + 0 )->v2 );
	EXPECT_FLOAT_EQ( ( a.begin() + 0 )->v3, ( b.begin() + 0 )->v3 );
	EXPECT_DOUBLE_EQ( ( a.begin() + 0 )->v4, ( b.begin() + 0 )->v4 );

	EXPECT_EQ( ( a.begin() + 1 )->v1, ( b.begin() + 1 )->v1 );
	EXPECT_EQ( ( a.begin() + 1 )->v2, ( b.begin() + 1 )->v2 );
	EXPECT_FLOAT_EQ( ( a.begin() + 1 )->v3, ( b.begin() + 1 )->v3 );
	EXPECT_DOUBLE_EQ( ( a.begin() + 1 )->v4, ( b.begin() + 1 )->v4 );

	b.Front().v1 = 600;
	EXPECT_EQ( ( a.begin() + 0 )->v1, 5 );
	EXPECT_EQ( ( b.begin() + 0 )->v1, 600 );

	A c { { 700 } };
	c = b;
	EXPECT_EQ( ( b.begin() + 0 )->v1, ( c.begin() + 0 )->v1 );
	EXPECT_EQ( ( b.begin() + 0 )->v2, ( c.begin() + 0 )->v2 );
	EXPECT_FLOAT_EQ( ( b.begin() + 0 )->v3, ( c.begin() + 0 )->v3 );
	EXPECT_DOUBLE_EQ( ( b.begin() + 0 )->v4, ( c.begin() + 0 )->v4 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = qup::List<Simple>;
	A a1 { { 5, 10, 20.0f, 50.0 }, {} };
	A a2 = a1;
	A b1 = std::move( a1 );
	A b2; b2 = std::move( a2 );

	EXPECT_EQ( a1.Size(), 0 );
	EXPECT_EQ( a2.Size(), 0 );
	EXPECT_EQ( b1.Size(), 2 );
	EXPECT_EQ( b2.Size(), 2 );

	EXPECT_EQ( ( b1.begin() + 0 )->v1, 5 );
	EXPECT_EQ( ( b1.begin() + 0 )->v2, 10 );
	EXPECT_FLOAT_EQ( ( b1.begin() + 0 )->v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( b1.begin() + 0 )->v4, 50.0 );

	EXPECT_EQ( ( b2.begin() + 0 )->v1, 5 );
	EXPECT_EQ( ( b2.begin() + 0 )->v2, 10 );
	EXPECT_FLOAT_EQ( ( b2.begin() + 0 )->v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( b2.begin() + 0 )->v4, 50.0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, MoveableOnlyStructure )
{
	struct MoveableOnly
	{
		MoveableOnly( size_t value ) : v1( value ) {};
		MoveableOnly( const MoveableOnly & other ) = delete;
		MoveableOnly( MoveableOnly && other ) = default;
		MoveableOnly & operator=( const MoveableOnly & other ) = delete;
		MoveableOnly & operator=( MoveableOnly && other ) = default;
		size_t v1 = {};
	};

	using A = qup::List<MoveableOnly>;
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
class List_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	List_CopyableOnly_Control() {}
	List_CopyableOnly_Control( const List_CopyableOnly_Control & other ) { ++copy_counter; }
	List_CopyableOnly_Control( List_CopyableOnly_Control && other ) { ++move_counter; }

	List_CopyableOnly_Control & operator=( const List_CopyableOnly_Control & other ) { ++copy_counter; }
	List_CopyableOnly_Control & operator=( List_CopyableOnly_Control && other ) { ++move_counter; }
};
int32_t List_CopyableOnly_Control::copy_counter		= 0;
int32_t List_CopyableOnly_Control::move_counter		= 0;

class List_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	List_CopyableOnly() {}
	List_CopyableOnly( const List_CopyableOnly & other ) { ++copy_counter; }
	List_CopyableOnly( List_CopyableOnly && other ) = delete;

	List_CopyableOnly & operator=( const List_CopyableOnly & other ) { ++copy_counter; }
	List_CopyableOnly & operator=( List_CopyableOnly && other ) = delete;
};
int32_t List_CopyableOnly::copy_counter		= 0;

TEST( CoreContainerList, List_CopyableOnlyStructure )
{
	List_CopyableOnly_Control::copy_counter	= 0;
	List_CopyableOnly_Control::move_counter	= 0;

	List_CopyableOnly::copy_counter			= 0;

	using A = qup::List<List_CopyableOnly>;
	using B = qup::List<List_CopyableOnly_Control>;
	A a;
	a.PushBack( List_CopyableOnly() );
	a.PushBack( List_CopyableOnly() );
	a.PushBack( List_CopyableOnly() );

	B ac;
	ac.PushBack( List_CopyableOnly_Control() );
	ac.PushBack( List_CopyableOnly_Control() );
	ac.PushBack( List_CopyableOnly_Control() );

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

	EXPECT_GT( List_CopyableOnly::copy_counter, List_CopyableOnly_Control::copy_counter );
	EXPECT_EQ( List_CopyableOnly::copy_counter, List_CopyableOnly_Control::copy_counter + List_CopyableOnly_Control::move_counter );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class List_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	List_CtorDtorCounted() { ++constructed_counter; }
	List_CtorDtorCounted( const List_CtorDtorCounted & other ) { ++constructed_counter; }
	List_CtorDtorCounted( List_CtorDtorCounted && other ) { ++constructed_counter; }
	~List_CtorDtorCounted() { --constructed_counter; }
	List_CtorDtorCounted & operator=( const List_CtorDtorCounted & other ) = default;
	List_CtorDtorCounted & operator=( List_CtorDtorCounted && other ) = default;
};
int32_t List_CtorDtorCounted::constructed_counter	= 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, CtorDtorCounter )
{
	List_CtorDtorCounted::constructed_counter	= 0;

	using A = qup::List<List_CtorDtorCounted>;
	{
		A a;

		a.PushBack( {} );
		a.PushBack( List_CtorDtorCounted() );
		a.PushBack( List_CtorDtorCounted {} );
		EXPECT_EQ( a.Size(), 3 );

		a.PushFront( {} );
		a.PushFront( List_CtorDtorCounted() );
		a.PushFront( List_CtorDtorCounted {} );
		EXPECT_EQ( a.Size(), 6 );

		a.EmplaceBack();
		a.EmplaceFront();
		EXPECT_EQ( a.Size(), 8 );

		a.PopBack();
		EXPECT_EQ( a.Size(), 7 );

		a.PopFront();
		EXPECT_EQ( a.Size(), 6 );

		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( List_CtorDtorCounted::constructed_counter, 0 );
	}
	{
		A a;

		for( size_t i = 0; i < 50; ++i ) {
			a.PushBack( {} );
		}
		EXPECT_EQ( a.Size(), 50 );

		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 49 );

		a.Erase( a.end() - 1 );
		EXPECT_EQ( a.Size(), 48 );

		a.Erase( a.begin() + 10 );
		EXPECT_EQ( a.Size(), 47 );

		a.Erase( a.begin(), a.end() - 5 );
		EXPECT_EQ( a.Size(), 5 );

		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( List_CtorDtorCounted::constructed_counter, 0 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, Iterator )
{
	using A = qup::List<uint32_t>;

	{
		A a;
		auto it = a.begin();
		auto temp_it = it;

		temp_it = it;
		EXPECT_THROW( --temp_it, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( ++temp_it, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it - 1, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it + 1, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it -= 1, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it += 1, qup::exception::Exception );
	}
	{
		A a;
		auto it = a.begin();

		a.PushBack( 5 );
		it = a.begin();
		auto temp_it = it;

		temp_it = it;
		EXPECT_THROW( --temp_it, qup::exception::Exception );

		temp_it = it;
		EXPECT_NO_THROW( ++temp_it );
		EXPECT_THROW( ++temp_it, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it - 1, qup::exception::Exception );
		EXPECT_NO_THROW( temp_it + 1 );
		EXPECT_THROW( temp_it + 2, qup::exception::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it -= 1, qup::exception::Exception );

		temp_it = it;
		EXPECT_NO_THROW( temp_it += 1 );
		EXPECT_THROW( temp_it += 1, qup::exception::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerList, SelfAssignment )
{
	{
		using A = qup::List<uint32_t>;
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
			a = AView( a.begin() + 1, a.end() - 1 );

			A comp { 10, 20, 50 };
			EXPECT_EQ( a.Size(), 3 );
			EXPECT_EQ( a, comp );
		}
		{
			A a = original;
			a.Append( a );

			EXPECT_EQ( a.Size(), 10 );
			A comp { 5, 10, 20, 50, 200, 5, 10, 20, 50, 200 };
			EXPECT_EQ( a, comp );
		}
	}
	{
		List_CtorDtorCounted::constructed_counter		= 0;

		using A = qup::List<List_CtorDtorCounted>;
		using AView = A::ViewType;
		A a;
		a.PushBack( {} );
		a.PushBack( {} );
		a.PushBack( {} );
		a.Insert( a.begin() + 1, {} );
		for( size_t i = 0; i < 5; i++ ) {
			a.PushFront( List_CtorDtorCounted() );
			a.PushBack( List_CtorDtorCounted() );
			a.Insert( a.begin() + 3, List_CtorDtorCounted() );
			a = a;
			a.Append( a );
		}
		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( List_CtorDtorCounted::constructed_counter, 0 );
	}
}



} // array
} // containers
