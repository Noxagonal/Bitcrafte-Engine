
#include <gtest/gtest.h>

#include <core/math/Matrix.hpp>



namespace core {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Matrix, BasicInitialize )
{
	using M2 = bc::math::Matrix2f64;
	using M3 = bc::math::Matrix3f64;
	using M4 = bc::math::Matrix4f64;

	{
		M2 a;
		M2 b;
		M2 c { 1.0 };
		M2 d = M2::Identity();

		EXPECT_EQ( a, a );
		EXPECT_EQ( a, b );

		EXPECT_NE( a, c );

		EXPECT_EQ( c, c );
		EXPECT_EQ( c, d );
		EXPECT_EQ( d, d );

		EXPECT_EQ( c[ 0 ][ 0 ], 1.0 ); EXPECT_EQ( c[ 0 ][ 1 ], 0.0 );
		EXPECT_EQ( c[ 1 ][ 0 ], 0.0 ); EXPECT_EQ( c[ 1 ][ 1 ], 1.0 );
		
		EXPECT_EQ( d[ 0 ][ 0 ], 1.0 ); EXPECT_EQ( d[ 0 ][ 1 ], 0.0 );
		EXPECT_EQ( d[ 1 ][ 0 ], 0.0 ); EXPECT_EQ( d[ 1 ][ 1 ], 1.0 );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( Matrix, Multiplication )
{
	using M2 = bc::math::Matrix2f64;
	using M3 = bc::math::Matrix3f64;
	using M4 = bc::math::Matrix4f64;

	using V2 = bc::math::Vec2f64;
	using V3 = bc::math::Vec3f64;
	using V4 = bc::math::Vec4f64;

	// 2D
	{
		// Matrix x matrix multiplication
		M2 a = M2::Identity();
		M2 b {
			0.75, 0.4,
			0.25, 0.7
		};
		M2 c {
			62.23, 23.64,
			41.54, 26.24
		};
		M2 b_b_expected {
			0.66249999999999998, 0.58000000000000007,
			0.36249999999999999, 0.58999999999999997
		};
		M2 b_c_expected {
			52.582499999999996, 41.439999999999998,
			37.715000000000003, 34.983999999999995
		};

		EXPECT_EQ( a * a, M2::Identity() );
		EXPECT_EQ( b * a, b );
		EXPECT_EQ( c * a, c );

		EXPECT_EQ( b * b, b_b_expected );
		EXPECT_EQ( b * c, b_c_expected );
	}
	{
		// Matrix x vector multiplication
		M2 ma {};
		M2 mb = M2::Identity();
		M2 mc {
			0.75, 0.4,
			0.25, 0.7
		};
		M2 md {
			35.2, 67.4,
			31.5, 26.9
		};

		V2 va {};
		V2 vb { 1.0, 1.0 };
		V2 vc { 26.3, 12.8 };

		V2 mc_vb_expected { 1.0000000000000000, 1.1000000000000001 };
		V2 mc_vc_expected { 22.925000000000001, 19.480000000000000 };
		V2 md_vb_expected { 66.700000000000003, 94.300000000000011 };
		V2 md_vc_expected { 1328.9600000000000, 2116.9400000000001 };

		EXPECT_EQ( ma * va, va );
		EXPECT_EQ( ma * vb, va );
		EXPECT_EQ( mb * va, va );
		EXPECT_EQ( mb * vb, vb );

		EXPECT_EQ( mc * vb, mc_vb_expected );
		EXPECT_EQ( mc * vc, mc_vc_expected );
		EXPECT_EQ( md * vb, md_vb_expected );
		EXPECT_EQ( md * vc, md_vc_expected );
	}

	// 3D
	{
		// Matrix x matrix multiplication
		M3 a = M3::Identity();
		M3 b {
			0.75, 0.4, 0.9,
			0.25, 0.7, 0.1,
			0.34, 0.2, 0.8
		};
		M3 c {
			62.23, 23.64, 86.25,
			41.54, 26.24, 64.63,
			12.56, 91.41, 52.95
		};
		M3 b_b_expected {
			0.96850000000000003, 0.76000000000000012, 1.4350000000000001,
			0.39649999999999996, 0.60999999999999999, 0.37500000000000000,
			0.57699999999999996, 0.43600000000000005, 0.96600000000000019
		};
		M3 b_c_expected {
			81.907499999999999, 58.689999999999998, 127.37100000000000,
			59.689200000000000, 47.909999999999997, 91.713999999999999,
			50.275500000000008, 79.600999999999999, 62.805000000000007
		};

		EXPECT_EQ( a * a, M3::Identity() );
		EXPECT_EQ( b * a, b );
		EXPECT_EQ( c * a, c );

		EXPECT_EQ( b * b, b_b_expected );
		EXPECT_EQ( b * c, b_c_expected );
	}
	{
		// Matrix x vector multiplication
		M3 ma {};
		M3 mb = M3::Identity();
		M3 mc {
			0.75, 0.4, 0.9,
			0.25, 0.7, 0.1,
			0.34, 0.2, 0.8
		};
		M3 md {
			62.23, 23.64, 86.25,
			41.54, 26.24, 64.63,
			12.56, 91.41, 52.95
		};

		V3 va {};
		V3 vb { 1.0, 1.0, 1.0 };
		V3 vc { 26.3, 12.8, 46.1 };

		V3 mc_vb_expected { 1.3400000000000001, 1.3000000000000000, 1.8000000000000000 };
		V3 mc_vc_expected { 38.599000000000004, 28.700000000000003, 61.830000000000005 };
		V3 md_vb_expected { 116.33000000000000, 141.28999999999999, 203.82999999999998 };
		V3 md_vc_expected { 2747.3770000000000, 5171.6050000000005, 5536.6340000000000 };

		EXPECT_EQ( ma * va, va );
		EXPECT_EQ( ma * vb, va );
		EXPECT_EQ( mb * va, va );
		EXPECT_EQ( mb * vb, vb );

		EXPECT_EQ( mc * vb, mc_vb_expected );
		EXPECT_EQ( mc * vc, mc_vc_expected );
		EXPECT_EQ( md * vb, md_vb_expected );
		EXPECT_EQ( md * vc, md_vc_expected );
	}

	// 4D
	{
		// Matrix x matrix multiplication
		M4 a = M4::Identity();
		M4 b {
			0.75, 0.4, 0.9, 0.6,
			0.25, 0.7, 0.1, 0.1,
			0.34, 0.2, 0.8, 0.9,
			0.81, 0.9, 0.7, 0.2
		};
		M4 c {
			62.23, 23.64, 86.25, 12.56,
			41.54, 26.24, 64.63, 56.06,
			12.56, 91.41, 52.95, 46.14,
			75.31, 67.14, 84.12, 70.21
		};
		M4 b_b_expected {
			1.4544999999999999, 1.3000000000000003, 1.8550000000000000, 1.4199999999999999,
			0.47749999999999998, 0.69999999999999996, 0.44500000000000001, 0.33000000000000002,
			1.3060000000000000, 1.2460000000000000, 1.5960000000000001, 1.1240000000000001,
			1.2324999999999999, 1.2740000000000000, 1.5189999999999999, 1.2460000000000000
		};
		M4 b_c_expected {
			92.081099999999992, 69.994000000000000, 136.16299999999998, 119.83900000000000,
			105.09780000000001, 98.364000000000004, 130.95599999999999, 96.926999999999992,
			87.648900000000012, 121.12700000000001, 95.103000000000009, 73.559999999999988,
			158.73840000000001, 157.13499999999999, 190.93600000000001, 141.65000000000001
		};

		EXPECT_EQ( a * a, M4::Identity() );
		EXPECT_EQ( b * a, b );
		EXPECT_EQ( c * a, c );

		EXPECT_EQ( b * b, b_b_expected );
		EXPECT_EQ( b * c, b_c_expected );
	}
	{
		// Matrix x vector multiplication
		M4 ma {};
		M4 mb { 1.0 };
		M4 mc {
			0.75, 0.4, 0.9, 0.6,
			0.25, 0.7, 0.1, 0.1,
			0.34, 0.2, 0.8, 0.9,
			0.81, 0.9, 0.7, 0.2
		};
		M4 md {
			62.23, 23.64, 86.25, 12.56,
			41.54, 26.24, 64.63, 56.06,
			12.56, 91.41, 52.95, 46.14,
			75.31, 67.14, 84.12, 70.21
		};

		V4 va {};
		V4 vb { 1.0, 1.0, 1.0, 1.0 };
		V4 vc { 26.3, 12.8, 46.1, 15.7 };

		V4 mc_vb_expected { 2.1500000000000004, 2.2000000000000002, 2.5000000000000000, 1.8000000000000000 };
		V4 mc_vc_expected { 51.316000000000003, 42.829999999999998, 72.820000000000007, 61.689999999999998 };
		V4 md_vb_expected { 191.63999999999999, 208.43000000000001, 287.94999999999999, 184.97000000000000 };
		V4 md_vc_expected { 3929.7439999999997, 6225.7030000000004, 6857.3180000000002, 4277.2469999999994 };

		EXPECT_EQ( ma * va, va );
		EXPECT_EQ( ma * vb, va );
		EXPECT_EQ( mb * va, va );
		EXPECT_EQ( mb * vb, vb );

		EXPECT_EQ( mc * vb, mc_vb_expected );
		EXPECT_EQ( mc * vc, mc_vc_expected );
		EXPECT_EQ( md * vb, md_vb_expected );
		EXPECT_EQ( md * vc, md_vc_expected );
	}
};



} // math
} // core
