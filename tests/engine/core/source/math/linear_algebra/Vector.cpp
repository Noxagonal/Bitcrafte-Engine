
#include <gtest/gtest.h>

#include <core/math/linear_algebra/Vector.hpp>
#include <core/utility/math/VectorComparison.hpp>



namespace core {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, BasicInitialize )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a;
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );
	}
	{
		A3 a;
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );
		EXPECT_DOUBLE_EQ( a.z, 0.0 );
	}
	{
		A4 a;
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );
		EXPECT_DOUBLE_EQ( a.z, 0.0 );
		EXPECT_DOUBLE_EQ( a.w, 0.0 );
	}

	{
		A2 a { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );
	}
	{
		A3 a { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );
		EXPECT_DOUBLE_EQ( a.z, 3.0 );
	}
	{
		A4 a { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );
		EXPECT_DOUBLE_EQ( a.z, 3.0 );
		EXPECT_DOUBLE_EQ( a.w, 4.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Comparisons )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a {};
		A3 b {};
		A4 c {};
		EXPECT_EQ( a, a );
		EXPECT_EQ( b, b );
		EXPECT_EQ( c, c );
	}
	{
		A2 a1 {};
		A3 b1 {};
		A4 c1 {};
		A2 a2 {};
		A3 b2 {};
		A4 c2 {};
		EXPECT_EQ( a1, a2 );
		EXPECT_EQ( b1, b2 );
		EXPECT_EQ( c1, c2 );
	}
	{
		A2 a1;
		A3 b1;
		A4 c1;
		A2 a2 {};
		A3 b2 {};
		A4 c2 {};
		EXPECT_EQ( a1, a2 );
		EXPECT_EQ( b1, b2 );
		EXPECT_EQ( c1, c2 );
	}
	{
		A2 a { 1.0, 1.0 };
		A3 b { 1.0, 1.0, 1.0 };
		A4 c { 1.0, 1.0, 1.0, 1.0 };
		EXPECT_EQ( a, a );
		EXPECT_EQ( b, b );
		EXPECT_EQ( c, c );
	}
	{
		A2 a1;
		A3 b1;
		A4 c1;
		A2 a2 { 1.0, 1.0 };
		A3 b2 { 1.0, 1.0, 1.0 };
		A4 c2 { 1.0, 1.0, 1.0, 1.0 };
		EXPECT_NE( a1, a2 );
		EXPECT_NE( b1, b2 );
		EXPECT_NE( c1, c2 );
	}
	{
		A2 a1 {};
		A3 b1 {};
		A4 c1 {};
		A2 a2 { 1.0, 1.0 };
		A3 b2 { 1.0, 1.0, 1.0 };
		A4 c2 { 1.0, 1.0, 1.0, 1.0 };
		EXPECT_NE( a1, a2 );
		EXPECT_NE( b1, b2 );
		EXPECT_NE( c1, c2 );
	}
	{
		A2 a1 { 1.0 };
		A3 b1 { 1.0 };
		A4 c1 { 1.0 };
		A2 a2 { 1.0, 1.0 };
		A3 b2 { 1.0, 1.0, 1.0 };
		A4 c2 { 1.0, 1.0, 1.0, 1.0 };
		EXPECT_EQ( a1, a2 );
		EXPECT_EQ( b1, b2 );
		EXPECT_EQ( c1, c2 );
	}
	{
		A2 a1;
		A3 b1;
		A4 c1;
		A2 a2 { 0.0 };
		A3 b2 { 0.0 };
		A4 c2 { 0.0 };
		EXPECT_EQ( a1, a2 );
		EXPECT_EQ( b1, b2 );
		EXPECT_EQ( c1, c2 );
	}
	{
		A2 am4 { -2.0 };
		A2 am3 { -1.5 };
		A2 am2 { -1.0 };
		A2 am1 { -0.5 };
		A2 a0 { 0.0 };
		A2 ap1 { 0.5 };
		A2 ap2 { 1.0 };
		A2 ap3 { 1.5 };
		A2 ap4 { 2 };

		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am4, 1.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am3, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am2, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am1, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, a0,  1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap1, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap2, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap3, 1.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap4, 1.5 ), false );

		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am4, 1.0 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am3, 1.0 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am2, 1.0 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am1, 1.0 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, a0,  1.0 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap1, 1.0 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap2, 1.0 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap3, 1.0 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap4, 1.0 ), false );

		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am4, 0.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am3, 0.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am2, 0.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, am1, 0.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, a0,  0.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap1, 0.5 ), true );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap2, 0.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap3, 0.5 ), false );
		EXPECT_EQ( bc::utility::AreValuesWithinRange( a0, ap4, 0.5 ), false );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Addition )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 1.0, 2.0 };

		a += { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );

		auto b = a + A2 { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( b.x, 3.0 );
		EXPECT_DOUBLE_EQ( b.y, 6.0 );
	}
	{
		A3 a { 1.0, 2.0, 3.0 };

		a += { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 6.0 );

		auto b = a + A3 { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( b.x, 3.0 );
		EXPECT_DOUBLE_EQ( b.y, 6.0 );
		EXPECT_DOUBLE_EQ( b.z, 9.0 );
	}
	{
		A4 a { 1.0, 2.0, 3.0, 4.0 };

		a += { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 6.0 );
		EXPECT_DOUBLE_EQ( a.w, 8.0 );

		auto b = a + A4 { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( b.x, 3.0 );
		EXPECT_DOUBLE_EQ( b.y, 6.0 );
		EXPECT_DOUBLE_EQ( b.z, 9.0 );
		EXPECT_DOUBLE_EQ( b.w, 12.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Subdivision )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 1.0, 2.0 };

		a -= { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );

		auto b = a - A2 { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( b.x, -1.0 );
		EXPECT_DOUBLE_EQ( b.y, -2.0 );
	}
	{
		A3 a { 1.0, 2.0, 3.0 };

		a -= { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );
		EXPECT_DOUBLE_EQ( a.z, 0.0 );

		auto b = a - A3 { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( b.x, -1.0 );
		EXPECT_DOUBLE_EQ( b.y, -2.0 );
		EXPECT_DOUBLE_EQ( b.z, -3.0 );
	}
	{
		A4 a { 1.0, 2.0, 3.0, 4.0 };

		a -= { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( a.x, 0.0 );
		EXPECT_DOUBLE_EQ( a.y, 0.0 );
		EXPECT_DOUBLE_EQ( a.z, 0.0 );
		EXPECT_DOUBLE_EQ( a.w, 0.0 );

		auto b = a - A4 { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( b.x, -1.0 );
		EXPECT_DOUBLE_EQ( b.y, -2.0 );
		EXPECT_DOUBLE_EQ( b.z, -3.0 );
		EXPECT_DOUBLE_EQ( b.w, -4.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Multiplication )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 1.0, 2.0 };

		a *= { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );

		auto b = a * A2 { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
	}
	{
		A3 a { 1.0, 2.0, 3.0 };

		a *= { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 9.0 );

		auto b = a * A3 { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
		EXPECT_DOUBLE_EQ( b.z, 27.0 );
	}
	{
		A4 a { 1.0, 2.0, 3.0, 4.0 };

		a *= { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 9.0 );
		EXPECT_DOUBLE_EQ( a.w, 16.0 );

		auto b = a * A4 { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
		EXPECT_DOUBLE_EQ( b.z, 27.0 );
		EXPECT_DOUBLE_EQ( b.w, 64.0 );
	}
	{
		A2 a { 1.0, 2.0 };

		a *= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );

		auto b = a * 2.0;
		EXPECT_DOUBLE_EQ( b.x, 4.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
	}
	{
		A3 a { 1.0, 2.0, 3.0 };

		a *= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 6.0 );

		auto b = a * 2.0;
		EXPECT_DOUBLE_EQ( b.x, 4.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
		EXPECT_DOUBLE_EQ( b.z, 12.0 );
	}
	{
		A4 a { 1.0, 2.0, 3.0, 4.0 };

		a *= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 6.0 );
		EXPECT_DOUBLE_EQ( a.w, 8.0 );

		auto b = a * 2.0;
		EXPECT_DOUBLE_EQ( b.x, 4.0 );
		EXPECT_DOUBLE_EQ( b.y, 8.0 );
		EXPECT_DOUBLE_EQ( b.z, 12.0 );
		EXPECT_DOUBLE_EQ( b.w, 16.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Division )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 1.0, 4.0 };

		a /= { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );

		auto b = a / A2 { 1.0, 2.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 1.0 );
	}
	{
		A3 a { 1.0, 4.0, 9.0 };

		a /= { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );
		EXPECT_DOUBLE_EQ( a.z, 3.0 );

		auto b = a / A3 { 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 1.0 );
		EXPECT_DOUBLE_EQ( b.z, 1.0 );
	}
	{
		A4 a { 1.0, 4.0, 9.0, 16.0 };

		a /= { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( a.x, 1.0 );
		EXPECT_DOUBLE_EQ( a.y, 2.0 );
		EXPECT_DOUBLE_EQ( a.z, 3.0 );
		EXPECT_DOUBLE_EQ( a.w, 4.0 );

		auto b = a / A4 { 1.0, 2.0, 3.0, 4.0 };
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 1.0 );
		EXPECT_DOUBLE_EQ( b.z, 1.0 );
		EXPECT_DOUBLE_EQ( b.w, 1.0 );
	}
	{
		A2 a { 4.0, 8.0 };

		a /= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );

		auto b = a / 2.0;
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 2.0 );
	}
	{
		A3 a { 4.0, 8.0, 16.0 };

		a /= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 8.0 );

		auto b = a / 2.0;
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 2.0 );
		EXPECT_DOUBLE_EQ( b.z, 4.0 );
	}
	{
		A4 a { 4.0, 8.0, 16.0, 32.0 };

		a /= 2.0;
		EXPECT_DOUBLE_EQ( a.x, 2.0 );
		EXPECT_DOUBLE_EQ( a.y, 4.0 );
		EXPECT_DOUBLE_EQ( a.z, 8.0 );
		EXPECT_DOUBLE_EQ( a.w, 16.0 );

		auto b = a / 2.0;
		EXPECT_DOUBLE_EQ( b.x, 1.0 );
		EXPECT_DOUBLE_EQ( b.y, 2.0 );
		EXPECT_DOUBLE_EQ( b.z, 4.0 );
		EXPECT_DOUBLE_EQ( b.w, 8.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Magnitude )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 0.0, 0.0 };
		A2 b { 0.0, 1.0 };
		A2 c { 1.0, 1.0 };
		A2 d { 1.0, 0.0 };
		A2 e { -1.0, -1.0 };
		A2 f { 1.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Magnitude(), 0.0 );
		EXPECT_DOUBLE_EQ( b.Magnitude(), 1.0 );
		EXPECT_DOUBLE_EQ( c.Magnitude(), 1.4142135623730951 );
		EXPECT_DOUBLE_EQ( d.Magnitude(), 1.0 );
		EXPECT_DOUBLE_EQ( e.Magnitude(), 1.4142135623730951 );
		EXPECT_DOUBLE_EQ( f.Magnitude(), 10.049875621120890 );
	}
	{
		A3 a { 0.0, 0.0, 0.0 };
		A3 b { 0.0, 1.0, 0.0 };
		A3 c { 1.0, 1.0, 1.0 };
		A3 d { 1.0, 0.0, 1.0 };
		A3 e { -1.0, -1.0, -1.0 };
		A3 f { 1.0, 10.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Magnitude(), 0.0 );
		EXPECT_DOUBLE_EQ( b.Magnitude(), 1.0 );
		EXPECT_DOUBLE_EQ( c.Magnitude(), 1.7320508075688772 );
		EXPECT_DOUBLE_EQ( d.Magnitude(), 1.4142135623730951 );
		EXPECT_DOUBLE_EQ( e.Magnitude(), 1.7320508075688772 );
		EXPECT_DOUBLE_EQ( f.Magnitude(), 14.177446878757825 );
	}
	{
		A4 a { 0.0, 0.0, 0.0, 0.0 };
		A4 b { 0.0, 1.0, 0.0, 0.0 };
		A4 c { 1.0, 1.0, 1.0, 1.0 };
		A4 d { 1.0, 0.0, 1.0, 0.0 };
		A4 e { -1.0, -1.0, -1.0, -1.0 };
		A4 f { 1.0, 10.0, 10.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Magnitude(), 0.0 );
		EXPECT_DOUBLE_EQ( b.Magnitude(), 1.0 );
		EXPECT_DOUBLE_EQ( c.Magnitude(), 2.0000000000000000 );
		EXPECT_DOUBLE_EQ( d.Magnitude(), 1.4142135623730951 );
		EXPECT_DOUBLE_EQ( e.Magnitude(), 2.0000000000000000 );
		EXPECT_DOUBLE_EQ( f.Magnitude(), 17.349351572897472 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Vector, Normalized )
{
	using A2 = bc::math::Vector2d;
	using A3 = bc::math::Vector3d;
	using A4 = bc::math::Vector4d;

	{
		A2 a { 0.0, 0.0 };
		A2 b { 0.0, 1.0 };
		A2 c { 1.0, 1.0 };
		A2 d { 10.0, 0.0 };
		A2 e { -1.0, -1.0 };
		A2 f { 1.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().y, 0.0 );

		EXPECT_DOUBLE_EQ( b.Normalized().x, 0.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().y, 1.0 );

		EXPECT_DOUBLE_EQ( c.Normalized().x, 0.70710678118654746 );
		EXPECT_DOUBLE_EQ( c.Normalized().y, 0.70710678118654746 );

		EXPECT_DOUBLE_EQ( d.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().y, 0.0 );

		EXPECT_DOUBLE_EQ( e.Normalized().x, -0.70710678118654746 );
		EXPECT_DOUBLE_EQ( e.Normalized().y, -0.70710678118654746 );

		EXPECT_DOUBLE_EQ( f.Normalized().x, 0.099503719020998915 );
		EXPECT_DOUBLE_EQ( f.Normalized().y, 0.99503719020998915 );
	}
	{
		A3 a { 0.0, 0.0, 0.0 };
		A3 b { 0.0, 1.0, 0.0 };
		A3 c { 1.0, 1.0, 1.0 };
		A3 d { 10.0, 0.0, 0.0 };
		A3 e { -1.0, -1.0, -1.0 };
		A3 f { 1.0, 10.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().y, 0.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().z, 0.0 );

		EXPECT_DOUBLE_EQ( b.Normalized().x, 0.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().y, 1.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().z, 0.0 );

		EXPECT_DOUBLE_EQ( c.Normalized().x, 0.57735026918962584 );
		EXPECT_DOUBLE_EQ( c.Normalized().y, 0.57735026918962584 );
		EXPECT_DOUBLE_EQ( c.Normalized().z, 0.57735026918962584 );

		EXPECT_DOUBLE_EQ( d.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().y, 0.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().z, 0.0 );

		EXPECT_DOUBLE_EQ( e.Normalized().x, -0.57735026918962584 );
		EXPECT_DOUBLE_EQ( e.Normalized().y, -0.57735026918962584 );
		EXPECT_DOUBLE_EQ( e.Normalized().z, -0.57735026918962584 );

		EXPECT_DOUBLE_EQ( f.Normalized().x, 0.070534561585859828 );
		EXPECT_DOUBLE_EQ( f.Normalized().y, 0.70534561585859823 );
		EXPECT_DOUBLE_EQ( f.Normalized().z, 0.70534561585859823 );
	}
	{
		A4 a { 0.0, 0.0, 0.0, 0.0 };
		A4 b { 0.0, 1.0, 0.0, 0.0 };
		A4 c { 1.0, 1.0, 1.0, 1.0 };
		A4 d { 10.0, 0.0, 0.0, 0.0 };
		A4 e { -1.0, -1.0, -1.0, -1.0 };
		A4 f { 1.0, 10.0, 10.0, 10.0 };

		EXPECT_DOUBLE_EQ( a.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().y, 0.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().z, 0.0 );
		EXPECT_DOUBLE_EQ( a.Normalized().w, 0.0 );

		EXPECT_DOUBLE_EQ( b.Normalized().x, 0.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().y, 1.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().z, 0.0 );
		EXPECT_DOUBLE_EQ( b.Normalized().w, 0.0 );

		EXPECT_DOUBLE_EQ( c.Normalized().x, 0.5 );
		EXPECT_DOUBLE_EQ( c.Normalized().y, 0.5 );
		EXPECT_DOUBLE_EQ( c.Normalized().z, 0.5 );
		EXPECT_DOUBLE_EQ( c.Normalized().w, 0.5 );

		EXPECT_DOUBLE_EQ( d.Normalized().x, 1.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().y, 0.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().z, 0.0 );
		EXPECT_DOUBLE_EQ( d.Normalized().w, 0.0 );

		EXPECT_DOUBLE_EQ( e.Normalized().x, -0.5 );
		EXPECT_DOUBLE_EQ( e.Normalized().y, -0.5 );
		EXPECT_DOUBLE_EQ( e.Normalized().z, -0.5 );
		EXPECT_DOUBLE_EQ( e.Normalized().w, -0.5 );

		EXPECT_DOUBLE_EQ( f.Normalized().x, 0.057639041770423498 );
		EXPECT_DOUBLE_EQ( f.Normalized().y, 0.57639041770423494 );
		EXPECT_DOUBLE_EQ( f.Normalized().z, 0.57639041770423494 );
		EXPECT_DOUBLE_EQ( f.Normalized().w, 0.57639041770423494 );
	}
};



} // math
} // core
