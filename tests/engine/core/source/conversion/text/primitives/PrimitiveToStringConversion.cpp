
#include <gtest/gtest.h>

#include <core/conversion/text/primitives/PrimitiveToStringConversion.hpp>

#include <string>



namespace core {
namespace conversion {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( PrimitiveToStringConversion, PrimitiveToTextConversion_Boolean )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr output pointer.
	{
		C actual = C( 64, '\0' );
		auto result = BoolToString<char>( nullptr, actual.data() + 0, true );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}
	{
		C actual = C( 64, '\0' );
		auto result = BoolToString<char>( nullptr, actual.data() + 0, false );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}

	// Test output iterators flipped.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data() + actual.size(), actual.data(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data() + actual.size(), actual.data(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test non-nullptr output pointer with zero length output.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + 0, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + 0, false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer one too small.
	{
		C expected = "";
		C actual = C( 3, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 4, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer just long enough.
	{
		C expected = "true";
		C actual = C( 4, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "false";
		C actual = C( 5, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer bigger than needed.
	{
		C expected = "true";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "false";
		C actual = C( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 expected = u8"true";
		C8 actual = C8( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C8 expected = u8"false";
		C8 actual = C8( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C16 expected = u"true";
		C16 actual = C16( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 expected = u"false";
		C16 actual = C16( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C32 expected = U"true";
		C32 actual = C32( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 expected = U"false";
		C32 actual = C32( 64, '\0' );
		auto result = BoolToString( actual.data(), actual.data() + actual.size(), false );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( PrimitiveToStringConversion, PrimitiveToTextConversion_Integer )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr output pointer.
	{
		C actual = C( 64, '\0' );
		auto result = IntegerToString<char>( nullptr, actual.data() + 0, 0 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}
	{
		C actual = C( 64, '\0' );
		auto result = IntegerToString<char>( nullptr, actual.data() + 0, 1234567890 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}
	{
		C actual = C( 64, '\0' );
		auto result = IntegerToString<char>( nullptr, actual.data() + 0, -1234567890 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}

	// Test output iterators flipped.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data() + actual.size(), actual.data(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data() + actual.size(), actual.data(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data() + actual.size(), actual.data(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test non-nullptr output pointer with zero length output.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + 0, 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + 0, 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + 0, -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer one too small.
	{
		C expected = "";
		C actual = C( 9, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 10, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer just long enough.
	{
		C expected = "0";
		C actual = C( 1, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "1234567890";
		C actual = C( 10, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "1234567890";
		C actual = C( 11, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer bigger than needed.
	{
		C expected = "0";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "1234567890";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-1234567890";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 expected = u8"0";
		C8 actual = C8( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C8 expected = u8"1234567890";
		C8 actual = C8( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C8 expected = u8"-1234567890";
		C8 actual = C8( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C16 expected = u"0";
		C16 actual = C16( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 expected = u"1234567890";
		C16 actual = C16( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 expected = u"-1234567890";
		C16 actual = C16( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C32 expected = U"0";
		C32 actual = C32( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 expected = U"1234567890";
		C32 actual = C32( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 expected = U"-1234567890";
		C32 actual = C32( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -1234567890 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test binary encodings.
	{
		C expected = "11110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 123456, IntegerToStringConversionFormat::BINARY );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-11110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -123456, IntegerToStringConversionFormat::BINARY );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0b11110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 123456, IntegerToStringConversionFormat::BINARY, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0b11110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -123456, IntegerToStringConversionFormat::BINARY, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "00000000000000011110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 123456, IntegerToStringConversionFormat::BINARY, false, true, 32 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-00000000000000011110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -123456, IntegerToStringConversionFormat::BINARY, false, true, 32 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0b00000000000000011110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 123456, IntegerToStringConversionFormat::BINARY, true, true, 32 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0b00000000000000011110001001000000";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -123456, IntegerToStringConversionFormat::BINARY, true, true, 32 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test octal encodings.
	{
		C expected = "12345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-12345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -012345670, IntegerToStringConversionFormat::OCTAL );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0o12345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0o12345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0000000012345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL, false, true, 16 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0000000012345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL, false, true, 16 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0o0000000012345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 012345670, IntegerToStringConversionFormat::OCTAL, true, true, 16 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0o0000000012345670";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -012345670, IntegerToStringConversionFormat::OCTAL, true, true, 16 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test hexadecimal encodings.
	{
		C expected = "1234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-1234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0x1234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0x1234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "001234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, false, true, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-001234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, false, true, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0x001234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true, true, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0x001234567890ABCDEF";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true, true, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0x001234567890abcdef";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), 0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true, false, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0x001234567890abcdef";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), -0x1234567890ABCDEF, IntegerToStringConversionFormat::HEX, true, false, 18 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test maximum and minimum values
	{
		C expected = "-9223372036854775808";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), std::numeric_limits<i64>::min() );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "9223372036854775807";
		C actual = C( 64, '\0' );
		auto result = IntegerToString( actual.data(), actual.data() + actual.size(), std::numeric_limits<i64>::max() );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "18446744073709551615";
		C actual = C( 64, '\0' );
		auto result = UnsignedIntegerToString( actual.data(), actual.data() + actual.size(), std::numeric_limits<u64>::max() );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( PrimitiveToStringConversion, PrimitiveToTextConversion_Float )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr output pointer.
	{
		C actual = C( 64, '\0' );
		auto result = FloatToString<char>( nullptr, actual.data() + 0, 0.0 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}
	{
		C actual = C( 64, '\0' );
		auto result = FloatToString<char>( nullptr, actual.data() + 0, 128.125 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}
	{
		C actual = C( 64, '\0' );
		auto result = FloatToString<char>( nullptr, actual.data() + 0, -128.125 );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
	}

	// Test output iterators flipped.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data() + actual.size(), actual.data(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data() + actual.size(), actual.data(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data() + actual.size(), actual.data(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test non-nullptr output pointer with zero length output.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + 0, 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + 0, 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + 0, -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer one too small.
	{
		C expected = "";
		C actual = C( 2, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 6, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 7, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 6, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 2 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 9, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "";
		C actual = C( 10, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::INCOMPLETE );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer just long enough.
	{
		C expected = "0.0";
		C actual = C( 3, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.125";
		C actual = C( 7, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-128.125";
		C actual = C( 8, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.13";
		C actual = C( 6, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 2 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.125";
		C actual = C( 10, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-128.125";
		C actual = C( 11, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test output buffer bigger than needed.
	{
		C expected = "0.0";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.125";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-128.125";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.13";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 2 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.125000";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-128.125000";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125, FloatToStringConversionFormat::GENERAL, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 expected = u8"0.0";
		C8 actual = C8( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C8 expected = u8"128.125";
		C8 actual = C8( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C8 expected = u8"-128.125";
		C8 actual = C8( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C16 expected = u"0.0";
		C16 actual = C16( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 expected = u"128.125";
		C16 actual = C16( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 expected = u"-128.125";
		C16 actual = C16( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	{
		C32 expected = U"0.0";
		C32 actual = C32( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0.0 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 expected = U"128.125";
		C32 actual = C32( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 expected = U"-128.125";
		C32 actual = C32( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test scientific encodings.
	{
		C expected = "1.28125e+02";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 1.28125e+02, FloatToStringConversionFormat::SCIENTIFIC );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-1.28125e+02";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -1.28125e+02, FloatToStringConversionFormat::SCIENTIFIC );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "1.28125000e+02";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 1.28125e+02, FloatToStringConversionFormat::SCIENTIFIC, 8 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-1.28125000e+02";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -1.28125e+02, FloatToStringConversionFormat::SCIENTIFIC, 8 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test fixed encodings.
	{
		C expected = "128.13";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::FIXED, 3 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "128.125000";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::FIXED, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-128.125000";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -128.125, FloatToStringConversionFormat::FIXED, 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test hexadecimal encodings.
	{
		C expected = "0x1.F8p+2";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0x1.F8p+2, FloatToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0x1.F8p+2";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -0x1.F8p+2, FloatToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "0x1.F8p+2";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 0x1.F8p+2, FloatToStringConversionFormat::HEX, /*Ignored*/ 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C expected = "-0x1.F8p+2";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), -0x1.F8p+2, FloatToStringConversionFormat::HEX, /*Ignored*/ 6 );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Ensure fixed and scientific notations are not used at the same time.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::FIXED | FloatToStringConversionFormat::SCIENTIFIC );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Ensure fixed and hexadecimal notations are not used at the same time.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::FIXED | FloatToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}

	// Ensure scientific and hexadecimal notations are not used at the same time.
	{
		C expected = "";
		C actual = C( 64, '\0' );
		auto result = FloatToString( actual.data(), actual.data() + actual.size(), 128.125, FloatToStringConversionFormat::SCIENTIFIC | FloatToStringConversionFormat::HEX );
		actual.resize( result.length );
		EXPECT_EQ( result.outcome, PrimitiveToStringResult::Outcome::ERROR );
		EXPECT_EQ( result.length, expected.size() );
		EXPECT_EQ( expected, actual );
	}
}



} // namespace conversion
} // namespace core
