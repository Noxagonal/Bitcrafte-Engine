
#include <gtest/gtest.h>

#include <core/containers/Optional.hpp>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, BasicInit )
{
	using A = bc::Optional<uint32_t>;
	A a1;
	EXPECT_EQ( a1.IsEmpty(), true );

	A b1 {};
	EXPECT_EQ( b1.IsEmpty(), true );

	A c1 = {};
	EXPECT_EQ( c1.IsEmpty(), true );

	A d1 = A();
	EXPECT_EQ( d1.IsEmpty(), true );

	A e1 = A {};
	EXPECT_EQ( e1.IsEmpty(), true );

	A a2( 5 );
	EXPECT_EQ( a2.IsEmpty(), false );

	A b2 { 5 };
	EXPECT_EQ( b2.IsEmpty(), false );

	A c2 = 5;
	EXPECT_EQ( c2.IsEmpty(), false );

	A d2 = A( 5 );
	EXPECT_EQ( d2.IsEmpty(), false );

	A e2 = A { 5 };
	EXPECT_EQ( e2.IsEmpty(), false );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, Clear )
{
	using A = bc::Optional<uint32_t>;
	{
		auto a = A( 5 );
		EXPECT_EQ( a.IsEmpty(), false );

		a.Clear();
		EXPECT_EQ( a.IsEmpty(), true );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Optional_EmplaceObject
{
public:
	Optional_EmplaceObject( size_t value ) : value( value ) {}
	Optional_EmplaceObject( const Optional_EmplaceObject & other ) = default;
	Optional_EmplaceObject( Optional_EmplaceObject && other ) = default;

	Optional_EmplaceObject & operator=( const Optional_EmplaceObject & other ) = default;
	Optional_EmplaceObject & operator=( Optional_EmplaceObject && other ) = default;

	bool operator==( const Optional_EmplaceObject & other ) const { return value == other.value; }

	size_t value = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, NonTrivialTypes )
{
	using A = bc::Optional<uint32_t>;
	using B = bc::Optional<Optional_EmplaceObject>;

	{
		A a( 5 );
		B b( 5 );

		EXPECT_EQ( a.Get(), 5 );
		EXPECT_EQ( b.Get(), 5 );
		EXPECT_EQ( b.Get().value, 5 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1		= {};
		uint32_t v2		= {};
		float v3		= {};
		double v4		= {};
	};

	using A = bc::Optional<Simple>;
	{
		A a { Simple{ 1, 2, 3.3f, 4.4 } };
		A b = a;

		EXPECT_EQ( a.IsEmpty(), false );
		EXPECT_EQ( b.IsEmpty(), false );

		EXPECT_EQ( a.Get().v1, b.Get().v1 );
		EXPECT_EQ( a.Get().v2, b.Get().v2 );
		EXPECT_FLOAT_EQ( a.Get().v3, b.Get().v3 );
		EXPECT_DOUBLE_EQ( a.Get().v4, b.Get().v4 );
	}
	{
		A a { Simple{ 1, 2, 3.3f, 4.4 } };
		A b = Simple { 1, 2, 3.3f, 4.4 };

		EXPECT_EQ( a.IsEmpty(), false );
		EXPECT_EQ( b.IsEmpty(), false );

		EXPECT_EQ( a.Get().v1, b.Get().v1 );
		EXPECT_EQ( a.Get().v2, b.Get().v2 );
		EXPECT_FLOAT_EQ( a.Get().v3, b.Get().v3 );
		EXPECT_DOUBLE_EQ( a.Get().v4, b.Get().v4 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, StructureMove )
{
	struct Simple
	{
		size_t v1		= {};
		uint32_t v2		= {};
		float v3		= {};
		double v4		= {};
	};

	using A = bc::Optional<Simple>;
	{
		A a { Simple{ 1, 2, 3.3f, 4.4 } };
		A b = std::move( a );

		EXPECT_EQ( a.IsEmpty(), true );
		EXPECT_EQ( b.IsEmpty(), false );

		EXPECT_EQ( b.Get().v1, 1 );
		EXPECT_EQ( b.Get().v2, 2 );
		EXPECT_FLOAT_EQ( b.Get().v3, 3.3f );
		EXPECT_DOUBLE_EQ( b.Get().v4, 4.4 );
	}
	{
		Simple s { 1, 2, 3.3f, 4.4 };
		A b = std::move( s );

		EXPECT_EQ( b.IsEmpty(), false );

		EXPECT_EQ( b.Get().v1, 1 );
		EXPECT_EQ( b.Get().v2, 2 );
		EXPECT_FLOAT_EQ( b.Get().v3, 3.3f );
		EXPECT_DOUBLE_EQ( b.Get().v4, 4.4 );
	}
};

template<typename T>
concept IsMoveableOnlyCopyable = requires( T object )
{
	object = object;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, MoveableOnlyStructure )
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

	using A = bc::Optional<MoveableOnly>;
	A a = MoveableOnly( 5 );

	// Expecting compile time error for this:
	//A b = a;
	static_assert( !IsMoveableOnlyCopyable<A> );

	A b = std::move( a );
	EXPECT_EQ( a.IsEmpty(), true );
	EXPECT_EQ( b.IsEmpty(), false );

	A c; c = std::move( b );
	EXPECT_EQ( a.IsEmpty(), true );
	EXPECT_EQ( b.IsEmpty(), true );
	EXPECT_EQ( c.IsEmpty(), false );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Optional_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	Optional_CopyableOnly_Control() {}
	Optional_CopyableOnly_Control( const Optional_CopyableOnly_Control & other ) { ++copy_counter; }
	Optional_CopyableOnly_Control( Optional_CopyableOnly_Control && other ) { ++move_counter; }

	Optional_CopyableOnly_Control & operator=( const Optional_CopyableOnly_Control & other ) { ++copy_counter; return *this; }
	Optional_CopyableOnly_Control & operator=( Optional_CopyableOnly_Control && other ) { ++move_counter; return *this; }
};
int32_t Optional_CopyableOnly_Control::copy_counter		= 0;
int32_t Optional_CopyableOnly_Control::move_counter		= 0;

class Optional_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	Optional_CopyableOnly() {}
	Optional_CopyableOnly( const Optional_CopyableOnly & other ) { ++copy_counter; }
	Optional_CopyableOnly( Optional_CopyableOnly && other ) = delete;

	Optional_CopyableOnly & operator=( const Optional_CopyableOnly & other ) { ++copy_counter; return *this; }
	Optional_CopyableOnly & operator=( Optional_CopyableOnly && other ) = delete;
};
int32_t Optional_CopyableOnly::copy_counter		= 0;

TEST( OptionalContainer, CopyableOnlyStructure )
{
	Optional_CopyableOnly_Control::copy_counter	= 0;
	Optional_CopyableOnly_Control::move_counter	= 0;

	Optional_CopyableOnly::copy_counter			= 0;

	using A = bc::Optional<Optional_CopyableOnly>;
	using B = bc::Optional<Optional_CopyableOnly_Control>;

	A a = Optional_CopyableOnly();
	EXPECT_EQ( Optional_CopyableOnly::copy_counter, 1 );

	B ac = Optional_CopyableOnly_Control();
	EXPECT_EQ( Optional_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( Optional_CopyableOnly_Control::move_counter, 1 );

	A b = a;
	EXPECT_EQ( Optional_CopyableOnly::copy_counter, 2 );

	B bc = ac;
	EXPECT_EQ( Optional_CopyableOnly_Control::copy_counter, 1 );
	EXPECT_EQ( Optional_CopyableOnly_Control::move_counter, 1 );

	EXPECT_EQ( a.IsEmpty(), false );
	EXPECT_EQ( b.IsEmpty(), false );

	EXPECT_EQ( ac.IsEmpty(), false );
	EXPECT_EQ( bc.IsEmpty(), false );

	A c = std::move( b );
	B cc = std::move( bc );

	EXPECT_EQ( a.IsEmpty(), false );
	EXPECT_EQ( b.IsEmpty(), false );
	EXPECT_EQ( c.IsEmpty(), false );

	EXPECT_EQ( ac.IsEmpty(), false );
	EXPECT_EQ( bc.IsEmpty(), true );
	EXPECT_EQ( cc.IsEmpty(), false );

	EXPECT_EQ( Optional_CopyableOnly::copy_counter, 3 );
	EXPECT_EQ( Optional_CopyableOnly_Control::copy_counter, 1 );
	EXPECT_EQ( Optional_CopyableOnly_Control::move_counter, 2 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Optional_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	Optional_CtorDtorCounted() { ++constructed_counter; data = constructed_counter; }
	Optional_CtorDtorCounted( const Optional_CtorDtorCounted & other ) { ++constructed_counter; data = constructed_counter; }
	Optional_CtorDtorCounted( Optional_CtorDtorCounted && other ) { ++constructed_counter; data = constructed_counter; }
	~Optional_CtorDtorCounted() { --constructed_counter; }
	Optional_CtorDtorCounted & operator=( const Optional_CtorDtorCounted & other ) = default;
	Optional_CtorDtorCounted & operator=( Optional_CtorDtorCounted && other ) = default;
};
int32_t Optional_CtorDtorCounted::constructed_counter	= 0;

TEST( OptionalContainer, CtorDtorCounter )
{
	Optional_CtorDtorCounted::constructed_counter		= 0;

	using A = bc::Optional<Optional_CtorDtorCounted>;
	{
		A a = Optional_CtorDtorCounted();
		EXPECT_EQ( a.IsEmpty(), false );
		EXPECT_EQ( Optional_CtorDtorCounted::constructed_counter, 1 );
	}
	EXPECT_EQ( Optional_CtorDtorCounted::constructed_counter, 0 );
	{
		A a = Optional_CtorDtorCounted();
		a.Clear();
		EXPECT_EQ( Optional_CtorDtorCounted::constructed_counter, 0 );
	}

	EXPECT_EQ( Optional_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( OptionalContainer, SelfAssignment )
{
	using A = bc::Optional<uint32_t>;
	{
		A a;
		a = a;
		EXPECT_EQ( a.IsEmpty(), true );
	}
	{
		A a = 5;
		a = a;
		EXPECT_EQ( a.IsEmpty(), false );
	}
	{
		A a;
		a = std::move( a );
		EXPECT_EQ( a.IsEmpty(), true );
	}
	{
		A a = 5;
		a = std::move( a );
		EXPECT_EQ( a.IsEmpty(), false );
	}
	{
		A a;
		A b = a;
		a = b;
		EXPECT_EQ( a.IsEmpty(), true );
		EXPECT_EQ( b.IsEmpty(), true );
	}
	{
		A a;
		A b = a;
		a = std::move( b );
		EXPECT_EQ( a.IsEmpty(), true );
		EXPECT_EQ( b.IsEmpty(), true );
	}
	{
		A a = 5;
		A b = a;
		a = b;
		EXPECT_EQ( a.IsEmpty(), false );
		EXPECT_EQ( b.IsEmpty(), false );
	}
	{
		A a = 5;
		A b = a;
		a = std::move( b );
		EXPECT_EQ( a.IsEmpty(), false ); // Not empty because a got swapped with b.
		EXPECT_EQ( b.IsEmpty(), false );
	}
}



} // containers
} // core
