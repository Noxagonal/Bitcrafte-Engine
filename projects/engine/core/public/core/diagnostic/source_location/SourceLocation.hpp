#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {
namespace diagnostic {



class SourceLocation
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation(
		const SourceLocation							&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation(
		SourceLocation									&&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation						&	operator=(
		const SourceLocation							&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr SourceLocation						&	operator=(
		SourceLocation									&&	other
	) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static inline consteval SourceLocation					Current(
		u64													line			= __builtin_LINE(),
		u64													column			= __builtin_COLUMN(),
		const char										*	file			= __builtin_FILE(),
		const char										*	function		= __builtin_FUNCTION()
	) noexcept
	{
		SourceLocation source_location;
		source_location.line		= line;
		source_location.column		= column;
		source_location.file		= file;
		source_location.function	= function;
		return source_location;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr u64 									GetLine() const noexcept
	{
		return line;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr u64	 								GetColumn() const noexcept
	{
		return column;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr const char							*	GetFile() const noexcept
	{
		return file;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr const char							*	GetFunction() const noexcept
	{
		return function;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	u64														line			= {};
	u64														column			= {};
	const char											*	file			= {};
	const char											*	function		= {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( SourceLocation ) == 32 );



} // diagnostic
} // bc
