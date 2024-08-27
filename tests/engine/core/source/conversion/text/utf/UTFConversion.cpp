
#include <gtest/gtest.h>

#include <core/conversion/text/utf/UTFConversion.hpp>

#include <string>



namespace core {
namespace conversion {

using SC = std::basic_string<char>;
using S8 = std::basic_string<bc::c8>;
using S16 = std::basic_string<bc::c16>;
using S32 = std::basic_string<bc::c32>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, Copy )
{
	using namespace bc::conversion;

	// Test buffer sizes
	{
		SC source = "Hello, World!";
		SC expect = "";
		SC result = SC( 0, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 0 );
		EXPECT_EQ( conversion_result.code_point_count, 0 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		SC expect = "Hello, World";
		SC result = SC( 12, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 12 );
		EXPECT_EQ( conversion_result.code_point_count, 12 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		SC expect = "Hello, World!";
		SC result = SC( 13, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 13 );
		EXPECT_EQ( conversion_result.code_point_count, 13 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		SC expect = "Hello, World!";
		SC result = SC( 14, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 13 );
		EXPECT_EQ( conversion_result.code_point_count, 13 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C8FromChar )
{
	using namespace bc::conversion;

	{
		SC source = "Hello, World!";
		S8 expect = u8"Hello, World";
		S8 result = S8( 12, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 12 );
		EXPECT_EQ( conversion_result.code_point_count, 12 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		S8 expect = u8"Hello, World!";
		S8 result = S8( 13, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 13 );
		EXPECT_EQ( conversion_result.code_point_count, 13 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C16FromChar )
{
	using namespace bc::conversion;

	{
		SC source = "Hello, World!";
		S16 expect = u"Hello, World";
		S16 result = S16( 12, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 12 );
		EXPECT_EQ( conversion_result.code_point_count, 12 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		S16 expect = u"Hello, World!";
		S16 result = S16( 13, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 13 );
		EXPECT_EQ( conversion_result.code_point_count, 13 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C32FromChar )
{
	using namespace bc::conversion;

	{
		SC source = "Hello, World!";
		S32 expect = U"Hello, World";
		S32 result = S32( 12, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 12 );
		EXPECT_EQ( conversion_result.code_point_count, 12 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		SC source = "Hello, World!";
		S32 expect = U"Hello, World!";
		S32 result = S32( 13, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );

		EXPECT_EQ( conversion_result.code_unit_count, 13 );
		EXPECT_EQ( conversion_result.code_point_count, 13 );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C8FromC16 )
{
	// TODO
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C8FromC32 )
{
	// TODO
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C16FromC8 )
{
	// TODO
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C16FromC32 )
{
	// TODO
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C32FromC8 )
{
	using namespace bc::conversion;

	{
		// Complete conversion ( Empty input )
		//
		// UTF-8 Input String: ""
		// UTF-8 Hex Representation: []
		//
		// Expected UTF-32 Output String( Hex ) :
		// []
		//
		// Expected Result: SUCCESS
		S8 source = u8"";
		S32 expect = U"";
		S32 result = S32( 128, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
	{
		// Complete conversion
		//
		// UTF-8 Input String: "Hello, 世界"
		// UTF-8 Hex Representation: [ 48 65 6C 6C 6F 2C 20 E4 B8 96 E7 95 8C ]
		//
		// Expected UTF-32 Output String( Hex ) :
		// [ 00000048 00000065 0000006C 0000006C 0000006F 0000002C 00000020 00004E16 0000754C ]
		//
		// Expected Result: SUCCESS
		S8 source = u8"Hello, 世界";
		S32 expect = U"Hello, 世界";
		S32 result = S32( 128, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
	{
		// Complete Conversion ( Emoji )
		// 
		// UTF-8 Input String: "😊"
		// UTF-8 Hex Representation: [ F0 9F 98 8A ]
		// 
		// Expected UTF-32 Output String( Hex ):
		// [ 0001F60A ]
		// 
		// Expected Result: SUCCESS
		S8 source = u8"😊";
		S32 expect = U"😊";
		S32 result = S32( 128, '\0' );
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::SUCCESS );
		EXPECT_EQ( result, expect );
	}
	{
		// Too short of an output string.
		//
		// UTF-8 Input String: "Hello, 世界"
		// UTF-8 Hex Representation: [ 48 65 6C 6C 6F 2C 20 E4 B8 96 E7 95 8C ]
		//
		// Expected UTF-32 Output String( Hex ) :
		// [ 00000048 00000065 0000006C 0000006C 0000006F 0000002C 00000020 00004E16 ]
		//
		// Expected Result: INCOMPLETE
		S8 source = u8"Hello, 世界";
		S32 expect = U"Hello, 世";
		S32 result = S32( expect.size(), '\0');
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result, expect );
	}
	{
		// Incomplete Multi-byte Sequence
		// ( The last character \xE4\xB8 is an incomplete sequence, so the conversion should stop here. )
		//
		// UTF-8 Input String: "Incomplete: \xE4\xB8"
		// UTF-8 Hex Representation: [ 49 6E 63 6F 6D 70 6C 65 74 65 3A 20 E4 B8 ]
		// 
		// Expected UTF-32 Output String( Hex ):
		// [ 00000049 0000006E 00000063 0000006F 0000006D 00000070 0000006C 00000065 00000074 00000065 0000003A 00000020 ]
		// 
		// Expected Result: ERROR

		S8 source = u8"Incomplete: \xE4\xB8";
		S32 expect = U"Incomplete: ";
		S32 result = S32( 128, '\0');
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::ERROR );
		EXPECT_EQ( result, expect );
	}
	{
		// Invalid Byte Sequence
		// (The sequence \xF0\x28\x8C\x28 is invalid in UTF-8)
		// 
		// UTF-8 Input String: "Invalid: \xF0\x28\x8C\x28Valid😊"
		// UTF-8 Hex Representation: [ 49 6E 76 61 6C 69 64 3A 20 F0 28 8C 28 56 61 6C 69 64 F0 9F 98 8A ]
		//
		// The conversion should stop at the first invalid sequence "\xF0\x28\x8C\x28".
		//
		// Expected UTF-32 Output String (Hex):
		// [ 00000049 0000006E 00000076 00000061 0000006C 00000069 00000064 0000003A 00000020 ]
		//
		// Expected Result: ERROR
		S8 source = u8"Invalid: \xF0\x28\x8C\x28Valid😊";
		S32 expect = U"Invalid: ";
		S32 result = S32( 128, '\0');
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::ERROR );
		EXPECT_EQ( result, expect );
	}
	{
		// Truncated Input
		// ( The sequence \xF0\x9F starts a 4-byte UTF-8 sequence but is incomplete. )
		// 
		// UTF-8 Input String: "Truncated: \xF0\x9F"
		// UTF-8 Hex Representation: [ 54 72 75 6E 63 61 74 65 64 3A 20 F0 9F ]
		// 
		// Expected UTF-32 Output String( Hex ):
		// [ 00000054 00000072 00000075 0000006E 00000063 00000061 00000074 00000065 00000064 0000003A 00000020 ]
		// 
		// Expected Result: ERROR
		S8 source = u8"Truncated: \xF0\x9F";
		S32 expect = U"Truncated: ";
		S32 result = S32( 128, '\0');
		auto conversion_result = UTFConvert( result.data(), result.data() + result.size(), source.data(), source.data() + source.size() );
		result.resize( conversion_result.code_unit_count );

		EXPECT_EQ( conversion_result.code_unit_count, expect.size() );
		EXPECT_EQ( conversion_result.code_point_count, expect.size() );
		EXPECT_EQ( conversion_result.outcome, UTFConvertResult::Outcome::ERROR );
		EXPECT_EQ( result, expect );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( UTFConversion, C32FromC16 )
{
	// TODO
}



} // namespace conversion
} // namespace core
