
#include <gtest/gtest.h>

#include <core/containers/Array.hpp>

#include <array>

namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, BasicInit )
{
	using A = bc::Array<uint32_t, 5>;
	using B = bc::Array<uint32_t, 0>;
	{
		A a;
		EXPECT_EQ( a.Size(), 5 );

		A b {};
		EXPECT_EQ( b.Size(), 5 );

		A c = {};
		EXPECT_EQ( c.Size(), 5 );

		A d = A();
		EXPECT_EQ( d.Size(), 5 );

		A e = A {};
		EXPECT_EQ( e.Size(), 5 );

		A f { 1, 2, 3, 4, 5 };
		EXPECT_EQ( f.Size(), 5 );
	}
	{
		B a;
		EXPECT_EQ( a.Size(), 0 );

		B b {};
		EXPECT_EQ( b.Size(), 0 );

		B c = {};
		EXPECT_EQ( c.Size(), 0 );

		B d = B();
		EXPECT_EQ( d.Size(), 0 );

		B e = B {};
		EXPECT_EQ( e.Size(), 0 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, IndexOperator )
{
	using A = bc::Array<uint32_t, 3>;
	A a { 5, 10, 20 };

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );

	try
	{
		a[ 3 ];
	}
	catch( const bc::diagnostic::Exception& e )
	{
		e.GetMessage();
	}
	EXPECT_THROW( a[ 3 ], bc::diagnostic::Exception );
	EXPECT_THROW( a[ 4 ], bc::diagnostic::Exception );
	EXPECT_THROW( a[ 5 ], bc::diagnostic::Exception );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, FrontBack )
{
	using A = bc::Array<uint32_t, 3>;
	using B = bc::Array<uint32_t, 0>;

	{
		A a { 1, 2, 3 };
		EXPECT_EQ( a.Front(), 1 );
		EXPECT_EQ( a.Back(), 3 );
	}
	{
		B a;

		EXPECT_THROW( auto t = a.Front(), bc::diagnostic::Exception );
		EXPECT_THROW( auto t = a.Back(), bc::diagnostic::Exception );
	}
	{
		using stl = std::array<uint32_t, 3>;

		auto s = stl { 5 };

		A a = { 5 };
		EXPECT_EQ( a.Front(), 5 );
		EXPECT_EQ( a.Back(), 0 );

		A b = { 5, 10 };
		EXPECT_EQ( b.Front(), 5 );
		EXPECT_EQ( b.Back(), 0 );

		A c = { 5, 10, 15 };
		EXPECT_EQ( c.Front(), 5 );
		EXPECT_EQ( c.Back(), 15 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Array<Simple, 3>;
	A a;
	A b {};
	A c = {};
	A d = bc::Array<Simple, 3>();
	A e = bc::Array<Simple, 3> {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, StructureInitializerListInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Array<Simple, 2>;
	A a { Simple{ 5, 10, 20.0f, 50.0 }, Simple {} };

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
TEST( ArrayContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Array<Simple, 2>;
	A a { Simple { 5, 10, 20.0f, 50.0 }, Simple {} };
	A b = a;

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

	A c { Simple { 700 } };
	c = b;
	EXPECT_EQ( b[ 0 ].v1, c[ 0 ].v1 );
	EXPECT_EQ( b[ 0 ].v2, c[ 0 ].v2 );
	EXPECT_FLOAT_EQ( b[ 0 ].v3, c[ 0 ].v3 );
	EXPECT_DOUBLE_EQ( b[ 0 ].v4, c[ 0 ].v4 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Array<Simple, 2>;
	A a1 { Simple { 5, 10, 20.0f, 50.0 }, Simple {} };
	A a2 = a1;
	A b1 = std::move( a1 );
	A b2; b2 = std::move( a2 );

	EXPECT_EQ( a1.Size(), 2 );
	EXPECT_EQ( a2.Size(), 2 );
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
TEST( ArrayContainer, MoveableOnlyStructure )
{
	struct MoveableOnly
	{
		MoveableOnly() {};
		MoveableOnly( size_t value ) : v1( value ) {};
		MoveableOnly( const MoveableOnly & other ) = delete;
		MoveableOnly( MoveableOnly && other ) = default;
		MoveableOnly & operator=( const MoveableOnly & other ) = delete;
		MoveableOnly & operator=( MoveableOnly && other ) = default;
		size_t v1 = {};
	};

	using A = bc::Array<MoveableOnly, 3>;
	A a { MoveableOnly { 5 }, MoveableOnly { 10 }, MoveableOnly { 15 } };

	// Expecting compile time error for this:
	// A ae = a;

	EXPECT_EQ( a[ 0 ].v1, 5 );
	EXPECT_EQ( a[ 1 ].v1, 10 );
	EXPECT_EQ( a[ 2 ].v1, 15 );

	// Moving a class with fundamental types only, this will just copy the values inside.
	// Proper constructor and assignment operator calls are tested elsewhere.
	A b = std::move( a );
	EXPECT_EQ( a[ 0 ].v1, 5 );
	EXPECT_EQ( a[ 1 ].v1, 10 );
	EXPECT_EQ( a[ 2 ].v1, 15 );
	EXPECT_EQ( b[ 0 ].v1, 5 );
	EXPECT_EQ( b[ 1 ].v1, 10 );
	EXPECT_EQ( b[ 2 ].v1, 15 );

	A c;
	c = std::move( b );
	EXPECT_EQ( a[ 0 ].v1, 5 );
	EXPECT_EQ( a[ 1 ].v1, 10 );
	EXPECT_EQ( a[ 2 ].v1, 15 );
	EXPECT_EQ( b[ 0 ].v1, 5 );
	EXPECT_EQ( b[ 1 ].v1, 10 );
	EXPECT_EQ( b[ 2 ].v1, 15 );
	EXPECT_EQ( c[ 0 ].v1, 5 );
	EXPECT_EQ( c[ 1 ].v1, 10 );
	EXPECT_EQ( c[ 2 ].v1, 15 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Array_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	Array_CopyableOnly_Control() {}
	Array_CopyableOnly_Control( const Array_CopyableOnly_Control & other ) { ++copy_counter; }
	Array_CopyableOnly_Control( Array_CopyableOnly_Control && other ) { ++move_counter; }

	Array_CopyableOnly_Control & operator=( const Array_CopyableOnly_Control & other ) { ++copy_counter; return *this; }
	Array_CopyableOnly_Control & operator=( Array_CopyableOnly_Control && other ) { ++move_counter; return *this; }
};
int32_t Array_CopyableOnly_Control::copy_counter		= 0;
int32_t Array_CopyableOnly_Control::move_counter		= 0;

class Array_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	Array_CopyableOnly() {}
	Array_CopyableOnly( const Array_CopyableOnly & other ) { ++copy_counter; }
	Array_CopyableOnly( Array_CopyableOnly && other ) = delete;

	Array_CopyableOnly & operator=( const Array_CopyableOnly & other ) { ++copy_counter; return *this; }
	Array_CopyableOnly & operator=( Array_CopyableOnly && other ) = delete;
};
int32_t Array_CopyableOnly::copy_counter		= 0;

TEST( ArrayContainer, Array_CopyableOnlyStructure )
{
	Array_CopyableOnly_Control::copy_counter	= 0;
	Array_CopyableOnly_Control::move_counter	= 0;

	Array_CopyableOnly::copy_counter			= 0;

	using A = bc::Array<Array_CopyableOnly, 3>;
	using B = bc::Array<Array_CopyableOnly_Control, 3>;
	A a;
	EXPECT_EQ( Array_CopyableOnly::copy_counter, 0 );

	B ac;
	EXPECT_EQ( Array_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( Array_CopyableOnly_Control::move_counter, 0 );

	A b = a;
	EXPECT_EQ( Array_CopyableOnly::copy_counter, 3 );

	B bc = ac;
	EXPECT_EQ( Array_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( Array_CopyableOnly_Control::move_counter, 0 );

	A c = std::move( b );
	B cc = std::move( bc );

	EXPECT_EQ( Array_CopyableOnly::copy_counter, 6 );
	EXPECT_EQ( Array_CopyableOnly_Control::copy_counter, 3 );
	EXPECT_EQ( Array_CopyableOnly_Control::move_counter, 3 );
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

TEST( ArrayContainer, CtorDtorCounter )
{
	Array_CtorDtorCounted::constructed_counter		= 0;

	using A = bc::Array<Array_CtorDtorCounted, 3>;
	{
		A a;
		EXPECT_EQ( Array_CtorDtorCounted::constructed_counter, 3 );
	}
	EXPECT_EQ( Array_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, Iterator )
{
	using A = bc::Array<uint32_t, 5>;
	using B = bc::Array<uint32_t, 0>;

	{
		A a;
		B b;
		EXPECT_NE( a.begin(), a.end() );

		EXPECT_EQ( b.begin(), b.end() );
	}
	{
		A a { 0 };
		auto it = a.begin();
		auto it_end = a.end();
		auto temp_it = it;
		auto temp_it_end = it_end;

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( --temp_it, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( ++temp_it );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( --temp_it_end );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( ++temp_it_end, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it - 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( temp_it + 1 );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( temp_it_end - 1 );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end + 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it -= 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( temp_it += 1 );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_NO_THROW( temp_it_end -= 1 );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end += 1, bc::diagnostic::Exception );
	}
	{
		B a;
		auto it = a.begin();
		auto it_end = a.begin();
		auto temp_it = it;
		auto temp_it_end = it_end;

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( --temp_it, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( ++temp_it, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( --temp_it_end, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( ++temp_it_end, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it - 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it + 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end - 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end + 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it -= 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it += 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end -= 1, bc::diagnostic::Exception );

		temp_it = it;
		temp_it_end = it_end;
		EXPECT_THROW( temp_it_end += 1, bc::diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( ArrayContainer, SelfAssignment )
{
	{
		using A = bc::Array<uint32_t, 5>;

		A original { 5, 10, 20, 50, 200 };
		{
			A a = original;
			a = a;
			EXPECT_EQ( a, original );
		}
	}
	{
		Array_CtorDtorCounted::constructed_counter = 0;

		using A = bc::Array<Array_CtorDtorCounted, 3>;
		A a;
		a = a;
		EXPECT_EQ( Array_CtorDtorCounted::constructed_counter, 3 );
	}
}



} // containers
} // core
