#pragma once

#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>
#include <core/diagnostic/stack_trace/StackTrace.hpp>
#include <core/containers/simple/SimpleUniquePtr.hpp>



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
	constexpr inline Exception(
		const PrintRecord		&	print_record			= PrintRecord{},
		const SourceLocation	&	source_location			= SourceLocation::Current(),
		const StackTrace		&	stack_trace 			= StackTrace::Current()
	) noexcept :
		message{ print_record },
		source_location{ source_location },
		stack_trace{ stack_trace },
		next{ nullptr }
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline Exception(
		const Exception									&	other
	)
	{
		this->CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Exception(
		Exception										&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline Exception							&	operator=(
		const Exception									&	other
	)
	{
		this->CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Exception									&	operator=(
		Exception										&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline const PrintRecord					&	GetMessage() const noexcept
	{
		return this->message;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline const SourceLocation				&	GetSourceLocation() const noexcept
	{
		return this->source_location;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline const StackTrace					&	GetStackTrace() const noexcept
	{
		return this->stack_trace;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline const Exception					*	GetNextException() const noexcept
	{
		return this->next.Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Sets the next exception in the exception chain.
	///
	/// Next exception is used when an exception is caught by something which will then throw its own exception with additional
	/// information. The original exception may be stored inside this exception by calling this function.
	///
	/// @param next_exception
	/// Exception we wish to store inside this exception.
	constexpr inline void									SetNextException(
		const Exception									&	next_exception
	)
	{
		assert( std::addressof( next_exception ) != this && "next_exception was this exception" );
		if( std::addressof( next_exception ) == this ) return;
		this->next = bc::internal_::MakeSimpleUniquePtr<Exception>( next_exception );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline bool									IsEmpty() const noexcept
	{
		return this->message.IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr inline void									CopyOther(
		const Exception									&	other
	)
	{
		this->message			= other.message;
		this->source_location	= other.source_location;
		this->stack_trace		= other.stack_trace;
		if( !other.next.IsEmpty() ) SetNextException( *other.next );
	}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecord												message;
	SourceLocation											source_location;
	StackTrace												stack_trace;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Next exception in exception chain.
	///
	/// This is used when an exception is caught by something which will then throw its own exception with additional information.
	/// The original exception may be stored here.
	bc::internal_::SimpleUniquePtr<Exception>				next;
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
void													Throw [[noreturn]] (
	const Exception									&	exception,
	SourceLocation										source_location					= SourceLocation::Current()
);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Throw exception with message and optional information.
///
/// Example:
/// Eg. Index out of scope: Assuming list size is 5 and accessed index was 8.
///		diagnostic::Throw(
///			diagnostic::MakePrintRecord_AssertText( "Index out of scope",
///				"List size", size,
///				"Accessed index", index
///			)
///		);
/// Will output:
///		Index out of scope
///		- List size: 5
///		- Accessed index: 8
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
/// @see bc::diagnostic::PrintRecord
///
/// @param print_record
///	bc::diagnostic::PrintRecord. General info of what caused the exception. Eg, "Index out of scope" when iterating over
/// an array.
///
/// @param source_location
/// Please leave this as default. This reports the source location where this function was called.
void													Throw [[noreturn]] (
	const PrintRecord								&	print_record,
	const SourceLocation							&	source_location				= SourceLocation::Current()
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Throw exception with message and optional information.
///
/// Example:
/// Eg. Index out of scope: Assuming list size is 5 and accessed index was 8.
///		diagnostic::Throw(
///			diagnostic::MakePrintRecord_AssertText( "Index out of scope",
///				"List size", size,
///				"Accessed index", index
///			)
///		);
/// Will output:
///		Index out of scope
///		- List size: 5
///		- Accessed index: 8
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
/// @see bc::diagnostic::PrintRecord
///
/// @param print_record
///	bc::diagnostic::PrintRecord. General info of what caused the exception. Eg, "Index out of scope" when iterating over
/// an array.
///
/// @param source_location
/// Please leave this as default. This reports the source location where this function was called.
void													Throw [[noreturn]] (
	const bc::internal_::SimpleTextView32				message,
	const SourceLocation							&	source_location				= SourceLocation::Current()
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Throw exception with message and optional information.
///
/// Example:
/// Eg. Index out of scope: Assuming list size is 5 and accessed index was 8.
///		diagnostic::Throw(
///			diagnostic::MakePrintRecord_AssertText( "Index out of scope",
///				"List size", size,
///				"Accessed index", index
///			)
///		);
/// Will output:
///		Index out of scope
///		- List size: 5
///		- Accessed index: 8
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
/// @see bc::diagnostic::PrintRecord
///
/// @param print_record
///	bc::diagnostic::PrintRecord. General info of what caused the exception. Eg, "Index out of scope" when iterating over
/// an array.
///
/// @param source_location
/// Please leave this as default. This reports the source location where this function was called.
template<utility::TextContainerCharacterType CharacterType, u64 StringArraySize>
void													Throw [[noreturn]] (
	const CharacterType( &message )[ StringArraySize ],
	const SourceLocation							&	source_location				= SourceLocation::Current()
)
{
	Throw(
		Exception{
			PrintRecord( message ),
			source_location,
			StackTrace::Current( 1 )
		}
	);
}



} // diagnostic
} // bc
