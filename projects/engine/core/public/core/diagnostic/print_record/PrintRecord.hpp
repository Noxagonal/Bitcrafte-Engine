#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/simple/SimpleList.hpp>
#include <core/containers/simple/SimpleText.hpp>
#include <core/diagnostic/print_record/PrintRecordSection.hpp>



namespace bc {
namespace diagnostic {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Represents text that is suitable for logging or other error reporting.
///
/// Provides utilities to construct and manipulate a multi-line and multi-color text entry suitable for printing via the system
/// console, in-editor console, or in-game console.
class BITCRAFTE_ENGINE_API PrintRecord
{
public:

	using PrintRecordSectionList = SimpleList<PrintRecordSection>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr PrintRecord() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr PrintRecord(
		const PrintRecord								&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr PrintRecord(
		PrintRecord										&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecord(
		const bc::internal_::SimpleTextView32				simple_text_view
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename CharacterType, u64 StringArraySize>
	PrintRecord(
		const CharacterType( &c_string )[ StringArraySize ]
	)
	{
		auto new_section = PrintRecordSection {};
		new_section.text.Append( c_string );
		AddSection( new_section );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecord											&	operator=(
		const PrintRecord								&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecord											&	operator=(
		PrintRecord										&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds another PrintRecord at the end of this.
	///
	/// @warning
	/// If print record has been finalized, this function does nothing.
	///
	/// @param other
	/// PrintRecord to append at the end.
	///
	/// @return
	/// Reference to this.
	PrintRecord											&	operator+=(
		const PrintRecord								&	other
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds an individual PrintRecordSection at the end of this.
	/// 
	/// @warning
	/// If print record has been finalized, this function does nothing.
	///
	/// @param section
	/// Section to append at the end.
	///
	/// @return
	/// Reference to this.
	PrintRecord											&	operator+=(
		const PrintRecordSection						&	section
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Combines this and another PrintRecord and returns the result.
	///
	/// @param other
	/// PrintRecord to add to the end.
	///
	/// @return
	/// A new PrintRecord with combined texts.
	PrintRecord												operator+(
		const PrintRecord								&	other
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const PrintRecordSectionList						&	GetSections() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get actual line count of this print record section.
	///
	/// @return
	/// Number of lines taken by this print record.
	const u32												CalculateLineCount() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds another PrintRecord at the end of this.
	///
	/// @warning
	/// If print record has been finalized, this function does nothing.
	///
	/// @param other
	/// PrintRecord to append at the end.
	///
	/// @return
	/// Reference to this.
	PrintRecord											&	Append(
		const PrintRecord								&	other
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds an individual PrintRecordSection at the end of this.
	/// 
	/// @warning
	/// If print record has been finalized, this function does nothing.
	///
	/// @param section
	/// Section to append at the end.
	///
	/// @return
	/// Reference to this.
	PrintRecord											&	AddSection(
		const PrintRecordSection						&	section
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Adds an indentation level to this record. Which may be later converted into spaces.
	///
	/// @note
	/// This is fast. Indentation is applied to all PrintRecordSection children by incrementing an integer.
	///
	/// @warning
	/// If print record has been finalized, this function does nothing.
	///
	/// @param add_indentation_level
	/// Value to add to indentation level. Positive values adds more indentation levels, negative values can be used to move back
	/// an indentation.
	///
	/// @return
	/// Reference to this.
	PrintRecord											&	AddIndent(
		i32													add_indentation_level			= 1
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts tabs to spaces, calculates number of lines, applies formatting, etc. And returns a copy of this.
	///
	/// This is usually the final stage before being presented to the viewer.
	///
	/// @note
	/// This print record cannot be modified after this step, any changes are ignored.
	///
	/// @note
	/// Print record must be finalized before using it to display information.
	///
	/// @param indentation_size
	/// Number of spaces to be inserted per indentation level. You can think of it as of spaces per tab.
	///
	/// @return
	/// Reference to this.
	PrintRecord												GetFinalized(
		u32													indentation_size				= 4
	) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool													IsEmpty() const noexcept;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Applies the indents to actual spaces in front of every new line and clears recorded indents from PrintRecordSection.
	///
	/// @param processed_section_list
	/// Section list source we should process.
	///
	/// @return
	/// New section list with applied indents.
	void													Finalize_ApplyIndents(
		u32													indentation_size				= 4
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PrintRecordSectionList									section_list;
};



} // diagnostic
} // bc
