
#include <gtest/gtest.h>
#include <TestsCommon.hpp>

#include <core/resource/serialization/json/JSONStructure.hpp>
#include <core/resource/serialization/json/JSONConvert.hpp>

#include <core/conversion/text/utf/UTFConversion.hpp>

#include <core/resource/serialization/json/specializations/ArrayJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/TextJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/ListJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/MapJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/GLMJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/UUIDJSONSerialization.hpp>
#include <core/resource/serialization/json/specializations/ParameterJSONSerialization.hpp>

#include <iostream>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_primitives =
R"json_comp_prim({
	"test_bool": true,
	"test_uint8_t": 5,
	"test_int8_t": -5,
	"test_uint16_t": 50,
	"test_int16_t": -50,
	"test_uint32_t": 500,
	"test_int32_t": -500,
	"test_uint64_t": 5000,
	"test_int64_t": -5000,
	"test_size_t": 50000,
	"test_float32": 5.05,
	"test_float64": -50.005001068115234,
	"test_binary": "0b111000",
	"test_octal": "0o12345676543210",
	"test_hex": "0xabcdef",
	"test_zerofill_0": 111,
	"test_zerofill_1": 111,
	"test_zerofill_2": 111,
	"test_zerofill_3": 111,
	"test_zerofill_4": 0111,
	"test_zerofill_5": 00111,
	"test_zerofill_8_hex": "0x00abcdef"
}
)json_comp_prim";

