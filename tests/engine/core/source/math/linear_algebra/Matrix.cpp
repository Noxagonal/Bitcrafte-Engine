
#include <gtest/gtest.h>

#include <core/math/linear_algebra/Matrix.hpp>



namespace core {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Matrix, BasicInitialize )
{
	using A2 = bc::math::Matrix2d;
	using A3 = bc::math::Matrix3d;
	using A4 = bc::math::Matrix4d;

	{
		A2 a;
		EXPECT_EQ( a, a );
	}
};



} // math
} // core
