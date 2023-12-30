#pragma once

#include "PrimitiveTextFormatter.hpp"
#include "../TextFormatter.hpp"
#include <core/containers/Pair.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	typename								FirstType,
	typename								SecondType
>
class TextFormatter<OutTextContainerType, Pair<FirstType, SecondType>>
{
	using OutTextContainerFullType = typename OutTextContainerType::ThisFullType;
	using OutTextContainerViewType = typename OutTextContainerType::template ThisViewType<true>;

	TextFormatter<OutTextContainerFullType, FirstType>		first_formatter;
	TextFormatter<OutTextContainerFullType, SecondType>		second_formatter;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextContainerViewType parse_text )
	{
		if( !parse_text.IsEmpty() ) {
			auto separator_it = parse_text.Find( ":" );
			if( separator_it == SIZE_MAX ) {
				first_formatter.Parse( parse_text );
			} else {
				first_formatter.Parse( parse_text.SubText( 0, separator_it ) );
				second_formatter.Parse( parse_text.SubText( separator_it + 1 ) );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextContainerFullType & out, const Pair<FirstType, SecondType> & in )
	{
		out.Append( "[" );
		first_formatter.Format( out, in.first );
		out.Append( ":" );
		second_formatter.Format( out, in.second );
		out.Append( "]" );
	}
};



} // text
} // bc
