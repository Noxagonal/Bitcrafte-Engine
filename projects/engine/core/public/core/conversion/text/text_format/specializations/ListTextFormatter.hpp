#pragma once

#include "PrimitiveTextFormatter.hpp"
#include "../TextFormatter.hpp"
#include <core/containers/List.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView	OutTextContainerType,
	typename					ElementType
>
class TextFormatter<OutTextContainerType, List<ElementType>>
{
	using OutTextContainerFullType = typename OutTextContainerType::ThisFullType;
	using OutTextContainerViewType = typename OutTextContainerType::template ThisViewType<true>;

	TextFormatter<OutT, ElementType> sub_formatter;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextContainerViewType parse_text )
	{
		sub_formatter.Parse( parse_text );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextContainerFullType & out, const List<ElementType> & in )
	{
		out.PushBack( '[' );
		if( in.Size() ) {
			for( u64 i = 0; i < in.Size() - 1; ++i ) {
				sub_formatter.Format( out, in[ i ] );
				out.Append( ", " );
			}
			sub_formatter.Format( out, in.Back() );
		}
		out.PushBack( ']' );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	utility::TextContainerView				OutTextContainerType,
	typename								ElementType,
	bool									IsViewConst
>
class TextFormatter<OutTextContainerType, ListViewBase<ElementType, IsViewConst>>
{
	using OutTextContainerFullType = typename OutTextContainerType::ThisFullType;
	using OutTextContainerViewType = typename OutTextContainerType::template ThisViewType<true>;

	TextFormatter<OutTextContainerType, ElementType> sub_formatter;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Parse( const OutTextContainerViewType parse_text )
	{
		sub_formatter.Parse( parse_text );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Format( OutTextContainerFullType & out, const ListViewBase<ElementType, IsArrayViewConst> & in )
	{
		out.PushBack( '[' );
		if( in.Size() ) {
			for( u64 i = 0; i < in.Size() - 1; ++i ) {
				sub_formatter.Format( out, in[ i ] );
				out.Append( ", " );
			}
			sub_formatter.Format( out, in.Back() );
		}
		out.PushBack( ']' );
	}
};



} // text
} // bc
