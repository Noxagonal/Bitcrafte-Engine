
#include <gtest/gtest.h>

#include <core/math/VectorMath.hpp>
#include <core/math/Trigonometry.hpp>
#include <core/math/MathConstants.hpp>
#include <core/math/SmallValues.hpp>



namespace core {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( VectorMath, DotProduct )
{
	using namespace bc::math;
	using A2 = Vec2f64;
	using A3 = Vec3f64;
	using A4 = Vec4f64;

	{
		auto angle = 0.0;
		A2 base { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( base.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		auto comp = base;

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 0.70710678118654757, bc::math::SMALL_F64_VALUE );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 0.0, bc::math::SMALL_F64_VALUE );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -0.70710678118654757, bc::math::SMALL_F64_VALUE );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -1.0, bc::math::SMALL_F64_VALUE );
	}
	{
		A3 base { 1.0, 0.25, 0.5 };
		base = base.Normalized();
		auto comp = base;

		comp = { 1.0, 0.25, 0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 1.0, bc::math::SMALL_F64_VALUE );

		comp = { -0.75, 0.5, 0.0 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -0.60522753266880247, bc::math::SMALL_F64_VALUE );

		comp = { 1.0, -0.5, -0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 0.44543540318737401, bc::math::SMALL_F64_VALUE );

		comp = { -1.0, 0.5, 0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -0.44543540318737401, bc::math::SMALL_F64_VALUE );
	}
	{
		A4 base { 1.0, 0.25, 0.5, -0.75 };
		base = base.Normalized();
		auto comp = base;

		comp = { 1.0, 0.25, 0.5, -0.75 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 1.0, bc::math::SMALL_F64_VALUE );

		comp = { -0.75, 0.5, 0.0, 1.0 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -0.7458698539828913, bc::math::SMALL_F64_VALUE );

		comp = { 1.0, -0.5, -0.5, 0.25 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), 0.25560386016907749, bc::math::SMALL_F64_VALUE );

		comp = { -1.0, 0.5, 0.5, -0.25 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::math::SMALL_F64_VALUE );
		EXPECT_NEAR( Dot( base, comp ), -0.25560386016907749, bc::math::SMALL_F64_VALUE );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( VectorMath, CrossProduct )
{
	using namespace bc::math;
	using A2 = Vec2f64;
	using A3 = Vec3f64;
	using A4 = Vec4f64;

	{
		// TODO: Vector cross product tests.
	}
}

} // math
} // core
