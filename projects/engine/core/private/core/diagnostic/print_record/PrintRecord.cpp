
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::operator+=(
	const PrintRecord & other
)
{
	Append( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::operator+=(
	const bc::diagnostic::PrintRecordSection & section
)
{
	AddSection( section );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord bc::diagnostic::PrintRecord::operator+(
	const bc::diagnostic::PrintRecord & other
) const
{
	auto result = *this;
	result += other;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const bc::diagnostic::PrintRecord::PrintRecordSectionList & bc::diagnostic::PrintRecord::GetSections() const
{
	return section_list;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const uint32_t bc::diagnostic::PrintRecord::GetLineCount() const
{
	if( is_finalized ) return calculated_line_count;
	return CalculateLineCount( section_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::Append(
	const PrintRecord & other
)
{
	if( is_finalized ) return *this;

	section_list.Append( other.section_list );

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::AddSection(
	const PrintRecordSection & section
)
{
	if( is_finalized ) return *this;

	section_list.PushBack( section );

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::AddIndent(
	int32_t add_indentation_level
)
{
	if( is_finalized ) return *this;

	for( auto & s : section_list )
	{
		s.indent += add_indentation_level;
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bool bc::diagnostic::PrintRecord::IsFinalized() const
{
	return is_finalized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bool bc::diagnostic::PrintRecord::IsEmpty() const
{
	return section_list.IsEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::Finalize(
	uint32_t indentation_size
)
{
	if( is_finalized ) return *this;

	auto indented_section_list	= ApplyIndents( section_list, indentation_size );
	calculated_line_count		= CalculateLineCount( indented_section_list );

	section_list = indented_section_list;

	is_finalized = true;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord::PrintRecordSectionList bc::diagnostic::PrintRecord::ApplyIndents(
	const PrintRecordSectionList	&	source_section_list,
	uint32_t							indentation_size
) const
{
	assert( !is_finalized && "Should not call this function when print record has already been finalized" );

	auto new_section_list = PrintRecordSectionList {};
	auto indent_next = true;

	for( auto & r : source_section_list )
	{
		auto new_section = PrintRecordSection();
		new_section.theme		= r.theme;
		new_section.indent		= r.indent;
		new_section.text.Reserve( r.text.Size() + 16 );

		for( int64_t i = 0; i < int64_t( r.text.Size() ); i++ )
		{
			auto c = r.text[ i ];
			if( indent_next )
			{
				new_section.text.FillBack( U' ', r.indent * indentation_size );
				indent_next = false;
			}

			new_section.text.PushBack( c );

			if( c == U'\n' )
			{
				// If last character was a newline, we do not insert indentation here but tell the next section to indent itself.
				if( i == int64_t( r.text.Size() ) - 1 )
				{
					indent_next = true;
				}
				else
				{
					new_section.text.FillBack( U' ', r.indent * indentation_size );
				}
				continue;
			}
		}

		new_section_list.PushBack( new_section );
	}

	return new_section_list;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr uint32_t bc::diagnostic::PrintRecord::CalculateLineCount(
	const bc::diagnostic::PrintRecord::PrintRecordSectionList & source_section_list
) const
{
	assert( !is_finalized && "Should not call this function when print record has already been finalized" );

	auto character_count = uint32_t { 1 };
	for( auto & section : source_section_list )
	{
		character_count += uint32_t( section.text.CountCharacters( U'\n' ) );
	}

	return character_count;
}
