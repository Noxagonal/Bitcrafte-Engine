#pragma once

#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>
#include <core/diagnostic/stack_trace/StackTrace.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Primary exception used throughout the engine.
///
///	Catching this will catch all exceptions that the engine can natively throw.
class BITCRAFTE_ENGINE_API Exception
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct an Exception object.
	///
	/// @param print_record
	///	General info of what caused the exception. Eg, "Index out of scope" when iterating over an array. See
	/// bc::diagnostic::PrintRecord for more info on how to construct a print record.
	///
	/// @param source_location
	/// This reports the source location where this function was called. If left as default, source location will point to the line
	/// where this function was called.
	///
	/// @param stack_trace
	/// This reports the source location where this function was called. If left as default, stack trace will point to the function
	/// where this function was called.
	Exception(
		const PrintRecord&		print_record		= PrintRecord(),
		const SourceLocation&	source_location		= SourceLocation::Current(),
		const StackTrace&		stack_trace 		= StackTrace::Current()
	) noexcept;
	Exception( const Exception& other ) noexcept;
	Exception( Exception&& other ) noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~Exception() noexcept;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto operator=( const Exception& other ) noexcept -> Exception&;
	auto operator=( Exception&& other ) noexcept -> Exception& = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetMessage() const noexcept -> const PrintRecord&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetSourceLocation() const noexcept -> const SourceLocation&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetStackTrace() const noexcept -> const StackTrace&;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto GetNextException() const noexcept -> const Exception*;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Sets the next exception in the exception chain.
	///
	/// Next exception is used when an exception is caught by something which will then throw its own exception with additional
	/// information. The original exception is be stored inside this exception by calling this function.
	///
	/// @note
	/// The exception and all of its exception chain will be copied to this exception. This makes a deep copy of the exception
	/// chain.
	///
	/// @param next_exception
	/// Exception we wish to store inside this exception.
	void SetNextException( const Exception& next_exception ) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto IsEmpty() const noexcept -> bool;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear() noexcept;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ClearNext() noexcept;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Copy( const Exception& other ) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Swap( Exception& other ) noexcept;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecord		message				= {};
	SourceLocation	source_location		= {};
	StackTrace		stack_trace			= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Next exception in exception chain.
	///
	/// @note
	/// Owned.
	///
	/// This is used when an exception is caught by something which will then throw its own exception with additional information.
	/// The original exception may be stored here.
	Exception*		next				= {};
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Throw already created exception.
///
/// @note
///	Depending on where the exception is thrown, this will either disable parts of the engine or cause general crash of the
/// engine. For example if an Entity throws in its update logic, then the game is shut down and editor displays what caused the
/// error. If the worker thread catches an exception directly, it'll cause the whole engine to shut down.
///
/// @warning
/// All exceptions are disabled in final release build of the game, in which case call to this function crashes the
/// application and lets the OS do the cleanup.
///
/// @param exception
///	Exception to throw.
BITCRAFTE_ENGINE_API
void Throw [[noreturn]] ( const Exception& exception );



} // diagnostic
} // bc
