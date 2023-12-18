#pragma once

#define BI_CONTAINER_IMPLEMENTATION_SIMPLE 1
#include "../backend/TextImpl.hpp"
#undef BI_CONTAINER_IMPLEMENTATION_SIMPLE

/*

#include <core/containers/backend/ContainerBase.hpp>
#include "SimpleList.hpp"
#include <core/diagnostic/assertion/HardAssert.hpp>

#include <type_traits>
#include <algorithm>
#include <assert.h>



namespace bc {
namespace internal {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<container_bases::TextContainerCharacterType CharacterType>
class SimpleTextBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	container_bases::TextContainerCharacterType			CharacterType,
	bool												IsConst
>
class SimpleTextViewBase : public SimpleListView<CharacterType, IsConst>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= SimpleListView<CharacterType, IsConst>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= IsConst;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerType					= SimpleTextViewBase<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<CharacterType, IsConst>;

	template<typename OtherValueType, bool IsOtherConst = true>
	using ThisContainerViewType				= SimpleTextViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<typename OtherValueType>
	using ThisContainerFullType				= SimpleTextBase<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= std::conditional_t<IsOtherConst, const CharacterType*, CharacterType*>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cannot friend non-class types. TODO: enable this if we ever implement real iterators for this class.
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<typename OtherValueType, bool IsOtherConst>
	friend class TextViewBase;

	template<typename OtherValueType>
	friend class TextBase;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextViewBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextViewBase(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) noexcept requires( container_bases::IsConstConvertible<IsConst, IsOtherConst> ) :
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextViewBase(
		const CharacterType											*	c_string
	) noexcept requires( IsConst == true ) :
		Base(
			c_string,
			std::char_traits<CharacterType>::length( c_string )
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextViewBase(
		CharacterType												*	c_string
	) noexcept requires( IsConst == false ) :
		Base(
			c_string,
			std::char_traits<CharacterType>::length( c_string )
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextViewBase(
		const CharacterType											*	c_string,
		size_t															size
	) noexcept requires( IsConst == true ) :
		Base(
			c_string,
			size ? ( c_string[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextViewBase(
		CharacterType												*	c_string,
		size_t															size
	) noexcept requires( IsConst == false ) :
		Base(
			c_string,
			size ? ( c_string[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextViewBase(
		const CharacterType( &text_literal )[ ArraySize ]
	) noexcept requires( IsConst == true ) :
		Base(
			text_literal,
			ArraySize ? ( text_literal[ ArraySize - 1 ] == '\0' ? ArraySize - 1 : ArraySize ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextViewBase									&	operator=(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) noexcept
	{
		this->data_ptr			= other.Data();
		this->data_size			= other.Size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextViewBase									&	operator=(
		CharacterType												*	c_string
	) noexcept requires( IsConst == false )
	{
		this->data_ptr			= c_string;
		this->data_size			= std::char_traits<CharacterType>::length( c_string );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextViewBase									&	operator=(
		const CharacterType											*	c_string
	) noexcept requires( IsConst == true )
	{
		this->data_ptr			= const_cast<CharacterType*>( c_string );
		this->data_size			= std::char_traits<CharacterType>::length( c_string );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextViewBase									&	operator=(
		const CharacterType( &text_literal )[ ArraySize ]
	) noexcept requires( IsConst == true )
	{
		this->data_ptr			= const_cast<CharacterType*>( text_literal );
		this->data_size			= ArraySize;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator==(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator!=(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief		Check if this text is lexicographically less than another text.
	/// 
	///				Needed for sorting and sorted lists.
	/// 
	/// @param		other
	///				Other text to compare with.
	/// 
	/// @return		True if this text is lexicographically less than the other.
	///				False if the other text equal or greater.
	template<bool IsOtherConst>
	constexpr bool														operator<(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		return std::lexicographical_compare<ConstIterator, ConstIterator>(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t										CountCharacters(
		CharacterType													character,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		auto counter = size_t {};

		auto my_size = this->data_size;
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto it = this->data_ptr + start_position;
		auto it_end = this->data_ptr + search_length;
		while( it != it_end )
		{
			if( *it == character )
			{
				++counter;
			}
			++it;
		}
		return counter;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t										CountCharacters(
		CharacterType													character,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		assert(
			( start_position >= this->data_ptr ) &&
			( start_position < this->data_ptr + this->data_size ) &&
			"Iterator out of range or wrong iterator used on this container"
		);
	
		size_t position = start_position - this->data_ptr;
		return this->CountCharacters( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								FindCharacter(
		CharacterType													character,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		auto my_size = this->Size();
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto it = this->data_ptr + start_position;
		auto it_end = this->data_ptr + search_length;
		while( it != it_end )
		{
			if( *it == character )
			{
				return ConstIterator( it );
			}
			++it;
		}
		return ConstIterator( it_end );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								FindCharacter(
		CharacterType													character,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		assert(
			start_position >= this->data_ptr &&
			start_position < this->data_ptr + this->data_size &&
			"Iterator out of range or wrong iterator used on this container"
		);
	
		size_t position = start_position - this->data_ptr;
		return this->FindCharacter( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								Find(
		SimpleTextViewBase<CharacterType, true>							text_to_find,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const requires( std::is_same_v<CharacterType, char32_t> || std::is_same_v<CharacterType, char> )
	{
		// TODO: UTF8 and UTF16 disabled, see bc::TextViewBase::Find for more info.

		auto my_size = this->Size();
		auto other_size = text_to_find.Size();
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto possible_start_count = int64_t( start_position ) - int64_t( std::max( other_size, search_length ) );
		for( int64_t outer = start_position; outer < possible_start_count; ++outer )
		{
			int64_t inner;
			for( inner = 0; inner < text_to_find.Size(); ++inner )
			{
				if( this->data_ptr[ outer + inner ] != text_to_find.Data()[ inner ] ) break;
			}
			if( inner == text_to_find.Size() )
			{
				return ConstIterator( this->data_ptr + outer );
			}
		}
		return ConstIterator( this->data_ptr + my_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								Find(
		ThisViewType<true>												text_to_find,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const requires( std::is_same_v<CharacterType, char32_t> || std::is_same_v<CharacterType, char> )
	{
		// TODO: UTF8 and UTF16 disabled, see bc::TextViewBase::Find for more info.
	
		assert(
			start_position >= this->data_ptr &&
			start_position < this->data_ptr + this->data_size &&
			"Iterator out of range or wrong iterator used on this container"
		);
	
		size_t position = start_position - this->data_ptr;
		return this->Find( text_to_find, position, search_length );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<container_bases::TextContainerCharacterType CharacterType>
class SimpleTextBase : public SimpleList<CharacterType>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= SimpleList<CharacterType>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherValueType>
	using ThisContainerType					= SimpleTextBase<OtherValueType>;
	using ThisType							= ThisContainerType<CharacterType>;

	template<typename OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= SimpleTextViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<typename OtherValueType>
	using ThisContainerFullType				= SimpleTextBase<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= std::conditional_t<IsOtherConst, const CharacterType*, CharacterType*>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cannot friend non-class types. TODO: enable this if we ever implement real iterators for this class.
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<typename OtherValueType, bool IsOtherConst>
	friend class TextViewBase;

	template<typename OtherValueType>
	friend class TextBase;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerView OtherContainer>
	constexpr SimpleTextBase(
		const OtherContainer										&	other
	) noexcept requires( std::is_same_v<CharacterType, typename OtherContainer::ContainedValueType> || std::is_same_v<char, typename OtherContainer::ContainedValueType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextBase(
		SimpleTextViewBase<CharacterType, IsOtherConst> other
	) noexcept
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase(
		const SimpleTextBase										&	other
	) noexcept
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase(
		SimpleTextBase												&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase(
		const CharacterType( &c_string )[ ArraySize ]
	) noexcept
	{
		this->Append( SimpleTextViewBase<CharacterType, true>( c_string, ArraySize ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase(
		const char														( &c_string )[ ArraySize ]
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		this->Append( SimpleTextViewBase<char, true>( c_string, ArraySize ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase(
		const CharacterType											*	c_string
	) noexcept
	{
		this->Append( SimpleTextViewBase<CharacterType, true>( c_string, std::char_traits<CharacterType>::length( c_string ) ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase(
		const char													*	c_string
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		this->Append( SimpleTextViewBase<char, true>( c_string, std::char_traits<char>::length( c_string ) ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerView OtherContainer>
	constexpr SimpleTextBase										&	operator=(
		const OtherContainer										&	other
	) noexcept requires( std::is_same_v<CharacterType, typename OtherContainer::ContainedValueType> || std::is_same_v<char, typename OtherContainer::ContainedValueType> )
	{
		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr SimpleTextBase										&	operator=(
		SimpleTextViewBase<CharacterType, IsOtherConst> other
	) noexcept
	{
		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase										&	operator=(
		const SimpleTextBase										&	other
	) noexcept
	{
		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr SimpleTextBase										&	operator=(
		SimpleTextBase												&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase										&	operator=(
		const CharacterType												( &c_string )[ ArraySize ]
	) noexcept
	{
		this->Clear();
		this->Append( SimpleTextViewBase<CharacterType, true>( c_string, ArraySize ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase										&	operator=(
		const char														( &c_string )[ ArraySize ]
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		this->Clear();
		this->Append( SimpleTextViewBase<char, true>( c_string, ArraySize ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool														operator==(
		const SimpleTextBase										&	other
	) const noexcept
	{
		if( std::addressof( other ) == this ) return *this;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator==(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool														operator!=(
		const SimpleTextBase										&	other
	) const noexcept
	{
		if( std::addressof( other ) == this ) return *this;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool														operator!=(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerView OtherContainer>
	constexpr SimpleTextBase										&	operator+=(
		const OtherContainer										&	other
	) noexcept requires( std::is_same_v<CharacterType, typename OtherContainer::ContainedValueType> || std::is_same_v<char, typename OtherContainer::ContainedValueType> )
	{
		auto reserve_space = this->data_size + other.Size();
		this->Append( other, 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase										&	operator+=(
		const CharacterType												( &c_string )[ ArraySize ]
	) noexcept
	{
		auto reserve_space = this->data_size + ArraySize;
		this->Append( SimpleTextViewBase<CharacterType, true>( c_string, ArraySize ), 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase										&	operator+=(
		const char														( &c_string )[ ArraySize ]
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		auto reserve_space = this->data_size + ArraySize;
		this->Append( SimpleTextViewBase<char, true>( c_string, ArraySize ), 1, reserve_space );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerView OtherContainer>
	constexpr SimpleTextBase											operator+(
		const OtherContainer										&	other
	) noexcept requires( std::is_same_v<CharacterType, typename OtherContainer::ContainedValueType> || std::is_same_v<char, typename OtherContainer::ContainedValueType> )
	{
		auto copy = SimpleTextBase {};
		copy.Reserve( this->data_size + other.Size() );
		copy = *this;
		copy += other;
		return copy;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase											operator+(
		const CharacterType												( &c_string )[ ArraySize ]
	) noexcept
	{
		auto copy = SimpleTextBase {};
		copy.Reserve( this->data_size + ArraySize );
		copy = *this;
		copy += SimpleTextViewBase<CharacterType, true>( c_string, ArraySize );
		return copy;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr SimpleTextBase											operator+(
		const char														( &c_string )[ ArraySize ]
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		auto copy = SimpleTextBase {};
		copy.Reserve( this->data_size + ArraySize );
		copy = *this;
		copy += SimpleTextViewBase<char, true>( c_string, ArraySize );
		return copy;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief		Check if this text is lexicographically less than another text.
	/// 
	///				Needed for sorting and sorted lists.
	/// 
	/// @param		other
	///				Other text to compare with.
	/// 
	/// @return		True if this text is lexicographically less than the other.
	///				False if the other text equal or greater.
	template<bool IsOtherConst>
	constexpr bool														operator<(
		SimpleTextViewBase<CharacterType, IsOtherConst>					other
	) const noexcept
	{
		return std::lexicographical_compare<ConstIterator, ConstIterator>(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief		Check if this text is lexicographically less than another text.
	/// 
	///				Needed for sorting and sorted lists.
	/// 
	/// @param		other
	///				Other text to compare with.
	/// 
	/// @return		True if this text is lexicographically less than the other.
	///				False if the other text equal or greater.
	constexpr bool														operator<(
		const SimpleTextBase										&	other
	) const noexcept
	{
		return std::lexicographical_compare<ConstIterator, ConstIterator>(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::ContainerView OtherContainerType>
	constexpr void														Append(
		const OtherContainerType									&	other,
		size_t															count				= 1,
		size_t															headroom			= 0
	) noexcept requires( std::is_same_v<CharacterType, typename OtherContainerType::ContainedValueType> || std::is_same_v<char, typename OtherContainerType::ContainedValueType> )
	{
		auto old_size				= this->data_size;
		size_t other_size			= other.Size();
		size_t total_insert_size	= other_size * count;

		// Other container has a Size function and is a Bitcrafte container.
		this->ResizeNoConstruct( old_size + total_insert_size, headroom );
		for( size_t c = 0; c < count; ++c ) {
			auto other_it			= other.begin();
			auto write_location		= other_size * c;
			for( size_t i = 0; i < other_size; ++i ) {
				new( &this->data_ptr[ write_location + i ] ) CharacterType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr void														Append(
		const CharacterType												( &c_string )[ ArraySize ],
		size_t															count							= 1,
		size_t															headroom						= 0
	) noexcept
	{
		this->Clear();
		this->Append( SimpleTextViewBase<CharacterType, true>( c_string, ArraySize ), count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr void														Append(
		const char														( &c_string )[ ArraySize ],
		size_t															count							= 1,
		size_t															headroom						= 0
	) noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		this->Clear();
		this->Append( SimpleTextViewBase<char, true>( c_string, ArraySize ), count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t										CountCharacters(
		CharacterType													character,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		return ThisViewType<true>( *this ).CountCharacters( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t										CountCharacters(
		CharacterType													character,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		return ThisViewType<true>( *this ).CountCharacters( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								FindCharacter(
		CharacterType													character,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		return ThisViewType<true>( *this ).FindCharacter( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								FindCharacter(
		CharacterType													character,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const
	{
		return ThisViewType<true>( *this ).FindCharacter( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								Find(
		ThisViewType<true>												text_to_find,
		size_t															start_position					= 0,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const requires( std::is_same_v<CharacterType, char32_t> || std::is_same_v<CharacterType, char> )
	{
		// TODO: UTF8 and UTF16 disabled, see bc::TextViewBase::Find for more info.

		return ThisViewType<true>( *this ).Find( text_to_find, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator								Find(
		ThisViewType<true>												text_to_find,
		ConstIterator													start_position,
		size_t															search_length					= std::numeric_limits<size_t>::max()
	) const requires( std::is_same_v<CharacterType, char32_t> || std::is_same_v<CharacterType, char> )
	{
		// TODO: UTF8 and UTF16 disabled, see bc::TextViewBase::Find for more info.
	
		return ThisViewType<true>( *this ).Find( text_to_find, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]] constexpr CharacterType							*	ToCStr()
	{
		if( this->data_ptr == nullptr ) return nullptr;
		this->Reserve( this->data_size + 1 );
		this->data_ptr[ this->data_size ] = '\0';
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator SimpleTextViewBase<CharacterType, true>() const
	{
		return SimpleTextViewBase<CharacterType, true> { this->data_ptr, this->data_size };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr operator SimpleTextViewBase<CharacterType, false>()
	{
		return SimpleTextViewBase<CharacterType, false> { this->data_ptr, this->data_size };
	}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void														SwapOther(
		SimpleTextBase												&&	other
	) noexcept
	{
		if( this != std::addressof( other ) ) {
			std::swap( this->data_ptr, other.data_ptr );
			std::swap( this->data_size, other.data_size );
			std::swap( this->data_capacity, other.data_capacity );
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using SimpleText				= SimpleTextBase<char>;
using SimpleText8				= SimpleTextBase<char8_t>;
using SimpleText16				= SimpleTextBase<char16_t>;
using SimpleText32				= SimpleTextBase<char32_t>;

using SimpleTextView			= SimpleTextViewBase<char,		true>;
using SimpleTextView8			= SimpleTextViewBase<char8_t,	true>;
using SimpleTextView16			= SimpleTextViewBase<char16_t,	true>;
using SimpleTextView32			= SimpleTextViewBase<char32_t,	true>;

using SimpleEditableTextView	= SimpleTextViewBase<char,		false>;
using SimpleEditableTextView8	= SimpleTextViewBase<char8_t,	false>;
using SimpleEditableTextView16	= SimpleTextViewBase<char16_t,	false>;
using SimpleEditableTextView32	= SimpleTextViewBase<char32_t,	false>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check that simple text view can be used as a template parameter in SimpleLinearContainer.
static_assert( SimpleListAllowedValueType<SimpleText> );
static_assert( SimpleListAllowedValueType<SimpleText8> );
static_assert( SimpleListAllowedValueType<SimpleText16> );
static_assert( SimpleListAllowedValueType<SimpleText32> );

static_assert( SimpleListAllowedValueType<SimpleTextView> );
static_assert( SimpleListAllowedValueType<SimpleTextView8> );
static_assert( SimpleListAllowedValueType<SimpleTextView16> );
static_assert( SimpleListAllowedValueType<SimpleTextView32> );

static_assert( SimpleListAllowedValueType<SimpleEditableTextView> );
static_assert( SimpleListAllowedValueType<SimpleEditableTextView8> );
static_assert( SimpleListAllowedValueType<SimpleEditableTextView16> );
static_assert( SimpleListAllowedValueType<SimpleEditableTextView32> );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( SimpleText ) == 24 );
static_assert( sizeof( SimpleText8 ) == 24 );
static_assert( sizeof( SimpleText16 ) == 24 );
static_assert( sizeof( SimpleText32 ) == 24 );

static_assert( sizeof( SimpleTextView ) == 16 );
static_assert( sizeof( SimpleTextView8 ) == 16 );
static_assert( sizeof( SimpleTextView16 ) == 16 );
static_assert( sizeof( SimpleTextView32 ) == 16 );

static_assert( sizeof( SimpleEditableTextView ) == 16 );
static_assert( sizeof( SimpleEditableTextView8 ) == 16 );
static_assert( sizeof( SimpleEditableTextView16 ) == 16 );
static_assert( sizeof( SimpleEditableTextView32 ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( container_bases::TextContainer<SimpleText> );
static_assert( container_bases::TextContainer<SimpleText8> );
static_assert( container_bases::TextContainer<SimpleText16> );
static_assert( container_bases::TextContainer<SimpleText32> );

static_assert( container_bases::TextContainerView<SimpleTextView> );
static_assert( container_bases::TextContainerView<SimpleTextView8> );
static_assert( container_bases::TextContainerView<SimpleTextView16> );
static_assert( container_bases::TextContainerView<SimpleTextView32> );

static_assert( container_bases::TextContainerEditableView<SimpleEditableTextView> );
static_assert( container_bases::TextContainerEditableView<SimpleEditableTextView8> );
static_assert( container_bases::TextContainerEditableView<SimpleEditableTextView16> );
static_assert( container_bases::TextContainerEditableView<SimpleEditableTextView32> );


static_assert( container_bases::ContainerView<SimpleTextView> );
static_assert( !container_bases::ContainerEditableView<SimpleTextView> );
static_assert( !container_bases::Container<SimpleTextView> );

static_assert( container_bases::ContainerView<SimpleEditableTextView> );
static_assert( container_bases::ContainerEditableView<SimpleEditableTextView> );
static_assert( !container_bases::Container<SimpleEditableTextView> );

static_assert( container_bases::ContainerView<SimpleText> );
static_assert( container_bases::ContainerEditableView<SimpleText> );
static_assert( container_bases::Container<SimpleText> );

static_assert( container_bases::TextContainerView<SimpleTextView> );
static_assert( !container_bases::TextContainerEditableView<SimpleTextView> );
static_assert( !container_bases::TextContainer<SimpleTextView> );

static_assert( container_bases::TextContainerView<SimpleEditableTextView> );
static_assert( container_bases::TextContainerEditableView<SimpleEditableTextView> );
static_assert( !container_bases::TextContainer<SimpleEditableTextView> );

static_assert( container_bases::TextContainerView<SimpleText> );
static_assert( container_bases::TextContainerEditableView<SimpleText> );
static_assert( container_bases::TextContainer<SimpleText> );



} // internal
} // bc

*/
