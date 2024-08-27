
#include <gtest/gtest.h>

#include <core/conversion/text/primitives/StringToPrimitiveConversion.hpp>

#include <string>



namespace core {
namespace conversion {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( StringToPrimitiveConversion, PrimitiveToTextConversion_Boolean )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr input pointer.
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool<char>( actual, nullptr, nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool<char>( actual, nullptr, input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool<char>( actual, input.data(), nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}

	// Test input iterators flipped.
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data() + input.size(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test non-nullptr input pointer with zero length input.
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test input buffer one too small.
	{
		C input = "false";
		auto expected = true;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual );
	}

	// Test input buffer just long enough.
	{
		C input = "false";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test input buffer that is longer than the value.
	{
		C input = "false true";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test different interpretations of true and false values.
	{
		C input = "TrUe";
		auto expected = true;
		auto actual = bool( false );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C input = "FaLsE";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C input = "1";
		auto expected = true;
		auto actual = bool( false );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C input = "0";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C input = "on";
		auto expected = true;
		auto actual = bool( false );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C input = "off";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 input = u8"false";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C16 input = u"false";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
	{
		C32 input = U"false";
		auto expected = false;
		auto actual = bool( true );
		auto result = StringToBool( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, input.size() );
		EXPECT_EQ( expected, actual );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( StringToPrimitiveConversion, PrimitiveToTextConversion_Integer )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr input pointer.
	{
		C input = "1234567890";
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger<char>( actual, nullptr, nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "1234567890";
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger<char>( actual, nullptr, input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "1234567890";
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger<char>( actual, input.data(), nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}

	// Test input iterators flipped.
	{
		C input = "1234567890";
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data() + input.size(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual ); // Should not have changed.
	}

	// Test non-nullptr input pointer with zero length.
	{
		C input = "1234567890";
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}

	// Test input buffer one too small.
	{
		C input = "1234567890";
		auto expected = 123456789;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 9 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1234567890";
		auto expected = -123456789;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}

	// Test input buffer just long enough.
	{
		C input = "1234567890";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1234567890";
		auto expected = -1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}

	// Test input buffer bigger than value. (terminating character)
	{
		C input = "1234567890 ";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1234567890\n";
		auto expected = -1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "1234567890\t";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1234567890\r";
		auto expected = -1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 input = u8"1234567890";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C16 input = u"1234567890";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C32 input = U"1234567890";
		auto expected = 1234567890;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}

	// Test zero input value.
	{
		C input = "0";
		auto expected = 0;
		auto actual = i64( 1234567890 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 1 );
		EXPECT_EQ( actual, expected );
	}

	// Test binary encodings.
	{
		C input = "11110001001000000";
		auto expected = 0b11110001001000000;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::BINARY );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 17 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-11110001001000000";
		auto expected = -0b11110001001000000;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::BINARY );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 18 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "11110001021000000"; // Should return invalid because binary cannot contain numbers larger than 1.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::BINARY );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		C input = "111100010a1000000"; // Should return invalid because binary cannot contain numbers larger than 1, letters might indicate a hex number.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::BINARY );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		C input = "0b11110001001000000";
		auto expected = 0b11110001001000000;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::BINARY );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 19 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0b11110001001000000";
		auto expected = 0b11110001001000000;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() ); // Automatic binary conversion.
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 19 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-0b11110001001000000";
		auto expected = -0b11110001001000000;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 20 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0b11110001021000000"; // Should return invalid because binary cannot contain numbers larger than 1.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}

	// Test octal encodings.
	{
		C input = "12345670";
		auto expected = 012345670;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::OCTAL );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-12345670";
		auto expected = -012345670;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::OCTAL );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 18 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "123456780"; // Should return invalid because octal cannot contain numbers larger than 7.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::OCTAL );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		C input = "1234567a0"; // Should return invalid because octal cannot contain numbers larger than 7, letters might indicate a hex number.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::OCTAL );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		C input = "0o12345670";
		auto expected = 012345670;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::OCTAL );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0o12345670";
		auto expected = 012345670;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() ); // Automatic octal conversion.
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-0o12345670";
		auto expected = -012345670;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0o123456780"; // Should return invalid because octal cannot contain numbers larger than 7.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}

	// Test hexadecimal encodings.
	{
		C input = "123456789abcdef";
		auto expected = 0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 15 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-123456789abcdef";
		auto expected = -0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 18 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "123456789abcdefg"; // Should return invalid because hex cannot contain numbers larger than F.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		C input = "0x123456789abcdef";
		auto expected = 0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size(), StringToIntegerConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 17 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0x123456789abcdef";
		auto expected = 0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() ); // Automatic hex conversion.
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 17 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-0x123456789abcdef";
		auto expected = -0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 18 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0x123456789abcdefg"; // Should return invalid because hex cannot contain numbers larger than F.
		auto expected = 0;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not change the actual value.
	}
	{
		// Upper case characters.
		C input = "0X123456789ABCDEF";
		auto expected = 0x123456789ABCDEF;
		auto actual = i64( 0 );
		auto result = StringToInteger( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 17 );
		EXPECT_EQ( actual, expected );
	}

	// StringToUnsignedInteger uses the same functionality as StringToInteger, testing it is redundant.
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( StringToPrimitiveConversion, PrimitiveToTextConversion_Float )
{
	using namespace bc;
	using namespace bc::conversion;

	using C = std::basic_string<char>;
	using C8 = std::basic_string<c8>;
	using C16 = std::basic_string<c16>;
	using C32 = std::basic_string<c32>;

	// Test nullptr input pointer.
	{
		C input = "128.125";
		auto expected = 0.0;
		auto actual = f64( 0.0 );
		auto result = StringToFloat<char>( actual, nullptr, nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "128.125";
		auto expected = 0.0;
		auto actual = f64( 0.0 );
		auto result = StringToFloat<char>( actual, nullptr, input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}
	{
		C input = "128.125";
		auto expected = 0.0;
		auto actual = f64( 0.0 );
		auto result = StringToFloat<char>( actual, input.data(), nullptr );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}

	// Test input iterators flipped.
	{
		C input = "128.125";
		auto expected = 0.0;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data() + input.size(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::ERROR );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( expected, actual ); // Should not have changed.
	}

	// Test non-nullptr input pointer with zero length.
	{
		C input = "128.125";
		auto expected = 0.0;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected ); // Should not have changed.
	}

	// Test input buffer one too small.
	{
		C input = "128.125";
		auto expected = 128.12;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 6 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125";
		auto expected = -128.12;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "128.125f";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125f";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() - 1 );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}

	// Test input buffer just long enough.
	{
		C input = "128.125";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "128.125f";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125f";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 9 );
		EXPECT_EQ( actual, expected );
	}

	// Test input buffer bigger than value. (terminating character)
	{
		C input = "128.125 ";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125\n";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "128.125\t";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125\r";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}

	// Test utf encodings, these work the same way as ascii.
	{
		C8 input = u8"128.125";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C16 input = u"128.125";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C32 input = U"128.125";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}

	// Test zero input value.
	{
		C input = "0";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 1 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0.0";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 3 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0.";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 2 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = ".0";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 2 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0.f";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 3 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = ".0f";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 3 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0.0f";
		auto expected = 0.0;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 4 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0f";
		auto expected = 128.125;
		auto actual = f64( 128.125 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::INVALID );
		EXPECT_EQ( result.length, 0 );
		EXPECT_EQ( actual, expected );
	}

	// Test scientific encodings.
	{
		C input = "1.28125e+02";
		auto expected = 1.28125e+02;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1.28125e+02";
		auto expected = -1.28125e+02;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 12 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "1.28125000e+02";
		auto expected = 1.28125e+02;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 14 );
		EXPECT_EQ( actual, expected );
	}

	// Test fixed encodings (extra zeroes)
	{
		C input = "128.125000";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125000";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "128.125000f";
		auto expected = 128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 11 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-128.125000f";
		auto expected = -128.125;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 12 );
		EXPECT_EQ( actual, expected );
	}

	// Test hexadecimal encodings.
	{
		C input = "1.F8p+2";
		auto expected = 0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size(), StringToFloatConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 7 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-1.F8p+2";
		auto expected = -0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size(), StringToFloatConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 8 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0x1.F8p+2";
		auto expected = 0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size(), StringToFloatConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 9 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-0x1.F8p+2";
		auto expected = -0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size(), StringToFloatConversionFormat::HEX );
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "0x1.F8p+2";
		auto expected = 0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() ); // Should automatically detect hex encoding.
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 9 );
		EXPECT_EQ( actual, expected );
	}
	{
		C input = "-0x1.F8p+2";
		auto expected = -0x1.F8p+2;
		auto actual = f64( 0.0 );
		auto result = StringToFloat( actual, input.data(), input.data() + input.size() ); // Should automatically detect hex encoding.
		EXPECT_EQ( result.outcome, StringToPrimitiveResult::Outcome::SUCCESS );
		EXPECT_EQ( result.length, 10 );
		EXPECT_EQ( actual, expected );
	}
}



} // namespace conversion
} // namespace core
