
#include <gtest/gtest.h>
#include <TestsCommon.hpp>

#include <core/containers/Text.hpp>



namespace containers {
namespace array {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, BasicInit )
{
	using A = qup::Text;
	using B = qup::Text8;
	using C = qup::Text16;
	using D = qup::Text32;

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
	}
	{
		A t = "Test";
		EXPECT_EQ( t.Size(), 4 );
		EXPECT_EQ( t, "Test" );

		t = "Normal";
		EXPECT_EQ( t.Size(), 6 );
		EXPECT_EQ( t, "Normal" );
	}
	{
		B t = "Test";
		EXPECT_EQ( t.Size(), 4 );
		EXPECT_EQ( t, u8"Test" );
		EXPECT_EQ( t, "Test" );

		t = "Normal";
		EXPECT_EQ( t.Size(), 6 );
		EXPECT_EQ( t, u8"Normal" );
		EXPECT_EQ( t, "Normal" );
	}
	{
		C t = "Test";
		EXPECT_EQ( t.Size(), 4 );
		EXPECT_EQ( t, u"Test" );
		EXPECT_EQ( t, "Test" );

		t = "Normal";
		EXPECT_EQ( t.Size(), 6 );
		EXPECT_EQ( t, u"Normal" );
		EXPECT_EQ( t, "Normal" );
	}
	{
		D t = "Test";
		EXPECT_EQ( t.Size(), 4 );
		EXPECT_EQ( t, U"Test" );
		EXPECT_EQ( t, "Test" );

		t = "Normal";
		EXPECT_EQ( t.Size(), 6 );
		EXPECT_EQ( t, U"Normal" );
		EXPECT_EQ( t, "Normal" );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, ConstructWithSize )
{
	using A = qup::Text32;
	A a;
	EXPECT_EQ( a.Size(), 0 );

	A c { char( 5 ), char( 10 ) };
	EXPECT_EQ( c.Size(), 2 );

	A d = { char( 5 ), char( 10 ), char( 20 ) };
	EXPECT_EQ( d.Size(), 3 );

	A f = A { char( 5 ), char( 10 ), char( 20 ), char( 50 ), char( 100 ) };
	EXPECT_EQ( f.Size(), 5 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, IndexOperator )
{
	using A = qup::Text32;
	A a { char( 5 ), char( 10 ), char( 20 ) };

	EXPECT_EQ( a.Size(), 3 );
	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, Erase )
{
	using A = qup::Text32;
	{
		A a = "abcdefghij";

		EXPECT_THROW( a.begin() - 1, qup::exception::Exception );
		EXPECT_THROW( a.end() + 1, qup::exception::Exception );
		EXPECT_THROW( a.Erase( a.end() ), qup::exception::Exception );
		EXPECT_THROW( a.Erase( a.begin() + 10 ), qup::exception::Exception );
	}

	using A = qup::Text32;
	{
		A a = "abcdefghij";

		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 9 );
		EXPECT_EQ( a[ 0 ], 'b' );
		EXPECT_EQ( a[ 1 ], 'c' );
		EXPECT_EQ( a[ 2 ], 'd' );
		EXPECT_EQ( a[ 3 ], 'e' );
		EXPECT_EQ( a[ 4 ], 'f' );
		EXPECT_EQ( a[ 5 ], 'g' );
		EXPECT_EQ( a[ 6 ], 'h' );
		EXPECT_EQ( a[ 7 ], 'i' );
		EXPECT_EQ( a[ 8 ], 'j' );
	}
	{
		A a = "abcdefghij";
		a.Erase( a.end() - 1 );
		a.Erase( a.begin() );
		EXPECT_EQ( a.Size(), 8 );
		EXPECT_EQ( a[ 0 ], 'b' );
		EXPECT_EQ( a[ 1 ], 'c' );
		EXPECT_EQ( a[ 2 ], 'd' );
		EXPECT_EQ( a[ 3 ], 'e' );
		EXPECT_EQ( a[ 4 ], 'f' );
		EXPECT_EQ( a[ 5 ], 'g' );
		EXPECT_EQ( a[ 6 ], 'h' );
		EXPECT_EQ( a[ 7 ], 'i' );
	}
	{
		A a = "abcdefghij";
		a.Erase( a.begin(), a.end() - 5 );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( a[ 0 ], 'f' );
		EXPECT_EQ( a[ 1 ], 'g' );
		EXPECT_EQ( a[ 2 ], 'h' );
		EXPECT_EQ( a[ 3 ], 'i' );
		EXPECT_EQ( a[ 4 ], 'j' );
	}
	{
		A a = "abcdefghij";
		a.Erase( a.begin() + 5, a.end() );
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( a[ 0 ], 'a' );
		EXPECT_EQ( a[ 1 ], 'b' );
		EXPECT_EQ( a[ 2 ], 'c' );
		EXPECT_EQ( a[ 3 ], 'd' );
		EXPECT_EQ( a[ 4 ], 'e' );
	}
	{
		A a = "abcdefghij";
		a.Erase( a.begin() + 3, a.end() - 3 );
		EXPECT_EQ( a.Size(), 6 );
		EXPECT_EQ( a[ 0 ], 'a' );
		EXPECT_EQ( a[ 1 ], 'b' );
		EXPECT_EQ( a[ 2 ], 'c' );
		EXPECT_EQ( a[ 3 ], 'h' );
		EXPECT_EQ( a[ 4 ], 'i' );
		EXPECT_EQ( a[ 5 ], 'j' );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, FrontBack )
{
	using A = qup::Text32;
	{
		A a;

		EXPECT_THROW( auto t = a.Front(), qup::exception::Exception );
		EXPECT_THROW( auto t = a.Back(), qup::exception::Exception );
	}
	{
		A b = { char( 5 ) };
		EXPECT_EQ( b.Front(), 5 );
		EXPECT_EQ( b.Back(), 5 );

		A c = { char( 5 ), char( 10 ) };
		EXPECT_EQ( c.Front(), 5 );
		EXPECT_EQ( c.Back(), 10 );

		A d = { char( 5 ), char( 10 ), char( 15 ) };
		EXPECT_EQ( d.Front(), 5 );
		EXPECT_EQ( d.Back(), 15 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, PushBack )
{
	using A = qup::Text32;

	A a;
	a.PushBack( 5 );
	a.PushBack( 10 );
	a.PushBack( 20 );

	EXPECT_EQ( a[ 0 ], 5 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 20 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, PushFront )
{
	using A = qup::Text32;

	A a;
	a.PushFront( 5 );
	a.PushFront( 10 );
	a.PushFront( 20 );

	EXPECT_EQ( a[ 0 ], 20 );
	EXPECT_EQ( a[ 1 ], 10 );
	EXPECT_EQ( a[ 2 ], 5 );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, MixedPushBackPushFront )
{
	using A = qup::Text32;
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
TEST( CoreContainerText, Append )
{
	using A = qup::Text32;

	{
		A a { char( 5 ), char( 10 ), char( 20 ) };
		A b { "abc" };
		a.Append( b );
		a.Append( { char32_t( 500 ) } );
		a.Append( { char32_t( 1000 ), char32_t( 2000 ) } );
		a.Append( { 'A', 'B' } );
		a.Append( "Test" );
		a.Append( U"UTest" );

		EXPECT_EQ( a.Size(), 20 );
		EXPECT_EQ( a[ 0 ], 5 );
		EXPECT_EQ( a[ 1 ], 10 );
		EXPECT_EQ( a[ 2 ], 20 );
		EXPECT_EQ( a[ 3 ], 'a' );
		EXPECT_EQ( a[ 4 ], 'b' );
		EXPECT_EQ( a[ 5 ], 'c' );
		EXPECT_EQ( a[ 6 ], 500 );
		EXPECT_EQ( a[ 7 ], 1000 );
		EXPECT_EQ( a[ 8 ], 2000 );
		EXPECT_EQ( a[ 9 ], 'A' );
		EXPECT_EQ( a[ 10 ], 'B' );
		EXPECT_EQ( a[ 11 ], 'T' );
		EXPECT_EQ( a[ 12 ], 'e' );
		EXPECT_EQ( a[ 13 ], 's' );
		EXPECT_EQ( a[ 14 ], 't' );
		EXPECT_EQ( a[ 15 ], 'U' );
		EXPECT_EQ( a[ 16 ], 'T' );
		EXPECT_EQ( a[ 17 ], 'e' );
		EXPECT_EQ( a[ 18 ], 's' );
		EXPECT_EQ( a[ 19 ], 't' );
	}
	{
		A a { char( 5 ), char( 10 ), char( 20 ) };
		A b { "abc" };
		a += b;
		a += { char32_t( 500 ) };
		a += { char32_t( 1000 ), char32_t( 2000 ) };
		a += { 'A', 'B' };
		a += "Test";
		a += U"UTest";

		EXPECT_EQ( a.Size(), 20 );
		EXPECT_EQ( a[ 0 ], 5 );
		EXPECT_EQ( a[ 1 ], 10 );
		EXPECT_EQ( a[ 2 ], 20 );
		EXPECT_EQ( a[ 3 ], 'a' );
		EXPECT_EQ( a[ 4 ], 'b' );
		EXPECT_EQ( a[ 5 ], 'c' );
		EXPECT_EQ( a[ 6 ], char32_t( 500 ) );
		EXPECT_EQ( a[ 7 ], char32_t( 1000 ) );
		EXPECT_EQ( a[ 8 ], char32_t( 2000 ) );
		EXPECT_EQ( a[ 9 ], 'A' );
		EXPECT_EQ( a[ 10 ], 'B' );
		EXPECT_EQ( a[ 11 ], 'T' );
		EXPECT_EQ( a[ 12 ], 'e' );
		EXPECT_EQ( a[ 13 ], 's' );
		EXPECT_EQ( a[ 14 ], 't' );
		EXPECT_EQ( a[ 15 ], 'U' );
		EXPECT_EQ( a[ 16 ], 'T' );
		EXPECT_EQ( a[ 17 ], 'e' );
		EXPECT_EQ( a[ 18 ], 's' );
		EXPECT_EQ( a[ 19 ], 't' );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, Replace )
{
	using A = qup::Text32;

	// Simple replacements.
	{
		A a = "Replace and place a new text somewhere here";

		a.Replace( U"Replace", U"Testing" );
		EXPECT_EQ( a, U"Testing and place a new text somewhere here" );

		a.Replace( U"Testing", U"Test" );
		EXPECT_EQ( a, U"Test and place a new text somewhere here" );

		a.Replace( U"Test", U"placing" );
		EXPECT_EQ( a, U"placing and place a new text somewhere here" );

		a.Replace( U"plac", U"" );
		EXPECT_EQ( a, U"ing and place a new text somewhere here" );

		a.Replace( U"", U"should not do anything" );
		EXPECT_EQ( a, U"ing and place a new text somewhere here" );

		a.Replace( U" here", U" there" );
		EXPECT_EQ( a, U"ing and place a new text somewhere there" );

		a.Replace( U"text", U"test" );
		EXPECT_EQ( a, U"ing and place a new test somewhere there" );
		
		a.Replace( U"ing and ", U"" );
		EXPECT_EQ( a, U"place a new test somewhere there" );

		a.Replace( U"a new", U"another" );
		EXPECT_EQ( a, U"place another test somewhere there" );

		a.Replace( U"should not find", U"should not replace" );
		EXPECT_EQ( a, U"place another test somewhere there" );
	}

	// Multiple replacements.
	{
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 0, 2 );
			EXPECT_EQ( a, U"1 1 multiple multiple" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 1, 2 );
			EXPECT_EQ( a, U"multiple 1 1 multiple" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 2, 2 );
			EXPECT_EQ( a, U"multiple 1 1 multiple" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 9, 2 );
			EXPECT_EQ( a, U"multiple 1 1 multiple" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 10, 2 );
			EXPECT_EQ( a, U"multiple multiple 1 1" );
		}

		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 9 );
			EXPECT_EQ( a, U"multiple 1 multiple multiple" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 10 );
			EXPECT_EQ( a, U"multiple multiple 1 multiple" );
		}

		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 0, SIZE_MAX );
			EXPECT_EQ( a, U"1 1 1 1" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 1, SIZE_MAX );
			EXPECT_EQ( a, U"multiple 1 1 1" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 9, SIZE_MAX );
			EXPECT_EQ( a, U"multiple 1 1 1" );
		}
		{
			A a = "multiple multiple multiple multiple";

			a.Replace( U"multiple", U"1", 10, SIZE_MAX );
			EXPECT_EQ( a, U"multiple multiple 1 1" );
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreContainerText, Iterator )
{
	using A = qup::Text32;

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
TEST( CoreContainerText, SelfAssignment )
{
	{
		using A = qup::Text32;
		using AView = A::ViewType;

		A original { char( 5 ), char( 10 ), char( 20 ), char( 50 ), char( 200 ) };
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

			A comp { char( 10 ), char( 20 ), char( 50 ) };
			EXPECT_EQ( a.Size(), 3 );
			EXPECT_EQ( a, comp );
		}
		{
			A a = original;
			a.Append( a );

			A comp { char( 5 ), char( 10 ), char( 20 ), char( 50 ), char( 200 ), char( 5 ), char( 10 ), char( 20 ), char( 50 ), char( 200 ) };
			EXPECT_EQ( a.Size(), 10 );
			EXPECT_EQ( a, comp );
		}
	}
}



} // array
} // containers