TEST( CoreResourceSerializationJSONConvert, Primitives )
{
	bool		test_bool				= true;
	uint8_t		test_uint8_t			= 5;
	int8_t		test_int8_t				= -5;
	uint16_t	test_uint16_t			= 50;
	int16_t		test_int16_t			= -50;
	uint32_t	test_uint32_t			= 500;
	int32_t		test_int32_t			= -500;
	uint64_t	test_uint64_t			= 5000;
	int64_t		test_int64_t			= -5000;
	size_t		test_size_t				= 50000;
	float		test_float32			= 5.05f;
	double		test_float64			= -50.005f;

	int64_t		test_binary				= 0b111000;
	int64_t		test_octal				= 012345676543210;		// Written in json text with "0o" prefix.
	int64_t		test_hex				= 0xABCDEF;
	
	int64_t		test_zerofill_0			= 111;
	int64_t		test_zerofill_1			= 111;
	int64_t		test_zerofill_2			= 111;
	int64_t		test_zerofill_3			= 111;
	int64_t		test_zerofill_4			= 111;
	int64_t		test_zerofill_5			= 111;
	int64_t		test_zerofill_8_hex		= 0x00ABCDEF;

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;

		#define SerializePrimitiveTest( m_variable, ... )								\
			qup::serialization::JSONSerialization<decltype( m_variable )>::Serialize(	\
				json_structure.GetRoot(), #m_variable, m_variable, __VA_ARGS__			\
			)

		SerializePrimitiveTest( test_bool );
		SerializePrimitiveTest( test_uint8_t );
		SerializePrimitiveTest( test_int8_t );
		SerializePrimitiveTest( test_uint16_t );
		SerializePrimitiveTest( test_int16_t );
		SerializePrimitiveTest( test_uint32_t );
		SerializePrimitiveTest( test_int32_t );
		SerializePrimitiveTest( test_uint64_t );
		SerializePrimitiveTest( test_int64_t );
		SerializePrimitiveTest( test_size_t );
		SerializePrimitiveTest( test_float32 );
		SerializePrimitiveTest( test_float64 );

		SerializePrimitiveTest( test_binary, qup::conversion::IntegerToTextConversionFormat::BINARY );
		SerializePrimitiveTest( test_octal, qup::conversion::IntegerToTextConversionFormat::OCTAL );
		SerializePrimitiveTest( test_hex, qup::conversion::IntegerToTextConversionFormat::HEX );

		SerializePrimitiveTest( test_zerofill_0, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 0 );
		SerializePrimitiveTest( test_zerofill_1, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 1 );
		SerializePrimitiveTest( test_zerofill_2, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 2 );
		SerializePrimitiveTest( test_zerofill_3, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 3 );
		SerializePrimitiveTest( test_zerofill_4, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 4 );
		SerializePrimitiveTest( test_zerofill_5, qup::conversion::IntegerToTextConversionFormat::DECIMAL, 5 );
		SerializePrimitiveTest( test_zerofill_8_hex, qup::conversion::IntegerToTextConversionFormat::HEX, 8 );

		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_primitives );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		#define DeserializePrimitiveTest( m_variable )									\
			qup::serialization::JSONSerialization<decltype( m_variable )>::Deserialize(	\
				deserialized_json_text.GetRoot()->FindChildByName( U#m_variable )		\
			)

		auto out_bool			= DeserializePrimitiveTest( test_bool );
		auto out_uint8_t		= DeserializePrimitiveTest( test_uint8_t );
		auto out_int8_t			= DeserializePrimitiveTest( test_int8_t );
		auto out_uint16_t		= DeserializePrimitiveTest( test_uint16_t );
		auto out_int16_t		= DeserializePrimitiveTest( test_int16_t );
		auto out_uint32_t		= DeserializePrimitiveTest( test_uint32_t );
		auto out_int32_t		= DeserializePrimitiveTest( test_int32_t );
		auto out_uint64_t		= DeserializePrimitiveTest( test_uint64_t );
		auto out_int64_t		= DeserializePrimitiveTest( test_int64_t );
		auto out_size_t			= DeserializePrimitiveTest( test_size_t );
		auto out_float32		= DeserializePrimitiveTest( test_float32 );
		auto out_float64		= DeserializePrimitiveTest( test_float64 );

		auto out_binary			= DeserializePrimitiveTest( test_binary );
		auto out_octal			= DeserializePrimitiveTest( test_octal );
		auto out_hex			= DeserializePrimitiveTest( test_hex );

		auto out_zerofill_0		= DeserializePrimitiveTest( test_zerofill_0 );
		auto out_zerofill_1		= DeserializePrimitiveTest( test_zerofill_1 );
		auto out_zerofill_2		= DeserializePrimitiveTest( test_zerofill_2 );
		auto out_zerofill_3		= DeserializePrimitiveTest( test_zerofill_3 );
		auto out_zerofill_4		= DeserializePrimitiveTest( test_zerofill_4 );
		auto out_zerofill_5		= DeserializePrimitiveTest( test_zerofill_5 );
		auto out_zerofill_8_hex	= DeserializePrimitiveTest( test_zerofill_8_hex );

		EXPECT_EQ( test_bool, out_bool );
		EXPECT_EQ( test_uint8_t, out_uint8_t );
		EXPECT_EQ( test_int8_t, out_int8_t );
		EXPECT_EQ( test_uint16_t, out_uint16_t );
		EXPECT_EQ( test_int16_t, out_int16_t );
		EXPECT_EQ( test_uint32_t, out_uint32_t );
		EXPECT_EQ( test_int32_t, out_int32_t );
		EXPECT_EQ( test_uint64_t, out_uint64_t );
		EXPECT_EQ( test_int64_t, out_int64_t );
		EXPECT_FLOAT_EQ( test_float32, out_float32 );
		EXPECT_FLOAT_EQ( test_float64, out_float64 );

		EXPECT_EQ( test_binary, out_binary );
		EXPECT_EQ( test_octal, out_octal );
		EXPECT_EQ( test_hex, out_hex );

		EXPECT_EQ( test_zerofill_0, out_zerofill_0 );
		EXPECT_EQ( test_zerofill_1, out_zerofill_1 );
		EXPECT_EQ( test_zerofill_2, out_zerofill_2 );
		EXPECT_EQ( test_zerofill_3, out_zerofill_3 );
		EXPECT_EQ( test_zerofill_4, out_zerofill_4 );
		EXPECT_EQ( test_zerofill_5, out_zerofill_5 );
		EXPECT_EQ( test_zerofill_8_hex, out_zerofill_8_hex );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_arrays =
R"json_comp_arrays({
	"test_array": [
		50,
		120,
		600
	],
	"test_array_2": [
		[
			500,
			20,
			0
		],
		[
			100,
			30,
			90,
			13
		]
	]
}
)json_comp_arrays";

