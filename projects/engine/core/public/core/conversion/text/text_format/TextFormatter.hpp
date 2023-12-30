#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/containers/backend/ContainerConcepts.hpp>



namespace bc {
namespace text {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Formatter default, need to specialize formatting for different types.
///
///	This is the base class that must be specialized for each type we wish to convert into text.
///
///	To create a new specialization, 2 functions must be added to that specialization:
///	<tt>constexpr void Parse( TextViewBase<OutT, true> parse_text ){};</tt>
///	<tt>constexpr void Format( TextBase<OutT> & out, const InT & in ){};</tt>
///
///	Parse function takes in a text view base, which contains the string inside of "{}" when TextFormat is called, minus the ":".
/// Consider this call to the TextFormat function: <tt>bc::TextFormat( U"{:u8}", "as_unicode" );</tt>. In this case the caller
/// wants to print out and treat the "as_unicode" line as unicode text instead of ascii. Parse function "parse_text" parameter
/// would get a string that looks like "u8", the formatter specialization can then save the parsed settings to it's members. The
/// specialization may have as many members as needed to store all parsed information.
///
///	If there are any errors in the format text, this is a good place to throw, before we get to parsing the text.
///
///	It is also possible to define Formatter specializations as members that will handle any sub elements or list members. Eg. in
/// a case we want to create a formatter for an array, and that array may contain any type, and we wish to format those types of
/// the array, we can define a sub formatter as a member. <tt>Formatter<OutT, ElementType> sub_formatter;</tt>
///
///	To use this sub_formatter while parsing, you must call it's Parse function inside this formatter's Parse function. Eg.
/// <tt>sub_formatter.Parse( parse_text );</tt>. It up to the formatter to decide what format text should be sent to the
/// sub_formatter.
///
///	Format function will convert any supported type and write it to the output buffer following the settings that were set by
/// the Parse function. Format function has one output parameter, it is a simple text container but you should only append to
/// it.
///
/// @tparam TextContainerType
/// Text container type which to use to input and output parsed text. Can be any bc text container type.
/// 
/// @tparam ValueType
///	Printed type, need to specialize this for each type.
template<utility::TextContainerView TextContainerType, typename ValueType>
class TextFormatter {};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TextFormatterSpecialization, typename TextContainerType, typename ValueType>
concept TextFormatAble = requires(
	TextFormatterSpecialization											formatter_specialization,
	typename TextContainerType::ThisFullType						&	out_buffer,
	typename TextContainerType::template ThisViewType<true>				parse_text,
	ValueType														&	input_value
)
{
	formatter_specialization.Parse( parse_text );
	formatter_specialization.Format( out_buffer, input_value );
};



} // text
} // bc
