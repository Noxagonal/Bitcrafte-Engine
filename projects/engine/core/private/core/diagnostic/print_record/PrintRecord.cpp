
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord::PrintRecord(
	const bc::internal_::SimpleTextView32				simple_text_view
)
{
	auto new_section = PrintRecordSection {};
	new_section.text = simple_text_view;
	AddSection( new_section );
}

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
constexpr const bc::u32 bc::diagnostic::PrintRecord::CalculateLineCount() const
{
	auto character_count = u32 { 1 };
	for( auto & section : section_list )
	{
		character_count += u32( section.text.CountCharacters( U'\n' ) );
	}

	return character_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::Append(
	const PrintRecord & other
)
{
	section_list.Append( other.section_list );

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::AddSection(
	const PrintRecordSection & section
)
{
	section_list.PushBack( section );

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord & bc::diagnostic::PrintRecord::AddIndent(
	i32 add_indentation_level
)
{
	for( auto & s : section_list )
	{
		s.indent += add_indentation_level;
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bc::diagnostic::PrintRecord bc::diagnostic::PrintRecord::GetFinalized(
	u32 indentation_size
) const
{
	auto result = *this;

	result.Finalize_ApplyIndents( indentation_size );

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr bool bc::diagnostic::PrintRecord::IsEmpty() const noexcept
{
	return section_list.IsEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr void bc::diagnostic::PrintRecord::Finalize_ApplyIndents(
	u32		indentation_size
)
{
	auto new_section_list = PrintRecordSectionList {};
	auto indent_next = true;

	for( auto & r : section_list )
	{
		auto new_section = PrintRecordSection();
		new_section.theme		= r.theme;
		new_section.indent		= r.indent;
		new_section.text.Reserve( r.text.Size() + 16 );

		for( i64 i = 0; i < i64( r.text.Size() ); i++ )
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
				if( i == i64( r.text.Size() ) - 1 )
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

	section_list = std::move( new_section_list );
}