TEST( CoreResourceSerializationJSONConvert, Arrays )
{
	qup::Array<uint32_t> test_array { 50, 120, 600 };
	qup::Array<qup::Array<uint32_t>> test_array_2 {
		{ 500, 20, 0 },
		{ 100, 30, 90, 13 }
	};

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;
		qup::serialization::JSONSerialization<decltype( test_array )>::Serialize( json_structure.GetRoot(), "test_array", test_array );
		qup::serialization::JSONSerialization<decltype( test_array_2 )>::Serialize( json_structure.GetRoot(), "test_array_2", test_array_2 );
		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_arrays );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		auto out_array		= qup::serialization::JSONSerialization<decltype( test_array )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_array" ) );
		auto out_array_2	= qup::serialization::JSONSerialization<decltype( test_array_2 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_array_2" ) );

		EXPECT_EQ( test_array, out_array );
		EXPECT_EQ( test_array_2, out_array_2 );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_texts =
R"json_comp_texts({
	"test_text8": "Test Text8",
	"test_text16": "Test Text16",
	"test_text32": "Test Text32"
}
)json_comp_texts";

TEST( CoreResourceSerializationJSONConvert, Texts )
{
	qup::Text8		test_text8		= "Test Text8";
	qup::Text16		test_text16		= "Test Text16";
	qup::Text32		test_text32		= "Test Text32";

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;
		qup::serialization::JSONSerialization<decltype( test_text8 )>::Serialize( json_structure.GetRoot(), "test_text8", test_text8 );
		qup::serialization::JSONSerialization<decltype( test_text16 )>::Serialize( json_structure.GetRoot(), "test_text16", test_text16 );
		qup::serialization::JSONSerialization<decltype( test_text32 )>::Serialize( json_structure.GetRoot(), "test_text32", test_text32 );
		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_texts );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		auto out_text8		= qup::serialization::JSONSerialization<decltype( test_text8 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_text8" ) );
		auto out_text16		= qup::serialization::JSONSerialization<decltype( test_text16 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_text16" ) );
		auto out_text32		= qup::serialization::JSONSerialization<decltype( test_text32 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_text32" ) );

		EXPECT_EQ( out_text8, test_text8 );
		EXPECT_EQ( out_text16, test_text16 );
		EXPECT_EQ( out_text32, test_text32 );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_lists =
R"json_comp_lists({
	"test_list": [
		50,
		120,
		600
	],
	"test_list_2": [
		[
			500,
			20,
			0
		],
		[
			100,
			30,
			90,
			13
		]
	]
}
)json_comp_lists";

TEST( CoreResourceSerializationJSONConvert, Lists )
{
	qup::List<uint32_t> test_list { 50, 120, 600 };
	qup::List<qup::List<uint32_t>> test_list_2 {
		{ 500, 20, 0 },
		{ 100, 30, 90, 13 }
	};

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;
		qup::serialization::JSONSerialization<decltype( test_list )>::Serialize( json_structure.GetRoot(), "test_list", test_list );
		qup::serialization::JSONSerialization<decltype( test_list_2 )>::Serialize( json_structure.GetRoot(), "test_list_2", test_list_2 );
		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_lists );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		auto out_list		= qup::serialization::JSONSerialization<decltype( test_list )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_list" ) );
		auto out_list_2		= qup::serialization::JSONSerialization<decltype( test_list_2 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_list_2" ) );

		EXPECT_EQ( test_list, out_list );
		EXPECT_EQ( test_list_2, out_list_2 );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_map =
R"json_comp_map({
	"test_map": [
		[
			5,
			50
		],
		[
			10,
			120
		],
		[
			20,
			600
		]
	],
	"test_map_2": [
		[
			5,
			[
				[
					1,
					10
				],
				[
					2,
					20
				]
			]
		],
		[
			10,
			[
				[
					100,
					1
				],
				[
					200,
					2
				]
			]
		]
	]
}
)json_comp_map";

