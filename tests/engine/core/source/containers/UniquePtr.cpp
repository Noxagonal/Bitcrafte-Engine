
#include <gtest/gtest.h>

#include <core/containers/UniquePtr.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, BasicInit )
{
	using namespace bc;
	
	using A = bc::UniquePtr<i32>;
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

	A a2( bc::MakeUniquePtr<i32>( 5 ) );
	EXPECT_FALSE( a2.IsEmpty() );

	A b2 { bc::MakeUniquePtr<i32>( 5 ) };
	EXPECT_FALSE( b2.IsEmpty() );

	A c2 = bc::MakeUniquePtr<i32>( 5 );
	EXPECT_FALSE( c2.IsEmpty() );

	A d2 = A( bc::MakeUniquePtr<i32>( 5 ) );
	EXPECT_FALSE( d2.IsEmpty() );

	A e2 = A { bc::MakeUniquePtr<i32>( 5 ) };
	EXPECT_FALSE( e2.IsEmpty() );

	A a3;
	a3 = bc::MakeUniquePtr<i32>( 5 );
	EXPECT_FALSE( a3.IsEmpty() );

	A b3;
	b3 = { bc::MakeUniquePtr<i32>( 5 ) };
	EXPECT_FALSE( b3.IsEmpty() );

	A c3;
	c3 = A { bc::MakeUniquePtr<i32>( 5 ) };
	EXPECT_FALSE( c3.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, Clear )
{
	using namespace bc;
	
	using A = bc::UniquePtr<i32>;
	{
		auto a = bc::MakeUniquePtr<i32>( 5 );
		EXPECT_FALSE( a.IsEmpty() );

		a.Clear();
		EXPECT_TRUE( a.IsEmpty() );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UniquePtr_EmplaceObject
{
public:
	UniquePtr_EmplaceObject( bc::i32 value ) : value( value ) {}
	UniquePtr_EmplaceObject( const UniquePtr_EmplaceObject & other ) = default;
	UniquePtr_EmplaceObject( UniquePtr_EmplaceObject && other ) = default;

	UniquePtr_EmplaceObject & operator=( const UniquePtr_EmplaceObject & other ) = default;
	UniquePtr_EmplaceObject & operator=( UniquePtr_EmplaceObject && other ) = default;

	bool operator==( const UniquePtr_EmplaceObject & other ) const { return value == other.value; }

	bc::i32 value = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, NonTrivialTypes )
{
	using namespace bc;
	
	using A = bc::UniquePtr<i32>;
	using B = bc::UniquePtr<UniquePtr_EmplaceObject>;

	{
		A a;
		a.Emplace( 5 );

		EXPECT_EQ( *a.Get(), 5 );
	}
	{
		B b;
		b.Emplace( 5 );

		EXPECT_EQ( b->value, 5 );
	}
};



template <typename T>
concept UniquePtr_CopyConstructibleType = requires( T a )
{
	{ a = a } -> std::same_as<T&>;
};

template <typename T>
concept UniquePtr_MoveConstructibleType = requires( T a )
{
	{ a = std::move( a ) } -> std::same_as<T&>;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, StructureCopy )
{
	using namespace bc;
	
	struct Simple
	{
		u64 v1		= {};
		i32 v2		= {};
		f32 v3		= {};
		f64 v4		= {};
	};

	using A = bc::UniquePtr<Simple>;
	{
		A a = bc::MakeUniquePtr<Simple>( u64( 1 ), 2, 3.3f, 4.4 );
		EXPECT_FALSE( UniquePtr_CopyConstructibleType<A> );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, StructureMove )
{
	using namespace bc;
	
	struct Simple
	{
		u64 v1		= {};
		i32 v2		= {};
		f32 v3		= {};
		f64 v4		= {};
	};

	using A = bc::UniquePtr<Simple>;
	{
		EXPECT_TRUE( UniquePtr_MoveConstructibleType<A> );
		A a = bc::MakeUniquePtr<Simple>( u64( 1 ), 2, 3.3f, 4.4 );
		A b = std::move( a );

		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );

		EXPECT_EQ( b->v1, 1 );
		EXPECT_EQ( b->v2, 2 );
		EXPECT_FLOAT_EQ( b->v3, 3.3f );
		EXPECT_DOUBLE_EQ( b->v4, 4.4 );
	}
};

template<typename T>
concept UniquePtr_IsMoveableOnlyCopyable = requires( T object )
{
	object = object;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, MoveableOnlyStructure )
{
	using namespace bc;
	
	struct MoveableOnly
	{
		MoveableOnly( bc::i32 value ) : v1( value ) {};
		MoveableOnly( const MoveableOnly & other ) = delete;
		MoveableOnly( MoveableOnly && other ) = default;
		MoveableOnly & operator=( const MoveableOnly & other ) = delete;
		MoveableOnly & operator=( MoveableOnly && other ) = default;
		bc::i32 v1 = {};
	};

	using A = bc::UniquePtr<MoveableOnly>;
	A a = bc::MakeUniquePtr<MoveableOnly>( 5 );

	// Expecting compile time error for this:
	//A b = a;
	EXPECT_FALSE( UniquePtr_IsMoveableOnlyCopyable<A> );

	A b = std::move( a );
	EXPECT_TRUE( a.IsEmpty() );
	EXPECT_FALSE( b.IsEmpty() );

	A c; c = std::move( b );
	EXPECT_TRUE( a.IsEmpty() );
	EXPECT_TRUE( b.IsEmpty() );
	EXPECT_FALSE( c.IsEmpty() );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UniquePtr_CopyableOnly_Control
{
public:
	static bc::i32 copy_counter;
	static bc::i32 move_counter;

	bc::i32 data = 0;

	UniquePtr_CopyableOnly_Control() {}
	UniquePtr_CopyableOnly_Control( const UniquePtr_CopyableOnly_Control & other ) { ++copy_counter; }
	UniquePtr_CopyableOnly_Control( UniquePtr_CopyableOnly_Control && other ) { ++move_counter; }

	UniquePtr_CopyableOnly_Control & operator=( const UniquePtr_CopyableOnly_Control & other ) { ++copy_counter; return *this; }
	UniquePtr_CopyableOnly_Control & operator=( UniquePtr_CopyableOnly_Control && other ) { ++move_counter; return *this; }
};
bc::i32 UniquePtr_CopyableOnly_Control::copy_counter		= 0;
bc::i32 UniquePtr_CopyableOnly_Control::move_counter		= 0;

class UniquePtr_CopyableOnly
{
public:
	static bc::i32 copy_counter;

	bc::i32 data = 0;

	UniquePtr_CopyableOnly() {}
	UniquePtr_CopyableOnly( const UniquePtr_CopyableOnly & other ) { ++copy_counter; }
	UniquePtr_CopyableOnly( UniquePtr_CopyableOnly && other ) = delete;

	UniquePtr_CopyableOnly & operator=( const UniquePtr_CopyableOnly & other ) { ++copy_counter; return *this; }
	UniquePtr_CopyableOnly & operator=( UniquePtr_CopyableOnly && other ) = delete;
};
int32_t UniquePtr_CopyableOnly::copy_counter		= 0;

TEST( UniquePtrContainer, CopyableOnlyStructure )
{
	using namespace bc;
	
	UniquePtr_CopyableOnly_Control::copy_counter	= 0;
	UniquePtr_CopyableOnly_Control::move_counter	= 0;

	UniquePtr_CopyableOnly::copy_counter			= 0;

	using A = bc::UniquePtr<UniquePtr_CopyableOnly>;
	using B = bc::UniquePtr<UniquePtr_CopyableOnly_Control>;

	A a = bc::MakeUniquePtr<UniquePtr_CopyableOnly>();
	EXPECT_EQ( UniquePtr_CopyableOnly::copy_counter, 0 );

	B ac = bc::MakeUniquePtr<UniquePtr_CopyableOnly_Control>();
	EXPECT_EQ( UniquePtr_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( UniquePtr_CopyableOnly_Control::move_counter, 0 );

	EXPECT_FALSE( a.IsEmpty() );
	EXPECT_FALSE( ac.IsEmpty() );

	A c = std::move( a );
	B cc = std::move( ac );

	EXPECT_TRUE( a.IsEmpty() );
	EXPECT_FALSE( c.IsEmpty() );

	EXPECT_TRUE( ac.IsEmpty() );
	EXPECT_FALSE( cc.IsEmpty() );

	EXPECT_EQ( UniquePtr_CopyableOnly::copy_counter, 0 );
	EXPECT_EQ( UniquePtr_CopyableOnly_Control::copy_counter, 0 );
	EXPECT_EQ( UniquePtr_CopyableOnly_Control::move_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UniquePtr_CtorDtorCounted
{
public:
	static bc::i32 constructed_counter;

	bc::i32 data = 0;

	UniquePtr_CtorDtorCounted() { ++constructed_counter; data = constructed_counter; }
	UniquePtr_CtorDtorCounted( const UniquePtr_CtorDtorCounted & other ) { ++constructed_counter; data = constructed_counter; }
	UniquePtr_CtorDtorCounted( UniquePtr_CtorDtorCounted && other ) { ++constructed_counter; data = constructed_counter; }
	~UniquePtr_CtorDtorCounted() { --constructed_counter; }
	UniquePtr_CtorDtorCounted & operator=( const UniquePtr_CtorDtorCounted & other ) = default;
	UniquePtr_CtorDtorCounted & operator=( UniquePtr_CtorDtorCounted && other ) = default;
};
int32_t UniquePtr_CtorDtorCounted::constructed_counter	= 0;

TEST( UniquePtrContainer, CtorDtorCounter )
{
	UniquePtr_CtorDtorCounted::constructed_counter		= 0;

	using A = bc::UniquePtr<UniquePtr_CtorDtorCounted>;
	{
		A a = bc::MakeUniquePtr<UniquePtr_CtorDtorCounted>();
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_EQ( UniquePtr_CtorDtorCounted::constructed_counter, 1 );
	}
	EXPECT_EQ( UniquePtr_CtorDtorCounted::constructed_counter, 0 );
	{
		A a = bc::MakeUniquePtr<UniquePtr_CtorDtorCounted>();
		a.Clear();
		EXPECT_EQ( UniquePtr_CtorDtorCounted::constructed_counter, 0 );
	}

	EXPECT_EQ( UniquePtr_CtorDtorCounted::constructed_counter, 0 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, SelfAssignment )
{
	using namespace bc;
	
	using A = bc::UniquePtr<i32>;
	{
		A a;
		a = std::move( a );
		EXPECT_TRUE( a.IsEmpty() );
	}
	{
		A a = bc::MakeUniquePtr<i32>( 5 );
		a = std::move( a );
		EXPECT_FALSE( a.IsEmpty() );
	}
	{
		A a = bc::MakeUniquePtr<i32>( 5 );
		A b = std::move( a );
		a = std::move( b );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( b.IsEmpty() );
	}
}



TEST( UniquePtrContainer, ImplicitDerivedTypeMove )
{
	using namespace bc;
	
	struct Base{};
	struct Derived : public Base {};

	using A = bc::UniquePtr<Base>;
	using B = bc::UniquePtr<Derived>;
	{
		A a;
		B b = bc::MakeUniquePtr<Derived>();
		a = std::move( b );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( b.IsEmpty() );
	}
	{
		A a = bc::MakeUniquePtr<Base>();
		B b = bc::MakeUniquePtr<Derived>();
		a = std::move( b );
		EXPECT_FALSE( a.IsEmpty() );
		EXPECT_TRUE( b.IsEmpty() );
	}
	{
		A a = bc::MakeUniquePtr<Derived>();
		EXPECT_FALSE( a.IsEmpty() );
	}
	{
		A a[ 3 ] = { bc::MakeUniquePtr<Derived>(), bc::MakeUniquePtr<Base>(), bc::MakeUniquePtr<Derived>() };
		EXPECT_FALSE( a[ 0 ].IsEmpty() );
		EXPECT_FALSE( a[ 1 ].IsEmpty() );
		EXPECT_FALSE( a[ 2 ].IsEmpty() );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UniquePtrContainer, CastTo )
{
	using namespace bc;
	
	struct Base{ virtual ~Base() = default; };
	struct Derived_1 : public Base {};
	struct Derived_2 : public Derived_1 {};
	struct Derived_3 : public Base {};

	using B = bc::UniquePtr<Base>;
	using D1 = bc::UniquePtr<Derived_1>;
	using D2 = bc::UniquePtr<Derived_2>;
	using D3 = bc::UniquePtr<Derived_3>;

	// Same-cast
	{
		auto a = bc::MakeUniquePtr<Base>();
		auto b = a.CastTo<Base>();
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = bc::MakeUniquePtr<Derived_1>();
		auto b = a.CastTo<Derived_1>();
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	{
		auto a = bc::MakeUniquePtr<Derived_2>();
		auto b = a.CastTo<Derived_2>();
		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_FALSE( b.IsEmpty() );
	}
	// Up-cast
	{
		auto d = bc::MakeUniquePtr<Derived_1>();
		auto base = d.CastTo<Base>();
		EXPECT_TRUE( d.IsEmpty() );
		EXPECT_FALSE( base.IsEmpty() );
	}
	{
		auto d = bc::MakeUniquePtr<Derived_2>();
		auto base = d.CastTo<Base>();
		EXPECT_TRUE( d.IsEmpty() );
		EXPECT_FALSE( base.IsEmpty() );
	}
	// Down-cast
	{
		B base = bc::MakeUniquePtr<Derived_1>();
		auto d = base.CastTo<Derived_1>();
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( base.IsEmpty() );
	}
	{
		B base = bc::MakeUniquePtr<Derived_2>();
		auto d = base.CastTo<Derived_2>();
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( base.IsEmpty() );
	}
	{
		B base = bc::MakeUniquePtr<Derived_2>();
		auto d = base.CastTo<Derived_1>();
		EXPECT_FALSE( d.IsEmpty() );
		EXPECT_TRUE( base.IsEmpty() );
	}
	// Cast failure
	{
		B base = bc::MakeUniquePtr<Derived_3>();
		// Note that in release configuration, this will return an empty UniquePtr instead of throwing an exception.
		EXPECT_THROW( base.CastTo<Derived_1>(), bc::diagnostic::Exception );
		EXPECT_FALSE( base.IsEmpty() );
	}
}



} // containers
} // core
