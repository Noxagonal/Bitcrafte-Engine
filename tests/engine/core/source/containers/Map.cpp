
#include <gtest/gtest.h>

#include <core/containers/Map.hpp>



namespace core {
namespace containers {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, BasicInit )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;
	{
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

		EXPECT_TRUE( a.IsEmpty() );
		EXPECT_TRUE( b.IsEmpty() );
		EXPECT_TRUE( c.IsEmpty() );
		EXPECT_TRUE( d.IsEmpty() );
		EXPECT_TRUE( e.IsEmpty() );
	}
	{
		A a { P( 1, 5 ), P( 2, 10 ) };
		EXPECT_EQ( a.Size(), 2 );
		EXPECT_FALSE( a.IsEmpty() );

		A b = { P( 1, 5 ), P( 1, 10 ) };
		EXPECT_EQ( b.Size(), 1 );
		EXPECT_FALSE( b.IsEmpty() );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, InsertEmplaceValue )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;

	{
		A a;

		// Regular insert.
		{
			auto insert_value = P( 1, 5 );
			a.Insert( insert_value );
			EXPECT_EQ( a[ 1 ], 5 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// Inserted value to same key, does not increase size but updates value.
		{
			auto insert_value = P( 1, 10 );
			a.Insert( insert_value );
			EXPECT_EQ( a[ 1 ], 10 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// same as above but for moved values
		{
			auto insert_value = P( 2, 5 );
			a.Insert( std::move( insert_value ) );
			EXPECT_EQ( a[ 2 ], 5 );
			EXPECT_EQ( a.Size(), 2 );
		}
		{
			auto insert_value = P( 2, 10 );
			a.Insert( std::move( insert_value ) );
			EXPECT_EQ( a[ 2 ], 10 );
			EXPECT_EQ( a.Size(), 2 );
		}
	}
	{
		A a;

		// Regular emplace.
		{
			uint32_t insert_key = 1;
			uint32_t insert_value = 5;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 1 ], 5 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// Inserted value to same key, does not increase size but updates value.
		{
			uint32_t insert_key = 1;
			uint32_t insert_value = 10;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 1 ], 10 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// same as above but for moved values
		{
			uint32_t insert_key = 2;
			uint32_t insert_value = 5;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 2 ], 5 );
			EXPECT_EQ( a.Size(), 2 );
		}
		{
			uint32_t insert_key = 2;
			uint32_t insert_value = 10;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 2 ], 10 );
			EXPECT_EQ( a.Size(), 2 );
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InsertObject
{
public:
	InsertObject() = default;
	InsertObject( size_t value ) : value( value ) {}
	InsertObject( const InsertObject & other ) = default;
	InsertObject( InsertObject && other ) = default;

	InsertObject & operator=( const InsertObject & other ) = default;
	InsertObject & operator=( InsertObject && other ) = default;

	bool operator==( const InsertObject & other ) const { return value == other.value; }

	size_t value = 0;
};

TEST( MapContainer, InsertEmplaceObject )
{
	using A = bc::Map<uint32_t, InsertObject>;
	using P = bc::Pair<uint32_t, InsertObject>;

	{
		A a;

		// Regular insert.
		{
			P insert_value = { 1, InsertObject( 5 ) };
			a.Insert( insert_value );
			EXPECT_EQ( a[ 1 ], 5 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// Inserted value to same key, does not increase size but updates value.
		{
			auto insert_value = P( 1, 10 );
			a.Insert( insert_value );
			EXPECT_EQ( a[ 1 ], 10 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// same as above but for moved values
		{
			auto insert_value = P( 2, 5 );
			a.Insert( std::move( insert_value ) );
			EXPECT_EQ( a[ 2 ], 5 );
			EXPECT_EQ( a.Size(), 2 );
		}
		{
			auto insert_value = P( 2, 10 );
			a.Insert( std::move( insert_value ) );
			EXPECT_EQ( a[ 2 ], 10 );
			EXPECT_EQ( a.Size(), 2 );
		}
	}
	{
		A a;

		// Regular emplace.
		{
			uint32_t insert_key = 1;
			uint32_t insert_value = 5;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 1 ], 5 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// Inserted value to same key, does not increase size but updates value.
		{
			uint32_t insert_key = 1;
			uint32_t insert_value = 10;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 1 ], 10 );
			EXPECT_EQ( a.Size(), 1 );
		}
		// same as above but for moved values
		{
			uint32_t insert_key = 2;
			uint32_t insert_value = 5;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 2 ], 5 );
			EXPECT_EQ( a.Size(), 2 );
		}
		{
			uint32_t insert_key = 2;
			uint32_t insert_value = 10;
			a.Emplace( insert_key, insert_value );
			EXPECT_EQ( a[ 2 ], 10 );
			EXPECT_EQ( a.Size(), 2 );
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, Iterator )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;

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

		a.Insert( P( 1, 5 ) );
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
	// Test basic iterator
	{
		A a;
		a.Emplace( 2, 10 );
		a.Emplace( 5, 100 );
		a.Emplace( 1, 5 );
		a.Emplace( 3, 20 );
		a.Emplace( 4, 50 );

		auto it = a.begin();
		EXPECT_EQ( it->first, 1 );
		EXPECT_EQ( it->second, 5 );
		++it;
		EXPECT_EQ( it->first, 2 );
		EXPECT_EQ( it->second, 10 );
		++it;
		EXPECT_EQ( it->first, 3 );
		EXPECT_EQ( it->second, 20 );
		++it;
		EXPECT_EQ( it->first, 4 );
		EXPECT_EQ( it->second, 50 );
		++it;
		EXPECT_EQ( it->first, 5 );
		EXPECT_EQ( it->second, 100 );

		EXPECT_EQ( a.begin()->first, 1 );
		EXPECT_EQ( a.begin()->second, 5 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second, 20 );
		EXPECT_EQ( ( a.begin() + 3 )->first, 4 );
		EXPECT_EQ( ( a.begin() + 3 )->second, 50 );
		EXPECT_EQ( ( a.begin() + 4 )->first, 5 );
		EXPECT_EQ( ( a.begin() + 4 )->second, 100 );

		EXPECT_EQ( ( a.end() - 5 )->first, 1 );
		EXPECT_EQ( ( a.end() - 5 )->second, 5 );
		EXPECT_EQ( ( a.end() - 4 )->first, 2 );
		EXPECT_EQ( ( a.end() - 4 )->second, 10 );
		EXPECT_EQ( ( a.end() - 3 )->first, 3 );
		EXPECT_EQ( ( a.end() - 3 )->second, 20 );
		EXPECT_EQ( ( a.end() - 2 )->first, 4 );
		EXPECT_EQ( ( a.end() - 2 )->second, 50 );
		EXPECT_EQ( ( a.end() - 1 )->first, 5 );
		EXPECT_EQ( ( a.end() - 1 )->second, 100 );
	}
	// Test order. Added ascending.
	{
		A a;
		a.Emplace( 1, 5 );
		a.Emplace( 2, 10 );
		a.Emplace( 3, 20 );
		a.Emplace( 4, 50 );
		a.Emplace( 5, 100 );

		uint32_t key = 0, value = 0;
		auto it = a.begin();
		while( it != a.end() ) {
			EXPECT_GT( it->first, key );
			EXPECT_GT( it->second, value );
			key = it->first;
			value = it->second;
			++it;
		}
	}
	// Test order. Added descending.
	{
		A a;
		a.Emplace( 5, 100 );
		a.Emplace( 4, 50 );
		a.Emplace( 3, 20 );
		a.Emplace( 2, 10 );
		a.Emplace( 1, 5 );

		uint32_t key = 0, value = 0;
		auto it = a.begin();
		while( it != a.end() ) {
			EXPECT_GT( it->first, key );
			EXPECT_GT( it->second, value );
			key = it->first;
			value = it->second;
			++it;
		}
	}
	// Test order. Added mixed.
	{
		A a;
		a.Emplace( 8, 1000 );
		a.Emplace( 1, 5 );
		a.Emplace( 5, 100 );
		a.Emplace( 2, 10 );
		a.Emplace( 10, 5000 );
		a.Emplace( 6, 200 );
		a.Emplace( 7, 500 );
		a.Emplace( 4, 50 );
		a.Emplace( 3, 20 );
		a.Emplace( 9, 2000 );

		uint32_t key = 0, value = 0;
		auto it = a.begin();
		while( it != a.end() ) {
			EXPECT_GT( it->first, key );
			EXPECT_GT( it->second, value );
			key = it->first;
			value = it->second;
			++it;
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, Erase )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;
	{
		A a {
			P( 1, 5 ),
			P( 2, 10 ),
			P( 3, 20 ),
			P( 4, 50 ),
			P( 5, 100 ),
		};
		EXPECT_EQ( a.Size(), 5 );

		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 4 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 4 );
		EXPECT_EQ( ( a.begin() + 3 )->first, 5 );

		a.Erase( a.begin() + 2 );
		EXPECT_EQ( a.Size(), 3 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 5 );

		a.Erase( a.end() - 1 );
		EXPECT_EQ( a.Size(), 2 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 3 );
	}
	// Erase range from start
	{
		A a {
			P( 1, 5 ),
			P( 2, 10 ),
			P( 3, 20 ),
			P( 4, 50 ),
			P( 5, 100 ),
		};
		EXPECT_EQ( a.Size(), 5 );

		a.Erase( a.begin(), a.begin() + 2 );
		EXPECT_EQ( a.Size(), 3 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 4 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 5 );
	}
	// Erase range from end
	{
		A a {
			P( 1, 5 ),
			P( 2, 10 ),
			P( 3, 20 ),
			P( 4, 50 ),
			P( 5, 100 ),
		};
		EXPECT_EQ( a.Size(), 5 );

		a.Erase( a.end() - 2, a.end() );
		EXPECT_EQ( a.Size(), 3 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
	}
	// Erase range from middle
	{
		A a {
			P( 1, 5 ),
			P( 2, 10 ),
			P( 3, 20 ),
			P( 4, 50 ),
			P( 5, 100 ),
		};
		EXPECT_EQ( a.Size(), 5 );

		a.Erase( a.begin() + 1, a.end() - 1 );
		EXPECT_EQ( a.Size(), 2 );

		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 5 );
	}
	// Trying to erase over limits
	{
		A a { P( 1, 5 ) };
		EXPECT_THROW( a.Erase( a.end() ), bc::diagnostic::Exception );
		EXPECT_THROW( a.Erase( a.end() - 2 ), bc::diagnostic::Exception );
		EXPECT_THROW( a.Erase( a.begin() - 1 ), bc::diagnostic::Exception );
		EXPECT_THROW( a.Erase( a.begin() + 2 ), bc::diagnostic::Exception );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, FrontBack )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;
	{
		A a;
		EXPECT_THROW( a.Front(), bc::diagnostic::Exception );
		EXPECT_THROW( a.Back(), bc::diagnostic::Exception );
	}
	{
		A a = { P( 1, 5 ) };
		EXPECT_EQ( a.Front().first, 1 );
		EXPECT_EQ( a.Back().first, 1 );

		A b = { P( 1, 5 ), P( 2, 10 ) };
		EXPECT_EQ( b.Front().first, 1 );
		EXPECT_EQ( b.Back().first, 2 );

		A c = { P( 1, 5 ), P( 2, 10 ), P( 3, 20 ) };
		EXPECT_EQ( c.Front().first, 1 );
		EXPECT_EQ( c.Back().first, 3 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, Append )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;

	// Append.
	{
		A a = { P( 1, 5 ), P( 2, 10 ), P( 3, 20 ) };
		A b = { P( 4, 50 ), P( 5, 100 ), P( 6, 200 ) };
		a.Append( b );
		a.Append( { P( 7, 500 ) } );
		a.Append( { P( 8, 1000 ), P( 9, 2000 ) } );

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 3 )->first, 4 );
		EXPECT_EQ( ( a.begin() + 4 )->first, 5 );
		EXPECT_EQ( ( a.begin() + 5 )->first, 6 );
		EXPECT_EQ( ( a.begin() + 6 )->first, 7 );
		EXPECT_EQ( ( a.begin() + 7 )->first, 8 );
		EXPECT_EQ( ( a.begin() + 8 )->first, 9 );
	}
	// Append via += operator.
	{
		A a = { P( 1, 5 ), P( 2, 10 ), P( 3, 20 ) };
		A b = { P( 4, 50 ), P( 5, 100 ), P( 6, 200 ) };
		a += b;
		a += { P( 7, 500 ) };
		a += { P( 8, 1000 ), P( 9, 2000 ) };

		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 3 )->first, 4 );
		EXPECT_EQ( ( a.begin() + 4 )->first, 5 );
		EXPECT_EQ( ( a.begin() + 5 )->first, 6 );
		EXPECT_EQ( ( a.begin() + 6 )->first, 7 );
		EXPECT_EQ( ( a.begin() + 7 )->first, 8 );
		EXPECT_EQ( ( a.begin() + 8 )->first, 9 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, FindKey )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;

	{
		A a;
		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( a.Find( 1 ), a.end() );
	}
	{
		A a = { P( 1, 5 ), P( 2, 10 ), P( 3, 20 ) };
		EXPECT_EQ( a.Size(), 3 );

		EXPECT_EQ( a.Find( 1 ), ( a.begin() + 0 ) );
		EXPECT_EQ( a.Find( 2 ), ( a.begin() + 1 ) );
		EXPECT_EQ( a.Find( 3 ), ( a.begin() + 2 ) );

		EXPECT_EQ( a.Find( 3 ), ( a.begin() + 2 ) );
		EXPECT_EQ( a.Find( 2 ), ( a.begin() + 1 ) );
		EXPECT_EQ( a.Find( 1 ), ( a.begin() + 0 ) );

		EXPECT_EQ( a.Find( 1 ), ( a.end() - 3 ) );
		EXPECT_EQ( a.Find( 2 ), ( a.end() - 2 ) );
		EXPECT_EQ( a.Find( 3 ), ( a.end() - 1 ) );

		EXPECT_EQ( a.Find( 3 ), ( a.end() - 1 ) );
		EXPECT_EQ( a.Find( 2 ), ( a.end() - 2 ) );
		EXPECT_EQ( a.Find( 1 ), ( a.end() - 3 ) );

		EXPECT_EQ( a.Find( 0 ), a.end() );
		EXPECT_EQ( a.Find( 4 ), a.end() );
		EXPECT_EQ( a.Find( 9 ), a.end() );
	}

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, IndexOperator )
{
	using A = bc::Map<uint32_t, uint32_t>;
	using P = bc::Pair<uint32_t, uint32_t>;

	A a;

	a[ 1 ] = 5;
	EXPECT_EQ( a.Size(), 1 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 5 );

	a[ 1 ] = 10;
	EXPECT_EQ( a.Size(), 1 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );

	a[ 2 ] = 20;
	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 20 );

	a[ 2 ] = 50;
	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );

	a[ 5 ] = 100;
	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );
	EXPECT_EQ( ( a.begin() + 2 )->first, 5 );
	EXPECT_EQ( ( a.begin() + 2 )->second, 100 );

	a[ 4 ] = 200;
	EXPECT_EQ( a.Size(), 4 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );
	EXPECT_EQ( ( a.begin() + 2 )->first, 4 );
	EXPECT_EQ( ( a.begin() + 2 )->second, 200 );
	EXPECT_EQ( ( a.begin() + 3 )->first, 5 );
	EXPECT_EQ( ( a.begin() + 3 )->second, 100 );

	a[ 4 ] = 500;
	EXPECT_EQ( a.Size(), 4 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );
	EXPECT_EQ( ( a.begin() + 2 )->first, 4 );
	EXPECT_EQ( ( a.begin() + 2 )->second, 500 );
	EXPECT_EQ( ( a.begin() + 3 )->first, 5 );
	EXPECT_EQ( ( a.begin() + 3 )->second, 100 );

	a[ 3 ] = 1000;
	EXPECT_EQ( a.Size(), 5 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );
	EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
	EXPECT_EQ( ( a.begin() + 2 )->second, 1000 );
	EXPECT_EQ( ( a.begin() + 3 )->first, 4 );
	EXPECT_EQ( ( a.begin() + 3 )->second, 500 );
	EXPECT_EQ( ( a.begin() + 4 )->first, 5 );
	EXPECT_EQ( ( a.begin() + 4 )->second, 100 );

	a[ 3 ] = 2000;
	EXPECT_EQ( a.Size(), 5 );
	EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
	EXPECT_EQ( ( a.begin() + 0 )->second, 10 );
	EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
	EXPECT_EQ( ( a.begin() + 1 )->second, 50 );
	EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
	EXPECT_EQ( ( a.begin() + 2 )->second, 2000 );
	EXPECT_EQ( ( a.begin() + 3 )->first, 4 );
	EXPECT_EQ( ( a.begin() + 3 )->second, 500 );
	EXPECT_EQ( ( a.begin() + 4 )->first, 5 );
	EXPECT_EQ( ( a.begin() + 4 )->second, 100 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, StructureBasicInit )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Map<uint32_t, Simple>;
	using P = bc::Pair<uint32_t, Simple>;

	{
		A a;
		A b {};
		A c = {};
		A d = A();
		A e = A {};

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( b.Size(), 0 );
		EXPECT_EQ( c.Size(), 0 );
		EXPECT_EQ( d.Size(), 0 );
		EXPECT_EQ( e.Size(), 0 );
	}
	{
		A a {
			P( 1, { 5, 10, 20.0f, 50.0 } ),
			P( 2, {} )
		};

		EXPECT_EQ( a.Size(), 2 );

		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v2, 10 );
		EXPECT_FLOAT_EQ( ( a.begin() + 0 )->second.v3, 20.0f );
		EXPECT_DOUBLE_EQ( ( a.begin() + 0 )->second.v4, 50.0 );

		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 0 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v2, 0 );
		EXPECT_FLOAT_EQ( ( a.begin() + 1 )->second.v3, 0.0f );
		EXPECT_DOUBLE_EQ( ( a.begin() + 1 )->second.v4, 0.0 );

		EXPECT_EQ( a.Size(), 2 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, StructureCopy )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Map<uint32_t, Simple>;
	using P = bc::Pair<uint32_t, Simple>;

	A a {
		P( 1, { 5, 10, 20.0f, 50.0 } ),
		P( 2, {} )
	};
	A b = a;

	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( b.Size(), 2 );

	EXPECT_EQ( ( a.begin() + 0 )->second.v1, ( b.begin() + 0 )->second.v1 );
	EXPECT_EQ( ( a.begin() + 0 )->second.v2, ( b.begin() + 0 )->second.v2 );
	EXPECT_FLOAT_EQ( ( a.begin() + 0 )->second.v3, ( b.begin() + 0 )->second.v3 );
	EXPECT_DOUBLE_EQ( ( a.begin() + 0 )->second.v4, ( b.begin() + 0 )->second.v4 );

	EXPECT_EQ( ( a.begin() + 1 )->second.v1, ( b.begin() + 1 )->second.v1 );
	EXPECT_EQ( ( a.begin() + 1 )->second.v2, ( b.begin() + 1 )->second.v2 );
	EXPECT_FLOAT_EQ( ( a.begin() + 1 )->second.v3, ( b.begin() + 1 )->second.v3 );
	EXPECT_DOUBLE_EQ( ( a.begin() + 1 )->second.v4, ( b.begin() + 1 )->second.v4 );

	// Make sure a copy is made.
	b.Front().second.v1 = 600;
	EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
	EXPECT_EQ( ( b.begin() + 0 )->second.v1, 600 );

	A c { P { 1, { 700 } } };
	EXPECT_EQ( c.Size(), 1 );
	EXPECT_EQ( ( c.begin() + 0 )->second.v1, 700 );
	EXPECT_EQ( ( c.begin() + 0 )->second.v2, 0 );
	EXPECT_FLOAT_EQ( ( c.begin() + 0 )->second.v3, 0.0f );
	EXPECT_DOUBLE_EQ( ( c.begin() + 0 )->second.v4, 0.0 );

	c = b;
	EXPECT_EQ( c.Size(), b.Size() );
	EXPECT_EQ( ( c.begin() + 0 )->second.v1, ( b.begin() + 0 )->second.v1 );
	EXPECT_EQ( ( c.begin() + 0 )->second.v2, ( b.begin() + 0 )->second.v2 );
	EXPECT_FLOAT_EQ( ( c.begin() + 0 )->second.v3, ( b.begin() + 0 )->second.v3 );
	EXPECT_DOUBLE_EQ( ( c.begin() + 0 )->second.v4, ( b.begin() + 0 )->second.v4 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, StructureMove )
{
	struct Simple
	{
		size_t v1;
		uint32_t v2;
		float v3;
		double v4;
	};

	using A = bc::Map<uint32_t, Simple>;
	using P = bc::Pair<uint32_t, Simple>;

	// Construct original.
	A a {
		P( 1, { 5, 10, 20.0f, 50.0 } ),
		P( 2, {} )
	};
	EXPECT_EQ( a.Size(), 2 );
	EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
	EXPECT_EQ( ( a.begin() + 0 )->second.v2, 10 );
	EXPECT_FLOAT_EQ( ( a.begin() + 0 )->second.v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( a.begin() + 0 )->second.v4, 50.0 );

	// Move construct.
	A b = std::move( a );
	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 2 );

	EXPECT_EQ( ( b.begin() + 0 )->second.v1, 5 );
	EXPECT_EQ( ( b.begin() + 0 )->second.v2, 10 );
	EXPECT_FLOAT_EQ( ( b.begin() + 0 )->second.v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( b.begin() + 0 )->second.v4, 50.0 );

	// Move assign.
	A c; c = std::move( b );
	EXPECT_EQ( a.Size(), 0 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 2 );

	EXPECT_EQ( ( c.begin() + 0 )->second.v1, 5 );
	EXPECT_EQ( ( c.begin() + 0 )->second.v2, 10 );
	EXPECT_FLOAT_EQ( ( c.begin() + 0 )->second.v3, 20.0f );
	EXPECT_DOUBLE_EQ( ( c.begin() + 0 )->second.v4, 50.0 );

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, MoveableOnlyStructure )
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

	using A = bc::Map<uint32_t, MoveableOnly>;
	using P = bc::Pair<uint32_t, MoveableOnly>;
	{
		A a;
		a.Insert( P( 1, MoveableOnly( 5 ) ) );
		a.Insert( P( 2, MoveableOnly( 10 ) ) );
		a.Insert( P( 3, MoveableOnly( 20 ) ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );
	}
	{
		auto mo1 = P( 1, MoveableOnly( 5 ) );
		auto mo2 = P( 2, MoveableOnly( 10 ) );
		auto mo3 = P( 3, MoveableOnly( 20 ) );

		A a;
		a.Insert( std::move( mo1 ) );
		a.Insert( std::move( mo2 ) );
		a.Insert( std::move( mo3 ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );
	}
	{
		A a;
		a.Emplace( 1, MoveableOnly( 5 ) );
		a.Emplace( 2, MoveableOnly( 10 ) );
		a.Emplace( 3, MoveableOnly( 20 ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );

		// Expecting compile time error for this:
		// A b = a;

		A b = std::move( a );
		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( b.Size(), 3 );

		A c; c = std::move( b );
		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( b.Size(), 0 );
		EXPECT_EQ( c.Size(), 3 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
TEST( MapContainer, CopyableOnlyStructure )
{
	struct CopyableOnly
	{
		CopyableOnly( size_t value ) : v1( value ) {};
		CopyableOnly( const CopyableOnly & other ) = default;
		CopyableOnly( CopyableOnly && other ) = delete;
		CopyableOnly & operator=( const CopyableOnly & other ) = default;
		CopyableOnly & operator=( CopyableOnly && other ) = delete;
		size_t v1;
	};

	using A = bc::Map<uint32_t, CopyableOnly>;
	using P = bc::Pair<uint32_t, CopyableOnly>;
	{
		A a;
		a.Insert( P( 1, CopyableOnly( 5 ) ) );
		a.Insert( P( 2, CopyableOnly( 10 ) ) );
		a.Insert( P( 3, CopyableOnly( 20 ) ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );
	}
	{
		auto mo1 = P( 1, CopyableOnly( 5 ) );
		auto mo2 = P( 2, CopyableOnly( 10 ) );
		auto mo3 = P( 3, CopyableOnly( 20 ) );

		A a;
		a.Insert( std::move( mo1 ) );
		a.Insert( std::move( mo2 ) );
		a.Insert( std::move( mo3 ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );
	}
	{
		A a;
		a.Emplace( 1, CopyableOnly( 5 ) );
		a.Emplace( 2, CopyableOnly( 10 ) );
		a.Emplace( 3, CopyableOnly( 20 ) );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( ( a.begin() + 0 )->first, 1 );
		EXPECT_EQ( ( a.begin() + 0 )->second.v1, 5 );
		EXPECT_EQ( ( a.begin() + 1 )->first, 2 );
		EXPECT_EQ( ( a.begin() + 1 )->second.v1, 10 );
		EXPECT_EQ( ( a.begin() + 2 )->first, 3 );
		EXPECT_EQ( ( a.begin() + 2 )->second.v1, 20 );

		A b = std::move( a );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( b.Size(), 3 );

		A c; c = std::move( b );
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( b.Size(), 3 );
		EXPECT_EQ( c.Size(), 3 );
	}
};
*/



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Map_CopyableOnly_Control
{
public:
	static int32_t copy_counter;
	static int32_t move_counter;

	size_t data = 0;

	Map_CopyableOnly_Control() {}
	Map_CopyableOnly_Control( const Map_CopyableOnly_Control & other ) { ++copy_counter; }
	Map_CopyableOnly_Control( Map_CopyableOnly_Control && other ) { ++move_counter; }

	Map_CopyableOnly_Control & operator=( const Map_CopyableOnly_Control & other ) { ++copy_counter; return *this; }
	Map_CopyableOnly_Control & operator=( Map_CopyableOnly_Control && other ) { ++move_counter; return *this; }
};
int32_t Map_CopyableOnly_Control::copy_counter		= 0;
int32_t Map_CopyableOnly_Control::move_counter		= 0;

class Map_CopyableOnly
{
public:
	static int32_t copy_counter;

	size_t data = 0;

	Map_CopyableOnly() {}
	Map_CopyableOnly( const Map_CopyableOnly & other ) { ++copy_counter; }
	Map_CopyableOnly( Map_CopyableOnly && other ) = delete;

	Map_CopyableOnly & operator=( const Map_CopyableOnly & other ) { ++copy_counter; return *this; }
	Map_CopyableOnly & operator=( Map_CopyableOnly && other ) = delete;
};
int32_t Map_CopyableOnly::copy_counter		= 0;

TEST( MapContainer, Map_CopyableOnlyStructure )
{
	Map_CopyableOnly_Control::copy_counter	= 0;
	Map_CopyableOnly_Control::move_counter	= 0;

	Map_CopyableOnly::copy_counter			= 0;

	using P = bc::Pair<uint32_t, Map_CopyableOnly>;
	using PC = bc::Pair<uint32_t, Map_CopyableOnly_Control>;

	using A = bc::Map<uint32_t, Map_CopyableOnly>;
	using AC = bc::Map<uint32_t, Map_CopyableOnly_Control>;

	A a;
	a.Insert( P( 1, {} ) );
	a.Insert( P( 2, {} ) );
	a.Insert( P( 3, {} ) );

	AC ac;
	ac.Insert( PC( 1, {} ) );
	ac.Insert( PC( 2, {} ) );
	ac.Insert( PC( 3, {} ) );

	A b = a;
	AC bc = ac;

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( b.Size(), 3 );

	EXPECT_EQ( ac.Size(), 3 );
	EXPECT_EQ( bc.Size(), 3 );

	A c = std::move( b );
	AC cc = std::move( bc );

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( b.Size(), 0 );
	EXPECT_EQ( c.Size(), 3 );

	EXPECT_EQ( ac.Size(), 3 );
	EXPECT_EQ( bc.Size(), 0 );
	EXPECT_EQ( cc.Size(), 3 );

	EXPECT_GT( Map_CopyableOnly::copy_counter, Map_CopyableOnly_Control::copy_counter );
	EXPECT_EQ( Map_CopyableOnly::copy_counter, Map_CopyableOnly_Control::copy_counter + Map_CopyableOnly_Control::move_counter );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Map_CtorDtorCounted
{
public:
	static int32_t constructed_counter;

	size_t data = 0;

	Map_CtorDtorCounted() { ++constructed_counter; }
	Map_CtorDtorCounted( const Map_CtorDtorCounted & other ) { ++constructed_counter; }
	Map_CtorDtorCounted( Map_CtorDtorCounted && other ) { ++constructed_counter; }
	~Map_CtorDtorCounted() { --constructed_counter; }
	Map_CtorDtorCounted & operator=( const Map_CtorDtorCounted & other ) = default;
	Map_CtorDtorCounted & operator=( Map_CtorDtorCounted && other ) = default;
};

int32_t Map_CtorDtorCounted::constructed_counter	= 0;

TEST( MapContainer, CtorDtorCounter )
{
	Map_CtorDtorCounted::constructed_counter	= 0;

	using A = bc::Map<size_t, Map_CtorDtorCounted>;
	using P = bc::Pair<size_t, Map_CtorDtorCounted>;

	{
		A a;

		a.Insert( P( 1, {} ) );
		a.Insert( P( 2, Map_CtorDtorCounted() ) );
		a.Insert( P( 3, Map_CtorDtorCounted {} ) );
		EXPECT_EQ( a.Size(), 3 );

		a.Emplace( 4, {} );
		a.Emplace( 5, Map_CtorDtorCounted() );
		a.Emplace( 6, Map_CtorDtorCounted {} );
		EXPECT_EQ( a.Size(), 6 );

		a.Erase( a.begin() + 2 );
		EXPECT_EQ( a.Size(), 5 );

		a.Erase( a.begin() + 2 );
		EXPECT_EQ( a.Size(), 4 );

		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( Map_CtorDtorCounted::constructed_counter, 0 );
	}
	{
		A a;

		for( size_t i = 0; i < 50; ++i ) {
			a.Emplace( i, {} );
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
		EXPECT_EQ( Map_CtorDtorCounted::constructed_counter, 0 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MapContainer, SelfAssignment )
{
	{
		using A = bc::Map<uint32_t, uint32_t>;
		using P = A::ContainedPairType;

		A original { P{ 5, 50 }, P{ 10, 100 }, P{ 20, 200 }, P{ 50, 500 }, P{ 200, 2000 } };
		{
			A a = original;
			a = a;
			EXPECT_EQ( a.Size(), 5 );
			EXPECT_EQ( a, original );
		}
		{
			A a = original;
			a.Append( a );

			EXPECT_EQ( a.Size(), 5 );
			A comp { P{ 5, 50 }, P{ 10, 100 }, P{ 20, 200 }, P{ 50, 500 }, P{ 200, 2000 } };
			EXPECT_EQ( a, comp );
		}
	}
	{
		Map_CtorDtorCounted::constructed_counter		= 0;

		using A = bc::Map<uint32_t, Map_CtorDtorCounted>;
		using P = A::ContainedPairType;
		A a;
		a.Insert( P { 5, Map_CtorDtorCounted() } );
		a.Insert( P { 10, Map_CtorDtorCounted() } );
		a.Insert( P { 20, Map_CtorDtorCounted() } );
		for( size_t i = 0; i < 5; i++ ) {
			a.Insert( P { uint32_t( i ), Map_CtorDtorCounted() } );
			a = a;
			a.Append( a );
		}
		a.Clear();

		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( Map_CtorDtorCounted::constructed_counter, 0 );
	}
}



} // containers
} // core
