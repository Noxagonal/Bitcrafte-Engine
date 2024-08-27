
#include <gtest/gtest.h>

#include <core/conversion/text/text_format/TextFormat.hpp>

#include <core/containers/Text.hpp>

#include <string>



namespace core {
namespace conversion {

using SC = std::basic_string<char>;
using S8 = std::basic_string<bc::c8>;
using S16 = std::basic_string<bc::c16>;
using S32 = std::basic_string<bc::c32>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, Basic )
{
	using namespace bc;

	{
		auto a = text::TextFormat( "Pass through" );
		EXPECT_EQ( a, "Pass through" );
	}
	{
		auto a = text::TextFormat( "{}", "Text parameter" );
		EXPECT_EQ( a, "Text parameter" );
	}
	{
		auto a = text::TextFormat( "Arguments {}", "embedded" );
		EXPECT_EQ( a, "Arguments embedded" );
	}
	{
		auto a = text::TextFormat( "Arguments {} in middle: {}", "embedded", 5 );
		EXPECT_EQ( a, "Arguments embedded in middle: 5" );
	}
	{
		// Too many arguments given to the function should not cause any issues.
		auto a = text::TextFormat( "{}, {}", "first", "second", "third" );
		EXPECT_EQ( a, "first, second" );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, TextFormatting )
{
	using namespace bc;

	{
		auto a = text::TextFormat( "{}", Text( "Text parameter" ) );
		EXPECT_EQ( a, "Text parameter" );
	}
	{
		auto a = text::TextFormat( "{}", TextView( "Text view parameter" ) );
		EXPECT_EQ( a, "Text view parameter" );
	}

	// Arguments: {}

	// Exceptions
	{
		// Unrecognized format text argument should throw an error
		EXPECT_THROW( text::TextFormat( "{a}", Text( "Should throw an error" ) ), diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, BoolFormatting )
{
	using namespace bc;

	{
		auto a = text::TextFormat( "{}", true );
		EXPECT_EQ( a, "true" );
	}
	{
		auto a = text::TextFormat( "{}", false );
		EXPECT_EQ( a, "false" );
	}

	// Arguments: {b}
	// b = binary
	//
	// Binary
	{
		auto a = text::TextFormat( "{b}", true );
		EXPECT_EQ( a, "1" );
	}
	{
		auto a = text::TextFormat( "{b}", false );
		EXPECT_EQ( a, "0" );
	}

	// Exceptions
	{
		// Unrecognized format text argument should throw an error
		EXPECT_THROW( text::TextFormat( "{a}", true ), diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, IntegerFormatting )
{
	using namespace bc;

	// Basic
	{
		auto a = text::TextFormat( "{}", 0 );
		EXPECT_EQ( a, "0" );
	}
	{
		auto a = text::TextFormat( "{}", -0 );
		EXPECT_EQ( a, "0" );
	}
	{
		auto a = text::TextFormat( "{}", 5 );
		EXPECT_EQ( a, "5" );
	}
	{
		auto a = text::TextFormat( "{}", -5 );
		EXPECT_EQ( a, "-5" );
	}
	{
		auto a = text::TextFormat( "{}", 525262 );
		EXPECT_EQ( a, "525262" );
	}
	{
		auto a = text::TextFormat( "{}", -525262 );
		EXPECT_EQ( a, "-525262" );
	}

	// Arguments: {z[n]boxpu}
	// z[n] = zero fill, [n] = minimum number of digits (square brackets [] should not be included)
	// b = binary
	// o = octal
	// x = hex
	// p = prefix
	// u = uppercase
	// 
	// Zero fill
	{
		auto a = text::TextFormat( "{z0}", 0 );
		EXPECT_EQ( a, "0" );
	}
	{
		auto a = text::TextFormat( "{z1}", 0 );
		EXPECT_EQ( a, "0" );
	}
	{
		auto a = text::TextFormat( "{z2}", 0 );
		EXPECT_EQ( a, "00" );
	}
	{
		auto a = text::TextFormat( "{z8}", 555 );
		EXPECT_EQ( a, "00000555" );
	}
	{
		auto a = text::TextFormat( "{z8}", 5555555 );
		EXPECT_EQ( a, "05555555" );
	}
	{
		auto a = text::TextFormat( "{z8}", 55555555 );
		EXPECT_EQ( a, "55555555" );
	}
	{
		auto a = text::TextFormat( "{z8}", 555555555 );
		EXPECT_EQ( a, "555555555" );
	}
	{
		auto a = text::TextFormat( "{z8}", -555 );
		EXPECT_EQ( a, "-00000555" );
	}
	{
		auto a = text::TextFormat( "{z8}", -5555555 );
		EXPECT_EQ( a, "-05555555" );
	}
	{
		auto a = text::TextFormat( "{z8}", -55555555 );
		EXPECT_EQ( a, "-55555555" );
	}
	{
		auto a = text::TextFormat( "{z8}", -555555555 );
		EXPECT_EQ( a, "-555555555" );
	}

	// Binary
	{
		auto a = text::TextFormat( "{b}", 0b0 );
		EXPECT_EQ( a, "0" );
	}
	{
		auto a = text::TextFormat( "{bp}", 0b0 );
		EXPECT_EQ( a, "0b0" );
	}
	{
		auto a = text::TextFormat( "{b}", 0b1 );
		EXPECT_EQ( a, "1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{bp}", 0b1 );
		EXPECT_EQ( a, "0b1" );
	}
	{
		// With prefix (flipped order)
		auto a = text::TextFormat( "{pb}", 0b1 );
		EXPECT_EQ( a, "0b1" );
	}
	{
		auto a = text::TextFormat( "{b}", -0b1 );
		EXPECT_EQ( a, "-1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{bp}", -0b1 );
		EXPECT_EQ( a, "-0b1" );
	}
	{
		// With prefix (flipped order)
		auto a = text::TextFormat( "{pb}", -0b1 );
		EXPECT_EQ( a, "-0b1" );
	}
	{
		auto a = text::TextFormat( "{b}", 0b1000 );
		EXPECT_EQ( a, "1000" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{bp}", 0b1000 );
		EXPECT_EQ( a, "0b1000" );
	}
	{
		// With prefix (flipped order)
		auto a = text::TextFormat( "{pb}", 0b1000 );
		EXPECT_EQ( a, "0b1000" );
	}
	{
		auto a = text::TextFormat( "{b}", -0b1000 );
		EXPECT_EQ( a, "-1000" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{bp}", -0b1000 );
		EXPECT_EQ( a, "-0b1000" );
	}
	{
		// With prefix (flipped order)
		auto a = text::TextFormat( "{pb}", -0b1000 );
		EXPECT_EQ( a, "-0b1000" );
	}

	// Octal
	{
		auto a = text::TextFormat( "{o}", 0 );
		EXPECT_EQ( a, "0" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{op}", 0 );
		EXPECT_EQ( a, "0o0" );
	}
	{
		auto a = text::TextFormat( "{o}", 01 );
		EXPECT_EQ( a, "1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{op}", 01 );
		EXPECT_EQ( a, "0o1" );
	}
	{
		auto a = text::TextFormat( "{o}", -01 );
		EXPECT_EQ( a, "-1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{o}", -01 );
		EXPECT_EQ( a, "-0o1" );
	}
	{
		auto a = text::TextFormat( "{o}", 01234567 );
		EXPECT_EQ( a, "1234567" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{op}", 01234567 );
		EXPECT_EQ( a, "0o1234567" );
	}
	{
		auto a = text::TextFormat( "{o}", -01234567 );
		EXPECT_EQ( a, "-1234567" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{op}", -01234567 );
		EXPECT_EQ( a, "-0o1234567" );
	}

	// Hex
	{
		auto a = text::TextFormat( "{x}", 0x0 );
		EXPECT_EQ( a, "0" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{xp}", 0x0 );
		EXPECT_EQ( a, "0x0" );
	}
	{
		auto a = text::TextFormat( "{x}", 0x1 );
		EXPECT_EQ( a, "1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{xp}", 0x1 );
		EXPECT_EQ( a, "0x1" );
	}
	{
		auto a = text::TextFormat( "{x}", -0x1 );
		EXPECT_EQ( a, "-1" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{xp}", -0x1 );
		EXPECT_EQ( a, "-0x1" );
	}
	{
		auto a = text::TextFormat( "{x}", 0x123456789ABCDEF );
		EXPECT_EQ( a, "123456789abcdef" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{xp}", 0x123456789ABCDEF );
		EXPECT_EQ( a, "0x123456789abcdef" );
	}
	{
		auto a = text::TextFormat( "{x}", -0x123456789ABCDEF );
		EXPECT_EQ( a, "-123456789abcdef" );
	}
	{
		// With prefix
		auto a = text::TextFormat( "{xp}", -0x123456789ABCDEF );
		EXPECT_EQ( a, "-0x123456789abcdef" );
	}
	{
		// Uppercase
		auto a = text::TextFormat( "{xu}", 0x123456789ABCDEF );
		EXPECT_EQ( a, "123456789ABCDEF" );
	}
	{
		// With prefix
		// Uppercase
		auto a = text::TextFormat( "{xpu}", 0x123456789ABCDEF );
		EXPECT_EQ( a, "0x123456789ABCDEF" );
	}
	{
		// Uppercase
		auto a = text::TextFormat( "{xu}", -0x123456789ABCDEF );
		EXPECT_EQ( a, "-123456789ABCDEF" );
	}
	{
		// With prefix
		// Uppercase
		auto a = text::TextFormat( "{xpu}", -0x123456789ABCDEF );
		EXPECT_EQ( a, "-0x123456789ABCDEF" );
	}

	// Combined
	{
		auto a = text::TextFormat( "{bz8}", 0b1000 );
		EXPECT_EQ( a, "0b00001000" );
	}

	// Exceptions
	{
		// Unrecognized format text argument should throw an error
		EXPECT_THROW( text::TextFormat( "{a}", 50 ), diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, FloatFormatting )
{
	using namespace bc;

	{
		auto a = text::TextFormat( "{}", 0.0f );
		EXPECT_EQ( a, "0.0" );
	}
	{
		auto a = text::TextFormat( "{}", -0.0f );
		EXPECT_EQ( a, "-0.0" );
	}
	{
		auto a = text::TextFormat( "{}", 1.0f );
		EXPECT_EQ( a, "1.0" );
	}
	{
		auto a = text::TextFormat( "{}", -1.0f );
		EXPECT_EQ( a, "-1.0" );
	}
	{
		// Rounding errors should be ignored when close to a whole number
		auto a = text::TextFormat( "{}", 3.0f );
		EXPECT_EQ( a, "3.0" );
	}

	// Arguments: {.[n]fsx}
	// . = precision, n = number of digits after the decimal point
	// f = fixed notation
	// s = scientific notation
	// x = hex notation
	// u = uppercase
	//
	// Precision
	{
		auto a = text::TextFormat( "{.0}", 50.245f );
		EXPECT_EQ( a, "50" );
	}
	{
		auto a = text::TextFormat( "{.1}", 50.245f );
		EXPECT_EQ( a, "50.2" );
	}
	{
		auto a = text::TextFormat( "{.2}", 50.245f );
		EXPECT_EQ( a, "50.25" );
	}
	{
		auto a = text::TextFormat( "{.6}", 50.245f );
		EXPECT_EQ( a, "50.245" );
	}

	// fixed notation
	{
		// Fixed notation should be used with precision (.[n]), it won'd to anything alone
		auto a = text::TextFormat( "{f}", 50.245f );
		EXPECT_EQ( a, "50.245" );
	}

	// scientific notation
	{
		auto a = text::TextFormat( "{s}", 50.245f );
		EXPECT_EQ( a, "5.024500e+01" );
	}
	{
		auto a = text::TextFormat( "{s}", -50.245f );
		EXPECT_EQ( a, "-5.024500e+01" );
	}

	// hex notation
	{
		auto a = text::TextFormat( "{x}", 50.245f );
		EXPECT_EQ( a, "0x1.90fb0ep+5" );
	}
	{
		auto a = text::TextFormat( "{x}", -50.245f );
		EXPECT_EQ( a, "-0x1.90fb0ep+5" );
	}

	// uppercase
	{
		// Uppercase only effects letters so it won't do anything alone.
		auto a = text::TextFormat( "{u}", 50.245f );
		EXPECT_EQ( a, "50.245" );
	}

	// Combined
	{
		// Fixed notation with precision (.[n]) forces a specific number of digits after the decimal point
		auto a = text::TextFormat( "{.0f}", 50.245f );
		EXPECT_EQ( a, "50" );
	}
	{
		// Fixed notation with precision (.[n]) forces a specific number of digits after the decimal point
		auto a = text::TextFormat( "{.6f}", 50.245f );
		EXPECT_EQ( a, "50.245000" );
	}
	{
		// Hex and uppercase
		auto a = text::TextFormat( "{xu}", 50.245f );
		EXPECT_EQ( a, "0x1.90FB0EP+5" );
	}

	// Exceptions
	{
		// Unrecognized format text argument should throw an error
		EXPECT_THROW( text::TextFormat( "{a}", 50.245f ), diagnostic::Exception );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, ParameterIndexing )
{
	using namespace bc;

	{
		auto a = text::TextFormat( "{:1}, {:0}", "first", "second" );
		EXPECT_EQ( a, "second, first" );
	}
	{
		// Reusing arguments should work fine.
		auto a = text::TextFormat( "{:0}, {:0}", "first", "second" );
		EXPECT_EQ( a, "first, first" );
	}
	{
		// Implicit increment for remaining parameters.
		auto a = text::TextFormat( "{:0}, {}", "first", "second" );
		EXPECT_EQ( a, "first, second" );
	}
	{
		auto a = text::TextFormat( "{:1}, {:0}, {:2}", "first", "second", "third" );
		EXPECT_EQ( a, "second, first, third" );
	}
	{
		// Skipping arguments should work.
		auto a = text::TextFormat( "{:1}, {}", "first", "second", "third" );
		EXPECT_EQ( a, "second, third" );
	}
	{
		// Implicit increment for remaining parameters.
		auto a = text::TextFormat( "{:1}, {:0}, {}, {}", "first", "second", "third" );
		EXPECT_EQ( a, "second, first, second, third" );
	}
	{
		// Too many arguments given to the function should not cause any issues.
		auto a = text::TextFormat( "{:1}, {:0}", "first", "second", "third" );
		EXPECT_EQ( a, "second, first" );
	}

	// Test convert options with repositioning
	{
		auto a = text::TextFormat( "{x:1}, {bz8:0}", 0b1011, 0x10 );
		EXPECT_EQ( a, "0x10, 0b00001011" );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( TextFormat, GeneralExceptions )
{
	using namespace bc;

	{
		// Too few arguments given to function should throw.
		EXPECT_THROW( text::TextFormat( "{}, {}", "first" ), diagnostic::Exception );
	}
}



} // namespace conversion
} // namespace core