TEST( CoreResourceSerializationJSONConvert, Maps )
{
	using M		= qup::Map<uint32_t, uint32_t>;
	using P		= M::ElementType;

	using M2	= qup::Map<uint32_t, M>;
	using P2	= M2::ElementType;

	M test_map {
		{ 5, 50 },
		{ 10, 120 },
		{ 20, 600 }
	};
	M2 test_map_2 {
		{ 5, M
			{
				{ 1, 10 },
				{ 2, 20 }
			}
		},
		{ 10, M
			{
				{ 100, 1 },
				{ 200, 2 }
			}
		}
	};

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;
		qup::serialization::JSONSerialization<decltype( test_map )>::Serialize( json_structure.GetRoot(), "test_map", test_map );
		qup::serialization::JSONSerialization<decltype( test_map_2 )>::Serialize( json_structure.GetRoot(), "test_map_2", test_map_2 );
		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_map );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		auto out_map		= qup::serialization::JSONSerialization<decltype( test_map )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_map" ) );
		auto out_map_2		= qup::serialization::JSONSerialization<decltype( test_map_2 )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_map_2" ) );

		EXPECT_EQ( test_map, out_map );
		EXPECT_EQ( test_map_2, out_map_2 );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_glm =
R"json_comp_glm({
	"test_vec1": [5.05],
	"test_vec2": [5.05, 10.1],
	"test_vec3": [5.05, 10.1, 20.2],
	"test_vec4": [5.05, 10.1, 20.2, 50.5],
	"test_dvec1": [5.05],
	"test_dvec2": [5.05, 10.1],
	"test_dvec3": [5.05, 10.1, 20.2],
	"test_dvec4": [5.05, 10.1, 20.2, 50.5],
	"test_ivec1": [5],
	"test_ivec2": [5, -10],
	"test_ivec3": [5, -10, 20],
	"test_ivec4": [5, -10, 20, -50],
	"test_uvec1": [5],
	"test_uvec2": [5, 10],
	"test_uvec3": [5, 10, 20],
	"test_uvec4": [5, 10, 20, 50],
	"test_quat": [10.1, 20.2, 50.5, 5.05],
	"test_dquat": [10.1, 20.2, 50.5, 5.05],
	"test_mat2": [
		[5.05, 10.1],
		[5.05, 10.1]
	],
	"test_mat3": [
		[5.05, 10.1, 20.2],
		[5.05, 10.1, 20.2],
		[5.05, 10.1, 20.2]
	],
	"test_mat4": [
		[5.05, 10.1, 20.2, 50.5],
		[5.05, 10.1, 20.2, 50.5],
		[5.05, 10.1, 20.2, 50.5],
		[5.05, 10.1, 20.2, 50.5]
	],
	"test_dmat2": [
		[5.050000190734863, 10.100000381469727],
		[5.050000190734863, 10.100000381469727]
	],
	"test_dmat3": [
		[5.050000190734863, 10.100000381469727, 20.200000762939453],
		[5.050000190734863, 10.100000381469727, 20.200000762939453],
		[5.050000190734863, 10.100000381469727, 20.200000762939453]
	],
	"test_dmat4": [
		[5.050000190734863, 10.100000381469727, 20.200000762939453, 50.5],
		[5.050000190734863, 10.100000381469727, 20.200000762939453, 50.5],
		[5.050000190734863, 10.100000381469727, 20.200000762939453, 50.5],
		[5.050000190734863, 10.100000381469727, 20.200000762939453, 50.5]
	]
}
)json_comp_glm";

