#pragma once

#include "PrimitiveTextFormatter.hpp"
#include "PairTextFormatter.hpp"
#include "../TextFormatter.hpp"
#include <core/containers/Map.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	typename								KeyType,
	typename								ValueType
>
class TextFormatter<OutTextContainerType, Map<KeyType, ValueType>>
{
	using OutTextContainerFullType = typename OutTextContainerType::ThisFullType;
	using OutTextContainerViewType = typename OutTextContainerType::template ThisViewType<true>;

	TextFormatter<OutTextContainerType, Pair<KeyType, ValueType>> pair_formatter;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextContainerViewType parse_text )
	{
		if( !parse_text.IsEmpty() ) {
			pair_formatter.Parse( parse_text );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextContainerFullType & out, const Map<KeyType, ValueType> & in )
	{
		out.PushBack( OutT( '{' ) );

		if( in.Size() ) {
			auto it = in.begin();
			auto end = in.end() - 1;
			while( it != end ) {
				pair_formatter.Format( out, *it );
				out.Append( ", " );
				++it;
			}
			pair_formatter.Format( out, *it );
		}
		out.PushBack( OutT( '}' ) );
	}
};



} // text
} // bc
