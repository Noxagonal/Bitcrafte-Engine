
#include <core/PreCompiledHeader.hpp>
#include <core/diagnostic/stack_trace/StackTrace.hpp>

#include <core/diagnostic/assertion/HardAssert.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::StackTrace::~StackTrace() noexcept
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::StackTrace::StackTrace( const StackTrace & other ) noexcept
{
	if( this == std::addressof( other ) ) [[unlikely]] return;
	Copy( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::StackTrace::operator=( const StackTrace& other ) noexcept -> StackTrace&
{
	if( this == std::addressof( other ) ) [[unlikely]] return *this;
	Clear();
	StackTrace( other ).Swap( *this );
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::StackTrace::Current( i64 leaf_calls_to_ignore ) noexcept -> StackTrace

{
	// TODO
	return {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::Clear() noexcept
{
	frame_count = 0;
	frame_list.Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto bc::diagnostic::StackTrace::IsEmpty() const noexcept -> bool
{
	return frame_count == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::Copy( const StackTrace & other ) noexcept
{
	frame_count = other.frame_count;
	frame_list.Allocate( frame_count, alignof( StackTraceFrame ) );

	for( i32 i = 0; i < frame_count; ++i )
	{
		new( &frame_list.data[ i ] ) StackTraceFrame( other.frame_list.data[ i ] );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::Swap( StackTrace& other ) noexcept
{
	std::swap( frame_count, other.frame_count );
	std::swap( frame_list, other.frame_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::CopyConstructStackTraceFrame( i64 at_index, const StackTraceFrame & other ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < frame_list.count, U"Stack trace frame Index out of bounds" );

	new( &frame_list.data[ at_index ] ) StackTraceFrame( other );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::MoveConstructStackTraceFrame( i64 at_index, StackTraceFrame && other ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < frame_list.count, U"Stack trace frame Index out of bounds" );

	new( &frame_list.data[ at_index ] ) StackTraceFrame( std::move( other ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::DestructStackTraceFrame( i64 at_index ) noexcept
{
	BEngineHardAssert( at_index >= 0 && at_index < frame_count, U"Stack trace frame Index out of bounds" );

	frame_list.data[ at_index ].~StackTraceFrame();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::diagnostic::StackTrace::DestructAllStackTraceFrames() noexcept
{
	for( i64 i = 0; i < frame_count; ++i )
	{
		frame_list.data[ i ].~StackTraceFrame();
	}
}