TEST( CoreResourceSerializationJSONConvert, GLM )
{
	auto test_vec1 = glm::vec1 { 5.05f };
	auto test_vec2 = glm::vec2 { 5.05f, 10.1f };
	auto test_vec3 = glm::vec3 { 5.05f, 10.1f, 20.2f };
	auto test_vec4 = glm::vec4 { 5.05f, 10.1f, 20.2f, 50.5f };
	
	auto test_dvec1 = glm::dvec1 { 5.05 };
	auto test_dvec2 = glm::dvec2 { 5.05, 10.1 };
	auto test_dvec3 = glm::dvec3 { 5.05, 10.1, 20.2 };
	auto test_dvec4 = glm::dvec4 { 5.05, 10.1, 20.2, 50.5 };

	auto test_ivec1 = glm::ivec1 { 5 };
	auto test_ivec2 = glm::ivec2 { 5, -10 };
	auto test_ivec3 = glm::ivec3 { 5, -10, 20 };
	auto test_ivec4 = glm::ivec4 { 5, -10, 20, -50 };
	
	auto test_uvec1 = glm::uvec1 { 5 };
	auto test_uvec2 = glm::uvec2 { 5, 10 };
	auto test_uvec3 = glm::uvec3 { 5, 10, 20 };
	auto test_uvec4 = glm::uvec4 { 5, 10, 20, 50 };

	auto test_quat = glm::quat { 5.05, 10.1, 20.2, 50.5 };
	auto test_dquat = glm::dquat { 5.05, 10.1, 20.2, 50.5 };

	auto test_mat2 = glm::mat2 {
		{ 5.05f, 10.1f },
		{ 5.05f, 10.1f }
	};

	auto test_mat3 = glm::mat3 {
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f }
	};

	auto test_mat4 = glm::mat4 {
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f }
	};

	auto test_dmat2 = glm::dmat2 {
		{ 5.05f, 10.1f },
		{ 5.05f, 10.1f }
	};

	auto test_dmat3 = glm::dmat3 {
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f },
		{ 5.05f, 10.1f, 20.2f }
	};

	auto test_dmat4 = glm::dmat4 {
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f },
		{ 5.05f, 10.1f, 20.2f, 50.5f }
	};

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;

		#define SerializeGLMTest( m_variable, ... )										\
			qup::serialization::JSONSerialization<decltype( m_variable )>::Serialize(	\
				json_structure.GetRoot(), #m_variable, m_variable, __VA_ARGS__			\
			)

		SerializeGLMTest( test_vec1 );
		SerializeGLMTest( test_vec2 );
		SerializeGLMTest( test_vec3 );
		SerializeGLMTest( test_vec4 );
		
		SerializeGLMTest( test_dvec1 );
		SerializeGLMTest( test_dvec2 );
		SerializeGLMTest( test_dvec3 );
		SerializeGLMTest( test_dvec4 );
		
		SerializeGLMTest( test_ivec1 );
		SerializeGLMTest( test_ivec2 );
		SerializeGLMTest( test_ivec3 );
		SerializeGLMTest( test_ivec4 );
		
		SerializeGLMTest( test_uvec1 );
		SerializeGLMTest( test_uvec2 );
		SerializeGLMTest( test_uvec3 );
		SerializeGLMTest( test_uvec4 );

		SerializeGLMTest( test_quat );
		SerializeGLMTest( test_dquat );

		SerializeGLMTest( test_mat2 );
		SerializeGLMTest( test_mat3 );
		SerializeGLMTest( test_mat4 );
		
		SerializeGLMTest( test_dmat2 );
		SerializeGLMTest( test_dmat3 );
		SerializeGLMTest( test_dmat4 );

		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_glm );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		#define DeserializeGLMTest( m_variable )										\
			qup::serialization::JSONSerialization<decltype( m_variable )>::Deserialize(	\
				deserialized_json_text.GetRoot()->FindChildByName( U#m_variable )		\
			)

		auto out_vec1			= DeserializeGLMTest( test_vec1 );
		auto out_vec2			= DeserializeGLMTest( test_vec2 );
		auto out_vec3			= DeserializeGLMTest( test_vec3 );
		auto out_vec4			= DeserializeGLMTest( test_vec4 );

		auto out_dvec1			= DeserializeGLMTest( test_dvec1 );
		auto out_dvec2			= DeserializeGLMTest( test_dvec2 );
		auto out_dvec3			= DeserializeGLMTest( test_dvec3 );
		auto out_dvec4			= DeserializeGLMTest( test_dvec4 );

		auto out_ivec1			= DeserializeGLMTest( test_ivec1 );
		auto out_ivec2			= DeserializeGLMTest( test_ivec2 );
		auto out_ivec3			= DeserializeGLMTest( test_ivec3 );
		auto out_ivec4			= DeserializeGLMTest( test_ivec4 );

		auto out_uvec1			= DeserializeGLMTest( test_uvec1 );
		auto out_uvec2			= DeserializeGLMTest( test_uvec2 );
		auto out_uvec3			= DeserializeGLMTest( test_uvec3 );
		auto out_uvec4			= DeserializeGLMTest( test_uvec4 );

		auto out_quat			= DeserializeGLMTest( test_quat );
		auto out_dquat			= DeserializeGLMTest( test_dquat );

		auto out_mat2			= DeserializeGLMTest( test_mat2 );
		auto out_mat3			= DeserializeGLMTest( test_mat3 );
		auto out_mat4			= DeserializeGLMTest( test_mat4 );
		
		auto out_dmat2			= DeserializeGLMTest( test_dmat2 );
		auto out_dmat3			= DeserializeGLMTest( test_dmat3 );
		auto out_dmat4			= DeserializeGLMTest( test_dmat4 );

		EXPECT_EQ( out_vec1, test_vec1 );
		EXPECT_EQ( out_vec2, test_vec2 );
		EXPECT_EQ( out_vec3, test_vec3 );
		EXPECT_EQ( out_vec4, test_vec4 );

		EXPECT_EQ( out_dvec1, test_dvec1 );
		EXPECT_EQ( out_dvec2, test_dvec2 );
		EXPECT_EQ( out_dvec3, test_dvec3 );
		EXPECT_EQ( out_dvec4, test_dvec4 );

		EXPECT_EQ( out_ivec1, test_ivec1 );
		EXPECT_EQ( out_ivec2, test_ivec2 );
		EXPECT_EQ( out_ivec3, test_ivec3 );
		EXPECT_EQ( out_ivec4, test_ivec4 );

		EXPECT_EQ( out_uvec1, test_uvec1 );
		EXPECT_EQ( out_uvec2, test_uvec2 );
		EXPECT_EQ( out_uvec3, test_uvec3 );
		EXPECT_EQ( out_uvec4, test_uvec4 );

		EXPECT_EQ( out_quat, test_quat );
		EXPECT_EQ( out_dquat, test_dquat );

		EXPECT_EQ( out_mat2, test_mat2 );
		EXPECT_EQ( out_mat3, test_mat3 );
		EXPECT_EQ( out_mat4, test_mat4 );

		EXPECT_EQ( out_dmat2, test_dmat2 );
		EXPECT_EQ( out_dmat3, test_dmat3 );
		EXPECT_EQ( out_dmat4, test_dmat4 );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_uuids =
R"json_comp_uuid({
	"test_uuid": [
		"0x1234567890abcdef",
		"0x0000ffff0000ffff"
	]
}
)json_comp_uuid";

