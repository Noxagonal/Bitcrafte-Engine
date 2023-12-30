
/*

// TODO: JSON serialization
#include <gtest/gtest.h>

#include <TestsCommon.hpp>
#include <core/resource/serialization/json/JSONStructure.hpp>
#include <core/resource/serialization/json/JSONConvert.hpp>

#include <core/resource/serialization/json/specializations/PrimitiveJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/ArrayJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/ListJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/MapJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/TextJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/GLMJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/UUIDJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/ParameterJSONSerialization.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializePrimitives )
{
	// Integers
	uint8_t		data_u8				= 5;
	int8_t		data_s8				= -5;
	uint16_t	data_u16			= 50;
	int16_t		data_s16			= -50;
	uint32_t	data_u32			= 500;
	int32_t		data_s32			= -500;
	uint64_t	data_u64			= 5000;
	int64_t		data_s64			= -5000;

	// Floats
	float		data_f32p			= 10.05f;
	float		data_f32n			= -10.05f;
	double		data_f64p			= 100.005;
	double		data_f64n			= -100.005;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	bc::serialization::JSONSerialization<uint8_t>::Serialize( root, "data_u8", data_u8, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<int8_t>::Serialize( root, "data_s8", data_s8, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<uint16_t>::Serialize( root, "data_u16", data_u16, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<int16_t>::Serialize( root, "data_s16", data_s16, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<uint32_t>::Serialize( root, "data_u32", data_u32, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<int32_t>::Serialize( root, "data_s32", data_s32, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<uint64_t>::Serialize( root, "data_u64", data_u64, bc::conversion::IntegerToTextConversionFormat::DECIMAL );
	bc::serialization::JSONSerialization<int64_t>::Serialize( root, "data_s64", data_s64, bc::conversion::IntegerToTextConversionFormat::DECIMAL );

	bc::serialization::JSONSerialization<float>::Serialize( root, "data_f32p", data_f32p, bc::conversion::FloatToTextConversionFormat::GENERAL );
	bc::serialization::JSONSerialization<float>::Serialize( root, "data_f32n", data_f32n, bc::conversion::FloatToTextConversionFormat::GENERAL );
	bc::serialization::JSONSerialization<double>::Serialize( root, "data_f64p", data_f64p, bc::conversion::FloatToTextConversionFormat::GENERAL );
	bc::serialization::JSONSerialization<double>::Serialize( root, "data_f64n", data_f64n, bc::conversion::FloatToTextConversionFormat::GENERAL );

	auto & child_entries = root->GetChildEntries();

	for( auto & e : child_entries ) {
		EXPECT_EQ( e.get()->GetType(), bc::serialization::JSONEntryBase::Type::VALUE );
	}

	EXPECT_EQ( child_entries[ 0 ]->GetName(), "data_u8" );
	EXPECT_EQ( child_entries[ 1 ]->GetName(), "data_s8" );
	EXPECT_EQ( child_entries[ 2 ]->GetName(), "data_u16" );
	EXPECT_EQ( child_entries[ 3 ]->GetName(), "data_s16" );
	EXPECT_EQ( child_entries[ 4 ]->GetName(), "data_u32" );
	EXPECT_EQ( child_entries[ 5 ]->GetName(), "data_s32" );
	EXPECT_EQ( child_entries[ 6 ]->GetName(), "data_u64" );
	EXPECT_EQ( child_entries[ 7 ]->GetName(), "data_s64" );

	EXPECT_EQ( child_entries[ 8 ]->GetName(), "data_f32p" );
	EXPECT_EQ( child_entries[ 9 ]->GetName(), "data_f32n" );
	EXPECT_EQ( child_entries[ 10 ]->GetName(), "data_f64p" );
	EXPECT_EQ( child_entries[ 11 ]->GetName(), "data_f64n" );

	#define DESERIALIZE_INTEGER_VALUE( m_data, m_entry_index )								\
		m_data = bc::serialization::JSONSerialization<decltype( m_data )>::Deserialize(	\
			child_entries[ m_entry_index ].get(),											\
			bc::conversion::TextToIntegerConversionFormat::AUTOMATIC						\
		);
	
	#define DESERIALIZE_FLOAT_VALUE( m_data, m_entry_index )								\
		m_data = bc::serialization::JSONSerialization<decltype( m_data )>::Deserialize(	\
			child_entries[ m_entry_index ].get(),											\
			bc::conversion::TextToFloatConversionFormat::AUTOMATIC							\
		);

	uint8_t		out_data_u8			= 0;
	int8_t		out_data_s8			= 0;
	uint16_t	out_data_u16		= 0;
	int16_t		out_data_s16		= 0;
	uint32_t	out_data_u32		= 0;
	int32_t		out_data_s32		= 0;
	uint64_t	out_data_u64		= 0;
	int64_t		out_data_s64		= 0;

	float		out_data_f32p		= 0.0f;
	float		out_data_f32n		= 0.0f;
	double		out_data_f64p		= 0.0;
	double		out_data_f64n		= 0.0;

	size_t		counter				= 0;
	DESERIALIZE_INTEGER_VALUE( out_data_u8, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_s8, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_u16, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_s16, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_u32, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_s32, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_u64, counter++ );
	DESERIALIZE_INTEGER_VALUE( out_data_s64, counter++ );

	DESERIALIZE_FLOAT_VALUE( out_data_f32p, counter++ );
	DESERIALIZE_FLOAT_VALUE( out_data_f32n, counter++ );
	DESERIALIZE_FLOAT_VALUE( out_data_f64p, counter++ );
	DESERIALIZE_FLOAT_VALUE( out_data_f64n, counter++ );

	EXPECT_EQ(	out_data_u8,	data_u8 );
	EXPECT_EQ(	out_data_s8,	data_s8 );
	EXPECT_EQ(	out_data_u16,	data_u16 );
	EXPECT_EQ(	out_data_s16,	data_s16 );
	EXPECT_EQ(	out_data_u32,	data_u32 );
	EXPECT_EQ(	out_data_s32,	data_s32 );
	EXPECT_EQ(	out_data_u64,	data_u64 );
	EXPECT_EQ(	out_data_s64,	data_s64 );

	EXPECT_FLOAT_EQ( out_data_f32p,	data_f32p );
	EXPECT_FLOAT_EQ( out_data_f32n,	data_f32n );
	EXPECT_FLOAT_EQ( out_data_f64p,	data_f64p );
	EXPECT_FLOAT_EQ( out_data_f64n,	data_f64n );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeArray )
{
	using A = bc::Array<uint32_t>;
	using B = bc::Array<A>;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	A a { 5, 10, 20, };

	B b {
		{ 5, 10, 20 },
		{ 50, 100, 200 }
	};

	bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
	bc::serialization::JSONSerialization<B>::Serialize( root, "b", b );

	A out_a;
	B out_b;

	out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
	out_b = bc::serialization::JSONSerialization<B>::Deserialize( root->GetChildEntries()[ 1 ].get() );

	EXPECT_EQ( a.Size(), out_a.Size() );
	EXPECT_EQ( b.Size(), out_b.Size() );

	if( a.Size() != out_a.Size() ) GTEST_SKIP();
	if( b.Size() != out_b.Size() ) GTEST_SKIP();

	EXPECT_EQ( a, out_a );
	EXPECT_EQ( b, out_b );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeList )
{
	using A = bc::List<uint32_t>;
	using B = bc::List<A>;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	A a { 5, 10, 20, };

	B b {
		{ 5, 10, 20 },
		{ 50, 100, 200 }
	};

	bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
	bc::serialization::JSONSerialization<B>::Serialize( root, "b", b );

	A out_a;
	B out_b;

	out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
	out_b = bc::serialization::JSONSerialization<B>::Deserialize( root->GetChildEntries()[ 1 ].get() );

	EXPECT_EQ( a.Size(), out_a.Size() );
	EXPECT_EQ( b.Size(), out_b.Size() );

	if( a.Size() != out_a.Size() ) GTEST_SKIP();
	if( b.Size() != out_b.Size() ) GTEST_SKIP();

	EXPECT_EQ( a, out_a );
	EXPECT_EQ( b, out_b );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeText )
{
	{
		using A = bc::Text8;

		bc::serialization::JSONStructure container;
		auto root = container.GetRoot();

		A a = "Normal";
		A b = "With escape characters n \n";
		A c = "With escape characters t \t";
		A d = "With escape characters quote \" in the middle";
		A e = "With escape characters quote at the end \"";

		bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
		bc::serialization::JSONSerialization<A>::Serialize( root, "b", b );
		bc::serialization::JSONSerialization<A>::Serialize( root, "c", c );
		bc::serialization::JSONSerialization<A>::Serialize( root, "d", d );
		bc::serialization::JSONSerialization<A>::Serialize( root, "e", e );

		A out_a;
		A out_b;
		A out_c;
		A out_d;
		A out_e;

		out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
		out_b = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 1 ].get() );
		out_c = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 2 ].get() );
		out_d = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 3 ].get() );
		out_e = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 4 ].get() );

		EXPECT_EQ( a.Size(), out_a.Size() );
		EXPECT_EQ( b.Size(), out_b.Size() );
		EXPECT_EQ( c.Size(), out_c.Size() );
		EXPECT_EQ( d.Size(), out_d.Size() );
		EXPECT_EQ( e.Size(), out_e.Size() );

		if( a.Size() != out_a.Size() ) GTEST_SKIP();
		if( b.Size() != out_b.Size() ) GTEST_SKIP();
		if( c.Size() != out_c.Size() ) GTEST_SKIP();
		if( d.Size() != out_d.Size() ) GTEST_SKIP();
		if( e.Size() != out_e.Size() ) GTEST_SKIP();

		EXPECT_EQ( a, out_a );
		EXPECT_EQ( b, out_b );
		EXPECT_EQ( c, out_c );
		EXPECT_EQ( d, out_d );
		EXPECT_EQ( e, out_e );
	}
	{
		using A = bc::Text16;

		bc::serialization::JSONStructure container;
		auto root = container.GetRoot();

		A a = "Normal";
		A b = "With escape characters n \n";
		A c = "With escape characters t \t";
		A d = "With escape characters quote \" in the middle";
		A e = "With escape characters quote at the end \"";

		bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
		bc::serialization::JSONSerialization<A>::Serialize( root, "b", b );
		bc::serialization::JSONSerialization<A>::Serialize( root, "c", c );
		bc::serialization::JSONSerialization<A>::Serialize( root, "d", d );
		bc::serialization::JSONSerialization<A>::Serialize( root, "e", e );

		A out_a;
		A out_b;
		A out_c;
		A out_d;
		A out_e;

		out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
		out_b = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 1 ].get() );
		out_c = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 2 ].get() );
		out_d = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 3 ].get() );
		out_e = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 4 ].get() );

		EXPECT_EQ( a.Size(), out_a.Size() );
		EXPECT_EQ( b.Size(), out_b.Size() );
		EXPECT_EQ( c.Size(), out_c.Size() );
		EXPECT_EQ( d.Size(), out_d.Size() );
		EXPECT_EQ( e.Size(), out_e.Size() );

		if( a.Size() != out_a.Size() ) GTEST_SKIP();
		if( b.Size() != out_b.Size() ) GTEST_SKIP();
		if( c.Size() != out_c.Size() ) GTEST_SKIP();
		if( d.Size() != out_d.Size() ) GTEST_SKIP();
		if( e.Size() != out_e.Size() ) GTEST_SKIP();

		EXPECT_EQ( a, out_a );
		EXPECT_EQ( b, out_b );
		EXPECT_EQ( c, out_c );
		EXPECT_EQ( d, out_d );
		EXPECT_EQ( e, out_e );
	}
	{
		using A = bc::Text32;

		bc::serialization::JSONStructure container;
		auto root = container.GetRoot();

		A a = "Normal";
		A b = "With escape characters n \n";
		A c = "With escape characters t \t";
		A d = "With escape characters quote \" in the middle";
		A e = "With escape characters quote at the end \"";

		bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
		bc::serialization::JSONSerialization<A>::Serialize( root, "b", b );
		bc::serialization::JSONSerialization<A>::Serialize( root, "c", c );
		bc::serialization::JSONSerialization<A>::Serialize( root, "d", d );
		bc::serialization::JSONSerialization<A>::Serialize( root, "e", e );

		A out_a;
		A out_b;
		A out_c;
		A out_d;
		A out_e;

		out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
		out_b = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 1 ].get() );
		out_c = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 2 ].get() );
		out_d = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 3 ].get() );
		out_e = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 4 ].get() );

		EXPECT_EQ( a.Size(), out_a.Size() );
		EXPECT_EQ( b.Size(), out_b.Size() );
		EXPECT_EQ( c.Size(), out_c.Size() );
		EXPECT_EQ( d.Size(), out_d.Size() );
		EXPECT_EQ( e.Size(), out_e.Size() );

		if( a.Size() != out_a.Size() ) GTEST_SKIP();
		if( b.Size() != out_b.Size() ) GTEST_SKIP();
		if( c.Size() != out_c.Size() ) GTEST_SKIP();
		if( d.Size() != out_d.Size() ) GTEST_SKIP();
		if( e.Size() != out_e.Size() ) GTEST_SKIP();

		EXPECT_EQ( a, out_a );
		EXPECT_EQ( b, out_b );
		EXPECT_EQ( c, out_c );
		EXPECT_EQ( d, out_d );
		EXPECT_EQ( e, out_e );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeMap )
{
	using A = bc::Map<uint32_t, bc::Text8>;
	using B = bc::Map<bc::Text8, uint32_t>;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	A a {
		bc::Pair<uint32_t, bc::Text8>( 5, "5" ),
		bc::Pair<uint32_t, bc::Text8>( 10, "10" ),
		bc::Pair<uint32_t, bc::Text8>( 20, "20" ),
	};

	B b {
		bc::Pair<bc::Text8, uint32_t>( "5", 5 ),
		bc::Pair<bc::Text8, uint32_t>( "10", 10 ),
		bc::Pair<bc::Text8, uint32_t>( "20", 20 ),
	};

	bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
	bc::serialization::JSONSerialization<B>::Serialize( root, "b", b );

	A out_a;
	B out_b;

	out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
	out_b = bc::serialization::JSONSerialization<B>::Deserialize( root->GetChildEntries()[ 1 ].get() );

	EXPECT_EQ( a.Size(), out_a.Size() );
	EXPECT_EQ( b.Size(), out_b.Size() );

	if( a.Size() != out_a.Size() ) GTEST_SKIP();
	if( b.Size() != out_b.Size() ) GTEST_SKIP();

	EXPECT_EQ( a, out_a );
	EXPECT_EQ( b, out_b );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeGLM )
{
	using V1 = glm::vec1;
	using V2 = glm::vec2;
	using V3 = glm::vec3;
	using V4 = glm::vec4;
	
	using DV1 = glm::dvec1;
	using DV2 = glm::dvec2;
	using DV3 = glm::dvec3;
	using DV4 = glm::dvec4;
	
	using IV1 = glm::ivec1;
	using IV2 = glm::ivec2;
	using IV3 = glm::ivec3;
	using IV4 = glm::ivec4;
	
	using UV1 = glm::uvec1;
	using UV2 = glm::uvec2;
	using UV3 = glm::uvec3;
	using UV4 = glm::uvec4;

	using Q = glm::quat;
	using DQ = glm::dquat;

	using M2 = glm::mat2;
	using M3 = glm::mat3;
	using M4 = glm::mat4;
	
	using DM2 = glm::dmat2;
	using DM3 = glm::dmat3;
	using DM4 = glm::dmat4;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	auto v1 = V1( 5.05f );
	auto v2 = V2( 5.05f, 10.1f );
	auto v3 = V3( 5.05f, 10.1f, 20.2f );
	auto v4 = V4( 5.05f, 10.1f, 20.2f, 50.5f );
	
	auto dv1 = DV1( 5.05f );
	auto dv2 = DV2( 5.05f, 10.1f );
	auto dv3 = DV3( 5.05f, 10.1f, 20.2f );
	auto dv4 = DV4( 5.05f, 10.1f, 20.2f, 50.5f );
	
	auto iv1 = IV1( 5.05f );
	auto iv2 = IV2( 5.05f, 10.1f );
	auto iv3 = IV3( 5.05f, 10.1f, 20.2f );
	auto iv4 = IV4( 5.05f, 10.1f, 20.2f, 50.5f );
	
	auto uv1 = UV1( 5.05f );
	auto uv2 = UV2( 5.05f, 10.1f );
	auto uv3 = UV3( 5.05f, 10.1f, 20.2f );
	auto uv4 = UV4( 5.05f, 10.1f, 20.2f, 50.5f );

	auto q = Q( 5.05, 10.1, 20.2, 50.5 );
	auto dq = DQ( 5.05, 10.1, 20.2, 50.5 );

	auto m2 = M2(
		{ 5.05f, 10.1f },
		{ 5.05f, 10.1f }
	);

	auto m3 = M3(
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f }
	);
	
	auto m4 = M4(
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f }
	);
	
	auto dm2 = DM2(
		{ 5.05f, 10.1f },
		{ 5.05f, 10.1f }
	);

	auto dm3 = DM3(
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f }
	);
	
	auto dm4 = DM4(
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f }
	);

	bc::serialization::JSONSerialization<V1>::Serialize( root, "v1", v1 );
	bc::serialization::JSONSerialization<V2>::Serialize( root, "v2", v2 );
	bc::serialization::JSONSerialization<V3>::Serialize( root, "v3", v3 );
	bc::serialization::JSONSerialization<V4>::Serialize( root, "v4", v4 );
	
	bc::serialization::JSONSerialization<DV1>::Serialize( root, "dv1", dv1 );
	bc::serialization::JSONSerialization<DV2>::Serialize( root, "dv2", dv2 );
	bc::serialization::JSONSerialization<DV3>::Serialize( root, "dv3", dv3 );
	bc::serialization::JSONSerialization<DV4>::Serialize( root, "dv4", dv4 );
	
	bc::serialization::JSONSerialization<IV1>::Serialize( root, "iv1", iv1 );
	bc::serialization::JSONSerialization<IV2>::Serialize( root, "iv2", iv2 );
	bc::serialization::JSONSerialization<IV3>::Serialize( root, "iv3", iv3 );
	bc::serialization::JSONSerialization<IV4>::Serialize( root, "iv4", iv4 );
	
	bc::serialization::JSONSerialization<UV1>::Serialize( root, "uv1", uv1 );
	bc::serialization::JSONSerialization<UV2>::Serialize( root, "uv2", uv2 );
	bc::serialization::JSONSerialization<UV3>::Serialize( root, "uv3", uv3 );
	bc::serialization::JSONSerialization<UV4>::Serialize( root, "uv4", uv4 );

	bc::serialization::JSONSerialization<Q>::Serialize( root, "q", q );
	bc::serialization::JSONSerialization<Q>::Serialize( root, "dq", dq );

	bc::serialization::JSONSerialization<M2>::Serialize( root, "m2", m2 );
	bc::serialization::JSONSerialization<M3>::Serialize( root, "m3", m3 );
	bc::serialization::JSONSerialization<M4>::Serialize( root, "m4", m4 );
	
	bc::serialization::JSONSerialization<DM2>::Serialize( root, "dm2", dm2 );
	bc::serialization::JSONSerialization<DM3>::Serialize( root, "dm3", dm3 );
	bc::serialization::JSONSerialization<DM4>::Serialize( root, "dm4", dm4 );


	V1 out_v1 {};
	V2 out_v2 {};
	V3 out_v3 {};
	V4 out_v4 {};
	
	DV1 out_dv1 {};
	DV2 out_dv2 {};
	DV3 out_dv3 {};
	DV4 out_dv4 {};
	
	IV1 out_iv1 {};
	IV2 out_iv2 {};
	IV3 out_iv3 {};
	IV4 out_iv4 {};
	
	UV1 out_uv1 {};
	UV2 out_uv2 {};
	UV3 out_uv3 {};
	UV4 out_uv4 {};

	Q out_q {};
	DQ out_dq {};

	M2 out_m2 {};
	M3 out_m3 {};
	M4 out_m4 {};
	
	DM2 out_dm2 {};
	DM3 out_dm3 {};
	DM4 out_dm4 {};

	size_t counter = 0;
	out_v1 = bc::serialization::JSONSerialization<V1>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_v2 = bc::serialization::JSONSerialization<V2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_v3 = bc::serialization::JSONSerialization<V3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_v4 = bc::serialization::JSONSerialization<V4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	
	out_dv1 = bc::serialization::JSONSerialization<DV1>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dv2 = bc::serialization::JSONSerialization<DV2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dv3 = bc::serialization::JSONSerialization<DV3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dv4 = bc::serialization::JSONSerialization<DV4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );

	out_iv1 = bc::serialization::JSONSerialization<IV1>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_iv2 = bc::serialization::JSONSerialization<IV2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_iv3 = bc::serialization::JSONSerialization<IV3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_iv4 = bc::serialization::JSONSerialization<IV4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	
	out_uv1 = bc::serialization::JSONSerialization<UV1>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_uv2 = bc::serialization::JSONSerialization<UV2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_uv3 = bc::serialization::JSONSerialization<UV3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_uv4 = bc::serialization::JSONSerialization<UV4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );

	out_q = bc::serialization::JSONSerialization<Q>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dq = bc::serialization::JSONSerialization<DQ>::Deserialize( root->GetChildEntries()[ counter++ ].get() );

	out_m2 = bc::serialization::JSONSerialization<M2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_m3 = bc::serialization::JSONSerialization<M3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_m4 = bc::serialization::JSONSerialization<M4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	
	out_dm2 = bc::serialization::JSONSerialization<M2>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dm3 = bc::serialization::JSONSerialization<M3>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	out_dm4 = bc::serialization::JSONSerialization<M4>::Deserialize( root->GetChildEntries()[ counter++ ].get() );

	EXPECT_EQ( v1, out_v1 );
	EXPECT_EQ( v2, out_v2 );
	EXPECT_EQ( v3, out_v3 );
	EXPECT_EQ( v4, out_v4 );
	
	EXPECT_EQ( dv1, out_dv1 );
	EXPECT_EQ( dv2, out_dv2 );
	EXPECT_EQ( dv3, out_dv3 );
	EXPECT_EQ( dv4, out_dv4 );
	
	EXPECT_EQ( iv1, out_iv1 );
	EXPECT_EQ( iv2, out_iv2 );
	EXPECT_EQ( iv3, out_iv3 );
	EXPECT_EQ( iv4, out_iv4 );
	
	EXPECT_EQ( uv1, out_uv1 );
	EXPECT_EQ( uv2, out_uv2 );
	EXPECT_EQ( uv3, out_uv3 );
	EXPECT_EQ( uv4, out_uv4 );

	EXPECT_EQ( q, out_q );
	EXPECT_EQ( dq, out_dq );

	EXPECT_EQ( m2, out_m2 );
	EXPECT_EQ( m3, out_m3 );
	EXPECT_EQ( m4, out_m4 );

	EXPECT_EQ( dm2, out_dm2 );
	EXPECT_EQ( dm3, out_dm3 );
	EXPECT_EQ( dm4, out_dm4 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeUUID )
{
	using A = bc::UUID;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	A a;
	A b;

	bc::serialization::JSONSerialization<A>::Serialize( root, "a", a );
	bc::serialization::JSONSerialization<A>::Serialize( root, "b", b );

	A out_a;
	A out_b;

	out_a = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 0 ].get() );
	out_b = bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ 1 ].get() );

	EXPECT_EQ( a, out_a );
	EXPECT_EQ( b, out_b );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( CoreResourceSerializationJSON, SerializeParameterData )
{
	using A = bc::reflection::ParameterData;
	using B = bc::reflection::MemoryBackedParameterData;

	bc::serialization::JSONStructure container;
	auto root = container.GetRoot();

	bool		data_actual_bool		= true;
	int8_t		data_actual_int8_t		= -5;
	uint8_t		data_actual_uint8_t		= 5;
	int16_t		data_actual_int16_t		= -50;
	uint16_t	data_actual_uint16_t	= 50;
	int32_t		data_actual_int32_t		= -500;
	uint32_t	data_actual_uint32_t	= 500;
	int64_t		data_actual_int64_t		= -500;
	uint64_t	data_actual_uint64_t	= 500;
	float		data_actual_float		= 5000.05f;
	double		data_actual_double		= -50000.005;
	bc::Text8	data_actual_text8		= "Test8";
	bc::Text16	data_actual_text16		= "Test16";
	bc::Text32	data_actual_text32		= "Test32";
	bc::UUID	data_actual_uuid;

	A data_bool			{ bc::reflection::TypeName<bool>::Get(),			&data_actual_bool,		false };
	A data_int8_t		{ bc::reflection::TypeName<int8_t>::Get(),			&data_actual_int8_t,	false };
	A data_uint8_t		{ bc::reflection::TypeName<uint8_t>::Get(),		&data_actual_uint8_t,	false };
	A data_int16_t		{ bc::reflection::TypeName<int16_t>::Get(),		&data_actual_int16_t,	false };
	A data_uint16_t		{ bc::reflection::TypeName<uint16_t>::Get(),		&data_actual_uint16_t,	false };
	A data_int32_t		{ bc::reflection::TypeName<int32_t>::Get(),		&data_actual_int32_t,	false };
	A data_uint32_t		{ bc::reflection::TypeName<uint32_t>::Get(),		&data_actual_uint32_t,	false };
	A data_int64_t		{ bc::reflection::TypeName<int64_t>::Get(),		&data_actual_int64_t,	false };
	A data_uint64_t		{ bc::reflection::TypeName<uint64_t>::Get(),		&data_actual_uint64_t,	false };
	A data_float		{ bc::reflection::TypeName<float>::Get(),			&data_actual_float,		false };
	A data_double		{ bc::reflection::TypeName<double>::Get(),			&data_actual_double,	false };
	A data_text8		{ bc::reflection::TypeName<bc::Text8>::Get(),		&data_actual_text8,		false };
	A data_text16		{ bc::reflection::TypeName<bc::Text16>::Get(),	&data_actual_text16,	false };
	A data_text32		{ bc::reflection::TypeName<bc::Text32>::Get(),	&data_actual_text32,	false };
	A data_uuid			{ bc::reflection::TypeName<bc::UUID>::Get(),		&data_actual_uuid,		false };

	bc::serialization::JSONSerialization<A>::Serialize( root, "data_bool",			data_bool );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_int8_t",		data_int8_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_uint8_t",		data_uint8_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_int16_t",		data_int16_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_uint16_t",		data_uint16_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_int32_t",		data_int32_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_uint32_t",		data_uint32_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_int64_t",		data_int64_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_uint64_t",		data_uint64_t );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_float",		data_float );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_double",		data_double );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_text8",		data_text8 );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_text16",		data_text16 );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_text32",		data_text32 );
	bc::serialization::JSONSerialization<A>::Serialize( root, "data_uuid",			data_uuid);

	size_t counter = 0;
	B out_bool		= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_int8_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_uint8_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_int16_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_uint16_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_int32_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_uint32_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_int64_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_uint64_t	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_float		= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_double	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_text8		= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_text16	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_text32	= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );
	B out_uuid		= bc::serialization::JSONSerialization<A>::Deserialize( root->GetChildEntries()[ counter++ ].get() );

	EXPECT_EQ( out_bool.type,		data_bool.type );
	EXPECT_EQ( out_int8_t.type,		data_int8_t.type );
	EXPECT_EQ( out_uint8_t.type,	data_uint8_t.type );
	EXPECT_EQ( out_int16_t.type,	data_int16_t.type );
	EXPECT_EQ( out_uint16_t.type,	data_uint16_t.type );
	EXPECT_EQ( out_int32_t.type,	data_int32_t.type );
	EXPECT_EQ( out_uint32_t.type,	data_uint32_t.type );
	EXPECT_EQ( out_int64_t.type,	data_int64_t.type );
	EXPECT_EQ( out_uint64_t.type,	data_uint64_t.type );
	EXPECT_EQ( out_float.type,		data_float.type );
	EXPECT_EQ( out_double.type,		data_double.type );
	EXPECT_EQ( out_text8.type,		data_text8.type );
	EXPECT_EQ( out_text16.type,		data_text16.type );
	EXPECT_EQ( out_text32.type,		data_text32.type );
	EXPECT_EQ( out_uuid.type,		data_uuid.type );

	EXPECT_EQ( *reinterpret_cast<bool*>			( out_bool.storage.Data() ),		data_actual_bool );
	EXPECT_EQ( *reinterpret_cast<int8_t*>		( out_int8_t.storage.Data() ),		data_actual_int8_t );
	EXPECT_EQ( *reinterpret_cast<uint8_t*>		( out_uint8_t.storage.Data() ),		data_actual_uint8_t );
	EXPECT_EQ( *reinterpret_cast<int16_t*>		( out_int16_t.storage.Data() ),		data_actual_int16_t );
	EXPECT_EQ( *reinterpret_cast<uint16_t*>		( out_uint16_t.storage.Data() ),	data_actual_uint16_t );
	EXPECT_EQ( *reinterpret_cast<int32_t*>		( out_int32_t.storage.Data() ),		data_actual_int32_t );
	EXPECT_EQ( *reinterpret_cast<uint32_t*>		( out_uint32_t.storage.Data() ),	data_actual_uint32_t );
	EXPECT_EQ( *reinterpret_cast<int64_t*>		( out_int64_t.storage.Data() ),		data_actual_int64_t );
	EXPECT_EQ( *reinterpret_cast<uint64_t*>		( out_uint64_t.storage.Data() ),	data_actual_uint64_t );
	EXPECT_EQ( *reinterpret_cast<float*>		( out_float.storage.Data() ),		data_actual_float );
	EXPECT_EQ( *reinterpret_cast<double*>		( out_double.storage.Data() ),		data_actual_double );
	EXPECT_EQ( *reinterpret_cast<bc::Text8*>	( out_text8.storage.Data() ),		data_actual_text8 );
	EXPECT_EQ( *reinterpret_cast<bc::Text16*>	( out_text16.storage.Data() ),		data_actual_text16 );
	EXPECT_EQ( *reinterpret_cast<bc::Text32*>	( out_text32.storage.Data() ),		data_actual_text32 );
	EXPECT_EQ( *reinterpret_cast<bc::UUID*>	( out_uuid.storage.Data() ),		data_actual_uuid );
}

*/
