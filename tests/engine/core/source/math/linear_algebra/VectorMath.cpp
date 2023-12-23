
#include <gtest/gtest.h>

#include <core/math/linear_algebra/VectorMath.hpp>
#include <core/math/linear_algebra/TrigonometicFunctions.hpp>
#include <core/math/MathConstants.hpp>
#include <core/utility/math/SmallValues.hpp>



namespace core {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( VectorMath, DotProduct )
{
	using namespace bc::math;
	using A2 = Vector2d;
	using A3 = Vector3d;
	using A4 = Vector4d;

	{
		auto angle = 0.0;
		A2 base { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( base.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		auto comp = base;

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 0.70710678118654757, bc::utility::SmallDoubleValue );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 0.0, bc::utility::SmallDoubleValue );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -0.70710678118654757, bc::utility::SmallDoubleValue );

		angle += bc::math::PI / 4.0;
		comp = { Cos( angle ), Sin( angle ) };
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -1.0, bc::utility::SmallDoubleValue );
	}
	{
		A3 base { 1.0, 0.25, 0.5 };
		base = base.Normalized();
		auto comp = base;

		comp = { 1.0, 0.25, 0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 1.0, bc::utility::SmallDoubleValue );

		comp = { -0.75, 0.5, 0.0 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -0.60522753266880247, bc::utility::SmallDoubleValue );

		comp = { 1.0, -0.5, -0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 0.44543540318737401, bc::utility::SmallDoubleValue );

		comp = { -1.0, 0.5, 0.5 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -0.44543540318737401, bc::utility::SmallDoubleValue );
	}
	{
		A4 base { 1.0, 0.25, 0.5, -0.75 };
		base = base.Normalized();
		auto comp = base;

		comp = { 1.0, 0.25, 0.5, -0.75 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 1.0, bc::utility::SmallDoubleValue );

		comp = { -0.75, 0.5, 0.0, 1.0 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -0.7458698539828913, bc::utility::SmallDoubleValue );

		comp = { 1.0, -0.5, -0.5, 0.25 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), 0.25560386016907749, bc::utility::SmallDoubleValue );

		comp = { -1.0, 0.5, 0.5, -0.25 }; comp = comp.Normalized();
		EXPECT_NEAR( comp.Magnitude(), 1.0, bc::utility::SmallDoubleValue );
		EXPECT_NEAR( Dot( base, comp ), -0.25560386016907749, bc::utility::SmallDoubleValue );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( VectorMath, CrossProduct )
{
	using namespace bc::math;
	using A2 = Vector2d;
	using A3 = Vector3d;
	using A4 = Vector4d;

	{
	}
}

} // math
} // core