TEST( CoreResourceSerializationJSONConvert, UUIDs )
{
	qup::UUID	test_uuid;
	test_uuid.data = {
		0x1234567890ABCDEF,
		0x0000FFFF0000FFFF
	};

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;
		qup::serialization::JSONSerialization<decltype( test_uuid )>::Serialize( json_structure.GetRoot(), "test_uuid", test_uuid );
		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		EXPECT_EQ( json_text, json_comparison_text_uuids );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		auto out_uuid		= qup::serialization::JSONSerialization<decltype( test_uuid )>::Deserialize( deserialized_json_text.GetRoot()->FindChildByName( U"test_uuid" ) );

		EXPECT_EQ( out_uuid, test_uuid );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr qup::TextView json_comparison_text_parameters =
R"json_comp_params({
	"test_bool": {
		"Type": "bool",
		"Value": true
	},
	"test_uint8_t": {
		"Type": "uint8_t",
		"Value": 5
	},
	"test_int8_t": {
		"Type": "int8_t",
		"Value": -5
	},
	"test_uint16_t": {
		"Type": "uint16_t",
		"Value": 10
	},
	"test_int16_t": {
		"Type": "int16_t",
		"Value": -10
	},
	"test_uint32_t": {
		"Type": "uint32_t",
		"Value": 20
	},
	"test_int32_t": {
		"Type": "int32_t",
		"Value": -20
	},
	"test_uint64_t": {
		"Type": "uint64_t",
		"Value": 50
	},
	"test_int64_t": {
		"Type": "int64_t",
		"Value": -50
	},
	"test_float32": {
		"Type": "float",
		"Value": 5.05
	},
	"test_float64": {
		"Type": "double",
		"Value": 50.005001068115234
	},
	"test_Text8": {
		"Type": "qup::Text8",
		"Value": "Test Text8"
	},
	"test_Text16": {
		"Type": "qup::Text16",
		"Value": "Test Text16"
	},
	"test_Text32": {
		"Type": "qup::Text32",
		"Value": "Test Text32"
	},
	"test_uuid": {
		"Type": "qup::UUID",
		"Value": [
			"0x000000000000000f",
			"0x00000000000000ff"
		]
	}
}
)json_comp_params";

