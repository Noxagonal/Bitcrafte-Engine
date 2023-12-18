#pragma once

#include <core/containers/backend/ContainerBase.hpp>
#include <core/diagnostic/assertion/Assert.hpp>

#include <cuchar>



namespace bc {
namespace conversion {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Convert any text container type into a UTF-8 representation.
///
/// @tparam TextContainerType
///	Generic text container type. May be a text container view, output will always be an memory backed text object.
///
/// @param text
///	Container containing the text we want to convert to UTF-8.
///
/// @return
/// Memory backed text container type similar to input type. Eg. If input type is bc::Text32, then return type is also
/// bc::Text32, if input type is bc::TextView32 or bc::EditableTextView32, then return type is still also bc::Text32.
/// Other text container types work in similar manner.
template<container_bases::TextContainerView		TextContainerType>
auto											ToUTF8(
	const TextContainerType					&	text
)
{
	using OutTextContainerType = typename TextContainerType::template ThisContainerFullType<char8_t>;
	using OutTextCharacterType = typename TextContainerType::ContainedCharacterType;

	OutTextContainerType out;
	out.Reserve( text.Size() * sizeof( OutTextCharacterType ) );

	if constexpr( std::is_same_v<char, OutTextCharacterType> ) {
		// From ASCII to UTF-8.
		out = text;

	} else if constexpr( std::is_same_v<char8_t, OutTextCharacterType> ) {
		// From UTF-8 to UTF-8.
		out = text;

	} else if constexpr( std::is_same_v<char16_t, OutTextCharacterType> ) {
		// From UTF-16 to UTF-8.
		std::mbstate_t state {};
		char c8_buffer[ MB_LEN_MAX ];

		for( size_t i = 0; i < text.Size(); ++i ) {
			char16_t c16 = text[ i ];
			size_t c8_length = std::c16rtomb( c8_buffer, c16, &state );
			if( c8_length != size_t( -1 ) ) {
				for( size_t c = 0; c < c8_length; ++c ) {
					out.PushBack( c8_buffer[ c ] );
				}
			}
		}


	} else if constexpr( std::is_same_v<char32_t, OutTextCharacterType> ) {
		// From UTF-32 to UTF-8.
		std::mbstate_t state {};
		char c8_buffer[ MB_LEN_MAX ];

		for( size_t i = 0; i < text.Size(); ++i ) {
			char32_t c32 = text[ i ];
			size_t c8_length = std::c32rtomb( c8_buffer, c32, &state );
			if( c8_length != size_t( -1 ) ) {
				for( size_t c = 0; c < c8_length; ++c ) {
					out.PushBack( c8_buffer[ c ] );
				}
			}
		}

	} else {
		assert( 0 && "Not a valid character type" );
	}

	return out;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Convert any text container type into a UTF-16 representation.
///
/// @tparam TextContainerType
///	Generic text container type. May be a text container view, output will always be an memory backed text object.
///
/// @param text
///	Container containing the text we want to convert to UTF-16.
///
/// @return
/// Memory backed text container type similar to input type. Eg. If input type is bc::Text32, then return type is also
/// bc::Text32, if input type is bc::TextView32 or bc::EditableTextView32, then return type is still also bc::Text32.
/// Other text container types work in similar manner.
template<container_bases::TextContainerView		TextContainerType>
auto											ToUTF16(
	const TextContainerType					&	text
)
{
	using OutTextContainerType = typename TextContainerType::template ThisContainerFullType<char16_t>;
	using OutTextCharacterType = typename TextContainerType::ContainedCharacterType;

	OutTextContainerType out;
	out.Reserve( text.Size() * sizeof( OutTextCharacterType ) );

	if constexpr( std::is_same_v<char, OutTextCharacterType> ) {
		// From ASCII to UTF-16.
		out = text;

	} else if constexpr( std::is_same_v<char8_t, OutTextCharacterType> ) {
		// From UTF-8 to UTF-16.
		std::mbstate_t state {};
		char16_t c16 {};
		auto data_in		= reinterpret_cast<const char*>( text.Data() );
		auto data_in_end	= reinterpret_cast<const char*>( text.Data() + text.Size() );
		while( data_in < data_in_end ) {
			size_t read_length = std::mbrtoc16( &c16, data_in, text.Size(), &state );
			if( read_length == 0 ) break;
			if( read_length == size_t( -1 ) ) break;
			if( read_length == size_t( -2 ) ) break;
			out.PushBack( c16 );
			data_in += read_length;
		}

	} else if constexpr( std::is_same_v<char16_t, OutTextCharacterType> ) {
		// From UTF-16 to UTF-16.
		out = text;

	} else if constexpr( std::is_same_v<char32_t, OutTextCharacterType> ) {
		// From UTF-32 to UTF-16.
		out = ToUTF16( ToUTF8( text ) );

	} else {
		assert( 0 && "Not a valid character type" );
	}

	return out;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Convert any text container type into a UTF-32 representation.
///
/// @tparam TextContainerType
///	Generic text container type. May be a text container view, output will always be an memory backed text object.
///
/// @param text
///	Container containing the text we want to convert to UTF-32.
///
/// @return
/// Memory backed text container type similar to input type. Eg. If input type is bc::Text32, then return type is also
/// bc::Text32, if input type is bc::TextView32 or bc::EditableTextView32, then return type is still also bc::Text32.
/// Other text container types work in similar manner.
template<container_bases::TextContainerView		TextContainerType>
auto											ToUTF32(
	const TextContainerType					&	text
)
{
	using OutTextContainerType = typename TextContainerType::template ThisContainerFullType<char32_t>;
	using OutTextCharacterType = typename TextContainerType::ContainedCharacterType;

	OutTextContainerType out;
	out.Reserve( text.Size() * sizeof( OutTextCharacterType ) );

	if constexpr( std::is_same_v<char, OutTextCharacterType> ) {
		// From ASCII to UTF-32.
		out = text;

	} else if constexpr( std::is_same_v<char8_t, OutTextCharacterType> ) {
		// From UTF-8 to UTF-32.
		std::mbstate_t state {};
		char32_t c32 {};
		auto data_in		= reinterpret_cast<const char*>( text.Data() );
		auto data_in_end	= reinterpret_cast<const char*>( text.Data() + text.Size() );
		while( data_in < data_in_end ) {
			size_t read_length = std::mbrtoc32( &c32, data_in, text.Size(), &state );
			BAssert( read_length != size_t( -3 ), "Corrupt UTF text, UTF-32 does not have surrogates" );
			if( read_length == 0 ) break;
			if( read_length == size_t( -1 ) ) break;
			if( read_length == size_t( -2 ) ) break;
			out.PushBack( c32 );
			data_in += read_length;
		}

	} else if constexpr( std::is_same_v<char16_t, OutTextCharacterType> ) {
		// From UTF-16 to UTF-32.
		out = ToUTF32( ToUTF8( text ) );

	} else if constexpr( std::is_same_v<char32_t, OutTextCharacterType> ) {
		// From UTF-32 to UTF-32.
		out = text;

	} else {
		assert( 0 && "Not a valid character type" );
	}

	return out;
}



} // conversion
} // bc
