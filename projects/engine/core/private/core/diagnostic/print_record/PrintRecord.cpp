
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/containers/backend/ContainerBase.hpp>

#include <core/memory/pod_auto_buffer/PODAutoBuffer.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::PrintRecord::PrintRecord( const PrintRecord& other ) noexcept
{
	this->Copy( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::PrintRecord::PrintRecord( PrintRecord&& other ) noexcept
{
	this->Swap( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::PrintRecord::~PrintRecord() noexcept
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::operator=( const PrintRecord& other ) noexcept -> PrintRecord&
{
	this->Copy( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::operator=( PrintRecord&& other ) noexcept -> PrintRecord&
{
	this->Copy( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::operator+=( const PrintRecord& other ) noexcept -> PrintRecord&
{
	this->Append( other );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::operator+=( const bc::diagnostic::PrintRecordSection& section ) noexcept -> PrintRecord&
{
	this->AddSection( section );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::operator+( const bc::diagnostic::PrintRecord& other ) const noexcept -> PrintRecord
{
	auto result = *this;
	result += other;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::GetSectionCount() const noexcept -> i64
{
	return this->section_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::GetSection( i64 index ) const noexcept -> const PrintRecordSection*
{
	if( index < 0 || index >= this->section_count ) [[unlikely]] return nullptr;
	return &this->section_list[ index ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::GetSections() const noexcept -> const memory::MemoryRange<const PrintRecordSection>
{
	return { this->section_list, this->section_count };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::CalculateLineCount() const noexcept -> i64
{
	auto line_count = i64 { 1 };

	for( i64 i = 0; i < this->section_count; i++ )
	{
		auto section = this->section_list[ i ];
		auto message = section.GetMessage();

		if( message.Size() == 0 ) [[unlikely]] continue;

		auto it = message.Data();
		auto it_end = message.Data() + message.Size();
		while( it != it_end )
		{
			if( *it == U'\n' )
			{
				++line_count;
			}
			++it;
		}
	}

	return line_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::Append( const PrintRecord& other ) noexcept -> PrintRecord&
{
	if( other.section_count == 0 ) [[unlikely]] return *this;

	auto old_count = section_count;
	ResizeNoConstruct( section_count + other.section_count );

	for( i64 i = 0; i < other.section_count; i++ )
	{
		new( &section_list[ old_count + i ] ) PrintRecordSection { other.section_list[ i ] };
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::AddSection( const PrintRecordSection& section ) noexcept -> PrintRecord&
{
	if( section.GetMessage().Size() == 0 ) [[unlikely]] return *this;

	auto old_count = section_count;
	this->ResizeNoConstruct( section_count + 1 );
	new( &this->section_list[ old_count ] ) PrintRecordSection { section };

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::AddIndent( u32 add_indentation_level ) noexcept -> PrintRecord&
{
	for( i64 i = 0; i < this->section_count; i++ )
	{
		auto & s = this->section_list[ i ];
		s.indent += add_indentation_level;
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::GetFinalized( u32 indentation_size ) const noexcept -> PrintRecord
{
	auto result = *this;

	result.Finalize_ApplyIndents( indentation_size );

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::PrintRecord::IsEmpty() const noexcept -> bool
{
	return this->section_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::PrintRecord::Clear() noexcept
{
	if( section_list == nullptr ) return;

	this->DestructRange( this->section_list, this->section_count );
	memory::FreeMemory( this->section_list, this->section_count );
	this->section_list	= {};
	this->section_count	= {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::PrintRecord::Copy( const PrintRecord& other ) noexcept
{
	Clear();

	if( other.section_count == 0 ) [[unlikely]] return;

	this->section_list = memory::AllocateMemory<PrintRecordSection>( other.section_count, alignof( PrintRecordSection ) );
	this->section_count = other.section_count;

	for( i64 i = 0; i < this->section_count; i++ )
	{
		new( &this->section_list[ i ] ) PrintRecordSection { other.section_list[ i ] };
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::PrintRecord::Swap( PrintRecord& other ) noexcept
{
	std::swap( this->section_list, other.section_list );
	std::swap( this->section_count, other.section_count );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::PrintRecord::ResizeNoConstruct( i64 new_count ) noexcept
{
	if( this->section_list == nullptr )
	{
		this->section_list = memory::AllocateMemory<PrintRecordSection>( new_count, alignof( PrintRecordSection ) );
	}
	else
	{
		this->section_list = this->ResizeRange( this->section_list, this->section_count, this->section_count, new_count );
	}
	this->section_count = new_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::PrintRecord::Finalize_ApplyIndents( u32 indentation_size ) noexcept
{
	auto indent_next = true;

	auto buffer_position	= i64 { 0 };
	auto character_buffer	= memory::PODAutoBuffer<c32>( 512 );

	for( i64 i = 0; i < this->section_count; i++ )
	{
		auto & r = this->section_list[ i ];
		auto message = r.GetMessage();
		auto message_data = message.Data();
		auto message_size = message.Size();
		auto section_theme = r.GetTheme();

		for( i64 i = 0; i < message_size; i++ )
		{
			auto c = message_data[ i ];
			if( indent_next )
			{
				character_buffer.FillRange( buffer_position, indentation_size, U' ' );
				buffer_position += indentation_size;
				indent_next = false;
			}

			character_buffer[ buffer_position++ ] = c;

			if( c == U'\n' )
			{
				// If last character was a newline, we do not insert indentation here but tell the next section to indent itself.
				if( i == message_size - 1 )
				{
					indent_next = true;
				}
				else
				{
					character_buffer.FillRange( buffer_position, indentation_size, U' ' );
					buffer_position += indentation_size;
				}
				continue;
			}
		}

		r.SetMessage( character_buffer.Data(), buffer_position );
		buffer_position = 0;
	}
}