TEST( CoreResourceSerializationJSONConvert, Parameters )
{
	auto test_bool		= qup::reflection::MemoryBackedParameterData::Create<bool>( true );
	auto test_uint8_t	= qup::reflection::MemoryBackedParameterData::Create<uint8_t>( 5 );
	auto test_int8_t	= qup::reflection::MemoryBackedParameterData::Create<int8_t>( -5 );
	auto test_uint16_t	= qup::reflection::MemoryBackedParameterData::Create<uint16_t>( 10 );
	auto test_int16_t	= qup::reflection::MemoryBackedParameterData::Create<int16_t>( -10 );
	auto test_uint32_t	= qup::reflection::MemoryBackedParameterData::Create<uint32_t>( 20 );
	auto test_int32_t	= qup::reflection::MemoryBackedParameterData::Create<int32_t>( -20 );
	auto test_uint64_t	= qup::reflection::MemoryBackedParameterData::Create<uint64_t>( 50 );
	auto test_int64_t	= qup::reflection::MemoryBackedParameterData::Create<int64_t>( -50 );
	auto test_float32	= qup::reflection::MemoryBackedParameterData::Create<float>( 5.05f );
	auto test_float64	= qup::reflection::MemoryBackedParameterData::Create<double>( 50.005f );
	auto test_Text8		= qup::reflection::MemoryBackedParameterData::Create<qup::Text8>( "Test Text8" );
	auto test_Text16	= qup::reflection::MemoryBackedParameterData::Create<qup::Text16>( "Test Text16" );
	auto test_Text32	= qup::reflection::MemoryBackedParameterData::Create<qup::Text32>( "Test Text32" );
	auto test_uuid		= qup::reflection::MemoryBackedParameterData::Create<qup::UUID>( std::array<uint64_t, 2> { 0xF, 0xFF } );

	qup::Text32 json_text;
	{
		qup::serialization::JSONStructure json_structure;

		#define SerializeParameterTest( m_variable, ... )										\
			qup::serialization::JSONSerialization<qup::reflection::ParameterData>::Serialize(	\
				json_structure.GetRoot(), #m_variable, m_variable, __VA_ARGS__					\
			)

		SerializeParameterTest( test_bool );
		SerializeParameterTest( test_uint8_t );
		SerializeParameterTest( test_int8_t );
		SerializeParameterTest( test_uint16_t );
		SerializeParameterTest( test_int16_t );
		SerializeParameterTest( test_uint32_t );
		SerializeParameterTest( test_int32_t );
		SerializeParameterTest( test_uint64_t );
		SerializeParameterTest( test_int64_t );
		SerializeParameterTest( test_float32 );
		SerializeParameterTest( test_float64 );
		SerializeParameterTest( test_Text8 );
		SerializeParameterTest( test_Text16 );
		SerializeParameterTest( test_Text32 );
		SerializeParameterTest( test_uuid );

		json_text = qup::serialization::JSONContainerToJSONText( json_structure );

		std::cout << reinterpret_cast<const char*>( qup::conversion::ToUTF8( json_text ).ToCStr() );

		EXPECT_EQ( json_text, json_comparison_text_parameters );
	}
	{
		auto deserialized_json_text = qup::serialization::JSONTextToJSONContainer( json_text );

		#define DeserializeParameterTest( m_variable )											\
			qup::serialization::JSONSerialization<qup::reflection::ParameterData>::Deserialize(	\
				deserialized_json_text.GetRoot()->FindChildByName( U#m_variable )				\
			)

		auto out_bool		= DeserializeParameterTest( test_bool );
		auto out_uint8_t	= DeserializeParameterTest( test_uint8_t );
		auto out_int8_t		= DeserializeParameterTest( test_int8_t );
		auto out_uint16_t	= DeserializeParameterTest( test_uint16_t );
		auto out_int16_t	= DeserializeParameterTest( test_int16_t );
		auto out_uint32_t	= DeserializeParameterTest( test_uint32_t );
		auto out_int32_t	= DeserializeParameterTest( test_int32_t );
		auto out_uint64_t	= DeserializeParameterTest( test_uint64_t );
		auto out_int64_t	= DeserializeParameterTest( test_int64_t );
		auto out_float32	= DeserializeParameterTest( test_float32 );
		auto out_float64	= DeserializeParameterTest( test_float64 );
		auto out_Text8		= DeserializeParameterTest( test_Text8 );
		auto out_Text16		= DeserializeParameterTest( test_Text16 );
		auto out_Text32		= DeserializeParameterTest( test_Text32 );
		auto out_uuid		= DeserializeParameterTest( test_uuid );

		EXPECT_EQ( out_bool, test_bool );
		EXPECT_EQ( out_uint8_t, test_uint8_t );
		EXPECT_EQ( out_int8_t, test_int8_t );
		EXPECT_EQ( out_uint16_t, test_uint16_t );
		EXPECT_EQ( out_int16_t, test_int16_t );
		EXPECT_EQ( out_uint32_t, test_uint32_t );
		EXPECT_EQ( out_int32_t, test_int32_t );
		EXPECT_EQ( out_uint64_t, test_uint64_t );
		EXPECT_EQ( out_int64_t, test_int64_t );
		EXPECT_EQ( out_float32, test_float32 );
		EXPECT_EQ( out_float64, test_float64 );
		EXPECT_EQ( out_Text8, test_Text8 );
		EXPECT_EQ( out_Text16, test_Text16 );
		EXPECT_EQ( out_Text32, test_Text32 );
		EXPECT_EQ( out_uuid, test_uuid );
	}
}
