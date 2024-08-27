#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SourceLocation
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation() noexcept = default;
	inline constexpr SourceLocation( const SourceLocation& other ) noexcept = default;
	inline constexpr SourceLocation( SourceLocation&& other ) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto operator=( const SourceLocation& other ) noexcept -> SourceLocation& = default;
	inline constexpr auto operator=( SourceLocation&& other ) noexcept -> SourceLocation& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto operator==( const SourceLocation& other ) const noexcept -> bool
	{
		return
			line == other.line &&
			column == other.column &&
			file == other.file &&
			function == other.function;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto operator!=( const SourceLocation& other ) const noexcept -> bool
	{
		return
			line != other.line ||
			column != other.column ||
			file != other.file ||
			function != other.function;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static inline constexpr auto Current(
		i64				line		= i64( __builtin_LINE() ),
		i64				column		= i64( __builtin_COLUMN() ),
		const char*		file		= __builtin_FILE(),
		const char*		function	= __builtin_FUNCTION()
	) noexcept -> SourceLocation
	{
		SourceLocation source_location;
		source_location.line		= line;
		source_location.column		= column;
		source_location.file		= file;
		source_location.function	= function;
		return source_location;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto GetLine() const noexcept -> i64
	{
		return line;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto GetColumn() const noexcept -> i64
	{
		return column;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto GetFile() const noexcept -> const char*
	{
		return file;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto GetFunction() const noexcept -> const char*
	{
		return function;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr auto IsEmpty() const noexcept -> bool
	{
		return *this == SourceLocation {};
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	i64														line			= {};
	i64														column			= {};
	const char											*	file			= {};
	const char											*	function		= {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( SourceLocation ) == 32 );



} // diagnostic
} // bc
