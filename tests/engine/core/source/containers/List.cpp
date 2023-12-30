
#include <gtest/gtest.h>

#include <core/containers/List.hpp>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, BasicInit )
{
	using A = bc::List<uint32_t>;
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
TEST( ListContainer, ConstructWithSize )
{
	using A = bc::List<uint32_t>;
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
TEST( ListContainer, IndexOperator )
{
	using A = bc::List<uint32_t>;
	A a { 5, 10, 20 };

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, Erase )
{
	using A = bc::List<uint32_t>;
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		EXPECT_THROW( a.begin() - 1, bc::diagnostic::Exception );
		EXPECT_THROW( a.end() + 1, bc::diagnostic::Exception );
		EXPECT_THROW( a.Erase( a.end() ), bc::diagnostic::Exception );
		EXPECT_THROW( a.Erase( a.begin() + 10 ), bc::diagnostic::Exception );
	}
	using A = bc::List<uint32_t>;
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( a[ 0 ], 1 );
		EXPECT_EQ( a[ 1 ], 2 );
		EXPECT_EQ( a[ 2 ], 3 );
		EXPECT_EQ( a[ 3 ], 4 );
		EXPECT_EQ( a[ 4 ], 5 );
		EXPECT_EQ( a[ 5 ], 6 );
		EXPECT_EQ( a[ 6 ], 7 );
		EXPECT_EQ( a[ 7 ], 8 );
		EXPECT_EQ( a[ 8 ], 9 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.end() - 1 );
		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 8 );
		EXPECT_EQ( a[ 0 ], 1 );
		EXPECT_EQ( a[ 1 ], 2 );
		EXPECT_EQ( a[ 2 ], 3 );
		EXPECT_EQ( a[ 3 ], 4 );
		EXPECT_EQ( a[ 4 ], 5 );
		EXPECT_EQ( a[ 5 ], 6 );
		EXPECT_EQ( a[ 6 ], 7 );
		EXPECT_EQ( a[ 7 ], 8 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin(), a.end() - 5 );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( a[ 0 ], 5 );
		EXPECT_EQ( a[ 1 ], 6 );
		EXPECT_EQ( a[ 2 ], 7 );
		EXPECT_EQ( a[ 3 ], 8 );
		EXPECT_EQ( a[ 4 ], 9 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin() + 5, a.end() );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( a[ 0 ], 0 );
		EXPECT_EQ( a[ 1 ], 1 );
		EXPECT_EQ( a[ 2 ], 2 );
		EXPECT_EQ( a[ 3 ], 3 );
		EXPECT_EQ( a[ 4 ], 4 );
	}
	{
		A a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		a.Erase( a.begin() + 3, a.end() - 3 );
		EXPECT_EQ( a.Size(), 6 );
		EXPECT_EQ( a[ 0 ], 0 );
		EXPECT_EQ( a[ 1 ], 1 );
		EXPECT_EQ( a[ 2 ], 2 );
		EXPECT_EQ( a[ 3 ], 7 );
		EXPECT_EQ( a[ 4 ], 8 );
		EXPECT_EQ( a[ 5 ], 9 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, FrontBack )
{
	using A = bc::List<uint32_t>;
	{
		A a;

		EXPECT_THROW( auto t = a.Front(), bc::diagnostic::Exception );
		EXPECT_THROW( auto t = a.Back(), bc::diagnostic::Exception );
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
TEST( ListContainer, PushBack )
{
	using A = bc::List<uint32_t>;
	using B = bc::List<PushObject>;

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
TEST( ListContainer, PushFront )
{
	using A = bc::List<uint32_t>;
	using B = bc::List<PushObject>;

	A a;
	a.PushFront( 5 );
	a.PushFront( 10 );
	a.PushFront( 20 );

	EXPECT_EQ( a[ 0 ], 20 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 5 );

	B b;
	b.PushFront( 5 );
	b.PushFront( 10 );
	b.PushFront( 20 );

	EXPECT_EQ( b[ 0 ], 20 );
	EXPECT_EQ( b[ 1 ], 10 );
	EXPECT_EQ( b[ 2 ], 5 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, MixedPushBackPushFront )
{
	using A = bc::List<uint32_t>;
	A a;
	a.PushFront( 5 );
	a.PushBack( 10 );
	a.PushFront( 20 );
	a.PushBack( 50 );
	a.PushFront( 100 );

	EXPECT_EQ( a.Size(), 5 );
	EXPECT_EQ( a[ 0 ], 100 );
	EXPECT_EQ( a[ 1 ], 20 );
	EXPECT_EQ( a[ 2 ], 5 );
	EXPECT_EQ( a[ 3 ], 10 );
	EXPECT_EQ( a[ 4 ], 50 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, Append )
{
	{
		using A = bc::List<uint32_t>;
		A a { 5, 10, 20 };
		A b { 50, 100, 200 };
		a.Append( b );
		a.Append( { 500 } );
		a.Append( { 1000, 2000 } );

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( a[ 0 ], 5 );
		EXPECT_EQ( a[ 1 ], 10 );
		EXPECT_EQ( a[ 2 ], 20 );
		EXPECT_EQ( a[ 3 ], 50 );
		EXPECT_EQ( a[ 4 ], 100 );
		EXPECT_EQ( a[ 5 ], 200 );
		EXPECT_EQ( a[ 6 ], 500 );
		EXPECT_EQ( a[ 7 ], 1000 );
		EXPECT_EQ( a[ 8 ], 2000 );
	}
	{
		using A = bc::List<uint32_t>;
		A a { 5, 10, 20 };
		A b { 50, 100, 200 };
		a += b;
		a += { 500 };
		a += { 1000, 2000 };

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( a[ 0 ], 5 );
		EXPECT_EQ( a[ 1 ], 10 );
		EXPECT_EQ( a[ 2 ], 20 );
		EXPECT_EQ( a[ 3 ], 50 );
		EXPECT_EQ( a[ 4 ], 100 );
		EXPECT_EQ( a[ 5 ], 200 );
		EXPECT_EQ( a[ 6 ], 500 );
		EXPECT_EQ( a[ 7 ], 1000 );
		EXPECT_EQ( a[ 8 ], 2000 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::List<Simple>;
	A a;
	A b {};
	A c = {};
	A d = bc::List<Simple>();
	A e = bc::List<Simple> {};

	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 0 );
	EXPECT_EQ( d.Size(), 0 );
	EXPECT_EQ( e.Size(), 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, StructureInitializerListInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::List<Simple>;
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
TEST( ListContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::List<Simple>;
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
TEST( ListContainer, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::List<Simple>;
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
TEST( ListContainer, MoveableOnlyStructure )
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

	using A = bc::List<MoveableOnly>;
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

	List_CopyableOnly_Control & operator=( const List_CopyableOnly_Control & other ) { ++copy_counter; return *this; }
	List_CopyableOnly_Control & operator=( List_CopyableOnly_Control && other ) { ++move_counter; return *this; }
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

	List_CopyableOnly & operator=( const List_CopyableOnly & other ) { ++copy_counter; return *this; }
	List_CopyableOnly & operator=( List_CopyableOnly && other ) = delete;
};
int32_t List_CopyableOnly::copy_counter		= 0;

TEST( ListContainer, List_CopyableOnlyStructure )
{
	List_CopyableOnly_Control::copy_counter	= 0;
	List_CopyableOnly_Control::move_counter	= 0;

	List_CopyableOnly::copy_counter			= 0;

	using A = bc::List<List_CopyableOnly>;
	using B = bc::List<List_CopyableOnly_Control>;
	A a; a.Reserve( 16 );
	a.PushBack( List_CopyableOnly() );
	a.PushBack( List_CopyableOnly() );
	a.PushBack( List_CopyableOnly() );
	EXPECT_EQ( List_CopyableOnly::copy_counter, 3 );

	B ac; ac.Reserve( 16 );
	ac.PushBack( List_CopyableOnly_Control() );
	ac.PushBack( List_CopyableOnly_Control() );
	ac.PushBack( List_CopyableOnly_Control() );
	EXPECT_EQ( List_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( List_CopyableOnly_Control::move_counter, 3 );

	A b = a;
	EXPECT_EQ( List_CopyableOnly::copy_counter, 6 );

	B bc = ac;
	EXPECT_EQ( List_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( List_CopyableOnly_Control::move_counter, 3 );

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

	EXPECT_EQ( List_CopyableOnly::copy_counter, 6 );
	EXPECT_EQ( List_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( List_CopyableOnly_Control::move_counter, 3 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Array_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	Array_CtorDtorCounted() { ++constructed_counter; data = constructed_counter; }
	Array_CtorDtorCounted( const Array_CtorDtorCounted & other ) { ++constructed_counter; data = constructed_counter; }
	Array_CtorDtorCounted( Array_CtorDtorCounted && other ) { ++constructed_counter; data = constructed_counter; }
	~Array_CtorDtorCounted() { --constructed_counter; }
	Array_CtorDtorCounted & operator=( const Array_CtorDtorCounted & other ) = default;
	Array_CtorDtorCounted & operator=( Array_CtorDtorCounted && other ) = default;
};
int32_t Array_CtorDtorCounted::constructed_counter	= 0;

TEST( ListContainer, CtorDtorCounter )
{
	Array_CtorDtorCounted::constructed_counter		= 0;

	using A = bc::List<Array_CtorDtorCounted>;
	A a;

	a.PushBack( {} );
	a.PushBack( Array_CtorDtorCounted() );
	a.PushBack( Array_CtorDtorCounted {} );
	EXPECT_EQ( a.Size(), 3 );

	a.PushFront( {} );
	a.PushFront( Array_CtorDtorCounted() );
	a.PushFront( Array_CtorDtorCounted {} );
	EXPECT_EQ( a.Size(), 6 );

	a.EmplaceBack();
	a.EmplaceFront();
	EXPECT_EQ( a.Size(), 8 );

	a.PopBack();
	EXPECT_EQ( a.Size(), 7 );

	a.PopFront();
	EXPECT_EQ( a.Size(), 6 );

	a.Resize( 50 );
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
	EXPECT_EQ( Array_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, Iterator )
{
	using A = bc::List<uint32_t>;

	{
		A a;
		EXPECT_EQ( a.begin(), a.end() );

		a.PushBack( 5 );
		EXPECT_NE( a.begin(), a.end() );
	}
	{
		A a;
		auto it = a.begin();
		auto temp_it = it;

		temp_it = it;
		EXPECT_THROW( --temp_it, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( ++temp_it, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it - 1, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it + 1, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it -= 1, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it += 1, bc::diagnostic::Exception );
	}
	{
		A a;
		auto it = a.begin();

		a.PushBack( 5 );
		it = a.begin();
		auto temp_it = it;

		temp_it = it;
		EXPECT_THROW( --temp_it, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_NO_THROW( ++temp_it );
		EXPECT_THROW( ++temp_it, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it - 1, bc::diagnostic::Exception );
		EXPECT_NO_THROW( temp_it + 1 );
		EXPECT_THROW( temp_it + 2, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_THROW( temp_it -= 1, bc::diagnostic::Exception );

		temp_it = it;
		EXPECT_NO_THROW( temp_it += 1 );
		EXPECT_THROW( temp_it += 1, bc::diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ListContainer, SelfAssignment )
{
	{
		using A = bc::List<uint32_t>;
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
		Array_CtorDtorCounted::constructed_counter		= 0;

		using A = bc::List<Array_CtorDtorCounted>;
		using AView = A::ThisViewType<true>;
		A a( 5 );
		a.Resize( 10 );
		a.Reserve( 50 );
		a.PushBack( {} );
		a.Insert( a.begin() + 5, {} );
		for( size_t i = 0; i < 5; i++ )
		{
			a.PushFront( Array_CtorDtorCounted() );
			a.PushBack( Array_CtorDtorCounted() );
			a.Insert( a.begin() + 10, Array_CtorDtorCounted() );
			a = a;
			a.Append( a );
		}
		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( Array_CtorDtorCounted::constructed_counter, 0 );
	}
}



} // containers
} // core
